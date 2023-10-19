EXE = xavi_apps_api
dynamic: generic_exe_install

clean:
	rm -f xavi_apps_api


xavi_apps_api:
	cp xavi_apps_api_$(PROFILE)_save xavi_apps_api

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
