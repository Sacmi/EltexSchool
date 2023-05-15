#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define SIZE 64

static int length = 0, maximum_length = 64, unreaded_length = 0;

static char *message;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Igor Anufriev");
MODULE_DESCRIPTION("A module that works with `/proc/` directory");

ssize_t read_proc(struct file *filp, char *buf, size_t count, loff_t *offp) {
  if (unreaded_length == 0) {
    unreaded_length = length;
    return 0;
  }
  
  if (count > unreaded_length) {
    count = unreaded_length;
  }

  int not_copied = copy_to_user(buf, message, count);
  unreaded_length -= count - not_copied;

  return count - not_copied;
}

ssize_t write_proc(struct file *filp, const char *buf, size_t count,
                   loff_t *offp) {
  if (count > maximum_length) {
    count = maximum_length;
  }

  int not_copied = (int)copy_from_user(message, buf, count);
  length = count - not_copied;
  unreaded_length = length;
  
  return length;
}

const struct proc_ops proc_fops = {.proc_read = read_proc,
                                   .proc_write = write_proc};

int __init proc_init(void) {
  proc_create("hello", 0666, NULL, &proc_fops);
  message = kmalloc(SIZE * sizeof(char), GFP_KERNEL);
  return 0;
}

void __exit proc_cleanup(void) {
  remove_proc_entry("hello", NULL);
  kfree(message);
}

module_init(proc_init);
module_exit(proc_cleanup);
