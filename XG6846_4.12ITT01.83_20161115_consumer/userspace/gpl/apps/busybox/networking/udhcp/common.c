/* vi: set sw=4 ts=4: */
/*
 * Rewrite by Russ Dill <Russ.Dill@asu.edu> July 2001
 *
 * Licensed under GPLv2, see file LICENSE in this tarball for details.
 */
#include "common.h"

#if defined CONFIG_UDHCP_DEBUG && CONFIG_UDHCP_DEBUG >= 1
unsigned dhcp_verbose;
#endif

const uint8_t MAC_BCAST_ADDR[6] ALIGN2 = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

/* Supported options are easily added here.
 * See RFC2132 for more options.
 * OPTION_REQ: these options are requested by udhcpc (unless -o).
 */
const struct dhcp_optflag dhcp_optflags[] = {
	/* flags                                    code */
	{ OPTION_IP                   | OPTION_REQ, 0x01 }, /* DHCP_SUBNET        */
	{ OPTION_S32                              , 0x02 }, /* DHCP_TIME_OFFSET   */
	{ OPTION_IP | OPTION_LIST     | OPTION_REQ, 0x03 }, /* DHCP_ROUTER        */
//	{ OPTION_IP | OPTION_LIST                 , 0x04 }, /* DHCP_TIME_SERVER   */
//	{ OPTION_IP | OPTION_LIST                 , 0x05 }, /* DHCP_NAME_SERVER   */
	{ OPTION_IP | OPTION_LIST     | OPTION_REQ, 0x06 }, /* DHCP_DNS_SERVER    */
//	{ OPTION_IP | OPTION_LIST                 , 0x07 }, /* DHCP_LOG_SERVER    */
//	{ OPTION_IP | OPTION_LIST                 , 0x08 }, /* DHCP_COOKIE_SERVER */
	{ OPTION_IP | OPTION_LIST                 , 0x09 }, /* DHCP_LPR_SERVER    */
	{ OPTION_STRING               | OPTION_REQ, 0x0c }, /* DHCP_HOST_NAME     */
	{ OPTION_U16                              , 0x0d }, /* DHCP_BOOT_SIZE     */
	{ OPTION_STRING               | OPTION_REQ, 0x0f }, /* DHCP_DOMAIN_NAME   */
	{ OPTION_IP                               , 0x10 }, /* DHCP_SWAP_SERVER   */
	{ OPTION_STRING                           , 0x11 }, /* DHCP_ROOT_PATH     */
	{ OPTION_U8                               , 0x17 }, /* DHCP_IP_TTL        */
	{ OPTION_U16                              , 0x1a }, /* DHCP_MTU           */
	{ OPTION_IP                   | OPTION_REQ, 0x1c }, /* DHCP_BROADCAST     */
	{ OPTION_STRING                           , 0x28 }, /* DHCP_NIS_DOMAIN    */
	{ OPTION_IP | OPTION_LIST                 , 0x29 }, /* DHCP_NIS_SERVER    */
	{ OPTION_IP | OPTION_LIST     | OPTION_REQ, 0x2a }, /* DHCP_NTP_SERVER    */
	{ OPTION_IP | OPTION_LIST                 , 0x2c }, /* DHCP_WINS_SERVER   */
	{ OPTION_U32                              , 0x33 }, /* DHCP_LEASE_TIME    */
	{ OPTION_IP                               , 0x36 }, /* DHCP_SERVER_ID     */
	{ OPTION_STRING                           , 0x38 }, /* DHCP_ERR_MESSAGE   */
//TODO: must be combined with 'sname' and 'file' handling:
#ifdef XAVI_UDHCPC_OPTION
	{ OPTION_STRING               | OPTION_REQ, 0x42 }, /* DHCP_TFTP_SERVER_NAME */
	{ OPTION_STRING               | OPTION_REQ, 0x43 }, /* DHCP_BOOT_FILE     */
#else
	{ OPTION_STRING                           , 0x42 }, /* DHCP_TFTP_SERVER_NAME */
#endif
//TODO: not a string, but a set of LASCII strings:
//	{ OPTION_STRING                           , 0x4D }, /* DHCP_USER_CLASS    */
#if ENABLE_FEATURE_UDHCP_RFC3397
	{ OPTION_DNS_STRING | OPTION_LIST         , 0x77 }, /* DHCP_DOMAIN_SEARCH */
	{ OPTION_SIP_SERVERS                      , 0x78 }, /* DHCP_SIP_SERVERS   */
#endif
	{ OPTION_STATIC_ROUTES        | OPTION_REQ, 0x79 }, /* DHCP_STATIC_ROUTES */
	{ OPTION_STRING                           , 0xfc }, /* DHCP_WPAD          */
#ifdef XAVI_UDHCPC_OPTION
	{ OPTION_STRING               | OPTION_REQ, 0x2b }, /* DHCP_VENDOR_SPECIFIC */
	{ OPTION_STRING               | OPTION_REQ, 0x80 }, /* DHCP_HTTP_IP */
	{ OPTION_STRING               | OPTION_REQ, 0x84 }, /* DHCP_8021Q_VLAN_ID */
	{ OPTION_STRING				  | OPTION_REQ, 0x7d }, /* DHCP_OPT125 */
#ifdef XAVI_UDHCPC_OPT224
	{ OPTION_STRING               | OPTION_REQ, 0xe0 }, /* DHCP_PRIVATE224 */
#endif
#ifdef XAVI_UDHCPC_OPT250_APAS
	{ OPTION_STRING               | OPTION_REQ, 0xFA }, /* DHCP_PRIVATE250 */
#endif
#else
	{ OPTION_STATIC_ROUTES                    , 0x79 }, /* DHCP_STATIC_ROUTES */
#endif

	/* Options below have no match in dhcp_option_strings[],
	 * are not passed to dhcpc scripts, and cannot be specified
	 * with "option XXX YYY" syntax in dhcpd config file.
	 * These entries are only used internally by udhcp[cd]
	 * to correctly encode options into packets.
	 */

	{ OPTION_IP                               , 0x32 }, /* DHCP_REQUESTED_IP  */
	{ OPTION_U8                               , 0x35 }, /* DHCP_MESSAGE_TYPE  */
	{ OPTION_U16                              , 0x39 }, /* DHCP_MAX_SIZE      */
	{ OPTION_STRING                           , 0x3c }, /* DHCP_VENDOR        */
//FIXME: handling of this option is not exactly correct:
	{ OPTION_STRING                           , 0x3d }, /* DHCP_CLIENT_ID     */
	{ 0, 0 } /* zeroed terminating entry */
};

