SHARED_HAL_DIR := src/shared/HAL/Src
SHARED := src/shared

SOURCES := $(wildcard src/m4/*/*.[cs]) \
					$(SHARED_HAL_DIR)/stm32mp1xx_hal.c \
					$(SHARED_HAL_DIR)/stm32mp1xx_hal_gpio.c \
					$(SHARED_HAL_DIR)/stm32mp1xx_hal_cortex.c \
					$(SHARED_HAL_DIR)/stm32mp1xx_hal_rcc.c \
					$(SHARED_HAL_DIR)/stm32mp1xx_hal_rcc_ex.c \
					src/shared/CMSIS/Device/ST/STM32MP1xx/Source/Templates/system_stm32mp1xx.c \
					$(SHARED_HAL_DIR)/stm32mp1xx_hal_msp.c \
					$(SHARED)/printfunc/myprint.c \
					$(SHARED)/ModuleDefinitions/syscalls.c \
					$(SHARED_HAL_DIR)/stm32mp1xx_hal_sai.c \
					$(SHARED_HAL_DIR)/stm32mp1xx_hal_sai_ex.c \
					$(SHARED_HAL_DIR)/stm32mp1xx_hal_i2c.c \
					$(SHARED_HAL_DIR)/stm32mp1xx_hal_i2c_ex.c \
					$(SHARED_HAL_DIR)/stm32mp1xx_hal_dma.c \
					$(SHARED_HAL_DIR)/stm32mp1xx_hal_tim.c \
					$(SHARED_HAL_DIR)/stm32mp1xx_hal_tim_ex.c
					

INCLUDES = -I. \
				-Isrc/shared/HAL/Inc \
				-Isrc/shared/CMSIS/Core/Include \
				-Isrc/shared/CMSIS/Device/ST/STM32MP1xx/Include \
				-Isrc/shared/ModuleDefinitions \
				-Isrc/shared/printfunc \
				-Isrc/m4/Audio \
				-Isrc/m4/I2C \
				-Isrc/m4/GPIO \
				-Isrc/m4/Core \
				-Isrc/m4/Display \
				-Isrc/m4/Timer \
				-Isrc/m4/Trellis \
				-Isrc/m4/States

LINKSCR ?= linkscriptm4.ld
TOOLS ?= tools
BUILDDIR ?= $(BUILDDIR_M4)
BINDIR ?= bin/m4
BINARYNAME ?= main
UIMAGENAME ?= $(BUILDDIR)/m4-main.uimg

OBJDIR = $(BUILDDIR)/obj
LOADADDR 	= 0xDFF00000
ENTRYPOINT 	= 0xDFF00000

OBJECTS   = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(basename $(SOURCES))))
DEPS   	  = $(addprefix $(OBJDIR)/, $(addsuffix .d, $(basename $(SOURCES))))

MCU ?=  -mcpu=cortex-m7 -mthumb -mfpu=fpv4-sp-d16 -mlittle-endian -mfloat-abi=hard

ARCH_CFLAGS ?= -DUSE_FULL_LL_DRIVER \
			   -DSTM32MP157Cxx \
			   -DSTM32MP1 \
			   -DCORE_CM4 \
			   -DARM_MATH_CM4 \

OPTFLAG ?= -O0

AFLAGS = $(MCU)

CFLAGS = -g2 \
		 -fno-common \
		 $(ARCH_CFLAGS) \
		 $(MCU) \
		 $(INCLUDES) \
		 -fdata-sections -ffunction-sections \
		 -nostartfiles \
		 -ffreestanding \
		 $(EXTRACFLAGS)\

CXXFLAGS = $(CFLAGS) \
		-std=c++2a \
		-fno-rtti \
		-fno-exceptions \
		-fno-unwind-tables \
		-ffreestanding \
		-fno-threadsafe-statics \
		-Werror=return-type \
		-Wdouble-promotion \
		-Wno-register \
		-Wno-volatile \
		 $(EXTRACXXFLAGS) \

LINK_STDLIB ?= -nostdlib

LFLAGS = -Wl,--gc-sections \
		 -Wl,-Map,$(BUILDDIR)/$(BINARYNAME).map,--cref \
		 $(MCU)  \
		 -T $(LINKSCR) \
		 $(LINK_STDLIB) \
		 -nostartfiles \
		 -ffreestanding \
		 $(EXTRALDFLAGS) \

DEPFLAGS = -MMD -MP -MF $(OBJDIR)/$(basename $<).d

ARCH 	= arm-none-eabi
CC 		= $(ARCH)-gcc
CXX 	= $(ARCH)-g++
LD 		= $(ARCH)-g++
AS 		= $(ARCH)-as
OBJCPY 	= $(ARCH)-objcopy
OBJDMP 	= $(ARCH)-objdump
GDB 	= $(ARCH)-gdb
SZ 		= $(ARCH)-size

SZOPTS 	= -d

ELF 	= $(BINDIR)/$(BINARYNAME).elf
HEX 	= $(BINDIR)/$(BINARYNAME).hex
BIN 	= $(BINDIR)/$(BINARYNAME).bin

all: Makefile $(ELF)

$(OBJDIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(info Building $< at $(OPTFLAG))
	@$(AS) $(AFLAGS) $< -o $@ 

$(OBJDIR)/%.o: %.c $(OBJDIR)/%.d
	@mkdir -p $(dir $@)
	$(info Building $< at $(OPTFLAG))
	@$(CC) -c $(DEPFLAGS) $(OPTFLAG) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: %.c[cp]* $(OBJDIR)/%.d
	@mkdir -p $(dir $@)
	$(info Building $< at $(OPTFLAG))
	@$(CXX) -c $(DEPFLAGS) $(OPTFLAG) $(CXXFLAGS) $< -o $@

$(ELF): $(OBJECTS) $(LINKSCR)
	$(info Linking...)
	@$(LD) $(LFLAGS) -o $@ $(OBJECTS) 

$(BIN): $(ELF)
	$(OBJCPY) -O binary $< $@

$(HEX): $(ELF)
	@$(OBJCPY) --output-target=ihex $< $@
	@$(SZ) $(SZOPTS) $(ELF)

$(UIMAGENAME): $(BIN) $(TOOLS)/mkimage
	$(info Creating uimg file)
	@$(TOOLS)/mkimage -A arm -C none -T kernel -a $(LOADADDR) -e $(ENTRYPOINT) -d $< $@

%.d: ;

clean:
	rm -rf build

ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPS)
endif

.PRECIOUS: $(DEPS) $(OBJECTS) $(ELF)
.PHONY: all clean

.PHONY: compile_commands
compile_commands:
	compiledb make
	compdb -p ./ list > compile_commands.tmp 2>/dev/null
	rm compile_commands.json
	mv compile_commands.tmp compile_commands.json