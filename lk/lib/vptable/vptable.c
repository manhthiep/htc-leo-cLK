/*

 Simple routines for reading and writing partition table
 Copyright (c) 2011 - Danijel Posilovic - dan1j3l

*/

#include <debug.h>
#include <arch/arm.h>
#include <dev/udc.h>
#include <string.h>
#include <kernel/thread.h>
#include <arch/ops.h>

#include <dev/flash.h>
#include <lib/ptable.h>
#include <dev/keys.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lib/vptable.h>

static const int MISC_PAGES = 3;
static const int MISC_COMMAND_PAGE = 0;
static char buf[4096];


extern struct ptable *flash_get_vptable(void);

int read_vptable(struct vpartitions *out){

	struct ptentry *ptn;
	struct ptable *ptable;
	unsigned offset = 0;
	unsigned pagesize = flash_page_size();

	ptable = flash_get_vptable();

	if (ptable == NULL) {
		dprintf(CRITICAL, "ERROR: VPTABLE not found!!!\n");
		return -1;
	}
	ptn = ptable_find(ptable, "vptable");

	if (ptn == NULL) {
		dprintf(CRITICAL, "ERROR: No vptable partition!!!\n");
		return -1;
	}

	offset += (pagesize * MISC_COMMAND_PAGE);
	if (flash_read(ptn, offset, buf, pagesize)) {
		//dprintf(CRITICAL, "ERROR: Cannot read vpt header\n");
		//return -1;
	}

	memcpy(out, buf, sizeof(*out));
	return 0;
}


int write_vptable(const struct vpartitions *in){

	struct ptentry *ptn;
	struct ptable *ptable;
	unsigned offset = 0;
	unsigned pagesize = flash_page_size();
	unsigned n = 0;

	ptable = flash_get_vptable();

	if (ptable == NULL) {
		printf(CRITICAL, "ERROR: VPTABLE table not found!!!\n");
		return -1;
	}
	ptn = ptable_find(ptable, "vptable");

	if (ptn == NULL) {
		printf(CRITICAL, "ERROR: No vptable partition!!!\n");
		return -1;
	}

	n = pagesize * (MISC_COMMAND_PAGE + 1);

	//if (flash_read(ptn, offset, SCRATCH_ADDR, n)) {
	//	printf(CRITICAL, "ERROR: Cannot read vpt header\n");
	//	return -1;
	//}

	offset += (pagesize * MISC_COMMAND_PAGE);
	offset += SCRATCH_ADDR;
	memcpy((void *)offset, in, sizeof(*in));
	if (flash_write(ptn, 0, (void *)SCRATCH_ADDR, n)) {
		printf(CRITICAL, "ERROR: flash write fail!\n");
		return -1;
	}
	return 0;
}


unsigned vpart_available_size(){
	unsigned used =0;

	for (unsigned i=0;;i++){
		if ((strlen(vparts.pdef[i].name) != 0) && (vparts.pdef[i].asize == 0)){
			used += vparts.pdef[i].size;
		} else {
			break;
		}
	}

	return (flash_size_blk - used);
}


bool vpart_variable_exist(){

	for (unsigned i=0;i < (unsigned)MAX_NUM_PART;i++){
		if ((strlen(vparts.pdef[i].name) != 0) &&  (vparts.pdef[i].asize == 1)) return 1;
	}
	return 0;
}


bool vpart_partition_exist(const char* pName){

	for (unsigned i=0;i < (unsigned)MAX_NUM_PART;i++){
		if (!memcmp(vparts.pdef[i].name,pName,strlen(pName)))return 1;
	}
	return 0;
}


void vpart_resize_asize(){

	for (unsigned i=0; i < (unsigned)MAX_NUM_PART;i++){
		if ((strlen(vparts.pdef[i].name) != 0) && (vparts.pdef[i].asize == 1)){
			vparts.pdef[i].size = vpart_available_size();
			return;
		}
	}
}


extern int atoi ( const char * str );
void vpart_add(const char *pData){

	char buff[64];
	char name[32];
	char* tmp_buff;
	unsigned size;

	strcpy(buff,pData);

	tmp_buff = strtok(buff,":");
	strcpy(name,tmp_buff);
	tmp_buff=strtok(NULL,":");
	size = atoi(tmp_buff)*blk_pmb;


	if (strlen(name)==0){
		//fastboot_fail("Undefined partition name ! usage: fastboot oem part-add name:size");
		return;
	}

	if (size==0 && vpart_available_size()<8){
		//fastboot_fail("No available space for variable partition!");
		return;
	}

	if (size==0 && vpart_variable_exist()){
		//fastboot_fail("Autosize partition already exist!");
		return;
	}


	if (vpart_partition_exist(name)){
		//fastboot_fail("Named partition already exist!");
		return;
	}

	for (unsigned i=0; i< (unsigned)MAX_NUM_PART; i++){

		if (strlen(vparts.pdef[i].name) == 0){
			strcpy(vparts.pdef[i].name,name);

			if (size==0){
				vparts.pdef[i].size=vpart_available_size();
				vparts.pdef[i].asize=1;
			}else{
				vparts.pdef[i].size = size;
				vparts.pdef[i].asize=0;
			}
			vpart_resize_asize();
			break;
		}
	}
}



