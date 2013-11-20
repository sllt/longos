/* ==================================================================
	ע�ͣ�լ
	ʱ�䣺2013��2��18��
	���ļ��ж��������ж��йصĺ���
   ================================================================== */
#include "bootpack.h"

/*
	����8259A��������ݿɲο��Բ�ʿ�ġ�Linux �ں���ȫ������������0.11�ںˡ�P215
	���ж�8259A����ϸ�Ľ���
 */
/* ��ʼ��PIC */
void init_pic(void)
{
	io_out8(PIC0_IMR,  0xff  ); /* ��ֹ��PIC�����ж� */
	io_out8(PIC1_IMR,  0xff  ); /* ��ֹ��PIC�����ж� */

	io_out8(PIC0_ICW1, 0x11  ); /* ��ҪICW4�� ��Ƭ������ ���ش�����ʽ */
	io_out8(PIC0_ICW2, 0x20  ); /* IRQ0-7����INT 0x20~0x27���� */
	io_out8(PIC0_ICW3, 1 << 2); /* PIC1��IRQ2���� */
	io_out8(PIC0_ICW4, 0x01  ); /* ��ͨȫǶ�� �ǻ��� ���Զ������жϷ�ʽ */

	io_out8(PIC1_ICW1, 0x11  ); /* ��ҪICW4�� ��Ƭ������ ���ش�����ʽ */
	io_out8(PIC1_ICW2, 0x28  ); /* IRQ8-15����INT 0x28~0x2f���� */
	io_out8(PIC1_ICW3, 2     ); /* PIC1��IRQ2���� */
	io_out8(PIC1_ICW4, 0x01  ); /* ��ͨȫǶ�� �ǻ��� ���Զ������жϷ�ʽ */

	io_out8(PIC0_IMR,  0xfb  ); /* 11111011 PIC1����ȫ����ֹ */
	io_out8(PIC1_IMR,  0xff  ); /* 11111111 ��ֹ��PIC�����ж� */

	return;
}

/* �������Լ��̵��ж�  ��naskfunc.nas�е�_asm_inthandler21���� */
void inthandler21(int *esp)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);	/* ����Ҫ�������Ϣ�ı��� */
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 21 (IRQ-1) : PS/2 keyboard");	/* �����Ϣ */
	for (;;) {
		io_hlt();		/* ���� */
	}
}

/* ��������PS/2�����ж� ��naskfunc.nas�е�_asm_inthandler2c���� */
void inthandler2c(int *esp)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15); /* ����Ҫ�������Ϣ�ı��� */
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 2C (IRQ-12) : PS/2 mouse"); /* �����Ϣ */
	for (;;) {
		io_hlt();	/* ���� */
	}
}

/* ����IRQ7�ж� ��naskfunc.nas�е�_asm_inthandler27���� */
/*
	����IRQ7�Ĵ���ɶ����Բ�ʿ�ġ�Linux �ں���ȫ������������0.11�ںˡ�P219
	�ı�������
 */
void inthandler27(int *esp)								
{
	io_out8(PIC0_OCW2, 0x67); /* ֱ�ӷ���EOI���� ��ʾ�жϴ������ */
	return;
}
