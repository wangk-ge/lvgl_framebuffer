#
# Makefile
#
CC = gcc
LVGL_DIR_NAME ?= lvgl
LVGL_DIR ?= ${shell pwd}
WARNINGS ?= -Werror -Wall -Wextra \
	-Wshadow -Wundef -Wmaybe-uninitialized -Wmissing-prototypes -Wno-discarded-qualifiers \
	-Wno-unused-function -Wno-error=strict-prototypes -Wpointer-arith -fno-strict-aliasing -Wno-error=cpp -Wuninitialized \
	-Wno-unused-parameter -Wno-missing-field-initializers -Wno-format-nonliteral -Wno-cast-qual -Wunreachable-code -Wno-switch-default  \
	-Wreturn-type -Wmultichar -Wformat-security -Wno-ignored-qualifiers -Wno-error=pedantic -Wno-sign-compare -Wno-error=missing-prototypes -Wdouble-promotion -Wclobbered -Wdeprecated  \
	-Wempty-body -Wshift-negative-value -Wstack-usage=2048 \
	-Wtype-limits -Wsizeof-pointer-memaccess -Wpointer-arith -Wno-error=cast-function-type
            
CFLAGS ?= -O3 -g0 -I$(LVGL_DIR)/ $(WARNINGS)
#CFLAGS ?= -g -I$(LVGL_DIR)/ $(WARNINGS)
#LDFLAGS += -lSDL2 -lm
BIN = lvgl_framebuffer

MAINSRC = $(LVGL_DIR)/main.c

include $(LVGL_DIR)/lvgl/lvgl.mk
include $(LVGL_DIR)/lv_drivers/lv_drivers.mk
include $(LVGL_DIR)/lv_examples/lv_examples.mk

CSRCS += $(LVGL_DIR)/mouse_cursor_icon.c
CSRCS += $(LVGL_DIR)/evdev_mouse.c

OBJEXT ?= .o

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))

MAINOBJ = $(MAINSRC:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS)

## MAINOBJ -> OBJFILES

all: default

%.o: %.c
	@$(CC)  $(CFLAGS) -c $< -o $@
	@echo "CC $<"

default: $(AOBJS) $(COBJS) $(MAINOBJ)
	$(CC) -o $(BIN) $(MAINOBJ) $(AOBJS) $(COBJS) $(LDFLAGS)

clean: 
	rm -f $(BIN) $(AOBJS) $(COBJS) $(MAINOBJ)
