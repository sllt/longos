/* ==================================================================
	注释：宅
	时间：2013年2月18日
	该文件中定义了与中断有关的函数
   ================================================================== */
#include "bootpack.h"

/*
	关于8259A的相关内容可参考赵博士的《Linux 内核完全剖析——基于0.11内核》P215
	书中对8259A有详细的介绍
 */
/* 初始化PIC */
void init_pic(void)
{
	io_out8(PIC0_IMR,  0xff  ); /* 禁止主PIC所有中断 */
	io_out8(PIC1_IMR,  0xff  ); /* 禁止从PIC所有中断 */

	io_out8(PIC0_ICW1, 0x11  ); /* 需要ICW4， 多片级联， 边沿触发方式 */
	io_out8(PIC0_ICW2, 0x20  ); /* IRQ0-7由于INT 0x20~0x27接收 */
	io_out8(PIC0_ICW3, 1 << 2); /* PIC1由IRQ2连接 */
	io_out8(PIC0_ICW4, 0x01  ); /* 普通全嵌套 非缓冲 非自动结束中断方式 */

	io_out8(PIC1_ICW1, 0x11  ); /* 需要ICW4， 多片级联， 边沿触发方式 */
	io_out8(PIC1_ICW2, 0x28  ); /* IRQ8-15由于INT 0x28~0x2f接收 */
	io_out8(PIC1_ICW3, 2     ); /* PIC1由IRQ2连接 */
	io_out8(PIC1_ICW4, 0x01  ); /* 普通全嵌套 非缓冲 非自动结束中断方式 */

	io_out8(PIC0_IMR,  0xfb  ); /* 11111011 PIC1以外全部禁止 */
	io_out8(PIC1_IMR,  0xff  ); /* 11111111 禁止从PIC所有中断 */

	return;
}

/* 处理来自键盘的中断  由naskfunc.nas中的_asm_inthandler21调用 */
void inthandler21(int *esp)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);	/* 画出要输出的信息的背景 */
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 21 (IRQ-1) : PS/2 keyboard");	/* 输出信息 */
	for (;;) {
		io_hlt();		/* 待机 */
	}
}

/* 处理来自PS/2鼠标的中断 由naskfunc.nas中的_asm_inthandler2c调用 */
void inthandler2c(int *esp)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15); /* 画出要输出的信息的背景 */
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 2C (IRQ-12) : PS/2 mouse"); /* 输出信息 */
	for (;;) {
		io_hlt();	/* 待机 */
	}
}

/* 处理IRQ7中断 由naskfunc.nas中的_asm_inthandler27调用 */
/*
	关于IRQ7的处理可对照赵博士的《Linux 内核完全剖析——基于0.11内核》P219
	的表格来理解
 */
void inthandler27(int *esp)								
{
	io_out8(PIC0_OCW2, 0x67); /* 直接发送EOI命令 表示中断处理结束 */
	return;
}
