TOOLCHAIN_PREFIX ?= toolchain/bin/arm-none-eabi-

SRC_DIR  = src
OUT_DIR  = build
BOOT_DIR = boot

C_FILES    = $(wildcard $(SRC_DIR)/*.c)
ASM_FILES  = $(wildcard $(SRC_DIR)/*.s)
KERNEL_LD  = $(SRC_DIR)/kernel.ld
# Compiling against ARMv7 32-bit, that's what Raspberry Pi 2 model B v1.1 uses
KERNEL_ELF = $(OUT_DIR)/kernel7.elf
KERNEL_BIN = $(BOOT_DIR)/kernel7.img

CFLAGS = \
	-std=c99 \
	-mcpu=cortex-a7 \
	-fpic \
	-ffreestanding \
	-O2 \
	-Wall \
	-Wextra \
	-Wpedantic \
	-Wno-char-subscripts \
	-Wno-parentheses \
	-Wimplicit-fallthrough \
	-Werror=incompatible-pointer-types \
	-Werror=enum-conversion

ASMFLAGS = \
	-mcpu=cortex-a7 \
	-fpic \
	-ffreestanding

LDFLAGS = \
	-ffreestanding \
	-O2 \
	-nostdlib \
	-lgcc

all: boot $(KERNEL_BIN)

clean:
	rm -rf $(OUT_DIR)

boot: boot/bootcode.bin

boot/bootcode.bin:
	cd $(BOOT_DIR) && make

$(OUT_DIR)/%.c.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(TOOLCHAIN_PREFIX)gcc $(CFLAGS) -MMD -c $< -o $@

$(OUT_DIR)/%.s.o: $(SRC_DIR)/%.s
	mkdir -p $(@D)
	$(TOOLCHAIN_PREFIX)gcc $(ASMFLAGS) -MMD -c $< -o $@

OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(OUT_DIR)/%.c.o)
OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.s=$(OUT_DIR)/%.s.o)

DEP_FILES = $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

$(KERNEL_ELF): $(KERNEL_LD) $(OBJ_FILES)
	$(TOOLCHAIN_PREFIX)gcc $(LDFLAGS) -T $(KERNEL_LD) -o $(KERNEL_ELF) $(OBJ_FILES)

$(KERNEL_BIN): $(KERNEL_ELF)
	$(TOOLCHAIN_PREFIX)objcopy $< -O binary $@

.clangd: Makefile
	@echo "# Run \`make .clangd\` to update" > $@
	@echo "CompileFlags:" >> $@
	@echo "  Add:" >> $@
	@for flag in -xc $(CFLAGS); do \
		if ! [[ "$$flag" =~ -mcpu* ]]; then \
		    echo "    - $$flag" >> $@; \
		fi \
	done
	@echo ".clangd file updated"