/* Used for converting options from incoming packets to env variables
 * for udhcpc stript, and for setting options for udhcpd via
 * "opt OPTION_NAME OPTION_VALUE" directives in udhcpd.conf file.
 */
/* Must match dhcp_optflags[] order */
const char dhcp_option_strings[] ALIGN1 =
	"subnet" "\0"      /* DHCP_SUBNET         */
	"timezone" "\0"    /* DHCP_TIME_OFFSET    */
	"router" "\0"      /* DHCP_ROUTER         */
//	"timesrv" "\0"     /* DHCP_TIME_SERVER    */
//	"namesrv" "\0"     /* DHCP_NAME_SERVER    */
	"dns" "\0"         /* DHCP_DNS_SERVER     */
//	"logsrv" "\0"      /* DHCP_LOG_SERVER     */
//	"cookiesrv" "\0"   /* DHCP_COOKIE_SERVER  */
	"lprsrv" "\0"      /* DHCP_LPR_SERVER     */
	"hostname" "\0"    /* DHCP_HOST_NAME      */
	"bootsize" "\0"    /* DHCP_BOOT_SIZE      */
	"domain" "\0"      /* DHCP_DOMAIN_NAME    */
	"swapsrv" "\0"     /* DHCP_SWAP_SERVER    */
	"rootpath" "\0"    /* DHCP_ROOT_PATH      */
	"ipttl" "\0"       /* DHCP_IP_TTL         */
	"mtu" "\0"         /* DHCP_MTU            */
	"broadcast" "\0"   /* DHCP_BROADCAST      */
	"nisdomain" "\0"   /* DHCP_NIS_DOMAIN     */
	"nissrv" "\0"      /* DHCP_NIS_SERVER     */
	"ntpsrv" "\0"      /* DHCP_NTP_SERVER     */
	"wins" "\0"        /* DHCP_WINS_SERVER    */
	"lease" "\0"       /* DHCP_LEASE_TIME     */
	"serverid" "\0"    /* DHCP_SERVER_ID      */
	"message" "\0"     /* DHCP_ERR_MESSAGE    */
	"tftp" "\0"        /* DHCP_TFTP_SERVER_NAME */
	"bootfile" "\0"    /* DHCP_BOOT_FILE      */
