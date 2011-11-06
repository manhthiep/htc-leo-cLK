/*
	Variable Partition table definition
	vptable is virtual configuration partition with settings and partition layout configuration
    Copyright (c) 2011 - Danijel Posilovic - dan1j3l
*/

extern int atoi ( const char * str );
extern struct ptable *flash_get_vptable(void);

struct part_def {
	char name[32]; // partition name (identifier in mtd device)
	short size; // size in blocks 1Mb = 8 Blocks
	bool asize; // autosize and use all available space 1=yes 0=no
};

struct vpartitions{
	char tag[7];
	struct part_def pdef[12];
	short extrom_enabled;
};
static const int MAX_NUM_PART =12;

struct vpartitions vparts;

unsigned blk_pmb;
unsigned flash_size_blk;
unsigned vpart_available_size();

int read_vptable(struct vpartitions *output);
int write_vptable(const struct vpartitions *input);

int vpart_variable_exist();
int vpart_partition_exist(const char* pName);
void vpart_resize_asize();
void vpart_add(const char *pData);
void vpart_restruct();
void vpart_del(const char *pName);
void vpart_list();
void vpart_clear();
void vpart_create_default();
void init_vpart();
void vpart_commit();
void vpart_read();
void vpart_enable_extrom();
void vpart_disable_extrom();
void vpart_resize(const char *pData);