#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include <linux/ioctl.h>
#include <linux/cpufreq.h>


#define WR_VALUE _IOW('a',1,int32_t*)
#define RD_VALUE _IOR('a',2,int32_t*)
#define NVPOWER  _IOR('a',3,struct nvpower *)



dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;

static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);
static int nv_open(struct inode *inode, struct file *file);
static int nv_release(struct inode *inode, struct file *file);
static ssize_t nv_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t nv_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long nv_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = nv_read,
        .write          = nv_write,
        .open           = nv_open,
        .unlocked_ioctl = nv_ioctl,
        .release        = nv_release,
};
static int nv_open(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Opened...!!!\n");
        return 0;
}

static int nv_release(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Closed...!!!\n");
        return 0;
}

static ssize_t nv_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Read Function\n");
        return 0;
}
static ssize_t nv_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Write function\n");
        return 0;
}







static long nv_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct nvpower *ioctl_p;//*ptr;
        	//ptr=&ioctl_p;
	printk(KERN_ALERT "inside my ioctl....\n");
        switch(cmd) {
		pr_info("File: %s, Function:%s, Line:%d \n", __FILE__, __FUNCTION__, __LINE__);
                case NVPOWER:
			ioctl_p = demo_nvcpufreq();

			pr_info("File: %s, Function:%s, Line:%d,  cpus:%d, cpufreq:%d \n", __FILE__, __FUNCTION__, __LINE__, ioctl_p->nv_cpus, ioctl_p->nv_cpufreq);	
			
			if(copy_to_user((struct nvpower *) arg, ioctl_p , sizeof(struct nvpower))<0);
			 
			{

				return -EACCES;


			}


		break;

		 default:printk(KERN_INFO "invalid command....\n");
                        break;
        }
        return 0;
}



static int __init etx_driver_init(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 2, "nv_Dev")) <0){
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

        /*Creating cdev structure*/
        cdev_init(&etx_cdev,&fops);

        /*Adding character device to the system*/
        if((cdev_add(&etx_cdev,dev,1)) < 0){
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto r_class;
        }

        /*Creating struct class*/
        if((dev_class = class_create(THIS_MODULE,"nv_class")) == NULL){
            printk(KERN_INFO "Cannot create the struct class\n");
            goto r_class;
        }

        /*Creating device*/
        if((device_create(dev_class,NULL,dev,NULL,"nv_device")) == NULL){
            printk(KERN_INFO "Cannot create the Device 1\n");
            goto r_device;
        }
        printk(KERN_INFO "Device Driver Insert...Done!!!\n");
    return 0;

r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}

void __exit etx_driver_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&etx_cdev);
        unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("gajananj@spanidea.com ");
MODULE_DESCRIPTION("A simple nvpower driver");

            