//	"userclass" "\0"   /* DHCP_USER_CLASS     */
#if ENABLE_FEATURE_UDHCP_RFC3397
	"search" "\0"      /* DHCP_DOMAIN_SEARCH  */
// doesn't work in udhcpd.conf since OPTION_SIP_SERVERS
// is not handled yet by "string->option" conversion code:
	"sipsrv" "\0"      /* DHCP_SIP_SERVERS    */
#endif
// doesn't work in udhcpd.conf since OPTION_STATIC_ROUTES
// is not handled yet by "string->option" conversion code:
	"staticroutes" "\0"/* DHCP_STATIC_ROUTES  */
	"wpad" "\0"        /* DHCP_WPAD           */
#ifdef XAVI_UDHCPC_OPTION
	"vendorinfo" "\0"  /* DHCP_VENDOR_SPECIFIC */
	"httpip" "\0"      /* DHCP_HTTP_IP        */
	"vlanid" "\0"      /* DHCP_8021Q_VLAN_ID  */
	"opt125" "\0"      /* DHCP_OPT125 */
#ifdef XAVI_UDHCPC_OPT224
	"opt224" "\0"      /* DHCP_PRIVATE224 */
#endif
#ifdef XAVI_UDHCPC_OPT250_APAS
	"opt250" "\0"      /* DHCP_PRIVATE250 */
#endif
#endif
	;

/* Lengths of the option types in binary form.
 * Used by:
 * udhcp_str2optset: to determine how many bytes to allocate.
 * xmalloc_optname_optval: to estimate string length
 * from binary option length: (option[LEN] / dhcp_option_lengths[opt_type])
 * is the number of elements, multiply in by one element's string width
 * (len_of_option_as_string[opt_type]) and you know how wide string you need.
 */
const uint8_t dhcp_option_lengths[] ALIGN1 = {
	[OPTION_IP] =      4,
	[OPTION_IP_PAIR] = 8,
//	[OPTION_BOOLEAN] = 1,
	[OPTION_STRING] =  1,  /* ignored by udhcp_str2optset */
#if ENABLE_FEATURE_UDHCP_RFC3397
	[OPTION_DNS_STRING] = 1,  /* ignored by both udhcp_str2optset and xmalloc_optname_optval */
	[OPTION_SIP_SERVERS] = 1,
#endif
	[OPTION_U8] =      1,
	[OPTION_U16] =     2,
//	[OPTION_S16] =     2,
	[OPTION_U32] =     4,
	[OPTION_S32] =     4,
	/* Just like OPTION_STRING, we use minimum length here */
	[OPTION_STATIC_ROUTES] = 5,
};


#if defined CONFIG_UDHCP_DEBUG && CONFIG_UDHCP_DEBUG >= 2
static void log_option(const char *pfx, const uint8_t *opt)
{
	if (dhcp_verbose >= 2) {
		char buf[256 * 2 + 2];
		*bin2hex(buf, (void*) (opt + OPT_DATA), opt[OPT_LEN]) = '\0';
		bb_info_msg("%s: 0x%02x %s", pfx, opt[OPT_CODE], buf);
	}
}
#else
# define log_option(pfx, opt) ((void)0)
#endif

