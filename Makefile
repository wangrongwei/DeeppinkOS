#####################################################################
# 目前kernel.bin的加载地址是0x8000，其大小不能越过BIOS的地址0x9fc00
# 当前kernel.bin的大小不能超过621568byte
#
#
# author : wangrongwei
# time : 2018/08/12
# usage : 	make
#			make qemu
#			make bochs
#			make dis
#####################################################################


BOOT:=boot/boot.asm
BOOT_O:=boot/boot.o
E820_C:=boot/e820.c
E820_S:=boot/e820.s
E820_O:=boot/e820.o
KERNEL:=init/kernel.asm
BOOT_BIN:=$(subst .asm,.bin,$(BOOT))

KERNEL_ELF:=$(subst .asm,.elf,$(KERNEL))
KERNEL_BIN:=$(subst .asm,.bin,$(KERNEL))

# 寻找当前目录下.c文件
C_SOURCES = $(shell find . -path "./.sicode*" -prune -o -path "./boot/*" -prune -o -name "*.c" -print)
C_OBJECTS = $(patsubst %.c,%.o,$(C_SOURCES))

# 寻找init下的汇编文件（待优化）
S_SOURCES = $(shell find ./init -name "*.asm")
S_OBJECTS = $(patsubst %.asm,%.o,$(S_SOURCES))


CC = gcc
AS = as
ASM = nasm
LD = ld
OBJCOPY = objcopy

C_FLAGS   = -c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-builtin -std=c99\
-fno-stack-protector -I include -I drivers

LD_FLAGS  = -T scripts/kernel.ld -m elf_i386
ASM_FLAGS = -f elf32 -g -F stabs

IMG:=deeppink.img

# 直接将内核代码写进deeppink
deeppink.img : $(BOOT_BIN) $(KERNEL_BIN)
	dd if=/dev/zero of=$(IMG) bs=512 count=2880
	dd if=$(BOOT_BIN) of=$(IMG) conv=notrunc
	dd if=$(KERNEL_BIN) of=$(IMG) seek=1 conv=notrunc

$(BOOT_BIN) : $(BOOT_O) $(E820_O)
	$(LD) -Ttext 0x7c00 -m elf_i386 $^ -o $@

$(BOOT_O) : $(BOOT)
	$(ASM) -f elf $< -o $@
$(E820_O) : $(E820_S)
	$(AS) --32 -march=i386 $< -o $@
$(E820_S) : $(E820_C)
	$(CC) -m16 -masm=intel -I include $< -S -o $@

$(KERNEL_BIN) : $(S_OBJECTS) $(C_OBJECTS)
	@echo 链接生成kernel.bin文件
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o $(KERNEL_ELF)
	$(OBJCOPY) -O binary -R .note -R .comment -S $(KERNEL_ELF) $(KERNEL_BIN)


# 将所有.c源文件生成.o目标文件
.c.o:
	@echo $(C_SOURCES)
	$(CC) $(C_FLAGS) $< -o $@

# 如果在init下不止一个.asm文件，此处要更改（需要测试）
$(S_OBJECTS):$(S_SOURCES)
	@echo 将.c文件编译为.o文件
	@echo $(S_SOURCES)
	$(ASM) $(ASM_FLAGS) $< -o $@


.PHONY: oldqemu qemu clean bochs debug dis
oldqemu:
	@echo '启动虚拟机...'
	qemu-system-i386 -boot order=a -drive file=deeppink.img,format=raw

qemu:
	@echo '启动虚拟机...'
	qemu-system-i386	\
	-accel tcg,thread=single		\
	-cpu core2duo					\
	-m 64							\
	-boot order=a -fda deeppink.img	\
	-serial stdio					\
	-smp 1							\
	-usb							\
	-vga std

clean :
	rm -f $(BOOT_BIN) $(KERNEL_BIN) $(S_OBJECTS) $(C_OBJECTS)
	rm ./boot/boot.txt ./init/kernel.txt

debug:
	qemu-system-i386 -s -S deeppink.img

	#qemu-system-i386 -s -S -boot order=a -fda deeppink.img

bochs:
	bochs -f ./bochsrc

dis:
	ndisasm ./boot/boot.bin > ./boot/boot.txt
	objdump -d ./init/kernel.elf > ./init/kernel.txt




