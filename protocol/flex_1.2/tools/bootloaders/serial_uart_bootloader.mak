# IAR 7.xx toolchain
ifdef ARM_IAR7_DIR
IAR_DIR = $(ARM_IAR7_DIR)
else
IAR_DIR = $(ARM_IAR6_DIR)
endif

ARCH = $(JAMEXE_PREFIX) $(IAR_DIR)/arm/bin/iarchive.exe
AS = $(JAMEXE_PREFIX) $(IAR_DIR)/arm/bin/iasmarm.exe
CC = $(JAMEXE_PREFIX) $(IAR_DIR)/arm/bin/iccarm.exe
ELFTOOL = $(JAMEXE_PREFIX) $(IAR_DIR)/arm/bin/ielftool.exe
LD = $(JAMEXE_PREFIX) $(IAR_DIR)/arm/bin/ilinkarm.exe

###############################################################################


GLOBAL_BASE_DIR := ../../../../platform/base
GLOBAL_EMLIB_DIR := ../../../../platform/emlib
GLOBAL_EMDRV_DIR := ../../../../platform/emdrv
GLOBAL_CMSIS_DIR := ../../../../platform/CMSIS
GLOBAL_DEVICE_DIR := ../../../../platform/Device
GLOBAL_KIT_DIR := ../../../../hardware/kit
GLOBAL_RAIL_LIB_DIR := ../../../../platform/radio/rail_lib
GLOBAL_GLIB_DIR := ../../../../platform/middleware/glib
GLOBAL_BOOTLOADER_DIR := ../../../../platform/bootloader

PLUGIN_DIR = connect/plugins
STACK_DIR  = connect/plugins/stack
BOARD_HEADER_PATH ?= $(GLOBAL_BASE_DIR)/hal/micro/cortexm3/efm32/board/
KIT_PATH ?= $(GLOBAL_KIT_DIR)/$(KIT)/config

BUILD_DIR = tools/bootloaders/build
OUTPUT_DIR = $(BUILD_DIR)/bootloader-$(BOARD)/serial-uart-bootloader

###############################################################################

CDEFS = \
      -DBOARD_HEADER=\"$(BOARD).h\" \
      -DPLATFORM_HEADER=\"$(GLOBAL_BASE_DIR)/hal/micro/cortexm3/compiler/iar-boot.h\" \
      -DHAL_MICRO \
      -DCORTEXM3 \
      -DCORTEXM3_EFR32 \
      -DPHY_NULL \
      -DBOARD_$(BOARD) \
      -DCORTEXM3_EFM32_MICRO \
      -D$(ARCHITECTURE) \
      -D$(FAMILY) \
      -DCORTEXM3_EFR32_MICRO \
      -DAESDRV_HWACCEL_DMA_ENABLED=false \
      -DEMDRV_RTCDRV_USE_LFRCO \
      -DBTL_HAS_SERIAL \
      -DBOOTLOADER \
      -DBOOTLOADER_TYPE=BL_EXT_TYPE_SERIAL_UART \
      -DEM_MSC_RUN_FROM_FLASH \
      -DEMBER_STACK_CONNECT \
      -DEFR32_SERIES1_CONFIG3_MICRO \
      -DJAMBUILD \
      -DHAL_CONFIG=\"connect/hal-config/flex-config.h\" \
      -DHAL_SERIAL_APP_PORT=1 \


ASMDEFS = \
	-DBOARD_HEADER=\"$(BOARD).h\" \
	-DPLATFORM_HEADER=\"$(GLOBAL_BASE_DIR)/hal/micro/cortexm3/compiler/iar-boot.h\" \
	-DHAL_MICRO \
	-DCORTEXM3 \
	-DCORTEXM3_EFR32 \
	-DPHY_NULL \
	-DBOARD_$(BOARD) \
	-DCORTEXM3_EFM32_MICRO \
	-D$(ARCHITECTURE) \
	-D$(FAMILY) \
	-DCORTEXM3_EFR32_MICRO \
	-DAESDRV_HWACCEL_DMA_ENABLED=false \
	-DEMDRV_RTCDRV_USE_LFRCO \
	-DBTL_HAS_SERIAL \
	-DBOOTLOADER \
	-DBOOTLOADER_TYPE=BL_EXT_TYPE_SERIAL_UART \
	-DEM_MSC_RUN_FROM_FLASH \
	-DEMBER_STACK_CONNECT \
  -DEFR32_SERIES1_CONFIG3_MICRO \
  -DJAMBUILD \
  -DHAL_CONFIG=\"connect/hal-config/flex-config.h\" \
  -DHAL_SERIAL_APP_PORT=1 \

