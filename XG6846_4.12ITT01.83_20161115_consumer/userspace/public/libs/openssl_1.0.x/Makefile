
openssl: conditional_build 

.PHONY: all distclean clean conditional_build openssl

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

PATCH_FILE = bcm_patches/lib_config.patch
PATCH_COMPLETE_FILE = .patch_complete
UNTAR_COMPLETE_FILE = .untar_complete
CONFIG_ID_FILE = BRCM_CONFIG_ID_$(TOOLCHAIN_PREFIX)_$(LINUX_VER_STR)

ifeq ($(strip $(XAVI_OPENSSL_1_0_X)),y)
conditional_build: all
else
conditional_build:
	@echo "skipping openssl 1.0.2a (not configured)"
endif

#clear CC to avoid conflict with openssl configuration
CC=

# APP must match the openssl folder name
APP=openssl-1.0.2a

SHLIBCMD= grep -Po '(?<=SHLIB_VERSION_NUMBER=).*' $(APP)/Makefile

# the options are ordered for the configure script
ifeq ($(strip $(PROFILE_ARCH)),arm)
SSLOPTIONS := linux-bcmarm
else
SSLOPTIONS := linux-bcmmips
endif
SSLOPTIONS += --prefix=/usr/local/ssl --openssldir=/usr/local/ssl
SSLOPTIONS += no-hw shared no-md2 no-md4 no-mdc2 no-ripemd no-rc2 no-krb5 no-idea no-unit-test no-cast no-bf

ifeq ($(strip $(BUILD_TR69C_SSL)$(BUILD_HTTPD_SSL)),)
SSLOPTIONS += no-rc4
endif
ifneq ($(strip $(BENCHMARKS_SMP)),y)
SSLOPTIONS += no-threads no-zlib no-speed
endif
ifeq ($(strip $(BUILD_HTTPD_SSL)),)
SSLOPTIONS += no-engine no-rc5 no-err no-ssl2 -DOPENSSL_NO_HEARTBEATS
endif
ifeq ($(strip $(ARCH_ENDIAN)),little)
SSLOPTIONS += -DL_ENDIAN
else
SSLOPTIONS += -DB_ENDIAN 
endif
ifneq ($(strip $(BUILD_OCF_LINUX)),)
SSLOPTIONS += -DHAVE_CRYPTODEV -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)
endif

SSLOPTIONS += $(BRCM_COMMON_CFLAGS) $(SSP_TYP_COMPILER_OPTS) $(SSP_TYP_LIBS) $(SSP_LIB_INC)

$(APP)/$(UNTAR_COMPLETE_FILE): $(PATCH_FILE) $(APP).tar.gz
	rm -rf $(APP)
	tar xzf $(APP).tar.gz
	-ln -s ../../crypto/rc5/rc5.h $(APP)/include/openssl/rc5.h
	@echo "$(APP) is untarred"
	-[ -e $(APP)/Makefile ] && $(MAKE) -j 1 -C $(APP) dclean
	patch -p1 -d$(APP) < $(PATCH_FILE)
	@echo "$(APP) is patched"
	touch $(APP)/$(UNTAR_COMPLETE_FILE)

$(APP)/$(CONFIG_ID_FILE): $(APP)/$(UNTAR_COMPLETE_FILE)
	@echo "$(APP) is being configured"
	(cd $(APP); ./Configure $(SSLOPTIONS);)
	$(MAKE) -j 1 -C $(APP) depend
	rm -f $(APP)/BRCM_CONFIG_ID_*
	touch $(APP)/$(CONFIG_ID_FILE)

all: $(APP)/$(CONFIG_ID_FILE)
	$(MAKE) -j 1 -C $(APP) all
	mkdir -p $(INSTALL_DIR)/bin
	rm -f $(INSTALL_DIR)/bin/openssl
	install -m 755 $(APP)/apps/openssl $(INSTALL_DIR)/bin
	(mkdir -p ../openssl;\
		cd ../openssl; \
		rm -rf include; \
		ln -s ../openssl_1.0.x/openssl-1.0.2a/include include; \
	)		
	mkdir -p $(INSTALL_DIR)/lib/public
	@SHLIB_VERSION_NUMBER=$$(grep -Po '(?<=SHLIB_VERSION_NUMBER=).*' ./$(APP)/Makefile ); \
	echo "install -m 755 $(APP)/libssl.so.$$SHLIB_VERSION_NUMBER $(INSTALL_DIR)/lib/public"; \
	install -m 755 $(APP)/libssl.so.$$SHLIB_VERSION_NUMBER $(INSTALL_DIR)/lib/public; \
	cd $(INSTALL_DIR)/lib/public; rm -f libssl.so; ln -s libssl.so.$$SHLIB_VERSION_NUMBER libssl.so; cd - > /dev/null; \
	cd $(INSTALL_DIR)/lib; rm -f libssl.so; ln -s public/libssl.so.$$SHLIB_VERSION_NUMBER libssl.so; cd - > /dev/null; \
	cd $(INSTALL_DIR)/lib; rm -f libssl.so.$$SHLIB_VERSION_NUMBER; ln -s public/libssl.so.$$SHLIB_VERSION_NUMBER libssl.so.$$SHLIB_VERSION_NUMBER; cd - > /dev/null; \
	echo "install -m 755 $(APP)/libcrypto.so.$$SHLIB_VERSION_NUMBER $(INSTALL_DIR)/lib/public"; \
	install -m 755 $(APP)/libcrypto.so.$$SHLIB_VERSION_NUMBER $(INSTALL_DIR)/lib/public; \
	cd $(INSTALL_DIR)/lib/public; rm -f libcrypto.so; ln -s libcrypto.so.$$SHLIB_VERSION_NUMBER libcrypto.so; cd - > /dev/null; \
	cd $(INSTALL_DIR)/lib; rm -f libcrypto.so; ln -s public/libcrypto.so.$$SHLIB_VERSION_NUMBER libcrypto.so; cd - > /dev/null; \
	cd $(INSTALL_DIR)/lib; rm -f libcrypto.so.$$SHLIB_VERSION_NUMBER; ln -s public/libcrypto.so.$$SHLIB_VERSION_NUMBER libcrypto.so.$$SHLIB_VERSION_NUMBER; cd - > /dev/null;

clean:
	rm -rf $(APP)
	rm -f $(INSTALL_DIR)/bin/openssl
	rm -f $(INSTALL_DIR)/lib/public/libssl.so*
	rm -f $(INSTALL_DIR)/lib/public/libcrypto.so*
	rm -f $(INSTALL_DIR)/lib/libssl.so*
	rm -f $(INSTALL_DIR)/lib/libcrypto.so*
	rm -rf ../openssl/include

distclean: clean

shell:
	bash -i

