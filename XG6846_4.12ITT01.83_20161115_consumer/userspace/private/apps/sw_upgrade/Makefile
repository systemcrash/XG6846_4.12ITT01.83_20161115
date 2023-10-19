EXE = sw_upgrade
dynamic: generic_exe_install

clean:
	rm -f sw_upgrade


sw_upgrade:
	cp sw_upgrade_$(PROFILE)_save sw_upgrade

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
