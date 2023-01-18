
#ifndef _SEC_CMD_H_
#define _SEC_CMD_H_

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/workqueue.h>
#include <linux/stat.h>
#include <linux/err.h>
#include <linux/input.h>
#ifdef CONFIG_SEC_SYSFS
#include <linux/sec_sysfs.h>
#endif

#ifndef CONFIG_SEC_FACTORY
#include <linux/kfifo.h>
#endif

#ifndef CONFIG_SEC_SYSFS
extern struct class *sec_class;
#endif

#define SEC_CLASS_DEVT_TSP		10
#define SEC_CLASS_DEVT_TKEY		11
#define SEC_CLASS_DEVT_WACOM		12

#define SEC_CLASS_DEV_NAME_TSP		"tsp"
#define SEC_CLASS_DEV_NAME_TKEY		"sec_touchkey"
#define SEC_CLASS_DEV_NAME_WACOM	"sec_epen"

#define SEC_CMD(name, func)		.cmd_name = name, .cmd_func = func

#define SEC_CMD_BUF_SIZE		(4096 - 1)
#define SEC_CMD_STR_LEN			256
#define SEC_CMD_RESULT_STR_LEN		(4096 - 1)
#define SEC_CMD_PARAM_NUM		8

struct sec_cmd {
	struct list_head	list;
	const char		*cmd_name;
	void			(*cmd_func)(void *device_data);
};

enum SEC_CMD_STATUS {
	SEC_CMD_STATUS_WAITING = 0,
	SEC_CMD_STATUS_RUNNING,		// = 1
	SEC_CMD_STATUS_OK,		// = 2
	SEC_CMD_STATUS_FAIL,		// = 3
	SEC_CMD_STATUS_NOT_APPLICABLE,	// = 4
};

#ifdef USE_SEC_CMD_QUEUE
#define SEC_CMD_MAX_QUEUE	10

struct command {
	char	cmd[SEC_CMD_STR_LEN];
};
#endif

struct sec_cmd_data {
	struct device		*fac_dev;
	struct list_head	cmd_list_head;
	u8			cmd_state;
	char			cmd[SEC_CMD_STR_LEN];
	int			cmd_param[SEC_CMD_PARAM_NUM];
	char			cmd_result[SEC_CMD_RESULT_STR_LEN];
	char			cmd_result_2[SEC_CMD_RESULT_STR_LEN];
	int			cmd_buffer_size;
	atomic_t		cmd_is_running;
#ifdef USE_SEC_CMD_QUEUE
	struct kfifo		cmd_queue;
	struct mutex		fifo_lock;
#endif
};

extern void sec_cmd_set_cmd_exit(struct sec_cmd_data *data);
extern void sec_cmd_set_default_result(struct sec_cmd_data *data);
extern void sec_cmd_set_cmd_result(struct sec_cmd_data *data, char *buff,
				   int len);
extern void sec_cmd_set_cmd_result_2(struct sec_cmd_data *data, char *buff,
				   int len);
extern int sec_cmd_init(struct sec_cmd_data *data,
				struct sec_cmd *cmds, int len, int devt);
extern void sec_cmd_exit(struct sec_cmd_data *data, int devt);

/*
 * sec Log
 */
#define SECLOG			"[sec_input]"
#define INPUT_LOG_BUF_SIZE	512

#ifdef CONFIG_SEC_DEBUG_TSP_LOG
#include <linux/sec_debug.h>

#define input_dbg(mode, dev, fmt, ...)					       \
({									       \
	static char input_log_buf[INPUT_LOG_BUF_SIZE];			       \
	snprintf(input_log_buf, sizeof(input_log_buf), "%s %s", SECLOG, fmt);  \
	dev_dbg(dev, input_log_buf, ## __VA_ARGS__);			       \
	if (mode) {							       \
		if (dev)						       \
			snprintf(input_log_buf, sizeof(input_log_buf), "%s %s",\
					dev_driver_string(dev), dev_name(dev));\
		else							       \
			snprintf(input_log_buf, sizeof(input_log_buf), "NULL");\
		sec_debug_tsp_log_msg(input_log_buf, fmt, ## __VA_ARGS__);     \
	}								       \
})
#define input_info(mode, dev, fmt, ...)					       \
({									       \
	static char input_log_buf[INPUT_LOG_BUF_SIZE];			       \
	snprintf(input_log_buf, sizeof(input_log_buf), "%s %s", SECLOG, fmt);  \
	dev_info(dev, input_log_buf, ## __VA_ARGS__);			       \
	if (mode) {							       \
		if (dev)						       \
			snprintf(input_log_buf, sizeof(input_log_buf), "%s %s",\
					dev_driver_string(dev), dev_name(dev));\
		else							       \
			snprintf(input_log_buf, sizeof(input_log_buf), "NULL");\
		sec_debug_tsp_log_msg(input_log_buf, fmt, ## __VA_ARGS__);     \
	}								       \
})
#define input_err(mode, dev, fmt, ...)					       \
({									       \
	static char input_log_buf[INPUT_LOG_BUF_SIZE];			       \
	snprintf(input_log_buf, sizeof(input_log_buf), "%s %s", SECLOG, fmt);  \
	dev_err(dev, input_log_buf, ## __VA_ARGS__);			       \
	if (mode) {							       \
		if (dev)						       \
			snprintf(input_log_buf, sizeof(input_log_buf), "%s %s",\
					dev_driver_string(dev), dev_name(dev));\
		else							       \
			snprintf(input_log_buf, sizeof(input_log_buf), "NULL");\
		sec_debug_tsp_log_msg(input_log_buf, fmt, ## __VA_ARGS__);     \
	}								       \
})
#define input_log_fix() {}
#else
#define input_dbg(mode, dev, fmt, ...)					       \
({									       \
	static char input_log_buf[INPUT_LOG_BUF_SIZE];			       \
	snprintf(input_log_buf, sizeof(input_log_buf), "%s %s", SECLOG, fmt);  \
	dev_dbg(dev, input_log_buf, ## __VA_ARGS__);			       \
})
#define input_info(mode, dev, fmt, ...)					       \
({									       \
	static char input_log_buf[INPUT_LOG_BUF_SIZE];			       \
	snprintf(input_log_buf, sizeof(input_log_buf), "%s %s", SECLOG, fmt);  \
	dev_info(dev, input_log_buf, ## __VA_ARGS__);			       \
})
#define input_err(mode, dev, fmt, ...)					       \
({									       \
	static char input_log_buf[INPUT_LOG_BUF_SIZE];			       \
	snprintf(input_log_buf, sizeof(input_log_buf), "%s %s", SECLOG, fmt);  \
	dev_err(dev, input_log_buf, ## __VA_ARGS__);			       \
})
#define input_log_fix() {}
#endif

#endif /* _SEC_CMD_H_ */


