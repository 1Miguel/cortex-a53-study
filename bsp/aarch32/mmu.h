/**
 * @file
 *
 * @brief
 *
 */
#ifndef __MMU__H
#define __MMU__H

#include <stdint.h>
#include <stddef.h>

/** Strongly ordered mem */
#define MMU_MEM_STRONG_ORDERED (0)
/** Shareable Device Memory */
#define MMU_MEM_DEVICE         (1)
/** Normal, cache write policy write-through, no allocated on write */
#define MMU_MEM_NORM_WT_CACHE  (1)
/** Normal, cache write policy write-back, no allocated on write */
#define MMU_MEM_NORM_WB_CACHE  (1)
/** Non-cachable memory */
#define MMU_MEM_NO_CACHE       (1)

/** Both Priv and Unpriv has no access */
#define MMU_MEM_AP_NONE    (0)
/** Priv has r/w access. */
#define MMU_MEM_AP_PRIV_RW (0x01)
/** Privilege mode as full access but User mode has read only access. */
#define MMU_MEM_AP_USER_RO (0x02)
/** Both Unprivilege and Privileged has r/w access */
#define MMU_MEM_AP_FULL    (0x03)
/** Privileged Read-only. */
#define MMU_MEM_AP_PRIV_RO ((1u << 5u) | 0x01)
/** Both Privileged and Unprivileged (user) has Read-only access. */
#define MMU_MEM_AP_RO      ((1u << 5u) | 0x02)

/** @brief execute never, the mmu prevents any speculative
 * execution fetch to take place in a memory with this attribute set.
 */
#define MMU_MEM_XN ((0x01 << 4) | 0x01)

/**
 * @brief Enable ARM mmu.
 */
void mmu_enable(void);

/**
 * @brief Disable ARM mmu.
 */
void mmu_disable(void);

/**
 * @brief Sets the L1 translation of virtual address to a
 *        physical address.
 *
 * @param v_addr virtual Address
 * @param p_addr physical Address
 * @param size   size of the memory region, must be multiple
 *               of section size
 * @param flags  memory attribute flags
 *
 */
void mmu_set_l1_map(uintptr_t v_addr, uintptr_t p_addr, size_t size, uint32_t flags);

#endif // __MMU__H
