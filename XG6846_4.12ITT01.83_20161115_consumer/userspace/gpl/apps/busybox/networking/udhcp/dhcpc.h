/* vi: set sw=4 ts=4: */
/*
 * Licensed under GPLv2, see file LICENSE in this tarball for details.
 */
#ifndef UDHCP_DHCPC_H
#define UDHCP_DHCPC_H 1

PUSH_AND_SET_FUNCTION_VISIBILITY_TO_HIDDEN

struct client_config_t {
	uint8_t client_mac[6];          /* Our mac address */
	char no_default_options;        /* Do not include default options in request */
	IF_FEATURE_UDHCP_PORT(uint16_t port;)
	int ifindex;                    /* Index number of the interface to use */
	uint8_t opt_mask[256 / 8];      /* Bitmask of options to send (-O option) */
	const char *interface;          /* The name of the interface to use */
	char *pidfile;                  /* Optionally store the process ID */
	const char *script;             /* User script to run at dhcp events */
	struct option_set *options;     /* list of DHCP options to send to server */
	uint8_t *clientid;              /* Optional client id to use */
	uint8_t *vendorclass;           /* Optional vendor class-id to use */
	uint8_t *hostname;              /* Optional hostname to use */
	uint8_t *fqdn;                  /* Optional fully qualified domain name to use */
} FIX_ALIASING;

/* server_config sits in 1st half of bb_common_bufsiz1 */
#define client_config (*(struct client_config_t*)(&bb_common_bufsiz1[COMMON_BUFSIZE / 2]))

#if ENABLE_FEATURE_UDHCP_PORT
#define CLIENT_PORT (client_config.port)
#else
#define CLIENT_PORT 68
#endif

POP_SAVED_FUNCTION_VISIBILITY

#ifdef XAVI_WAN_PROTO
#ifdef XAVI_MULTI_MAC
#define WAN_IF_NAME		"tap1"
#else
#define WAN_IF_NAME		"br0:1"
#endif
#endif

#if defined(XAVI_MODEL_NAME) || defined(XAVI_UDHCPC_OPTION)
#define _PATH_IP                "ipaddress"
#define _PATH_MASK              "subnetmask"
#define _PATH_ROUTER            "routerip"

#if defined(XAVI_UDHCPC_OPTION)
#define _PATH_OPTION_43         "option43"
#define _PATH_OPTION_66         "option66"
#define _PATH_OPTION_67         "option67"
#define _PATH_OPTION_121        "option121"
#define _PATH_OPTION_125        "option125"
#define _PATH_OPTION_128        "option128"
#define _PATH_OPTION_132        "option132"
#define _PATH_OPTION_224        "option224"
#define _PATH_OPTION_250        "option250"
#endif

#define _PATH_WAN_IF		"wan_if"
#define _PATH_WAN_DIR		"/var/fyi/wan"
#endif

#endif
