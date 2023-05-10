#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

static int len = 0, maxLen = 64, unreaded = 0;

static char *msg;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Igor Anufriev");
MODULE_DESCRIPTION("A module that works with `/proc/` directory");

ssize_t read_proc(struct file *filp, char *buf, size_t count, loff_t *offp) {
  int notCopied;
  if (unreaded == 0) {
    unreaded = len;
    return 0;
  }
  if (count > unreaded) {
    count = unreaded;
  }
  notCopied = (int)copy_to_user(buf, msg, count);
  unreaded -= count - notCopied;

  return count - notCopied;
}

ssize_t write_proc(struct file *filp, const char *buf, size_t count,
                   loff_t *offp) {
  int notCopied;
  if (count > maxLen) {
    count = maxLen;
  }
  notCopied = (int)copy_from_user(msg, buf, count);
  len = count - notCopied;
  unreaded = len;
  return len;
}

const struct proc_ops proc_fops = {.proc_read = read_proc,
                                   .proc_write = write_proc};

int __init proc_init(void) {
  proc_create("hello", 0666, NULL, &proc_fops);
  msg = kmalloc(64 * sizeof(char), GFP_KERNEL);
  return 0;
}

void __exit proc_cleanup(void) {
  remove_proc_entry("hello", NULL);
  kfree(msg);
}

module_init(proc_init);
module_exit(proc_cleanup);
