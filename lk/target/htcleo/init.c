/*
 * author: cedesmith
 * license: GPL
 * Added inhouse partitioning code: dan1j3l
 * Optimized by zeus; inspired, consulted by cotulla, my bitch :D
 */


#include <string.h>
#include <debug.h>
#include <dev/keys.h>
#include <dev/gpio_keypad.h>
#include <lib/ptable.h>
#include <lib/vptable.h>
#include <dev/fbcon.h>
#include <dev/flash.h>
#include <smem.h>
#include <platform/iomap.h>
#include <reg.h>

#define LINUX_MACHTYPE  2524
#define HTCLEO_FLASH_OFFSET	0x219

#define VPART_CFG_OFFSET 0x216
#define VPART_CFG_SIZE 2

static struct ptable flash_ptable;
static struct ptable flash_vptable;

extern unsigned load_address;
extern unsigned boot_into_recovery;

void keypad_init(void);
void display_init(void);
void htcleo_ptable_dump(struct ptable *ptable);
void cmd_dmesg(const char *arg, void *data, unsigned sz);
void reboot(unsigned reboot_reason);
void target_display_init();
void cmd_oem_register();
void shutdown_device(void);
void dispmid(const char *fmt, int sel);
void flash_set_vptable(struct ptable * new_ptable);

unsigned get_boot_reason(void);
unsigned boot_reason = 0xFFFFFFFF;
unsigned android_reboot_reason = 0;


void target_init(void)
{
	struct flash_info *flash_info;
	unsigned start_block;

	unsigned blocks_per_plen = 1;
	unsigned nand_num_blocks;

	unsigned extrom_offset;
	unsigned extrom_size = 191;
	
	keys_init();
	keypad_init();

	if(get_boot_reason()==2) 
	{
		boot_into_recovery = 1;
	}

	start_block = HTCLEO_FLASH_OFFSET;

	flash_init();
	flash_info = flash_get_info();

	ASSERT(flash_info);
	ASSERT(flash_info->num_blocks);

	nand_num_blocks = flash_info->num_blocks;
	blocks_per_plen = (1024*1024)/flash_info->block_size;
	blk_pmb = blocks_per_plen;

	extrom_offset = nand_num_blocks - extrom_size;

	flash_size_blk =(nand_num_blocks - start_block);

	ptable_init(&flash_vptable);
	ptable_add(&flash_vptable, "vptable",  VPART_CFG_OFFSET, VPART_CFG_SIZE , 0, TYPE_APPS_PARTITION, PERM_WRITEABLE);

	ptable_add(&flash_vptable, "task29", start_block, flash_size_blk , 0, TYPE_APPS_PARTITION, PERM_WRITEABLE);
	//zeus: rick had set this to offset of 5mb, maybe he wanted to preserve misc and recovery after format all ? 
	//This lead lk trying to format NAND_LAST_PAGE + 5mb range.
	
	ptable_add(&flash_vptable, "misc", start_block,(int)(1 * blocks_per_plen), 0, TYPE_APPS_PARTITION, PERM_WRITEABLE);
	flash_set_vptable(&flash_vptable);

	init_vpart();

	ptable_init(&flash_ptable);

	unsigned sblock;
	sblock = start_block;

	for (unsigned i = 0; ; i++){ //zeus: just break if partition name is not set, why waste cycle till max possible partition ?
		if (strlen(vparts.pdef[i].name) > 0){
			ptable_add(&flash_ptable,vparts.pdef[i].name,sblock,vparts.pdef[i].size,0,TYPE_APPS_PARTITION,PERM_WRITEABLE);
			sblock +=vparts.pdef[i].size;
		} else { break; } 
	}
	
	flash_set_ptable(&flash_ptable);
}

struct fbcon_config* fbcon_display(void);

int htcleo_fastboot_init()
{
	cmd_oem_register();
	return 1;
}
void target_early_init(void)
{
	//cedesmith: write reset vector while we can as MPU kicks in after flash_init();
	writel(0xe3a00546, 0); //mov r0, #0x11800000
	writel(0xe590f004, 4); //ldr	r15, [r0, #4]
}
unsigned board_machtype(void)
{
    return LINUX_MACHTYPE;
}

void reboot_device(unsigned reboot_reason)
{
	fill_screen(0x0000);
	writel(reboot_reason, 0x2FFB0000);
	writel(reboot_reason^0x004b4c63, 0x2FFB0004); //XOR with cLK signature
    reboot(reboot_reason);
}

unsigned get_boot_reason(void)
{
	if(boot_reason==0xFFFFFFFF)
	{
		boot_reason = readl(MSM_SHARED_BASE+0xef244);
		//dprintf(INFO, "boot reason %x\n", boot_reason);
		if(boot_reason!=2)
		{
			if(readl(0x2FFB0000)==(readl(0x2FFB0004)^0x004b4c63))
			{
				android_reboot_reason = readl(0x2FFB0000);
				//dprintf(INFO, "android reboot reason %x\n", android_reboot_reason);
				writel(0, 0x2FFB0000);
			}
		}
	}
	return boot_reason;
}

unsigned check_reboot_mode(void)
{
	get_boot_reason();
	return android_reboot_reason;
}

unsigned target_pause_for_battery_charge(void)
{
    if (get_boot_reason() == 2) return 1;
    return 0;
}

int target_is_emmc_boot(void)
{
	return 0;
}

void htcleo_ptable_dump(struct ptable *ptable)
{
	ptable_dump(ptable);
}

//cedesmith: current version of qsd8k platform missing display_shutdown so add it
void lcdc_shutdown(void);
void display_shutdown(void)
{
    lcdc_shutdown();
}
