# This file is part of the CircuitPython project: https://circuitpython.org
#
# SPDX-FileCopyrightText: Copyright 2019 Sony Semiconductor Solutions Corporation
#
# SPDX-License-Identifier: MIT

include ../../py/circuitpy_mkenv.mk

CROSS_COMPILE = arm-none-eabi-

SPRESENSE_SDK = spresense-exported-sdk

FIRMWARE = $(SPRESENSE_SDK)/firmware

BOOTLOADER_FILES += \
	$(FIRMWARE)/AESM.espk \
	$(FIRMWARE)/dnnrt-mp.espk \
	$(FIRMWARE)/gnssfw.espk \
	$(FIRMWARE)/loader.espk \
	$(FIRMWARE)/sysutil.spk \

# Platforms are: Linux, Darwin, MSYS, CYGWIN
PLATFORM := $(firstword $(subst _, ,$(shell uname -s 2>/dev/null)))

ifeq ($(PLATFORM),Darwin)
  # macOS
  MKSPK = mkspk/mkspk
else ifeq ($(PLATFORM),Linux)
  # Linux
  MKSPK = mkspk/mkspk
else
  # Cygwin/MSYS2
  MKSPK = mkspk/mkspk.exe
endif

SERIAL ?= /dev/ttyUSB0

INC += \
	-I. \
	-I../.. \
	-I../lib/mp-readline \
	-I../shared/timeutils \
	-I../../lib/tinyusb/src \
	-I../../supervisor/shared/usb \
	-Iboards/$(BOARD) \
	-I$(BUILD) \
	-I$(SPRESENSE_SDK)/nuttx/include \
	-I$(SPRESENSE_SDK)/nuttx/arch \
	-I$(SPRESENSE_SDK)/nuttx/arch/chip \
	-I$(SPRESENSE_SDK)/nuttx/arch/os \
	-I$(SPRESENSE_SDK)/sdk/include \

CFLAGS += \
	$(INC) \
	-DCONFIG_WCHAR_BUILTIN \
	-DCONFIG_HAVE_DOUBLE \
	-DNDEBUG \
	-Dmain=spresense_main \
	-D_estack=__stack \
	-DCIRCUITPY_BOARD_ID="\"$(BOARD)\"" \
	-c \
	-pipe \
	-std=gnu11 \
	-mcpu=cortex-m4 \
	-mthumb \
	-mfpu=fpv4-sp-d16 \
	-mfloat-abi=hard \
	-mabi=aapcs \
	-fno-builtin \
	-fno-strict-aliasing \
	-fno-strength-reduce \
	-fomit-frame-pointer \
	-ffunction-sections \
	-fdata-sections \
	-Wall \

OPTIMIZATION_FLAGS ?= -O2 -fno-inline-functions

# option to override compiler optimization level, set in boards/$(BOARD)/mpconfigboard.mk
CFLAGS += $(OPTIMIZATION_FLAGS)

LIBM = "${shell "$(CC)" $(CFLAGS) -print-file-name=libm.a}"

LIBGCC = "${shell "$(CC)" $(CFLAGS) -print-libgcc-file-name}"

LDFLAGS = \
	--entry=__start \
	-T$(SPRESENSE_SDK)/nuttx/scripts/ramconfig.ld \
	--gc-sections \
	-Map=$(BUILD)/output.map \
	-o $(BUILD)/firmware.elf \
	--start-group \
	-u spresense_main \
	-u board_timerhook \
	$(BUILD)/libmpy.a \
	$(SPRESENSE_SDK)/nuttx/libs/libapps.a \
	$(SPRESENSE_SDK)/nuttx/libs/libarch.a \
	$(SPRESENSE_SDK)/nuttx/libs/libbinfmt.a \
	$(SPRESENSE_SDK)/nuttx/libs/libboard.a \
	$(SPRESENSE_SDK)/nuttx/libs/libboards.a \
	$(SPRESENSE_SDK)/nuttx/libs/libc.a \
	$(SPRESENSE_SDK)/nuttx/libs/libdrivers.a \
	$(SPRESENSE_SDK)/nuttx/libs/libfs.a \
	$(SPRESENSE_SDK)/nuttx/libs/libmm.a \
	$(SPRESENSE_SDK)/nuttx/libs/libsched.a \
	$(LIBM) \
	$(LIBGCC) \
	--end-group \
	-L$(BUILD) \

