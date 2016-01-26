#include <linux/init.h> // __init and __exit
#include <linux/kernel.h>
#include <linux/module.h> // stuff needed for kernel business.
#include <linux/moduleparam.h>
#include <linux/fs.h>     // Need this for file operations
#include <linux/miscdevice.h> //need this to register as a dev
#include <linux/slab.h>   // Provides kzalloc
#include <linux/sched.h> // wait queues
#include <linux/uaccess.h> // getting the data from user
#include <linux/string.h> // string manipulation.

MODULE_LICENSE("GPL"); // Need this for functionality.
MODULE_AUTHOR("Stephen Moo-Young & Michael Hernandez");
MODULE_DESCRIPTION("In-kernel phrase reverser and uppercase converter");

static unsigned long buffer_size = 8192;
module_param(buffer_size, ulong, (S_IRUSR | S_IRGRP | S_IROTH)); // Required so we can pass buffer_size
																// as a parameter when we install this
																// to the kernel.
MODULE_PARM_DESC(buffer_size, "Internal buffer size");


// This is the main struct that handles all the data being passed to
// this module. 
struct buffer {
	wait_queue_head_t read_queue;
	char *data, *end, *read_ptr;
	unsigned long size;
};

// Initializes the buffer essentially with the given buffer_size
static struct buffer *buffer_alloc(unsigned long size) {
	struct buffer *buf;
	buf = kzalloc(sizeof(*buf), GFP_KERNEL); //kzalloc allocates memory of this size and sets it to zero.
	if(unlikely(!buf))
		goto out;

	buf->data = kzalloc(size, GFP_KERNEL); // Allocates the set amount of memory we've determined the buffer to hold
	if (unlikely(!buf->data))
	goto out_free;	

	init_waitqueue_head(&buf->read_queue); // Required for read function added functionality.
										   // Further elaborated on in the read function.
	out:
		return buf;

	out_free:
		kfree(buf);
		return NULL;
}

static void buffer_free(struct buffer *buffer) {
	kfree(buffer->data);
	kfree(buffer);
}

// Reverses the entire given string by way of reassembling pointers
// We've decided to switch on every other char in the string
static inline char *reverse_word(char *start, char *end) {
	char *orig_start = start, tmp;
	
	for (; start < end; start++, end--) {
		tmp = *start;
		*start = *end;
		*end = tmp;
		start++;
		end--;
	}
	
	return orig_start;
}

// Hard code for uppercasing since the simpler way didn't work
static char uppercasing(char lowercasing)
{
	if(lowercasing == 'a')
		return 'A';
	else if(lowercasing == 'b')
		return 'B';
	else if(lowercasing == 'c')
		return 'C';
	else if(lowercasing == 'd')
		return 'D';
	else if(lowercasing == 'e')
		return 'E';
	else if(lowercasing == 'e')
		return 'E';
	else if(lowercasing == 'f')
		return 'F';
	else if(lowercasing == 'g')
		return 'G';
	else if(lowercasing == 'h')
		return 'H';
	else if(lowercasing == 'i')
		return 'I';
	else if(lowercasing == 'j')
		return 'J';
	else if(lowercasing == 'k')
		return 'K';
	else if(lowercasing == 'l')
		return 'L';
	else if(lowercasing == 'm')
		return 'M';
	else if(lowercasing == 'n')
		return 'N';
	else if(lowercasing == 'o')
		return 'O';
	else if(lowercasing == 'p')
		return 'P';
	else if(lowercasing == 'q')
		return 'Q';
	else if(lowercasing == 'r')
		return 'R';
	else if(lowercasing == 's')
		return 'S';
	else if(lowercasing == 't')
		return 'T';
	else if(lowercasing == 'u')
		return 'U';
	else if(lowercasing == 'v')
		return 'V';
	else if(lowercasing == 'w')
		return 'W';
	else if(lowercasing == 'x')
		return 'X';
	else if(lowercasing == 'y')
		return 'Y';
	else if(lowercasing == 'z')
		return 'Z';
	else
		return lowercasing;
}

