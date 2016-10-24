#ifndef __MULTIBOOT_H__
#define __MULTIBOOT_H__
struct multiboot_info {
	uint32_t flags;
	
	uint32_t mem_lower;
	uint32_t mem_upper;
	
	uint32_t boot_device;
	
	uint32_t cmdline;
	
	uint32_t mods_count;
	uint32_t mods_addr;
 
	char placeholder[16];
   
	uint32_t mmap_length;
	uint32_t mmap_addr;
	
	uint32_t drives_length;
	uint32_t drives_addr;
	
	uint32_t config_table;
	
	uint32_t boot_loader_name;
	
	uint32_t apm_table;
	
	uint32_t vbe_control_info;
	uint32_t vbe_mode_info;
	uint16_t vbe_mode;
	uint16_t vbe_interface_seg;
	uint16_t vbe_interface_off;
	uint16_t vbe_interface_len;
} __attribute__((packed));
#endif /*__MULTIBOOT_H__*/