CFLAGS += -DCFG_TUSB_MCU=OPT_MCU_CXD56 -DCFG_TUD_MIDI_RX_BUFSIZE=512 -DCFG_TUD_CDC_RX_BUFSIZE=1024 -DCFG_TUD_MIDI_TX_BUFSIZE=512 -DCFG_TUD_CDC_TX_BUFSIZE=1024 -DCFG_TUD_MSC_BUFSIZE=512 $(CFLAGS_MOD)

SRC_S_UPPER = supervisor/shared/cpu_regs.S

SRC_C += \
	background.c \
	mphalport.c \
	boards/$(BOARD)/board.c \
	boards/$(BOARD)/pins.c \
	lib/tinyusb/src/portable/sony/cxd56/dcd_cxd56.c \

OBJ = $(PY_O) $(SUPERVISOR_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_S_UPPER:.S=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_COMMON_HAL_SHARED_MODULE_EXPANDED:.c=.o))
ifeq ($(INTERNAL_LIBM),1)
OBJ += $(addprefix $(BUILD)/, $(SRC_LIBM:.c=.o))
endif
OBJ += $(addprefix $(BUILD)/, $(SRC_CIRCUITPY_COMMON:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_MOD:.c=.o))

# List of sources for qstr extraction
SRC_QSTR += $(SRC_C) $(SRC_SUPERVISOR) $(SRC_COMMON_HAL_SHARED_MODULE_EXPANDED) $(SRC_CIRCUITPY_COMMON)
# Sources that only hold QSTRs after pre-processing.
SRC_QSTR_PREPROCESSOR +=

all: $(BUILD)/firmware.spk

$(FIRMWARE):
	$(ECHO) ""
	$(ECHO) "Download the spresense binaries zip archive from:"
	$(ECHO) "https://developer.sony.com/file/download/download-spresense-firmware-v2-0-000"
	$(ECHO) "Extract spresense binaries to $(FIRMWARE)"
	$(ECHO) ""
	$(ECHO) "run make flash-bootloader again to flash bootloader."
	exit 1

$(BUILD)/libmpy.a: $(SPRESENSE_SDK) $(OBJ)
	$(ECHO) "AR $@"
	$(Q)$(AR) rcs $(BUILD)/libmpy.a $(OBJ)

$(BUILD)/firmware.elf: $(BUILD)/libmpy.a
	$(ECHO) "LD $@"
	$(Q)$(LD) $(LDFLAGS)

$(MKSPK):
	$(MAKE) -C mkspk

ifeq ($(VALID_BOARD),)
$(BUILD)/firmware.spk: invalid-board
else
$(BUILD)/firmware.spk: $(BUILD)/firmware.elf $(MKSPK)
	$(ECHO) "Creating $@"
	$(MKSPK) -c 2 $(BUILD)/firmware.elf nuttx $(BUILD)/firmware.spk
endif

flash: $(BUILD)/firmware.spk
	$(ECHO) "Writing $< to the board"
	tools/flash_writer.py -s -c $(SERIAL) -d -b 115200 -n $(BUILD)/firmware.spk

flash-bootloader: $(SPRESENSE_SDK) $(FIRMWARE)
	$(ECHO) "Writing loader to the board"
	tools/flash_writer.py -s -c $(SERIAL) -d -b 115200 -n $(BOOTLOADER_FILES)

include $(TOP)/py/mkrules.mk