extern int atoi ( const char * str );
void vpart_resize(const char *pData){

	char buff[64];
	char name[32];
	char* tmp_buff;
	unsigned size;
	unsigned old_size=0;
	unsigned tmp_available_size=0;

	strcpy(buff,pData);

	tmp_buff = strtok(buff,":");
	strcpy(name,tmp_buff);
	tmp_buff=strtok(NULL,":");
	size = atoi(tmp_buff)*blk_pmb;

	if (strlen(name)==0){
		//fastboot_fail("Undefined partition size!");
		return;
	}

	
	for (unsigned i=0; i < (unsigned)MAX_NUM_PART;i++){
		if ((strlen(vparts.pdef[i].name) != 0) && (!memcmp(vparts.pdef[i].name,name,strlen(name))) && (vparts.pdef[i].asize == 0) ){
			old_size = vparts.pdef[i].size;
		}
	}

	tmp_available_size = vpart_available_size() + old_size;

	if (size>tmp_available_size)
			size=tmp_available_size;

	if (size==0 && tmp_available_size<8){
		//fastboot_fail("No more space for variable partition!");
		return;
	}

	if (size==0 && vpart_variable_exist()){
		//fastboot_fail("Unable to set to autosize, autosize partition already exist!");
		return;
	}

	for (unsigned i=0;i<(unsigned)MAX_NUM_PART;i++){

		if (!memcmp(vparts.pdef[i].name,name,strlen(name))){

			if (size==0){
				vparts.pdef[i].size=tmp_available_size;
				vparts.pdef[i].asize=1;
			}else{
				vparts.pdef[i].size = size;
				vparts.pdef[i].asize=0;
			}
			vpart_resize_asize();
			break;
		}
	}
}


// Restruct ptable and remove empty space between partitions
void vpart_restruct(){

	for (unsigned i=0;i < (unsigned)MAX_NUM_PART;i++){

		if (strlen(vparts.pdef[i].name)==0){

			for (unsigned j=i; j < (unsigned)MAX_NUM_PART;j++){

				if (strlen(vparts.pdef[j].name) != 0){

					// Copy values to empty space
					strcpy(vparts.pdef[i].name,vparts.pdef[j].name);
					vparts.pdef[i].size=vparts.pdef[j].size;
					vparts.pdef[i].asize=vparts.pdef[j].asize;

					// Mark old values dirty
					strcpy(vparts.pdef[j].name,"");
					vparts.pdef[j].size=0;
					vparts.pdef[j].asize=0;

					break;
				}
			}
		}
	}
}


// Remove partition from ptable
void vpart_del(const char *pName){

	for (unsigned i=0;i < (unsigned)MAX_NUM_PART;i++){

		if (!memcmp(vparts.pdef[i].name,pName,strlen(pName))){
			strcpy(vparts.pdef[i].name,"");
			vparts.pdef[i].size=0;
			vparts.pdef[i].asize=0;

			vpart_restruct();
			vpart_resize_asize();
		}
	}
}


// clear curent layout
void vpart_clear(){

	strcpy(vparts.tag,"");
	for (unsigned i=0;i < (unsigned)MAX_NUM_PART;i++){

			strcpy(vparts.pdef[i].name,"");
			vparts.pdef[i].size=0;
			vparts.pdef[i].asize=0;
	}
}


// Print current ptable
void vpart_list(){

	printf("\n\n============================== PARTITION TABLE ==============================\n\n");

	for (unsigned i=0;;i++){

		if (strlen(vparts.pdef[i].name) != 0){

			printf("%i.",i+1);

			//fbcon_move_right(4);
			printf("%s", vparts.pdef[i].name);

			//fbcon_move_right(25);
			printf( "|   blocks: %i",vparts.pdef[i].size);

			//fbcon_move_right(50);
			printf( "|   size: %i Mb\n",(vparts.pdef[i].size/blk_pmb));

		} else {
			break;
		}
	}

	printf("\n\n============================== PARTITION TABLE ==============================\n\n");
}


// Create default partition layout
void vpart_create_default(){

	vpart_clear();
	vpart_add("misc:1");
	vpart_add("recovery:5");
	vpart_add("boot:5");
	vpart_add("system:150");
	vpart_add("cache:5");
	vpart_add("userdata:0");

}

void vpart_commit(){
	strcpy(vparts.tag,"VPTABLE");
	write_vptable(&vparts);
}

void vpart_read(){
	vpart_clear();
	read_vptable(&vparts);
}

void vpart_enable_extrom(){
	vparts.extrom_enabled = 1;
	vpart_commit();
	//cprint(cRed,cWhite,"ExtROM Enabled ! \n Reboot your phone and repartition nand to use ExtROM !!!\n\n");
}

void vpart_disable_extrom(){
	vparts.extrom_enabled = 0;
	vpart_commit();
	//cprint(cRed,cWhite,"ExtROM Disabled ! \n Reboot your phone and repartition nand to remove ExtROM !!!\n\n");
}

// Init and load ptable from nand
void init_vpart(){

	vpart_read();

	if (memcmp(vparts.tag,"VPTABLE",7)){ // Bad mojo ! ptable does not exist, lets create default one !!!
		vparts.extrom_enabled = 0;
		flash_size_blk = flash_size_blk - 191; // extrom size
		vpart_create_default();
		vpart_commit();
	}

	if (vparts.extrom_enabled==0)
		flash_size_blk = flash_size_blk - 191; // extrom size

}

