# Memory

ARM AArch32 has two Memory Systems Architecture: VMSA and PMSA. **PMSA** or Protected Memory System Architecture is a memory protection architecture scheme that utilizes **MPU**, while **VMSA** or Virtual Memory System Architecture provides both memory protection and address virtualization via **MMU**. Think of **MMU** as an **MPU** but with added virtualization.

The Memory Model Feature Register 0 (ID_MMFR0 bits[3:0] VMSA/PMSA support) allows program to identify if the core (i.e Cortex A-53) supports **PMSA** or **VMSA**.
``` asm
  MRC p15, 0, r0, c0, c1, 4
```

## Memory Management Unit (MMU)
The MMU provides both address virtualization and memory protection. It translates CPU memory access from virtual address to physical address which allows writing programs without knowing the underlying physical memory organization. In Baremetal application, MMU can be used to partition memory map and assign protection attributes such as permission and memory attributes such as regions being cache-able or executable.

<Add image here>

The ARM MMU uses 2 levels of translation. The 1st level of translation splits the entire AArch32 memory space (2^32 or **4GiB**) into **4096 sections**, each is **1MiB** (4GiB / 4096) in size. A **section** is a unit of translatable memory space.

### L1 Translation
When CPU wants to access memory with a virtual address, the MMU will look into the L1 Translation Table to translate virtual to physical address. The L1 Translation table, aka 'page table' is an array of words(u32) with length of **4096**, with each entry holds either pointer to the base address of L2 Translation table or the *mapped physical address* and protection/memory attributes for translating and accessing a **1MiB** section.

```text
Page Table

  idx       words(u32)
        +--------------+
 0x0FFF |              |
        +--------------+
 0x0008 |              |
        +--------------+
 0x0007 |              |
        +--------------+
 ....   |              |
        +--------------+
 0x0005 |              |
        +--------------+
 0x0004 |              |
        +--------------+
 0x0003 |              |
        +--------------+
 0x0002 |              |
        +--------------+
 0x0001 |              |
        +--------------+
 0x0000 |              |
        +--------------+
```
> [!Note] Pages and Page Table
> **Page**: Refers to the unit of translation i.e a 1MiB *virtual* page will be translated to the same *physical* page size.
> **Page Table**: Is a table that is used to translate virtual to physical. Typically each element is word size and can be address via virtual address and each element contains the physical page address.

## Write Cache Policy

  * `write-through`: When data is written to cache, it is immediately written to memory. This keeps the cache and main memory sync but at the cost of increase in main memory traffic due to write transactions.
  * `write-back`: Writes are only performed in cache. This could cause data in memory to be **stale**. When cache is written , a **dirty** bit in cache is set to `1` to indicate that data is written in cache but not yet in main memory.

## Clean and Invalidation
  * `invalidation` refers to cache line **valid** bit to be set to `0`. If its valid, then invalidate!
  * `clean` refers to write contents of all cache lines with **dirty** bits set to main memory.

CP15 instructions provides operations to clean or/and invalidate cache.

## L2 Translation

## PIPT and VIPT

> [!Note] ARM core main cache (L1) always use N-way set associative cache. 
> ``` text 
> Associative cache address format:
>  +----------------+--------+---------+----------+
>  |     Tag        |   Set  |   Word  |   Byte   |
>  +----------------+--------+---------+----------+
> 31              13 12     5 4       2 1         0
> ```

