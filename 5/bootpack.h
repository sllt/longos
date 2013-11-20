/* ==================================================================
	ע�ͣ�լ
	ʱ�䣺2013��2��18��
	��ͷ�ļ����и��ļ��еĺ��������Լ�һЩ���ų����Ķ���
   ================================================================== */
   
/* asmhead.nas */
struct BOOTINFO { /* 0x0ff0-0x0fff */
	char cyls; 			/* �������ipl10�ж���������� */
	char leds; 			/* ����ż���led�Ƶ�״̬ */
	char vmode; 		/* �Կ�ģʽΪ����λ��ɫ */
	char reserve;		/* ��"ռλ��" ��asmhead.nas�и��ֽ�δʹ�� */
	short scrnx, scrny; /* ����ֱ��� */
	char *vram;			/* ͼ�񻺳����׵�ַ */
};
#define ADR_BOOTINFO	0x00000ff0	/* ���������Ϣ�ĵ�ַ */

/* naskfunc.nas */
void io_hlt(void);					/* hlt */
void io_cli(void);					/* �ر����п������ж� */
void io_sti(void);					/* �����п������ж� */
void io_out8(int port, int data);	/* ��dataд�뵽port�˿� */
int io_load_eflags(void);			/* ��ȡeflags�Ĵ�����ֵ */
void io_store_eflags(int eflags);	/* ����eflags�Ĵ�����ֵ */
void load_gdtr(int limit, int addr);/* ����GDT(����˵ ����GDTR�Ĵ���) */
void load_idtr(int limit, int addr);/* ����IDT(����˵ ����IDTR�Ĵ���) */
void asm_inthandler21(void);		/* IRQ1������� */
void asm_inthandler27(void);		/* IRQ7������� */
void asm_inthandler2c(void);		/* IRQ12������� */

/* graphic.c */
void init_palette(void);			/* ��ʼ����ɫ�� */
void set_palette(int start, int end, unsigned char *rgb);	/* ���õ�ɫ�� */
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1); /* ����Ļ�ϻ���һ������ͼ�� */
void init_screen8(char *vram, int x, int y);				/* ����"����" */
void putfont8(char *vram, int xsize, int x, int y, char c, char *font); /* ����Ļ����������ַ� */
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s); /* ���ascii���ַ��� */
void init_mouse_cursor8(char *mouse, char bc);				/* ��ʼ�����ͼ�� */
void putblock8_8(char *vram, int vxsize, int pxsize,		/* ��ʾĳһͼ�� */
	int pysize, int px0, int py0, char *buf, int bxsize);
/* ������Ӧ��ɫ�ڵ�ɫ���е��±� */
#define COL8_000000		0		/* ��ɫ �ڵ�ɫ�����±�Ϊ0 */
#define COL8_FF0000		1		/* ����ɫ �ڵ�ɫ�����±�Ϊ1 */
#define COL8_00FF00		2		/* ����ɫ �ڵ�ɫ�����±�Ϊ2 */
#define COL8_FFFF00		3		/* ����ɫ �ڵ�ɫ�����±�Ϊ3 */
#define COL8_0000FF		4		/* ����ɫ �ڵ�ɫ�����±�Ϊ4 */	
#define COL8_FF00FF		5		/* ����ɫ �ڵ�ɫ�����±�Ϊ5 */
#define COL8_00FFFF		6		/* ǳ����ɫ �ڵ�ɫ�����±�Ϊ6 */
#define COL8_FFFFFF		7		/* ��ɫ �ڵ�ɫ�����±�Ϊ7 */
#define COL8_C6C6C6		8		/* ����ɫ �ڵ�ɫ�����±�Ϊ8 */
#define COL8_840000		9		/* ����ɫ �ڵ�ɫ�����±�Ϊ9 */
#define COL8_008400		10		/* ����ɫ �ڵ�ɫ�����±�Ϊ10 */
#define COL8_848400		11		/* ����ɫ �ڵ�ɫ�����±�Ϊ11 */
#define COL8_000084		12		/* ����ɫ �ڵ�ɫ�����±�Ϊ12 */
#define COL8_840084		13		/* ����ɫ �ڵ�ɫ�����±�Ϊ13 */
#define COL8_008484		14		/* ǳ����ɫ �ڵ�ɫ�����±�Ϊ14 */
#define COL8_848484		15		/* ����ɫ �ڵ�ɫ�����±�Ϊ15 */


/* dsctbl.c */
/* ��Ŷ��������Ľṹ�� */
/* ���ڶ������������аٶ� ��Ϊ�漰����ģʽ ����������ݺܶ� �޷�һһע��
   ������Ϊ��ʱû��Ҫ�����ڱ���ģʽ���������,��������Ҳû�д��������ھ�
   ���������ڱ���ģʽ��֪ʶ, �������Ȥ�Ļ����Բο�Intel 80386�ֲ�ľ�3
   �����Բ�ʿ�ġ�Linux �ں���ȫ������������0.11�ںˡ��ĵ����µ��������
 */
/*
	limit_low	���޳���0~15bit		base_low	�λ�ַ��0~15bit
	base_mid	�λ�ַ��16~23bit	access_right(0~3bit TYPE�ֶ�, 4bit S�ֶ� 5~6 DPL�ֶ� 7bit P�ֶ�)
	limit_high	���޳���16~19bit+AVL+D/B+G��	base_high	�λ�ַ��24~31bit
	- - �ð� ������������ڶ���������ע�Ͳ��Ǻ������ �����Ŀ��Բο��������Ӵ���ͼƬ
	http://baike.baidu.com/picview/3289301/3289301/0/0db52faddf823c3d4b36d686.html#albumindex=0&picindex=0
 */
struct SEGMENT_DESCRIPTOR {
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};

/* ������������Ľṹ�� */
/*
	���������������������,ֻ����Щ�ֶ���Щ����
	offset_low	����ƫ�Ƶ�ַ�ĵ�16bit	selector ��ѡ����
	dw_count	��������, ֻ�ǵ���������Ч
	access_right	0~3bit:TYPE�ֶ�, 4bit:S�ֶ�, 5~6bit:DPL�ֶ�, 7bit:P�ֶ�
	offset_high	����ƫ�Ƶ�ַ�ĸ�16bit
 */
struct GATE_DESCRIPTOR {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};

void init_gdtidt(void);				/* ��ʼ��GDT, IDT */
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);	/* ���ö������� */
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);		/* ������������ */
#define ADR_IDT			0x0026f800		/* IDT�׵�ַ */
#define LIMIT_IDT		0x000007ff		/* IDT�޳� */
#define ADR_GDT			0x00270000		/* GDT�׵�ַ */
#define LIMIT_GDT		0x0000ffff		/* GDT�޳� */
#define ADR_BOTPAK		0x00280000		/* �ں˴����׵�ַ */
#define LIMIT_BOTPAK	0x0007ffff		/* �ں˴����޳�(��512KB) */
#define AR_DATA32_RW	0x4092			/* 32λ���ݶ�����ֵ */
#define AR_CODE32_ER	0x409a			/* 32λ���������ֵ */
#define AR_INTGATE32	0x008e			/* 32λ������������ֵ */

/* int.c */
void init_pic(void);					/* ��ʼ��PIC */
void inthandler21(int *esp);						
void inthandler27(int *esp);
void inthandler2c(int *esp);
/* PIC�и����˿ں� */
#define PIC0_ICW1		0x0020			
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1
