# This Makefile defines how to build 'bootload-ncp-uart' for a unix host.
#

# Variables

GLOBAL_BASE_DIR = ../../platform/base

.SUFFIXES:

ifdef E_CC
  CC = $(E_CC)
endif
LD = $(CC)

SHELL = /bin/sh

DEFINES = \
  -DUNIX \
  -DUNIX_HOST \
  -DPHY_NULL \
  -DBOARD_HEADER=\"micro/unix/host/board/host.h\" \
  -DCONFIGURATION_HEADER=\"connect/plugins/serial-bootloader/bootload-ncp-uart-configuration.h\" \
  -DPLATFORM_HEADER=\"micro/unix/compiler/gcc.h\" \
  -DBOARD_HOST \
  -DEMBER_STACK_CONNECT \
  -DEZSP_HOST \
  -DEZSP_ASH \


INCLUDES = \
  -I. \
  -I$(GLOBAL_BASE_DIR) \
  -I$(GLOBAL_BASE_DIR)/hal \
  -I$(GLOBAL_BASE_DIR)/hal/plugin \
  -I./connect/plugins \
  -I./connect/plugins/serial-bootloader \
  -I./connect/plugins/stack \


FILES = \
  connect/plugins/serial-bootloader/ezsp-host/ash/ash-host-ui.c \
  connect/plugins/serial-bootloader/ezsp-host/ash/ash-host.c \
  connect/plugins/serial-bootloader/ezsp-host/ezsp-host-io.c \
  connect/plugins/serial-bootloader/ezsp-host/ezsp-host-queues.c \
  connect/plugins/serial-bootloader/ezsp-host/ezsp-host-ui.c \
  connect/plugins/serial-bootloader/bootload-ncp-app.c \
  connect/plugins/serial-bootloader/bootload-ncp-uart.c \
  connect/plugins/serial-bootloader/bootload-ncp.c \
  connect/plugins/serial-bootloader/bootload-xmodem.c \
  connect/plugins/serial-bootloader/gateway/backchannel-stub.c \
  connect/plugins/serial/simple-linux-serial.c \
  connect/plugins/stack/framework/byte-utilities.c \
  $(GLOBAL_BASE_DIR)/hal/micro/generic/ash-common.c \
  $(GLOBAL_BASE_DIR)/hal/micro/generic/crc.c \
  $(GLOBAL_BASE_DIR)/hal/micro/generic/system-timer.c \


LIBRARIES = \


CFLAGS = -ggdb -O0 -Wall
CPPFLAGS = $(DEFINES) $(INCLUDES) -MF $(BUILD_PATH)/$(@F:.o=.d) -MMD -MP

ifeq ($(shell uname -s | sed -e 's/^\(CYGWIN\).*/\1/'),Darwin)
  LDFLAGS =
else
  LDFLAGS = -Wl,-export-dynamic
endif
LDLIBS =

BUILD_PATH = $(CURDIR)/build/bootload-ncp-uart-app

DEPENDENCIES = $(addprefix $(BUILD_PATH)/, $(notdir $(FILES:.c=.d)))
EXECUTABLE = $(BUILD_PATH)/bootload-ncp-uart-app
OBJECTS = $(addprefix $(BUILD_PATH)/, $(notdir $(FILES:.c=.o)))

VPATH = $(dir $(FILES))

.PHONY: all
all: $(EXECUTABLE)

.PHONY: clean
clean:
	@-rm -f $(DEPENDENCIES) $(EXECUTABLE) $(OBJECTS)

ifneq ($(MAKECMDGOALS),clean)
  -include $(DEPENDENCIES)
endif

$(EXECUTABLE): $(OBJECTS) $(LIBRARIES) | $(BUILD_PATH)
ifeq ($(shell uname -s | sed -e 's/^\(CYGWIN\).*/\1/'),Darwin)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@
else
	$(LD) $(LDFLAGS) -Xlinker --start-group $^ $(LDLIBS) -Xlinker --end-group -o $@
endif

$(BUILD_PATH)/%.o: %.c | $(BUILD_PATH)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(BUILD_PATH):
	@mkdir -p $(BUILD_PATH)