**Virtual Index Physical Tag**, means that *virtual address is used to determine the cache line index* but the *physical address is used for tag*. This resolves the issue of cache need for invalidation when MMU virtual to physical mapping changes. However this introduces new problem, since the associative cache uses bit [12:5] to determine the `set`, virtual address uses bit [12] for address translation (64KiB page size). This potentially creates a problem, two or more virtual address with different bit [13:12] could also point to the same physical address. This creates two or more cache entries that points to the same physical address, which means memory can already exist in the cache line with different virtual cache line index.
```text
                                  Virtual Address
 +----------------+-----------------------------+
 |      Index     |           offset            |
 +----------------+-----------------------------+
31       |      12 11          |                0
         v                     |
 +----------------+            |
 |   Translation  |            |
 +----------------+            |
         |                     |
         v                     v  Physical Address
 +----------------+-----------------------------+
 |      Index     |           offset            |
 +----------------+-----------------------------+
31              12 11                           0
```
The solution to this is to use [page colouring](https://developer.arm.com/community/arm-community-blogs/b/architectures-and-processors-blog/posts/page-colouring-on-armv6-and-a-bit-on-armv7) scheme. Page colouring uses `bits[13:12]` to indicate a colour. A page allocator, when mapping virtual address to a physical address, will impose restriction, to ensure that a physical address will only be mapped to a single colour. physical address will be use both as cache line index and cache tag. This ensures that there would be no duplicate physical address tag in cache.

All ARM cortex uses `PIPT` scheme for data cache and `VIPT` for instruction cache since usually, restriction in size can be imposed in instructions, which prevents wrapping of `bits[13:12]` which prevents having two virtual address being mapped to the same physical address.

# Bootflow
On the high level part of the bootflow is to
  1. clear and/or invalidate all caches: *I-Cache*, *D-Cache*, *TLB*
  2. create a translation table
  3. initialise the Translation Table Register (`TTBR`)
  4. enable caches
  5. enable MMU

## Invalidating Caches

The first step is to make sure the caches are all invalidated. At power-up (cold boot) this might not matter as we expect caches are disabled and invalidate but at runtime reset, caches might be dirty. Part of boot process is to invalidate the I-Cache, D-Cache and TLB.

Example code invalidating icache
```asm
setup_cache:
  /* disable mmu and all caches */
  mrc p15, 0, r1, c1, c0, 0
  bic r1, r1, #1
  bic r1, r1, #(1 << 12)
  bic r1, r1, #(1 << 2)
  mcr p15, 0, r1, c1, c0, 0

  /* invalidate all caches and TLB */
  mov	r0, #0
	mcr	p15, 0, r0, c7, c5, 0		// invalidate instruction cache
	mcr	p15, 0, r0, c7, c5, 6		// Invalidate branch predictor array
	mcr	p15, 0, r0, c8, c7, 0		// invalidate entire unified TLB
  isb
  bl invalidate_dcache

  /* enable all caches */
  mrc p15, 0, r1, c1, c0, 0
  bic r1, r1, #(1 << 12)
  bic r1, r1, #(1 << 2)
  mcr p15, 0, r1, c1, c0, 0
```

# Invalidating the Data Cache
The coprocessor 15(`cp15`) operation `DCISW` is used to invalidate the data cache. This can be done with `MCR p15, 0, rt, c7, c6, 2`. The operation requires a register data with format
``` text
31   32-A                  B         L     4       1  0
+-----+--------------------+---------+-----+-------+---+
| Way |         SBZ        |   Set   | SBZ | Level | 0 |
+-----+--------------------+---------+-----+-------+---+
```
Where:
  * `A` Is Log2(ASSOCIATIVITY), rounded up to the next integer if necessary.
  * `B` Is (L + S).
  * `L` Is Log2(LINELEN). The LINELEN can be calculated from CCSIDR.LineSize.
  * `S` Is Log2(NSETS), rounded up to the next integer if necessary.
  * `Level, bits[4:1]` ((Cache level to operate on) -1) For example, this field is 0 for operations on L1 cache, or 1 for operations on L2 cache.
  * `Set, bits[B-1:L]` The number of the set to operate on.
  * `Way, bits[31:(32-A)]` The number of the way to operate on.

The `A`, `Level` and `Sets` can be found via `CCSIDR`(Cache Size ID Registers).
```text
  31   30   29   28  27                    13 12             3 2          0
+----+----+----+----+------------------------+----------------+------------+
| WT | WB | RA | WA |         NumSets        |  Associativity |  LineSize  |
+----+----+----+----+------------------------+----------------+------------+
```
Fields Descriptions are:
  * `LineSize, bits[2:0]` is Log2(N) - 4, where *N=Number of Cache Line Bytes*.
  * `Associativity, bits[12:3]` *=Associative(`A`)-1* Is the degree of associativity for a *set associative* cache.
  * `NumSets, bits[27:13]` *=Number of Sets(`NSETS`)-1* The number of sets in the cache.
  * `WT, bit [31]` Indicates whether the cache level supports write-through.
  * `WB, bit [30]` Indicates whether the cache level supports write-back.
  * `RA, bit [29]` Indicates whether the cache level supports read-allocation.
  * `WA, bit [28]` Indicates whether the cache level supports write-allocation.

D Cache invalidation flow
  1. Read `CCSIDR` to get *LineSize* `L`, *Associativity* `A`, *Number of Sets* `N`.
  2. for each way, iterate each set
  3. for each (way, set), clear the d-cache via `DCISW`

Note that each cache level L1, L2 ... L7 has each its own `CCSIDR`. To get the value for each L*n* Cache, `CSSELR` Cache Size Selection Register must be set

```
31                                        4 3     1  0
+-----+--------------------+---------+-----+-------+---+
| Way |         SBZ        |   Set   | SBZ | Level | 0 |
+-----+--------------------+---------+-----+-------+---+
```

Below is simple example on how to invalidate `L1` cache.
```asm
__l1_d_cache_invalidate:
  mrc 15, 1, r0, c0, c0, 0 // r0 = CCSIDR

  and r1, r0, #0x0F       // r1 = r0 & 0x0F ; ccsidr.LineSize
  add r1, r1, #4          // r1 = (r1 + 4)  ; line length

  ldr r2, =0x3FF          // r7 = 0x3FF
  and r2, r2, r0, lsr #3  // r2 = r2 & (ccsidr >> 3) ; ccsidr.Assoc

  rsb r8, r2, #32         // r8 = (32 - A) = (32 - r2)

  ldr r3, =0x7FFF         // r3 = 0x7FFF
  and r3, r3, r0, lsr #13 // r3 = r3 & (r0 >> 13) ; ccsidr.NumSets

  mov r4, #0              // r4 = i_way = 0 ; running iterator count of way

  mov r0, #0              // r0 = 0 will use this as 0 register initializer
  mov r9, #0              // r9 = 0, cache level, 0 = L1 cache, 1 = L2 cache

for_each_way:
  mov r5, #0              // r5 = i_set = 0 ; running iterator count of set

for_each_set:
  orr r6, r0, r4, lsl r8  // r6 = (i_way << (32-A)) = (r4 << r8)
  orr r6, r6, r5, lsl r1  // r6 = r6 | (r5 << r1)
  orr r6, r6, r5, lsl r1  // r6 = r6 | (r5 << r1)

  add r5, r5, #1          // r5++ ; i_set++
  cmp r5, r3              // if (i_set < num_set)
  ble for_each_set        //   goto for_each_set

  add r4, r4, #1          // r4++ ; i_way++
  cmp r4, r3              // if (i_way < num_way)
  ble for_each_way        //   goto for_each_way
```