CINC = -I. \
	-I$(BOARD_HEADER_PATH) \
	-I$(KIT_PATH) \
	-I$(STACK_DIR) \
	-I$(IAR_DIR)/ARM/INC \
	-I$(GLOBAL_CMSIS_DIR)/Include  \
	-I$(GLOBAL_BOOTLOADER_DIR)  \
	-I$(GLOBAL_BASE_DIR) \
	-I$(GLOBAL_BASE_DIR)/hal \
	-I$(GLOBAL_BASE_DIR)/hal/micro/cortexm3/efm32/config \
	-I$(GLOBAL_EMDRV_DIR)/common/inc \
	-I$(GLOBAL_BASE_DIR)/platform/emdrv/rtcdrv/inc \
	-I$(GLOBAL_BASE_DIR)/platform/emdrv/sleep/inc \
	-I$(GLOBAL_EMDRV_DIR)/aesdrv/inc \
	-I$(GLOBAL_EMDRV_DIR)/aesdrv/src \
	-I$(GLOBAL_EMDRV_DIR)/trace/inc \
	-I$(GLOBAL_EMDRV_DIR)/gpiointerrupt/inc \
	-I$(GLOBAL_EMDRV_DIR)/ecode \
	-I$(GLOBAL_EMDRV_DIR)/uartdrv/config \
	-I$(GLOBAL_EMDRV_DIR)/uartdrv/inc \
	-I$(GLOBAL_EMDRV_DIR)/spidrv/config \
	-I$(GLOBAL_EMDRV_DIR)/spidrv/inc \
	-I$(GLOBAL_EMDRV_DIR)/dmadrv/config \
	-I$(GLOBAL_EMDRV_DIR)/dmadrv/inc \
	-I$(GLOBAL_EMDRV_DIR)/ustimer/config \
	-I$(GLOBAL_EMDRV_DIR)/ustimer/inc \
	-I$(GLOBAL_EMDRV_DIR)/cryptodrv/src \
	-I$(GLOBAL_EMDRV_DIR)/tempdrv/config \
	-I$(GLOBAL_EMDRV_DIR)/tempdrv/inc \
	-I$(GLOBAL_BASE_DIR)/hal/micro/cortexm3/common \
	-I$(GLOBAL_EMLIB_DIR)/inc \
	-I$(GLOBAL_EMDRV_DIR)/rtcdrv/config \
	-I$(GLOBAL_EMDRV_DIR)/rtcdrv/inc \
	-I$(GLOBAL_CMSIS_DIR)/Include \
	-I$(GLOBAL_KIT_DIR)/common/bsp \
	-I$(GLOBAL_KIT_DIR)/common/drivers \
	-I$(GLOBAL_BASE_DIR)/hal/micro/cortexm3/efm32 \
	-I$(GLOBAL_BASE_DIR)/hal/micro/cortexm3/efm32/efr32 \
	-I$(GLOBAL_GLIB_DIR) \
	-I$(GLOBAL_GLIB_DIR)/glib \
	-I$(GLOBAL_GLIB_DIR)/dmd \
	-I$(GLOBAL_DEVICE_DIR)/SiliconLabs/$(FAMILY)/Include \
	-I$(GLOBAL_BASE_DIR)/hal/plugin \
	-I$(GLOBAL_BASE_DIR)/app/util \
	-I$(GLOBAL_BASE_DIR)/lwip/ \
	-I$(GLOBAL_BASE_DIR)/lwip/src/include/ \
	-I$(GLOBAL_BASE_DIR)/lwip/contrib/port/FreeRTOS/EM358X/ \
	-I$(GLOBAL_BASE_DIR)/lwip/src/include/ipv4/ \
	-I$(GLOBAL_BASE_DIR)/lwip/src/include/ipv6/ \
	-I$(STACK_DIR)/include/ \
	-I$(PLUGIN_DIR) \

CCFLAGS = --cpu=cortex-m3 \
					--cpu_mode=thumb \
					--diag_suppress=Pa050 \
					-e \
					--endian=little \
					--fpu=none \
					--no_path_in_file_macros \
					--separate_cluster_for_initialized_variables \
					--dlib_config=$(IAR_DIR)/ARM/inc/c/DLib_Config_Normal.h \
					--debug \
					--dependencies=m $*.d \
					-Ohz \
					$(CDEFS) \
					$(CINC)

ASMFLAGS = --cpu cortex-M3 \
					 --fpu None \
					 -s+ \
					 "-M<>" \
					 -w+ \
					 -t2 \
					 -r \
					 $(CINC) \
					 $(ASMDEFS)

LDFLAGS = --entry halEntryPoint \
					--log initialization,modules,sections,veneers \
					--diag_suppress=Lp012 \
					--config_def BOOTLOADER=1 \
					--config $(GLOBAL_BASE_DIR)/hal/micro/cortexm3/efm32/iar-cfg.icf

