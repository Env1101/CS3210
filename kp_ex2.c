/* 
 * Simple kernel module that intercepts the sys_mkdir() system call.
 * Also shows how to obtain user ID and process ID correctly in kernel verison 3.13
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>

#define THE_UID 1000

/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp;

/* kprobe pre_handler: called just before the probed instruction is executed */
static int sysmon_intercept_before(struct kprobe *p, struct pt_regs *regs)
{
  int ret = 0;
  kuid_t cuid = current_uid();
  pid_t pid = task_pid_nr(current);
  printk(KERN_INFO "my sysmon_intercept_before: uid = %d, pid = %d, regs->ax = %lu, regs->di = %s\n", 
    cuid, pid, regs->ax, regs->di);
  printk(KERN_INFO "__NR_mkdir: %lu\n", __NR_mkdir);
  return 0;
}

/* kprobe post_handler: called after the probed instruction is executed */
static void sysmon_intercept_after(struct kprobe *p, struct pt_regs *regs,
				unsigned long flags)
{
  printk(KERN_INFO "my sysmon_intercept_after\n");
}

/*
 * fault_handler: this is called if an exception is generated for any
 * instruction within the pre- or post-handler, or when Kprobes
 * single-steps the probed instruction.
 */
static int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
	printk(KERN_INFO "fault_handler: p->addr = 0x%p, trap #%dn",
		p->addr, trapnr);
	/* Return 0 because we don't handle the fault. */
	return 0;
}

static int __init kprobe_init(void)
{
  int ret;
	kp.symbol_name = "sys_mkdir";
  kp.pre_handler = sysmon_intercept_before;
  kp.post_handler = sysmon_intercept_after;
  ret = register_kprobe(&kp);
  if (ret < 0){
    printk(KERN_INFO "register_kbrobe failed, returned %d\n", ret);
    return ret;
  }
  printk(KERN_INFO "Planted kprobe at %d\n", kp.addr);
  return 0;
}

static void __exit kprobe_exit(void)
{
	unregister_kprobe(&kp);
	printk(KERN_INFO "kprobe at %p unregistered\n", kp.addr);
}

module_init(kprobe_init)
module_exit(kprobe_exit)
MODULE_LICENSE("GPL");