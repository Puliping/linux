#include <linux/linkage.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>


static struct kobject *sys_last_nr_kobject;
extern unsigned long last_nr;

asmlinkage unsigned long sys_last_scno(void){
   return last_nr;
}

static ssize_t last_nr_show(struct kobject *kobj, struct kobj_attribute *attr,
			    char *buf)
{
	return sprintf(buf, "%lu\n", last_nr);
}

static struct kobj_attribute nr_attribute = __ATTR_RO(last_nr);

static struct attribute *attrs[] = {
	&nr_attribute.attr,
	NULL,
};

static int __init sys_nr_init(void)
{
	int retval;
	sys_last_nr_kobject = kobject_create_and_add("sys_last_nr", kernel_kobj);
	
	if (!sys_last_nr_kobject)
		pr_debug("lastnr: can't create kobject");

	retval = sysfs_create_file(sys_last_nr_kobject, *attrs);
	
	if (retval) {
		pr_debug("lastnr: can't create sysfs group");
		kobject_put(sys_last_nr_kobject);
		return retval;
	}

	return 0;
}

static void __exit sys_nr_exit(void)
{
	kobject_put(sys_last_nr_kobject);
}


module_init(sys_nr_init);
module_exit(sys_nr_exit);