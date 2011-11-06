#make ARCH=arm CROSS_COMPILE=arm-none-eabi-
mkbootimg --kernel zImage --ramdisk initrd.gz --cmdline "console=null" --base 0x11800000 -o android_boot.img
