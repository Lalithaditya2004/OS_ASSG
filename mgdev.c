#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/version.h> 
#include <linux/cdev.h>

#define DEVICE_NAME "mgdev"
#define BUFFER_SIZE 256

static int major;
static int minor = 0;
static char device_buffer[BUFFER_SIZE] = {0};
static int kernel_version[3] = {0, 0, 0};
static dev_t dev_number;
static struct cdev my_cdev;
module_param_array(kernel_version, int, NULL, S_IRUGO);
MODULE_PARM_DESC(kernel_version, "Kernel version array (major, minor, patch)");


static int device_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device opened\n");
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device closed\n");
    return 0;
}

static ssize_t device_read(struct file *file, char __user *user_buffer, size_t length, loff_t *offset) {
    ssize_t bytes_read = 0;
    printk(KERN_INFO "Read function called\n");

    if (*offset >= BUFFER_SIZE)
        return 0;

    if (*offset + length > BUFFER_SIZE)
        length = BUFFER_SIZE - *offset;

    bytes_read = length - copy_to_user(user_buffer, device_buffer + *offset, length);
    *offset += bytes_read;
    return bytes_read;
}

static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t length, loff_t *offset) {
    ssize_t bytes_written = 0;
    printk(KERN_INFO "Write function called\n");

    if (length > BUFFER_SIZE - 1)
        length = BUFFER_SIZE - 1;

    bytes_written = length - copy_from_user(device_buffer, user_buffer, length);
    device_buffer[length] = '\0';
    return bytes_written;
}

static struct file_operations oops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
};

static int __init Intialize(void) {
int current_ver[3] = {
        (LINUX_VERSION_CODE >> 16) & 0xFF,
        (LINUX_VERSION_CODE >> 8) & 0xFF,
        LINUX_VERSION_CODE & 0xFF
    };

    if (kernel_version[0] != current_ver[0] || kernel_version[1] != current_ver[1] || kernel_version[2] != current_ver[2]) {
        printk(KERN_ERR "Kernel version mismatch: Expected %d.%d.%d, got %d.%d.%d\n",
               current_ver[0], current_ver[1], current_ver[2],
               kernel_version[0], kernel_version[1], kernel_version[2]);
        return -EINVAL;
    }

    if (alloc_chrdev_region(&dev_number, minor, 1, DEVICE_NAME) < 0) {
        printk(KERN_ALERT "Failed to allocate major and minor numbers\n");
        return -1;
    }

    major = MAJOR(dev_number);
    minor = MINOR(dev_number);

    printk(KERN_INFO "Device registered: %s with Major number %d, Minor number %d\n", DEVICE_NAME, major, minor);
	
    cdev_init(&my_cdev, &oops);
    if (cdev_add(&my_cdev, dev_number, 1) < 0) {
        unregister_chrdev_region(dev_number, 1);
        return -1;
    }
    return 0;
}

static void __exit Exit(void) {
    cdev_del(&my_cdev);
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Device unregistered\n");
}

module_init(Intialize);
module_exit(Exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lalithaditya");
MODULE_DESCRIPTION("Character Device Driver with Kernel Version Check and Read/Write Operations");
MODULE_VERSION("1.0");


