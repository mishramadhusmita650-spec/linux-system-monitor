#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/ktime.h>
#include <linux/cpu.h>
#include <linux/sysinfo.h>

static ssize_t sysmon_read(struct file *file,
                           char __user *buffer,
                           size_t length,
                           loff_t *offset)
{
    char message[256];
    int size;
    struct sysinfo info;
    unsigned long uptime;
    unsigned int cpus;

    if (*offset != 0)
        return 0;

    si_meminfo(&info);

    uptime = ktime_get_boottime_seconds();
    cpus = num_online_cpus();

    size = snprintf(message, sizeof(message),
        "Linux Device Driver Information\n"
        "CPU Cores : %u\n"
        "Total RAM : %lu MB\n"
        "Free RAM  : %lu MB\n"
        "Uptime    : %lu seconds\n",
        cpus,
        (info.totalram * (unsigned long)PAGE_SIZE) / (1024 * 1024),
        (info.freeram * (unsigned long)PAGE_SIZE) / (1024 * 1024),
        uptime);

    if (copy_to_user(buffer, message, size))
        return -EFAULT;

    *offset = size;
    return size;
}

static const struct file_operations sysmon_fops = {
    .owner = THIS_MODULE,
    .read = sysmon_read,
};

static struct miscdevice sysmon_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "sysmon",
    .fops = &sysmon_fops,
    .mode = 0444,
};

static int __init sysmon_init(void)
{
    int result;

    result = misc_register(&sysmon_device);

    if (result != 0)
        return result;

    pr_info("sysmon driver loaded\n");

    return 0;
}

static void __exit sysmon_exit(void)
{
    misc_deregister(&sysmon_device);
    pr_info("sysmon driver unloaded\n");
}

module_init(sysmon_init);
module_exit(sysmon_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("Simple Linux System Monitor Device Driver");
