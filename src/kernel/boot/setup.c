/*
 * Copyright (C) 2011-2013 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 * 
 * setup.c - Sets up machine
 */

#include <asm/io.h>
#include <asm/util.h>
#include <i386/i386.h>
#include <nanvix/config.h>
#include <nanvix/const.h>
#include <nanvix/int.h>
#include <nanvix/klib.h>

/* Descriptor tables. */
PRIVATE struct gdte gdt[GDT_SIZE]; /* Global descriptor table.    */
PRIVATE struct idte idt[IDT_SIZE]; /* Interrupt descriptor table. */

/* Descriptor tables pointers. */
PRIVATE struct gdtptr gdtptr; /* Global descriptor table pointer.    */
PRIVATE struct idtptr idtptr; /* Interrupt descriptor table pointer. */

/* Task state segment. */
PUBLIC struct tss tss;

/*
 * Sets a GDT entry.
 */
PRIVATE void set_gdte
(int n, unsigned base, unsigned limit, unsigned granularity, unsigned access)
{
    /* Set segment base address. */
    gdt[n].base_low = (base & 0xffffff);
    gdt[n].base_high = (base >> 24) & 0xff;
	
    /* Set segment limit. */ 
    gdt[n].limit_low = (limit & 0xffff);
    gdt[n].limit_high = (limit >> 16) & 0xf;
    
    /* Set granularity and access. */
    gdt[n].granularity = granularity;
    gdt[n].access = access;
}

/*
 * Sets up TSS.
 */
PRIVATE void tss_setup()
{
	/* Size-error checking. */
	CHKSIZE(sizeof(struct tss), TSS_SIZE);
	
	/* Blank TSS. */
	kmemset(&tss, 0, TSS_SIZE);
	
	/* Fill up TSS. */
	tss.ss0 = KERNEL_DS;
	tss.iomap = (TSS_SIZE - 1) << 16;
	
	/* Flush TSS. */
	tss_flush();	
}

/*
 * Sets up GDT.
 */
PRIVATE void gdt_setup()
{
	/* Size-error checking. */
	CHKSIZE(sizeof(struct gdte), GDTE_SIZE);
	CHKSIZE(sizeof(struct gdtptr), GDTPTR_SIZE);

	/* Blank GDT and GDT pointer. */
	kmemset(gdt, 0, sizeof(gdt));
	kmemset(&gdtptr, 0, GDTPTR_SIZE);
	
    /* Set GDT entries. */
    set_gdte(GDT_NULL, 0, 0x00000, 0x0, 0x00);
    set_gdte(GDT_CODE_DPL0, 0, 0xfffff, 0xc, 0x9a);
    set_gdte(GDT_DATA_DPL0, 0, 0xfffff, 0xc, 0x92);
    set_gdte(GDT_CODE_DPL3, 0, 0xfffff, 0xc, 0xfa);
    set_gdte(GDT_DATA_DPL3, 0, 0xfffff, 0xc, 0xf2);
	set_gdte(GDT_TSS, (unsigned) &tss, (unsigned)&tss + TSS_SIZE, 0x0, 0xe9);
    
    /* Set GDT pointer. */
    gdtptr.size = sizeof(gdt) - 1;
    gdtptr.ptr = (unsigned) gdt;
    
    /* Flush GDT. */
    gdt_flush(&gdtptr);
    
    kprintf("gdt at %x", &gdt);
    kprintf("gdt pointer at %x", &gdtptr);
}

/*
 * Sets a IDT entry.
 */
PRIVATE void set_idte
(int n, unsigned handler, unsigned selector, unsigned flags, unsigned type)
{
	/* Set handler. */
	idt[n].handler_low = (handler & 0xffff);
    idt[n].handler_high = (handler >> 16) & 0xffff;
     
    /* Set GDT selector. */
    idt[n].selector = selector;
     
   /* Set type and flags. */
    idt[n].type = type;
    idt[n].flags = flags;
}
 
/*
 * Sets up IDT.
 */
PUBLIC void idt_setup()
{
	int i;
	
	/* Size-error checking. */
	CHKSIZE(sizeof(struct idte), IDTE_SIZE);
	CHKSIZE(sizeof(struct idtptr), IDTPTR_SIZE);
 
	/* Blank IDT and IDT pointer. */
	kmemset(idt, 0, sizeof(idt));
	kmemset(&idtptr, 0, IDTPTR_SIZE);
     
    /* Program PIC. */
    outputb(PIC_CTRL_MASTER, 0x11); 
    outputb(PIC_CTRL_SLAVE,  0x11);
    outputb(PIC_DATA_MASTER, 0x20);
    outputb(PIC_DATA_SLAVE,  0x28);
    outputb(PIC_DATA_MASTER, 0x04);
    outputb(PIC_DATA_SLAVE,  0x02);
    outputb(PIC_DATA_MASTER, 0x01);
    outputb(PIC_DATA_SLAVE,  0x01);
    outputb(PIC_DATA_MASTER, 0x00);
    outputb(PIC_DATA_SLAVE,  0x00);

    /* Set software interrupts. */
    set_idte(0, (unsigned)swint0, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(1, (unsigned)swint1, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(2, (unsigned)swint2, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(3, (unsigned)swint3, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(4, (unsigned)swint4, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(5, (unsigned)swint5, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(6, (unsigned)swint6, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(7, (unsigned)swint7, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(8, (unsigned)swint8, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(9, (unsigned)swint9, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(10, (unsigned)swint10, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(11, (unsigned)swint11, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(12, (unsigned)swint12, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(13, (unsigned)swint13, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(14, (unsigned)swint14, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(15, (unsigned)swint15, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(16, (unsigned)swint16, KERNEL_CS, 0x8, IDT_INT32);
    for (i = 17; i < 32; i++)
        set_idte(i, (unsigned)swint17, KERNEL_CS, 0x8, IDT_INT32);
     
    /* Set hardware interrupts. */
    set_idte(32, (unsigned)hwint0, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(33, (unsigned)hwint1, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(34, (unsigned)hwint2, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(35, (unsigned)hwint3, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(36, (unsigned)hwint4, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(37, (unsigned)hwint5, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(38, (unsigned)hwint6, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(39, (unsigned)hwint7, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(40, (unsigned)hwint8, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(41, (unsigned)hwint9, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(42, (unsigned)hwint10, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(43, (unsigned)hwint11, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(44, (unsigned)hwint12, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(45, (unsigned)hwint13, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(46, (unsigned)hwint14, KERNEL_CS, 0x8, IDT_INT32);
    set_idte(47, (unsigned)hwint15, KERNEL_CS, 0x8, IDT_INT32);
     
    /* Set system call interrupt. */
    set_idte(128, (unsigned)syscall, KERNEL_CS, 0xe, IDT_INT32);
  
    /* Set IDT pointer. */
    idtptr.size = sizeof(idt) - 1;
    idtptr.ptr = (unsigned)&idt;
     
    /* Flush IDT. */
    idt_flush(&idtptr);  
    
    kprintf("idt at %x", &idt);
    kprintf("idt pointer at %x", &idtptr);
}

/*
 * Sets up machine.
 */
PUBLIC void setup()
{
	/* Setup descriptor tables. */
	gdt_setup();
	tss_setup();
	idt_setup();
}
