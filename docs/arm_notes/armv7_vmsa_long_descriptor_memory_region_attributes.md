# ARMv7 VMSA Long-descriptor Memory Region Attributes

This table summarizes the memory-region attribute bits commonly used in ARMv7 VMSA long-descriptor translation entries (for example, the LPAE page/block descriptors used by the MMU). The main memory-type fields are `TEX[2:0]`, `C`, `B`, and `S`; `XN` and `AP` control execution and access permissions.

## Core attribute bits

| Bit(s) | Name | Meaning |
| --- | --- | --- |
| `TEX[2:0]` | Type Extension | Encodes the memory type family together with cacheability and shareability behavior. |
| `C` | Cacheable | Enables caching for normal memory. |
| `B` | Bufferable | Controls write buffering / write-back behavior for normal memory. |
| `S` | Shareable | Marks the region as shareable across cores/clusters and/or devices. |
| `AP[2:1]` | Access Permission | Controls Privileged/User read/write access. |
| `XN` | Execute Never | Prevents execution from the region. |

## Long-descriptor memory type encoding

| `TEX[2:0]` | `C` | `B` | `S` | Memory type | Typical behavior |
| --- | :---: | :---: | :---: | --- | --- |
| `000` | `0` | `0` | `0` | Strongly Ordered | Strict ordering; no cache; no speculative access; used for MMIO control registers. |
| `000` | `0` | `1` | `0` | Device, non-shareable | Peripheral memory; no cache; order preserved; not shared across cores. |
| `000` | `0` | `1` | `1` | Device, shareable | Peripheral memory that may be shared across cores/clusters. |
| `000` | `1` | `0` | `0` | Normal memory, WT, no WA | Write-through cache policy; writes propagate to memory; no write-allocate on miss. |
| `000` | `1` | `0` | `1` | Normal memory, WT, shareable | Same as WT but shareable across cores/clusters. |
| `000` | `1` | `1` | `0` | Normal memory, WB, WA | Write-back cache policy; write-allocate on miss; typical DRAM mapping. |
| `000` | `1` | `1` | `1` | Normal memory, WB, shareable | Same as WB/WA but shareable. |
| `001` | `x` | `x` | `x` | Normal memory, alternate TEX encoding | Alternative normal-memory attribute set; the exact cacheability/write policy depends on the implementation and MAIR/auxiliary settings. |
| `010`..`111` | `x` | `x` | `x` | Reserved / implementation-defined | Use only with the exact ARM architecture encoding for the target system; do not assume generic behavior. |

- `Strongly Ordered`: best for control/status registers and memory that must have strict ordering.
- `Device`: used for memory-mapped peripherals; usually non-cacheable and may be shareable for multicore access.
- `Normal`: used for RAM and application data; choose `WT` vs `WB` based on performance and coherency needs.
- `S=1`: indicates a memory region may be shared with other cores or hardware agents.
- `XN=1`: mark non-executable memory (for example, data or stack regions).

## Access permission examples (`AP`)

| `AP[2:1]` | Meaning |
| --- | --- |
| `00` | No access |
| `01` | Privileged read-only, user no access |
| `10` | Reserved / architecture-defined depending on descriptor format |
| `11` | Full read/write access (privileged + user) |

## Descriptor example

```text
0xC0E
  = S=0, TEX=000, AP=11, Domain=0, C=1, B=1
  = Normal, Write-Back, Write-Allocate memory
```

This corresponds to a typical cached RAM region for application data.

## Notes

- The exact meaning of `TEX`, `C`, and `B` depends on the descriptor type and the memory model in use.
- `Strongly Ordered` and `Device` are non-cacheable by design.
- In practice, ARM memory attributes are often summarized as `TEX/C/B/S` rather than as a standalone 1-byte attribute value.
