/*
* hello-3.c - illustrating the __init, __initdata, and __exit macros.
*/

#include <linux/module.h>  /* needed by all modules */
#include <linux/kernel.h>  // needed for KERN_INFO
#include <linux/init.h>   // needed for the macro

static int hello3_data __initdata = 3;

static int __init hello_3_init(void)
{
	printk (KERN_INFO "Hello world %d\n", hello3_data);
	
	// A non 0 return means this function failed, it cannot be loaded

	return 0;
}

static void __exit hello_3_exit(void)
{
	printk (KERN_INFO "Goodbye world 3. \n");
}

module_init (hello_3_init);
module_exit(hello_3_exit);

