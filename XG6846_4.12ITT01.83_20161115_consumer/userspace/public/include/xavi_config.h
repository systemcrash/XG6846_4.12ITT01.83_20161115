/* This is an automatically generated file. Do not modify. */
/* The config file only define about configure */
/* Dom<dom_hsu@xavi.com.tw> 20100727 Create */
#ifndef _XAVI_CONFIG_H_
#define _XAVI_CONFIG_H_

#define XAVI_FW_BUILD_DATE	get_file_string("cat /etc/build_date")
#define XAVI_VENDOR_NAME	get_file_string("cat /etc/vendor_name")
#define XAVI_MODEL_NAME		get_file_string("cat /etc/model_name")
#define XAVI_SW_VERSION		get_file_string("cat /etc/sw_version")
#define XAVI_DATA_SIZE		64

static char *get_file_string(const char *file_path)
{	static FILE *fp = NULL;
	static char *data_string = NULL, data_line[XAVI_DATA_SIZE];

	if ((fp = popen(file_path, "r")) != NULL)
		fgets(data_line, sizeof(data_line), fp);
	(fp == NULL) ? : pclose(fp);
	data_string = data_line;

	return data_string;
}

#endif  /* _XAVI_CONFIG_H_ */
