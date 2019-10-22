#include <asm/arch_timer.h>
#include <asm/cachetype.h>
#include <asm/cpu.h>
#include <asm/cputype.h>
#include <asm/cpufeature.h>

#include <linux/bitops.h>
#include <linux/bug.h>
#include <linux/compat.h>
#include <linux/elf.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/personality.h>
#include <linux/preempt.h>
#include <linux/printk.h>
#include <linux/seq_file.h>
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/delay.h>
#include <linux/mman.h>
#include <linux/mmzone.h>
#include <linux/proc_fs.h>
#include <linux/quicklist.h>
#include <linux/seq_file.h>
#include <linux/swap.h>
#include <linux/vmstat.h>
#include <linux/atomic.h>
#include <linux/vmalloc.h>

#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>
#include <linux/module.h>





static int  nvcpu_proc_show(struct seq_file *m,void *v)
{


u32 nidr;
int k;
struct cpuinfo_arm64 *infonv =this_cpu_ptr(&cpu_data);
infonv->reg_midr = read_cpuid_id();



  for_each_online_cpu(k)
  {


struct cpuinfo_arm64 *nvinfo = &per_cpu(cpu_data,k);

 nidr = nvinfo->reg_midr;

              seq_printf(m, "processor\t: %d\n",k);
                seq_printf(m, "model name\t: ARMv8 Processor rev %d (%s)\n",
                           MIDR_REVISION(nidr), COMPAT_ELF_PLATFORM);

                 seq_printf(m, "BogoMIPS\t: %lu.%02lu\n",
                           loops_per_jiffy / (500000UL/HZ),
                           loops_per_jiffy / (5000UL/HZ) % 100);
                seq_printf(m, "CPU implementer\t: 0x%02x\n",
                           MIDR_IMPLEMENTOR(nidr));
                seq_printf(m, "CPU architecture: 8\n");
                seq_printf(m, "CPU variant\t: 0x%x\n", MIDR_VARIANT(nidr));
                seq_printf(m, "CPU part\t: 0x%03x\n", MIDR_PARTNUM(nidr));
                seq_printf(m, "CPU revision\t: %d\n", MIDR_REVISION(nidr));
		seq_printf(m,"\n");
}
return 0;
};


static int nvcpu_proc_open(struct inode *inode, struct file *file)
{
        return single_open(file, nvcpu_proc_show, NULL);
}

static const struct file_operations nvcpu_proc_fops = {
        .open           = nvcpu_proc_open,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};

static int __init proc_nvcpu_init(void)
{
        proc_create("nvinfo", 0, NULL, &nvcpu_proc_fops);
        return 0;
}
fs_initcall(proc_nvcpu_init);

