#!/bin/bash

BOOT_ASM="kernel/boot/boot.s"
KERNEL_C="kernel/kernel.c"
LINKER_SCRIPT="kernel/linker.ld"
KERNEL_BIN="BDoomOS.bin"
ISO_NAME="disk_images/BDoomOS.iso"
ISODIR="grub"
GRUB_CFG="$ISODIR/boot/grub/grub.cfg"
KERNEL_LOG_H="kernel/modules/kernel_log.h" 

echo "[INFO] Compiling $BOOT_ASM..."
i686-elf-as $BOOT_ASM -o kernel/boot/boot.o || { echo "Error compiling $BOOT_ASM"; exit 1; }

echo "[INFO] Compiling $KERNEL_C..."
i686-elf-gcc -c $KERNEL_C -o kernel/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra || { echo "Error compiling $KERNEL_C"; exit 1; }

echo "[INFO] Generating $LINKER_SCRIPT..."
cat <<EOF > $LINKER_SCRIPT
ENTRY(_start)

SECTIONS
{
    . = 2M;

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
}
EOF

echo "[INFO] Linking files into $KERNEL_BIN..."
i686-elf-gcc -T $LINKER_SCRIPT -o $KERNEL_BIN -ffreestanding -O2 -nostdlib kernel/boot/boot.o kernel/kernel.o -lgcc || { echo "Error linking"; exit 1; }

echo "[INFO] Verifying multiboot support..."
if grub-file --is-x86-multiboot $KERNEL_BIN; then
  echo "Multiboot confirmed"
else
  echo "The file is not multiboot"
  exit 1
fi

echo "[INFO] Preparing ISO structure..."
mkdir -p $ISODIR/boot/grub
cp $KERNEL_BIN $ISODIR/boot/
rm -rf BDoomOS.bin

cat <<EOF > $GRUB_CFG
set default=0
set timeout=10
set background_color=blue
set menu_color_normal=cyan/blue
set menu_color_highlight=yellow/blue

clear
echo " ******   *******                                   *******    ********"
echo " /*////** /**////**                                 **/////**  **////// "
echo " /*   /** /**    /**  ******   ******  **********  **     //**/**       "
echo " /******  /**    /** **////** **////**//**//**//**/**      /**/*********"
echo " /*//// **/**    /**/**   /**/**   /** /** /** /**/**      /**////////**"
echo " /*    /**/**    ** /**   /**/**   /** /** /** /**//**     **        /**"
echo " /******* /*******  //****** //******  *** /** /** //*******   ******** "
echo " ///////  ///////    //////   //////  ///  //  //   ///////   ////////   "

echo "                         Welcome to BDoomOS 0.1"
echo "                      Please wait, loading system..."

sleep 3

menuentry "BDoomOS Boot" {
    multiboot /boot/BDoomOS.bin
}

menuentry "Reboot" {
    reboot
}

menuentry "Shutdown" {
    halt
}

insmod gfxterm
insmod vbe

EOF


echo "[INFO] Generating ISO image $ISO_NAME..."
grub-mkrescue -o $ISO_NAME $ISODIR || { echo "Error creating ISO"; exit 1; }

echo "[INFO] Process completed! ISO image generated: $ISO_NAME"
