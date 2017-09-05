
# IAR 7.xx toolchain
ifndef ARM_IAR7_DIR
$(error ARM_IAR7_DIR is not set. Please define ARM_IAR7_DIR pointing to your IAR 7.xx installation folder.)
endif

ARCH = $(JAMEXE_PREFIX) $(ARM_IAR7_DIR)/arm/bin/iarchive.exe
AS = $(JAMEXE_PREFIX) $(ARM_IAR7_DIR)/arm/bin/iasmarm.exe
CC = $(JAMEXE_PREFIX) $(ARM_IAR7_DIR)/arm/bin/iccarm.exe
ELFTOOL = $(JAMEXE_PREFIX) $(ARM_IAR7_DIR)/arm/bin/ielftool.exe
LD = $(JAMEXE_PREFIX) $(ARM_IAR7_DIR)/arm/bin/ilinkarm.exe

# Architecture directories
ARCHITECTURE_DIR = ezr32
BUILD_DIR = build
OUTPUT_DIR = $(BUILD_DIR)/$(ARCHITECTURE_DIR)

# Stack and submodule directories
GLOBAL_BASE_DIR     = $--halDirFromProjFs--$/..

CDEFS = -D$--bootloader--$ \
$--dashDMacrosMak:"\"--$

ASMDEFS = -D$--bootloader--$ \
$--dashDMacrosMak:"\"--$

CINC = -I. \
  -I$(ARM_IAR7_DIR)/ARM/INC \
$--includePathsMak--$

CCFLAGS = --cpu_mode=thumb \
          --diag_suppress=Pa050 \
          -e \
          --endian=little \
          --fpu=none \
          --no_path_in_file_macros \
          --separate_cluster_for_initialized_variables \
          --dlib_config=$(ARM_IAR7_DIR)/ARM/inc/c/DLib_Config_Normal.h \
          --debug \
          --dependencies=m $*.d \
          -Ohz \
          $(CDEFS) \
          $(CINC)


ASMFLAGS = --fpu None \
           -s+ \
           "-M<>" \
           -w+ \
           -t2 \
           -r \
           $(CINC) \
           $(ASMDEFS)

ifeq ($(findstring HG320,$(ARCHITECTUREID)), HG320)
CCFLAGS  += --cpu=cortex-m0
ASMFLAGS += --cpu cortex-M0
else
CCFLAGS  += --cpu=cortex-m3
ASMFLAGS += --cpu cortex-M3
endif

LDFLAGS = --entry halEntryPoint \
          --log initialization,modules,sections,veneers \
          --diag_suppress=Lp012 \
          --config $(GLOBAL_BASE_DIR)/hal/micro/cortexm3/efm32/iar-cfg.icf \
          --config_def BTL_SIZE=0K \
          --config_def $--bootloader--$=1

SOURCE_FILES = \
$--includeFilesMak--$

LIB_FILES = \
$--includeLibFilesMak--$

TARGET = $--deviceName--$

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
	@$(ELFTOOL) $(OUTPUT_DIR)/$(TARGET).out --bin $(OUTPUT_DIR)/$(TARGET).bin
	@$(ELFTOOL) $(OUTPUT_DIR)/$(TARGET).out --ihex $(OUTPUT_DIR)/$(TARGET).hex
	@$(ELFTOOL) $(OUTPUT_DIR)/$(TARGET).out --srec --srec-s3only $(OUTPUT_DIR)/$(TARGET).s37
	@echo 'Done.'

PROLOGUE:
#	@echo $(COBJS)
#	@echo $(ASMOBJS)
#	@echo $(ASMOBJS2)

$(OUTPUT_DIRS):
	@mkdir -p $@

$(COBJS): %.o:
	@echo 'Building $(notdir $(@:%.o=%.c))...'
	@$(CC) $(CCFLAGS) -o $@ $(filter %$(@:$(OUTPUT_DIR)/%.o=%.c),$(CSOURCES)) > /dev/null; \

$(ASMOBJS): %.o:
	@echo 'Building $(notdir $(@:%.o=%.s79))...'
	@$(AS) $(ASMFLAGS) -o $@ $(filter %$(@:$(OUTPUT_DIR)/%.o=%.s79),$(ASMSOURCES)) > /dev/null

$(ASMOBJS2): %.o:
	@echo 'Building $(notdir $(@:%.o=%.s))...'
	@$(AS) $(ASMFLAGS) -o $@ $(filter %$(@:$(OUTPUT_DIR)/%.o=%.s),$(ASMSOURCES2)) > /dev/null
  
clean:
	$(RM) -r $(OUTPUT_DIR)

