#ifndef __MEMORY_H__
#define __MEMORY_H__

#define VIRTUAL_BASE	0xffffffff80000000
#define PAGE_SIZE	0x1000
#define KERNEL_CS	0x08
#define KERNEL_DS	0x10

#define BOOTMEM_SIZE      (4ull * 1024ull * 1024ull * 1024ull) // 4GB

#define PAGE_MASK         (PAGE_SIZE - 1)
#define KERNEL_BASE       0xffffffff80000000
#define HIGH_BASE         0xffff800000000000
#define PHYSICAL_BASE     0x0000000000000000

#define KERNEL_PHYS(x)    ((x) - KERNEL_BASE)
#define KERNEL_VIRT(x)    ((x) + KERNEL_BASE)
#define PA(x)             ((x) - HIGH_BASE)
#define VA(x)             ((x) + HIGH_BASE)

#ifndef __ASM_FILE__

#include <stdint.h>

static inline uintptr_t kernel_phys(void *addr)
{ return KERNEL_PHYS((uintptr_t)addr); }

static inline void *kernel_virt(uintptr_t addr)
{ return (void *)KERNEL_VIRT(addr); }

typedef uintptr_t phys_t;
typedef uintptr_t virt_t;

static inline phys_t pa(const void *addr)
{ return PA((virt_t)addr); }

static inline void *va(phys_t addr)
{ return (void *)VA(addr); }

#endif /*__ASM_FILE__*/

#endif /*__MEMORY_H__*/
