#compile kernel and make boot.img/recovery.img
#make ARCH=arm CROSS_COMPILE=arm-none-eabi-
mkbootimg --kernel zImage --ramdisk initrd.gz --cmdline "" --base 0x11800000 -o android_boot.img
mkbootimg --cmdline "" --base 0x11800000 --kernel zImage --ramdisk initrd.gz -o boot.img
mkbootimg --base 0x11800000 --cmdline "no_console_suspend=0" --kernel zImage --ramdisk initrd.gz -o android_boot.img

#create initrd.gz
find . | cpio -o -H newc | gzip > ../initrd.gz

#flash boot img from zImage and initrd.gz
fastboot -c "" -b 0x11800000 flash:raw boot zImage initrd.gz