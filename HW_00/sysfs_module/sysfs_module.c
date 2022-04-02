#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/sched.h>

static struct kobject *team15_02_89_object;
volatile int pid = 0;

static ssize_t roots(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
	
	struct task_struct *temp;
	temp = current;
	pid = temp->pid;
	printk("find_roots sysfs opened by process %d", temp->pid);
	while (temp->pid != 1) {
		printk("id: %d, name: %s\n", temp->pid, temp->comm);
		
		temp = temp->real_parent;
	
	}

	printk("id: %d, name: %s\n", temp->pid, temp->comm);
	return sprintf(buf, "%d\n", pid);
}

struct kobj_attribute foo_attribute = __ATTR(find_roots, 0660, roots, NULL);

static int __init mymodule_init (void) {

	int error = 0;

	team15_02_89_object = kobject_create_and_add("team15_02_89", kernel_kobj);

	if (!team15_02_89_object)
		return -ENOMEM;

	error = sysfs_create_file(team15_02_89_object, &foo_attribute.attr);

	if (error) {
		printk("failed to create the foo file in /sys/kernel/team15_02_89 \n");
	}

	return error;
}


static void __exit mymodule_exit(void) {

	printk("Module un initialized successfully \n");
	kobject_put(team15_02_89_object);

}

module_init(mymodule_init);
module_exit(mymodule_exit);
MODULE_LICENSE("GPL");


















