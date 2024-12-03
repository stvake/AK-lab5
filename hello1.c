// SPDX-License-Identifier: Dual BSD/GPL
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>
#include "hello1.h"

MODULE_AUTHOR("Pyrohov Pavlo <pyrohovpavlo05@gmail.com>");
MODULE_DESCRIPTION("Hello1 Module - manages printing and time measurement");
MODULE_LICENSE("Dual BSD/GPL");

struct hello_entry {
	struct list_head list;
	ktime_t start_time;
	ktime_t end_time;
};

static LIST_HEAD(hello_list);

void print_hello(void)
{
	struct hello_entry *entry = kmalloc(sizeof(*entry), GFP_KERNEL);

	if (!entry) {
		pr_err("Error: Memory allocation failed\n");
		return;
	}

	entry->start_time = ktime_get();
	pr_info("Hello, world!\n");
	entry->end_time = ktime_get();

	list_add_tail(&entry->list, &hello_list);
}
EXPORT_SYMBOL(print_hello);

static void cleanup_list(void)
{
	struct hello_entry *entry, *tmp;

	list_for_each_entry_safe(entry, tmp, &hello_list, list) {
		pr_info("Hello took %lld ns\n",
		ktime_to_ns(ktime_sub(entry->end_time, entry->start_time)));
		list_del(&entry->list);
		kfree(entry);
	}
}

static int __init hello1_init(void)
{
	pr_info("Hello1 module loaded\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	cleanup_list();
	pr_info("Hello1 module unloaded\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
