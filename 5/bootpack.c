/* ==================================================================
	ע�ͣ�լ
	ʱ�䣺2013��2��18��
   ================================================================== */
#include "bootpack.h"
#include <stdio.h>

void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	/* s�����������	mcursor������ͼ�� */
	char s[40], mcursor[256];
	int mx, my;

	init_gdtidt();		/* ��ʼ��GDT, IDT */
	init_pic();			/* ��ʼ��PIC */
	io_sti();			/* �����п������ж� */

	init_palette();		/* ��ʼ����ɫ�� */
	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);	/* ����"����" */
	mx = (binfo->scrnx - 16) / 2; /* �������ͼ������Ļ�ϵ�λ�� �����������������λ�� */
	my = (binfo->scrny - 28 - 16) / 2;	
	init_mouse_cursor8(mcursor, COL8_008484);	
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16); /* ��ʾ���ͼ�� */
	sprintf(s, "(%d, %d)", mx, my);										
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);		/* ������ͼ�����Ͻ�����Ļ�ϵ����� */


	io_out8(PIC0_IMR, 0xf9); /* PIC0(11111001) (��IRQ1�����жϺ����Ӵ�PIC��IRQ2)*/
	io_out8(PIC1_IMR, 0xef); /* PIC1(11101111) (��PS2����ж� ��IRQ12)*/

	for (;;) {
		io_hlt();
	}
}
