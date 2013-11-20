TOOLPATH = /home/sllt/z_tools/

INCPATH =/home/sllt/z_tools/haribote/

MAKE = make -r

NASK = $(TOOLPATH)nask.exe

CC1 = $(TOOLPATH)cc1.exe -I $(INCPATH) -Os -Wall -quiet

GAS2NASK = $(TOOLPATH)gas2nask.exe -a

OBJ2BIM = $(TOOLPATH)obj2bim.exe

BIN2OBJ = $(TOOLPATH)bin2obj.exe

BIM2HRB = $(TOOLPATH)bim2hrb.exe

RULEFILE = $(TOOLPATH)haribote/haribote.rul

EDIMG = $(TOOLPATH)edimg.exe

IMGTOL = $(TOOLPATH)imgtol.com

MAKEFONT = $(TOOLPATH)makefont.exe

GOLIB = $(TOOLPATH)golib00

COPY = cp

DEL = rm
OBJS_BOOTPACK = bootpack.obj naskfunc.obj hankaku.obj graphic.obj dsctbl.obj \
		int.obj

default:
	$(MAKE) img

ipl10.bin : ipl10.nas Makefile
		$(NASK) ipl10.nas ipl10.bin ipl10.lst

asmhead.bin : asmhead.nas Makefile
		$(NASK) asmhead.nas asmhead.bin asmhead.lst

bootpack.gas : bootpack.c Makefile
		$(CC1) -o bootpack.gas bootpack.c

bootpack.nas : bootpack.gas Makefile
		$(GAS2NASK) bootpack.gas bootpack.nas

bootpack.obj : bootpack.nas Makefile
		$(NASK) bootpack.nas bootpack.obj bootpack.lst

bootpack.bim : $(OBJS_BOOTPACK) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
			$(OBJS_BOOTPACK)

naskfunc.obj : naskfunc.nas Makefile
		$(NASK) naskfunc.nas naskfunc.obj naskfunc.lst

bootpack.hrb : bootpack.bim Makefile
		$(BIM2HRB) bootpack.bim bootpack.hrb 0

haribote.sys : asmhead.bin bootpack.hrb Makefile
		cat asmhead.bin bootpack.hrb > haribote.sys

hankaku.bin : hankaku.txt Makefile
	$(MAKEFONT) hankaku.txt hankaku.bin

hankaku.obj : hankaku.bin Makefile
	$(BIN2OBJ) hankaku.bin hankaku.obj _hankaku

haribote.img : ipl10.bin haribote.sys  Makefile
	$(EDIMG) imgin:$(TOOLPATH)fdimg0at.tek \
		wbinimg src:ipl10.bin len:512 from:0 to:0 \
		copy from:haribote.sys to:@: \
	imgout:haribote.img

%.gas : %.c Makefile

	$(CC1) -o $*.gas $*.c

%.nas : %.gas Makefile

	$(GAS2NASK) $*.gas $*.nas

%.obj : %.nas Makefile

	$(NASK) $*.nas $*.obj $*.lst

asm:
	$(MAKE) ipl10.bin

img: 
	$(MAKE) haribote.img

install:
	$(MAKE) img
	$(IMGTOL) w a:helloos.img

run:
	$(MAKE) img
	qemu-system-i386 -fda haribote.img

clean:
	-$(DEL) *.bin
	-$(DEL) *.lst
	-$(DEL) *.gas
	-$(DEL) *.obj
	-$(DEL) bootpack.nas
	-$(DEL) bootpack.map
	-$(DEL) bootpack.bim
	-$(DEL) bootpack.hrb
	-$(DEL) haribote.sys

