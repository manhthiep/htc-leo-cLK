#make ARCH=arm CROSS_COMPILE=arm-none-eabi-
mkbootimg --kernel zImage --ramdisk initrd.gz --cmdline "" --base 0x11800000 -o android_boot.img
mkbootimg --base 0x11800000 --cmdline "no_console_suspend=0" --kernel zImage --ramdisk initrd.gz -o android_boot.img
