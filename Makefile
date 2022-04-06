# Don't us normal gcc, use the arm cross compiler
CC = ./build/gcc-arm/bin/aarch64-none-elf-gcc
LD  = ./build/gcc-arm/bin/aarch64-none-elf-ld
OBJCPY  = ./build/gcc-arm/bin/aarch64-none-elf-objcopy
ASM = ./build/gcc-arm/bin/aarch64-none-elf-gcc

CFLAGS = -g3 -mcpu=cortex-a72 -fpic -ffreestanding
AFLAGS = -D_ASM_
# Uncomment if debugging with QEMU
#CFLAGS += -DQEMU_DEBUG
# Uncomment for boot code debugging
#AFLAGS+= -DDEBUG_STARTUP

CSRCFLAGS= -O0 -Wall -Wextra -std=c99 -Werror-implicit-function-declaration

all: bootloader kernel
.PHONY: all

# COMMON
COMMON_OBJ_DIR = ./build/objects_common
COMMON_SRC = ./src/common
COMMON_INCLUDE = ./include/common
COMMON_SOURCES = $(wildcard $(COMMON_SRC)/*.c)
COMMON_ASMSOURCES = $(wildcard $(COMMON_SRC)/*.S)

$(COMMON_OBJ_DIR)/%.o: $(COMMON_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(COMMON_INCLUDE) -c $< -o $@ $(CSRCFLAGS)

$(COMMON_OBJ_DIR)/%.o: $(COMMON_SRC)/%.S
	mkdir -p $(@D)
	$(ASM) -c $< -o $@

# LINKER FILE PREPROCESSING
./build/linker/%_gen.ld: ./build/linker/%.ld $(COMMON_INCLUDE)/memory_layout.h
	@echo "Generating linker command file"
	echo "/* Generated file, don't modify it! */" > $@
	$(CC) -E -x c $< -I$(COMMON_INCLUDE) $(CFLAGS)| grep -v '^#' >>$@

# BOOTLOADER
BOOTLDR_OBJ_DIR = ./build/objects_bootloader
BOOTLOADER_SRC = ./src/bootloader
BOOTLOADER_INCLUDE = ./include/bootloader
BOOTLOADER_SOURCES = $(wildcard $(BOOTLOADER_SRC)/*.c)
BOOTLOADER_ASMSOURCES = $(wildcard $(BOOTLOADER_SRC)/*.S)

BOOTLOADER_OBJECTS = $(patsubst $(BOOTLOADER_SRC)/%.c, $(BOOTLDR_OBJ_DIR)/%.o, $(BOOTLOADER_SOURCES))
BOOTLOADER_OBJECTS += $(patsubst $(COMMON_SRC)/%.c, $(COMMON_OBJ_DIR)/%.o, $(COMMON_SOURCES))
BOOTLOADER_OBJECTS += $(patsubst $(COMMON_SRC)/%.S, $(COMMON_OBJ_DIR)/%.o, $(COMMON_ASMSOURCES))
BOOTLOADER_OBJECTS += $(patsubst $(BOOTLOADER_SRC)/%.S, $(BOOTLDR_OBJ_DIR)/%.o, $(BOOTLOADER_ASMSOURCES))

BOOTLOADER_NAME=bootloader

bootloader: $(BOOTLOADER_OBJECTS) ./build/linker/bootloader_gen.ld
	echo $(BOOTLOADER_OBJECTS)
	$(LD) -nostdlib $(BOOTLOADER_OBJECTS) -T ./build/linker/bootloader_gen.ld -o ./build/$(BOOTLOADER_NAME).elf -Map=./build/$(BOOTLOADER_NAME).map
	$(OBJCPY) -O binary ./build/$(BOOTLOADER_NAME).elf ./build/$(BOOTLOADER_NAME).img 

$(BOOTLDR_OBJ_DIR)/%.o: $(BOOTLOADER_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(COMMON_INCLUDE) -I$(BOOTLOADER_INCLUDE) -c $< -o $@ $(CSRCFLAGS)

$(BOOTLDR_OBJ_DIR)/%.o: $(BOOTLOADER_SRC)/%.S
	mkdir -p $(@D)
	$(ASM) -I$(BOOTLOADER_SRC) -c $< -o $@

# KERNEL
KERNEL_OBJ_DIR = ./build/objects_kernel
KERNEL_SRC = ./src/kernel
KERNEL_INCLUDE = ./include/kernel
KERNEL_SOURCES = $(wildcard $(KERNEL_SRC)/*.c)
KERNEL_ASMSOURCES = $(wildcard $(KERNEL_SRC)/*.S)

KERNEL_OBJECTS = $(patsubst $(KERNEL_SRC)/%.c, $(KERNEL_OBJ_DIR)/%.o, $(KERNEL_SOURCES))
KERNEL_OBJECTS += $(patsubst $(COMMON_SRC)/%.c, $(COMMON_OBJ_DIR)/%.o, $(COMMON_SOURCES))
KERNEL_OBJECTS += $(patsubst $(COMMON_SRC)/%.S, $(COMMON_OBJ_DIR)/%.o, $(COMMON_ASMSOURCES))
KERNEL_OBJECTS += $(patsubst $(KERNEL_SRC)/%.S, $(KERNEL_OBJ_DIR)/%.o, $(KERNEL_ASMSOURCES))

KERNEL_NAME=kernel

kernel: $(KERNEL_OBJECTS) ./build/linker/kernel_gen.ld
		echo $(KERNEL_OBJECTS)
		$(LD) -nostdlib $(KERNEL_OBJECTS) -T ./build/linker/kernel_gen.ld -o ./build/$(KERNEL_NAME).elf -Map=./build/$(KERNEL_NAME).map
		$(OBJCPY) -O binary ./build/$(KERNEL_NAME).elf ./build/$(KERNEL_NAME).img 

$(KERNEL_OBJ_DIR)/%.o: $(KERNEL_SRC)/%.c
		mkdir -p $(@D)
		$(CC) $(CFLAGS) -I$(COMMON_INCLUDE) -I$(KERNEL_INCLUDE) -c $< -o $@ $(CSRCFLAGS)

$(KERNEL_OBJ_DIR)/%.o: $(KERNEL_SRC)/%.S
		mkdir -p $(@D)
		$(ASM) $(AFLAGS) -I$(KERNEL_SRC) -I$(KERNEL_INCLUDE) -I$(COMMON_SRC) -I$(COMMON_INCLUDE) -c $< -o $@ 

.PHONY: clean
clean:
	rm -rf $(BOOTLDR_OBJ_DIR)
	rm -rf $(COMMON_OBJ_DIR)
	rm -rf $(KERNEL_OBJ_DIR)
	rm ./build/*.img
	rm ./build/*.elf
	rm ./build/*.map
	rm ./build/./linker/*_gen.ld

deploy:
	cp ./build/$(BOOTLOADER_NAME).img /run/media/phil/boot/
	cp ./build/config.txt /run/media/phil/boot/
	cp ./tools/armstubs/armstub8.bin /run/media/phil/boot/
	cp -r /run/media/phil/boot/* ./tools/sdcard

run-debug: all
	qemu-system-aarch64 -m 4096 -M raspi4 -serial /dev/null -serial stdio -s -S -kernel ./build/$(KERNEL_NAME).img

debug:
	./build/gcc-arm/bin/aarch64-none-elf-gdb -ex 'target remote 127.0.0.1:1234' ./build/$(KERNEL_NAME).elf

run-debug-target: 
	sudo openocd -f ./tools/adafruit.cfg  -f ./tools/raspi4.cfg 

debug-target-kernel0:
	./build/gcc-arm/bin/aarch64-none-elf-gdb -ex 'target ext :3333' ./build/$(KERNEL_NAME).elf

debug-target-kernel1:
	./build/gcc-arm/bin/aarch64-none-elf-gdb -ex 'target ext :3334' ./build/$(KERNEL_NAME).elf

debug-target-kernel2:
	./build/gcc-arm/bin/aarch64-none-elf-gdb -ex 'target ext :3335' ./build/$(KERNEL_NAME).elf

debug-target-bootloader:
	./build/gcc-arm/bin/aarch64-none-elf-gdb -ex 'target ext :3334' ./build/$(BOOTLOADER_NAME).elf
reset:
	echo "ftdi_set_signal nSRST 1" | nc localhost 4444
	echo "ftdi_set_signal nSRST 0" | nc localhost 4444