// Takes in the entire string and uppercases it.
static char *upper_word(char *start, char *end) {
	char *orig_start = start;
	
	for (; start < end; start++)
	{
		*start = uppercasing(*start);
	}
	
	
	return orig_start;
}

// Whenever Open(2) is called, allocate the memory to the buffer, and
// then assign the buffer to handle the data for the file.
static int upperReverse_open(struct inode *inode, struct file *file) {
	int err = 0;
	struct buffer *buf;
	
	buf = buffer_alloc(buffer_size);
	
	if (unlikely(!buf)) {
		err = -ENOMEM;
		goto out;
	}
	
	file->private_data = buf;
	
	out:
		return err;
}


// The function for whenever read(3) is called.
static ssize_t upperReverse_read(struct file *file, char __user * out,
	size_t size, loff_t * off) {
	struct buffer *buf = file->private_data; // Retrieve the assigned buffer.
	ssize_t result;
	
	// This while loop is here just in case we were to go for added functionality
	// Like... if we wanted to fork or something. Mutex stuff.
	// Straight up contained error checking.
	// Too scared to comment it out.
	while (buf->read_ptr == buf->end){
		
		// If there's no data available yet.
		if(file->f_flags & O_NONBLOCK){
			result = -EAGAIN;
			goto out;
		}
		
		// If it's sleeping while the reading pointer isn't at
		// the end, it throws an error pretty much.
		if(wait_event_interruptible
			(buf->read_queue, buf->read_ptr != buf->end)){
				result = -ERESTARTSYS;
				goto out;
		}
	}
	
	// Takes whatever's in the buffer, and outputs it into userspace.
	size = min(size, (size_t)(buf->end - buf->read_ptr));
	if(copy_to_user(out, buf->read_ptr, size)) {
		result = -EFAULT;
		goto out;
	}
	
	// Puts the buffers initial read pointer to the end from the beginning.
	// The data we had initially is no longer needed.
	buf->read_ptr += size;
	result = size;
	
	out:
		return result;
}

// The function for write(3)
static ssize_t upperReverse_write(struct file *file, const char __user * in,
								size_t size, loff_t * off) {
	struct buffer *buf = file->private_data;
	ssize_t result;
	
	// Input too big
	if (size > buffer_size) {
		result = -EFBIG;
		goto out;
	}
	
	// Get data from user and put in the buffer
	copy_from_user(buf->data, in, size);
	
	buf->end = buf->data + size;
	buf->read_ptr = buf->data; // We need to keep track of the initial
							   // pointer since data is going to be
							   // altered.
	
	// This if statement only doesn't ring true if there's
	// only one char, with which these parameters would be out
	// of bounds.
	if (buf->end > buf->data) {
		reverse_word(buf->data, buf->end - 1);
		upper_word(buf->data, buf->end - 1);
	}
	
	// Not necessary for the purposes of this project.	
	wake_up_interruptible(&buf->read_queue);
	
	result = size;
	
	out:
		return result;
}

static int upperReverse_close(struct inode *inode, struct file *file) {
	struct buffer *buf = file->private_data;
	
	buffer_free(buf);
	
	return 0;
}

// This assigns the functions to their respective calls.
// Owner and llseek are there for semantic purposes.
static struct file_operations upperReverse_fops = {
	.owner = THIS_MODULE,
	.open = upperReverse_open,
	.read = upperReverse_read,
	.write = upperReverse_write,
	.release = upperReverse_close,
	.llseek = noop_llseek
};

// The data for the device object.
static struct miscdevice upperReverse_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "upperReverse",
	.fops = &upperReverse_fops
};

// Functions called on insertion and removal from the kernel.
// Text only displays on a dmesg command.
static int __init upperReverse_init(void) {
	if(!buffer_size)
		return -1;
	
	misc_register(&upperReverse_misc_device);
	printk(KERN_INFO "uppercase and reverse device has been registered\n");
	return 0;
}

static void __exit upperReverse_exit(void) {
	misc_deregister(&upperReverse_misc_device);
	printk(KERN_INFO "uppercase and reverse device has been unregistered\n");
}

module_init(upperReverse_init);
module_exit(upperReverse_exit);