SOURCE_FILES = \
	     $(GLOBAL_BASE_DIR)/hal/plugin/antenna-stub/antenna-stub.c \
	     $(GLOBAL_BASE_DIR)/hal/micro/cortexm3/efm32/bootloader/bootloader-gpio.c \
	     $(GLOBAL_BASE_DIR)/hal/micro/cortexm3/efm32/bootloader/bootloader-uart.c \
	     $(GLOBAL_KIT_DIR)/common/bsp/bsp_stk.c \
	     $(GLOBAL_BASE_DIR)/hal/micro/generic/crc.c \
	     $(GLOBAL_BASE_DIR)/hal/micro/cortexm3/efm32/bootloader/cstartup-iar-boot-entry.s79 \
	     $(GLOBAL_BASE_DIR)/hal/micro/cortexm3/efm32/bootloader/cstartup-iar-boot.c \
	     $(GLOBAL_EMLIB_DIR)/src/em_assert.c \
	     $(GLOBAL_EMLIB_DIR)/src/em_cmu.c \
	     $(GLOBAL_EMLIB_DIR)/src/em_emu.c \
	     $(GLOBAL_EMLIB_DIR)/src/em_gpio.c \
	     $(GLOBAL_EMLIB_DIR)/src/em_core.c \
	     $(GLOBAL_EMLIB_DIR)/src/em_ldma.c \
	     $(GLOBAL_EMLIB_DIR)/src/em_msc.c \
	     $(GLOBAL_EMLIB_DIR)/src/em_rmu.c \
	     $(GLOBAL_EMLIB_DIR)/src/em_system.c \
	     $(GLOBAL_EMLIB_DIR)/src/em_timer.c \
	     $(GLOBAL_EMLIB_DIR)/src/em_usart.c \
	     $(GLOBAL_EMLIB_DIR)/src/em_wdog.c \
	     $(GLOBAL_BASE_DIR)/hal/micro/cortexm3/efm32/isr-stubs.s79 \
	     $(GLOBAL_BASE_DIR)/hal/micro/cortexm3/efm32/led.c \
	     $(GLOBAL_BASE_DIR)/hal/micro/generic/mem-util.c \
	     $(GLOBAL_BASE_DIR)/hal/micro/cortexm3/efm32/mfg-token.c \
	     $(GLOBAL_BASE_DIR)/hal/micro/cortexm3/efm32/micro-common.c \
	     $(GLOBAL_BASE_DIR)/hal/micro/generic/random.c \
	     $(GLOBAL_BASE_DIR)/hal/micro/cortexm3/common/standalone-bootloader-custom.c \
	     $(GLOBAL_DEVICE_DIR)/SiliconLabs/$(FAMILY)/Source/system_$(FAMILY).c \

LIB_FILES = \
	  $(BUILD_DIR)/standalone-bootloader-library-cortexm3-iarboot-$(FAMILY)-null/standalone-bootloader-library.a \

DEP_FILES = $(addprefix $(OUTPUT_DIR)/,$(subst ../,,$(CSOURCES:.c=.d)))

TARGET = serial-uart-bootloader

CSOURCES = $(filter %.c, $(SOURCE_FILES))
ASMSOURCES = $(filter %.s79, $(SOURCE_FILES))
ASMSOURCES2 = $(filter %.s, $(SOURCE_FILES))

COBJS = $(addprefix $(OUTPUT_DIR)/,$(subst ../,,$(CSOURCES:.c=.o)))
ASMOBJS = $(addprefix $(OUTPUT_DIR)/,$(subst ../,,$(ASMSOURCES:.s79=.o)))
ASMOBJS2 = $(addprefix $(OUTPUT_DIR)/,$(subst ../,,$(ASMSOURCES2:.s=.o)))

OUTPUT_DIRS = $(sort $(dir $(COBJS)) $(dir $(ASMOBJS)) $(dir $(ASMOBJS2)))

.PHONY: all clean PROLOGUE

all: PROLOGUE $(OUTPUT_DIRS) $(COBJS) $(ASMOBJS) $(ASMOBJS2) $(LIB_FILES)
	@echo 'Linking...'
	@$(LD) $(LIB_FILES) $(COBJS) $(ASMOBJS) $(ASMOBJS2) -o $(OUTPUT_DIR)/$(TARGET).out $(LDFLAGS) --map $(OUTPUT_DIR)/$(TARGET).map --log_file $(OUTPUT_DIR)/$(TARGET).log
	@$(ELFTOOL) $(OUTPUT_DIR)/$(TARGET).out --srec --srec-s3only $(OUTPUT_DIR)/$(TARGET).s37
	@echo 'Done.'

PROLOGUE:
# @echo $(COBJS)
# @echo $(ASMOBJS)
# @echo $(ASMOBJS2)

$(OUTPUT_DIRS):
	@mkdir -p $@

$(COBJS): %.o:
	@echo 'Building $(notdir $(@:%.o=%.c))...'
	@$(CC) $(CCFLAGS) -o $@ $(filter %$(@:$(OUTPUT_DIR)/%.o=%.c),$(CSOURCES)) > /dev/null; \
	$(CLEAN_UP_DEP_CMD)

$(ASMOBJS): %.o:
	@echo 'Building $(notdir $(@:%.o=%.s79))...'
	@$(AS) $(ASMFLAGS) -o $@ $(filter %$(@:$(OUTPUT_DIR)/%.o=%.s79),$(ASMSOURCES)) > /dev/null

$(ASMOBJS2): %.o:
	@echo 'Building $(notdir $(@:%.o=%.s))...'
	@$(AS) $(ASMFLAGS) -o $@ $(filter %$(@:$(OUTPUT_DIR)/%.o=%.s),$(ASMSOURCES2)) > /dev/null

clean:
	$(RM) -r $(OUTPUT_DIR)

