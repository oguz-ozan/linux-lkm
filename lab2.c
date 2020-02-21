#include <linux/init.h>
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/module.h> //try_module_get, module_put
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/fs.h> //file_operations
#include <asm/uaccess.h> //memory access
#include <linux/uaccess.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DEUCENG");
MODULE_DESCRIPTION("An example Linux kernel module.");
MODULE_VERSION("0.01");

#define DEVICE_NAME "deuceng"
#define MSG_BUFFER_LEN 250
#define procfs_name "cpuinfo"


/* Prototypes for device functions */
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

int procfile_read(char *buffer,  char **buffer_location,  off_t offset, int buffer_length, int *eof, void *data){
int ret;
static char my_buffer[4096];

if (offset > 0) {

    ret  = 0;
} else {

    ret = sprintf(my_buffer, "asdasdasd");
}

*buffer_location=my_buffer;
return ret;
}

static int major_num;
static int device_open_count = 0;
static short  size_of_message;
static char   message[256] = {0};           ///< Memory for the string that is passed from userspace

static int myPid = 0;
static char *allOption = "no";
static char *processOption = "no";

module_param(myPid, int, 0660);
module_param(allOption, charp, 0660);
module_param(processOption, charp, 0660);

/* This structure points to all of the device functions, file_operations structure is defined in /linux/fs.h */
static struct file_operations file_ops = {
 .read = device_read,
 .write = device_write,
 .open = device_open,
 .release = device_release
};

/* When a process reads from our device, this gets called. */
static ssize_t device_read(struct file *flip, char *buffer, size_t len, loff_t *offset) {
int error_count = 0;
   // copy_to_user has the format ( * to, *from, size) and returns 0 on success
   error_count = copy_to_user(buffer, message, size_of_message);
   if (error_count==0){            // if true then have success
         printk(KERN_INFO "EBBChar: Sent %d characters to the user, and the message is: %s\n", size_of_message,buffer);
      return (size_of_message=0);  // clear the position to the start and return 0
   }
   else {
      printk(KERN_INFO "EBBChar: Failed to send %d characters to the user\n", error_count);
      return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
   }
}

/* Called when a process tries to write to our device */
static ssize_t device_write(struct file *flip, const char *buffer, size_t len, loff_t *offset) {
        sprintf(message, "%s(%zu letters)", buffer, len);   // appending received string with its length
        size_of_message = strlen(message);                 // store the length of the stored message
        printk(KERN_INFO "EBBChar: Received %zu characters from the user\n", len);
        printk(KERN_INFO "message is: %s, and size of the message is: %d\n",message,size_of_message);
        return len;
}

/* Called when a process opens our device */
static int device_open(struct inode *inode, struct file *file) {
        /* If device is open, return busy */
        if (device_open_count) {
                return -EBUSY; //device or resource is busy
        }
        device_open_count++;
        try_module_get(THIS_MODULE); //before calling into module code. if it fails, the module is being removed. Otherwise, you can safely enter module
        return 0;
}

/* Called when a process closes our device */
static int device_release(struct inode *inode, struct file *file) {
        /* Decrement the open counter and usage count. Without this, the module would not unload. */
        device_open_count--;
        module_put(THIS_MODULE); //when you are finished the usage of module
        return 0;
}

static int __init lab2_init(void) {
        /* Try to register character device, register a major number for character devices */
        major_num = register_chrdev(0, "lab2", &file_ops); //dynamically allocate a major and return its major number
        if (major_num < 0) {
                printk(KERN_ALERT "Could not register device: %d", major_num);
                return major_num;
        } else {
                printk(KERN_INFO "lab2 module loaded with device major number %d\n", major_num);
                printk(KERN_INFO "string is %s, process id is %d, process is %s\n", allOption, myPid,processOption);
                return 0;
        }
}

static void __exit lab2_exit(void) {
        /* Remember — we have to clean up after ourselves. Unregister the character device. */
        unregister_chrdev(major_num, DEVICE_NAME);
        printk(KERN_INFO "Goodbye, World!\n");
}

/* Register module functions */
module_init(lab2_init);
module_exit(lab2_exit);
