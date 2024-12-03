// SPDX-License-Identifier: Dual BSD/GPL
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include "hello1.h"

MODULE_AUTHOR("Pyrohov Pavlo <pyrohovpavlo05@gmail.com>");
MODULE_DESCRIPTION("Hello2 Module - calls hello1 functions");
MODULE_LICENSE("Dual BSD/GPL");

static uint hello_count = 1;
module_param(hello_count, uint, 0444);
MODULE_PARM_DESC(hello_count, "Number of times to print Hello, world!");

static int __init hello2_init(void)
{
	int i;

	if (hello_count == 0 || (hello_count >= 5 && hello_count <= 10)) {
		pr_warn("Warning: Invalid count (%u), is 0 or between 5 and 10\n",
		hello_count);
	} else if (hello_count > 10) {
		pr_err("Error: Count exceeds limit 10, exiting with -EINVAL\n");
		return -EINVAL;
	}

	for (i = 0; i < hello_count; i++)
		print_hello();

	return 0;
}

static void __exit hello2_exit(void)
{
	pr_info("Hello2 module unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