unsigned FAST_FUNC udhcp_option_idx(const char *name)
{
	int n = index_in_strings(dhcp_option_strings, name);
	if (n >= 0)
		return n;

	{
		char buf[sizeof(dhcp_option_strings)];
		char *d = buf;
		const char *s = dhcp_option_strings;
		while (s < dhcp_option_strings + sizeof(dhcp_option_strings) - 2) {
			*d++ = (*s == '\0' ? ' ' : *s);
			s++;
		}
		*d = '\0';
		bb_error_msg_and_die("unknown option '%s', known options: %s", name, buf);
	}
}

/* Get an option with bounds checking (warning, result is not aligned) */
uint8_t* FAST_FUNC udhcp_get_option(struct dhcp_packet *packet, int code)
{
	uint8_t *optionptr;
	int len;
	int rem;
	int overload = 0;
	enum {
		FILE_FIELD101  = FILE_FIELD  * 0x101,
		SNAME_FIELD101 = SNAME_FIELD * 0x101,
	};

	/* option bytes: [code][len][data1][data2]..[dataLEN] */
	optionptr = packet->options;
	rem = sizeof(packet->options);
	while (1) {
		if (rem <= 0) {
			bb_error_msg("bad packet, malformed option field");
			return NULL;
		}
		if (optionptr[OPT_CODE] == DHCP_PADDING) {
			rem--;
			optionptr++;
			continue;
		}
		if (optionptr[OPT_CODE] == DHCP_END) {
			if ((overload & FILE_FIELD101) == FILE_FIELD) {
				/* can use packet->file, and didn't look at it yet */
				overload |= FILE_FIELD101; /* "we looked at it" */
				optionptr = packet->file;
				rem = sizeof(packet->file);
				continue;
			}
			if ((overload & SNAME_FIELD101) == SNAME_FIELD) {
				/* can use packet->sname, and didn't look at it yet */
				overload |= SNAME_FIELD101; /* "we looked at it" */
				optionptr = packet->sname;
				rem = sizeof(packet->sname);
				continue;
			}
			break;
		}
		len = 2 + optionptr[OPT_LEN];
		rem -= len;
		if (rem < 0)
			continue; /* complain and return NULL */

		if (optionptr[OPT_CODE] == code) {
			log_option("Option found", optionptr);
			return optionptr + OPT_DATA;
		}

		if (optionptr[OPT_CODE] == DHCP_OPTION_OVERLOAD) {
			overload |= optionptr[OPT_DATA];
			/* fall through */
		}
		optionptr += len;
	}

	/* log3 because udhcpc uses it a lot - very noisy */
	log3("Option 0x%02x not found", code);
	return NULL;
}

/* Return the position of the 'end' option (no bounds checking) */
int FAST_FUNC udhcp_end_option(uint8_t *optionptr)
{
	int i = 0;

	while (optionptr[i] != DHCP_END) {
		if (optionptr[i] != DHCP_PADDING)
			i += optionptr[i + OPT_LEN] + OPT_DATA-1;
		i++;
	}
	return i;
}

/* Add an option (supplied in binary form) to the options.
 * Option format: [code][len][data1][data2]..[dataLEN]
 */
void FAST_FUNC udhcp_add_binary_option(struct dhcp_packet *packet, uint8_t *addopt)
{
	unsigned len;
	uint8_t *optionptr = packet->options;
	unsigned end = udhcp_end_option(optionptr);

	len = OPT_DATA + addopt[OPT_LEN];
	/* end position + (option code/length + addopt length) + end option */
	if (end + len + 1 >= DHCP_OPTIONS_BUFSIZE) {
//TODO: learn how to use overflow option if we exhaust packet->options[]
		bb_error_msg("option 0x%02x did not fit into the packet",
				addopt[OPT_CODE]);
		return;
	}
	log_option("Adding option", addopt);
	memcpy(optionptr + end, addopt, len);
	optionptr[end + len] = DHCP_END;
}

/* Add an one to four byte option to a packet */
void FAST_FUNC udhcp_add_simple_option(struct dhcp_packet *packet, uint8_t code, uint32_t data)
{
	const struct dhcp_optflag *dh;

	for (dh = dhcp_optflags; dh->code; dh++) {
		if (dh->code == code) {
			uint8_t option[6], len;

			option[OPT_CODE] = code;
			len = dhcp_option_lengths[dh->flags & OPTION_TYPE_MASK];
			option[OPT_LEN] = len;
			if (BB_BIG_ENDIAN)
				data <<= 8 * (4 - len);
			/* Assignment is unaligned! */
			move_to_unaligned32(&option[OPT_DATA], data);
			udhcp_add_binary_option(packet, option);
			return;
		}
	}

	bb_error_msg("can't add option 0x%02x", code);
}

