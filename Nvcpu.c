#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

extern const struct seq_operations cpuinfo_op;
static int nvidiainfo_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &cpuinfo_op);
}

static const struct file_operations proc_nvidiainfo_operations = {
	.open		= nvidiainfo_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

static int __init proc_nvidiainfo_init(void)
{
	proc_create("nvidiainfo", 0, NULL, &proc_nvidiainfo_operations);
	return 0;
}
fs_initcall(proc_nvidiainfo_init);
