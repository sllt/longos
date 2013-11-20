TOOLPATH = /home/sllt/z_tools/

INCPATH =/home/sllt/z_tools/

MAKE = make -r

NASK = $(TOOLPATH)nask.exe

CC1 = $(TOOLPATH)cc1 -I $(INCPATH) -Os -Wall -quiet

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

default:
	$(MAKE) img

ipl10.bin : ipl10.nas Makefile
		$(NASK) ipl10.nas ipl10.bin ipl10.lst

haribote.sys: haribote.nas Makefile
	$(NASK) haribote.nas haribote.sys harbote.lst

haribote.img : ipl10.bin haribote.sys  Makefile
	$(EDIMG) imgin:$(TOOLPATH)fdimg0at.tek \
		wbinimg src:ipl10.bin len:512 from:0 to:0 \
		copy from:haribote.sys to:@: \
	imgout:haribote.img

asm:
	$(MAKE) ipl10.bin

img: 
	$(MAKE) haribote.img

install:
	$(MAKE) img
	$(IMGTOL) w a:helloos.img

clean:
	DEL ipl.bin
	DEL ipl.lst


