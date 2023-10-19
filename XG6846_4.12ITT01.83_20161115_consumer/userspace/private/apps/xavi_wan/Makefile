EXE = xavi_wan
dynamic: generic_exe_install

clean:
	rm -f xavi_wan


xavi_wan:
	cp xavi_wan_$(PROFILE)_save xavi_wan

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
