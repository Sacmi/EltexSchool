#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Igor Anufriev");
MODULE_DESCRIPTION("A module that prints \"Hello World\" from kernel");

static int __init hello_init(void) {
  printk(KERN_INFO "Hello World for Kernel!\n");
  return 0;
}

static void __exit hello_cleanup(void) { printk(KERN_INFO "Bye bye!\n"); }

module_init(hello_init);
module_exit(hello_cleanup);
