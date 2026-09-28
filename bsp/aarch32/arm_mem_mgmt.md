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
When CPU wants to access memory with a virtual address, the MMU will look into the L1 Translation Table to translate virtual to physical address. The L1 Translation table is an array of words(u32) with length of **4096**, with each entry holds either pointer to the base address of L2 Translation table or the mapped physical address and protection/memory attributes for translating and accessing a **1MiB** section.

```text
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

``` text 
Associative cache address format:
 +----------------+--------+---------+----------+
 |     Tag        |   Set  |   Word  |   Byte   |
 +----------------+--------+---------+----------+
31              13 12     5 4       2 1         0

```

Virtual Index Physical Tag, means that Virtual address is used to determine the cache line index but the physical address is used for tag. This resolves the issue of cache need for invalidation when MMU virtual to physical mapping changes. However this introduces new problem, since the associative cache uses bit [12:5] to determine the `set`, virtual address uses bit [12] for address translation (64KiB page size). This potentially creates a problem, two or more virtual address with different bit [13:12] could also point to the same physical address. This creates two or more cache entries that points to the same physical address, which means memory can already exist in the cache line with different virtual cache line index.
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
The solution to this is to use [page colouring](https://developer.arm.com/community/arm-community-blogs/b/architectures-and-processors-blog/posts/page-colouring-on-armv6-and-a-bit-on-armv7). Page colouring uses `bits[13:12]` to indicate a colour. A page allocator, when mapping virtual address to a physical address, will impose restriction, to ensure that a physical address will only be mapped to a single colour.

The solution to this is to use physical address both as cache line index and cache tag. This ensures that there would be no duplicate physical address tag in cache. All ARM cortex uses `PIPT` scheme for data cache and `VIPT` for instruction cache.
