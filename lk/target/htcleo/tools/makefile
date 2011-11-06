#Makefile to generate tinboot

SRC_DIR  := target/$(TARGET)/tools
EXTRA_CLEANDEPS += tinboot_clean

APPSBOOTHEADER: $(BUILDDIR)/tinboot

$(BUILDDIR)/tinboot.o: $(SRC_DIR)/tinboot.S $(OUTBIN)
	$(TOOLCHAIN_PREFIX)as $(SRC_DIR)/tinboot.S -I "$(BUILDDIR)" -o $(BUILDDIR)/tinboot.o

$(BUILDDIR)/tinboot: $(BUILDDIR)/tinboot.o
	$(OBJCOPY) $(BUILDDIR)/tinboot.o -O binary $(BUILDDIR)/tinboot


	
tinboot_clean: 
	rm -f $(BUILDDIR)/tinboot.o $(BUILDDIR)/tinboot