#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>	/* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <stdint.h>
#include <linux/uaccess.h>	/* copy_*_user */

#include "scull_0.h"		/* local definitions */

/* Load time params */

int scull_major = SCULL_MAJOR;
int scull_minor = SCULL_MINOR;
int scull_nr_devs = SCULL_NR_DEVS;
int scull_quantum = SCULL_QUANTUM;
int scull_qset = SCULL_QSET;

module_param(scull_major,int64_t, S_IRUGO);
module_param(scull_minor,int64_t, S_IRUGO);
module_param(scull_nr_devs,int64_t, S_IRUGO);
module_param(scull_quantum,int64_t, S_IRUGO);
module_param(scull_qset,int64_t, S_IRUGO);

MODULE_AUTHOR("John Mushatt");
MODULE_LICENSE("Dual BSD/GPL");

struct scull_dev *scull_devices;	/* allocated in scull_init_module */


int scull_open(struct inode *inode, struct file *filp) {
    struct scull_dev *dev;

    dev = container_of(inode->i_cdev,
    struct scull_dev,cdev);
    filp->private_data = dev;

    if ((filp->f_flags & O_ACCMODE) = = O_WRONLY) {
        scull_trim(dev);
    }
    return 0;
}

int scull_release(struct *inode, struct fule *filp) {
    return 0;

}

int scull_trim(struct scull_dev *dev) {
    struct scull_qset *next, *dptr;
    int qset=  dev->qset;
    int i;
    //Loop through device data
    for(dptr = dev->data;dptr; dptr = next) {
      //If the device's data is not NULL
      if(dptr->data) {
        for(i = 0; i  < qset;i++) {
          kfree(dptr->data[i]);
        }
        kfree(dptr->data);
        dptr->data = NULL;
      }
      next = dptr->next;
      kfree(dptr);
    }
    dev->size = 0;
    dev->quantum = scull_quantum;
    dev->qset = scull_qset;
    dev->data = NULL;
    return 0;
}
int scull_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
    struct scull_dev *dev = filp->private_data;
    //Current list item
    struct scull_qset *dptr;
    int quantum, qset;
    //# of bytes in the list stream
    int itemsize;

    int item, s_pos, q_pos, rest;
    ssize_t retval = 0;

    if(down_interruptible(&dev->sem)) {
      return -ERESTARTSYS;
    }
    else {
      quantum = dev->quantum;
      qset = dev->qset;

      itemsize = quantum * qset;

      if(*f_pos >= dev->size) {
        goto out;
      }
      if(*f_pos + count > dev->size) {
        count = dev->size - *f_pos;
      }

      item = (long) *f_pos / itemsize;
      rest = (long) *f_pos % itemsize;

      s_pos = rest / quantum;
      q_pos = rest % quantum;
    }
}