/* Find option 'code' in opt_list */
struct option_set* FAST_FUNC udhcp_find_option(struct option_set *opt_list, uint8_t code)
{
	while (opt_list && opt_list->data[OPT_CODE] < code)
		opt_list = opt_list->next;

	if (opt_list && opt_list->data[OPT_CODE] == code)
		return opt_list;
	return NULL;
}

/* Parse string to IP in network order */
int FAST_FUNC udhcp_str2nip(const char *str, void *arg)
{
	len_and_sockaddr *lsa;

	lsa = host_and_af2sockaddr(str, 0, AF_INET);
	if (!lsa)
		return 0;
	*(uint32_t*)arg = lsa->u.sin.sin_addr.s_addr;
	free(lsa);
	return 1;
}

/* udhcp_str2optset:
 * Parse string option representation to binary form and add it to opt_list.
 * Called to parse "udhcpc -x OPTNAME:OPTVAL"
 * and to parse udhcpd.conf's "opt OPTNAME OPTVAL" directives.
 */
/* helper for the helper */
static char *allocate_tempopt_if_needed(
		const struct dhcp_optflag *optflag,
		char *buffer,
		int *length_p)
{
	char *allocated = NULL;
	if ((optflag->flags & OPTION_TYPE_MASK) == OPTION_BIN) {
		const char *end;
		allocated = xstrdup(buffer); /* more than enough */
		end = hex2bin(allocated, buffer, 255);
		if (errno)
			bb_error_msg_and_die("malformed hex string '%s'", buffer);
		*length_p = end - allocated;
	}
	return allocated;
}
/* helper: add an option to the opt_list */
static NOINLINE void attach_option(
		struct option_set **opt_list,
		const struct dhcp_optflag *optflag,
		char *buffer,
		int length)
{
	struct option_set *existing, *new, **curr;
	char *allocated = NULL;

	existing = udhcp_find_option(*opt_list, optflag->code);
	if (!existing) {
		log2("Attaching option %02x to list", optflag->code);
		allocated = allocate_tempopt_if_needed(optflag, buffer, &length);
#if ENABLE_FEATURE_UDHCP_RFC3397
		if ((optflag->flags & OPTION_TYPE_MASK) == OPTION_DNS_STRING) {
			/* reuse buffer and length for RFC1035-formatted string */
			allocated = buffer = (char *)dname_enc(NULL, 0, buffer, &length);
		}
#endif
		/* make a new option */
		new = xmalloc(sizeof(*new));
		new->data = xmalloc(length + OPT_DATA);
		new->data[OPT_CODE] = optflag->code;
		new->data[OPT_LEN] = length;
		memcpy(new->data + OPT_DATA, buffer, length);

		curr = opt_list;
		while (*curr && (*curr)->data[OPT_CODE] < optflag->code)
			curr = &(*curr)->next;

		new->next = *curr;
		*curr = new;
		goto ret;
	}

	if (optflag->flags & OPTION_LIST) {
		unsigned old_len;

		/* add it to an existing option */
		log2("Attaching option %02x to existing member of list", optflag->code);
		allocated = allocate_tempopt_if_needed(optflag, buffer, &length);
		old_len = existing->data[OPT_LEN];
#if ENABLE_FEATURE_UDHCP_RFC3397
		if ((optflag->flags & OPTION_TYPE_MASK) == OPTION_DNS_STRING) {
			/* reuse buffer and length for RFC1035-formatted string */
			allocated = buffer = (char *)dname_enc(existing->data + OPT_DATA, old_len, buffer, &length);
		}
#endif
		if (old_len + length < 255) {
			/* actually 255 is ok too, but adding a space can overlow it */

			existing->data = xrealloc(existing->data, OPT_DATA + 1 + old_len + length);
			if ((optflag->flags & OPTION_TYPE_MASK) == OPTION_STRING) {
				/* add space separator between STRING options in a list */
				existing->data[OPT_DATA + old_len] = ' ';
				old_len++;
			}
			memcpy(existing->data + OPT_DATA + old_len, buffer, length);
			existing->data[OPT_LEN] = old_len + length;
		} /* else, ignore the data, we could put this in a second option in the future */
	} /* else, ignore the new data */

 ret:
	free(allocated);
}

int FAST_FUNC udhcp_str2optset(const char *const_str, void *arg)
{
	struct option_set **opt_list = arg;
	char *opt, *val, *endptr;
	char *str;
	const struct dhcp_optflag *optflag;
	struct dhcp_optflag bin_optflag;
	unsigned optcode;
	int retval, length;
	char buffer[8] ALIGNED(4);
	uint16_t *result_u16 = (uint16_t *) buffer;
	uint32_t *result_u32 = (uint32_t *) buffer;

	/* Cheat, the only *const* str possible is "" */
	str = (char *) const_str;
	opt = strtok(str, " \t=");
	if (!opt)
		return 0;

	optcode = bb_strtou(opt, NULL, 0);
	if (!errno && optcode < 255) {
		/* Raw (numeric) option code */
		bin_optflag.flags = OPTION_BIN;
		bin_optflag.code = optcode;
		optflag = &bin_optflag;
	} else {
		optflag = &dhcp_optflags[udhcp_option_idx(opt)];
	}

	retval = 0;
	do {
		val = strtok(NULL, ", \t");
		if (!val)
			break;
		length = dhcp_option_lengths[optflag->flags & OPTION_TYPE_MASK];
		retval = 0;
		opt = buffer; /* new meaning for variable opt */
		switch (optflag->flags & OPTION_TYPE_MASK) {
		case OPTION_IP:
			retval = udhcp_str2nip(val, buffer);
			break;
		case OPTION_IP_PAIR:
			retval = udhcp_str2nip(val, buffer);
			val = strtok(NULL, ", \t/-");
			if (!val)
				retval = 0;
			if (retval)
				retval = udhcp_str2nip(val, buffer + 4);
			break;
		case OPTION_STRING:
#if ENABLE_FEATURE_UDHCP_RFC3397
		case OPTION_DNS_STRING:
#endif
			length = strnlen(val, 254);
			if (length > 0) {
				opt = val;
				retval = 1;
			}
			break;
//		case OPTION_BOOLEAN: {
//			static const char no_yes[] ALIGN1 = "no\0yes\0";
//			buffer[0] = retval = index_in_strings(no_yes, val);
//			retval++; /* 0 - bad; 1: "no" 2: "yes" */
//			break;
//		}
		case OPTION_U8:
			buffer[0] = strtoul(val, &endptr, 0);
			retval = (endptr[0] == '\0');
			break;
		/* htonX are macros in older libc's, using temp var
		 * in code below for safety */
		/* TODO: use bb_strtoX? */
		case OPTION_U16: {
			unsigned long tmp = strtoul(val, &endptr, 0);
			*result_u16 = htons(tmp);
			retval = (endptr[0] == '\0' /*&& tmp < 0x10000*/);
			break;
		}
//		case OPTION_S16: {
//			long tmp = strtol(val, &endptr, 0);
//			*result_u16 = htons(tmp);
//			retval = (endptr[0] == '\0');
//			break;
//		}
		case OPTION_U32: {
			unsigned long tmp = strtoul(val, &endptr, 0);
			*result_u32 = htonl(tmp);
			retval = (endptr[0] == '\0');
			break;
		}
		case OPTION_S32: {
			long tmp = strtol(val, &endptr, 0);
			*result_u32 = htonl(tmp);
			retval = (endptr[0] == '\0');
			break;
		}
		case OPTION_BIN: /* handled in attach_option() */
			opt = val;
			retval = 1;
		default:
			break;
		}
		if (retval)
			attach_option(opt_list, optflag, opt, length);
	} while (retval && optflag->flags & OPTION_LIST);

	return retval;
}
