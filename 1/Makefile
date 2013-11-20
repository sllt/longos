TOOLPATH = /home/sllt/z_tools/

INCPATH =/home/sllt/z_tools/

MAKE = make -r

NASK = $(TOOLPATH)nask.exe

CC1 = $(TOOLPATH)cc1 -I $(INCPATH) -Os -Wall -quiet

GAS2NASK = $(TOOLPATH)gas2nask -a

OBJ2BIM = $(TOOLPATH)obj2bim

BIN2OBJ = $(TOOLPATH)bin2obj

BIM2HRB = $(TOOLPATH)bim2hrb

RULEFILE = $(TOOLPATH)haribote/haribote.rul

EDIMG = $(TOOLPATH)edimg.exe

IMGTOL = $(TOOLPATH)imgtol.com

MAKEFONT = $(TOOLPATH)makefont

GOLIB = $(TOOLPATH)golib00

COPY = cp

DEL = rm

default:
	$(MAKE) img


ipl.bin : ipl.nas Makefile
	$(NASK) ipl.nas ipl.bin ipl.lst

hello.img : ipl.bin Makefile
	$(EDIMG) imgin:$(TOOLPATH)fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0 \
	imgout:helloos.img

asm:
	$(MAKE) ipl.bin

img: 
	$(MAKE) helloos.img

install:
	$(MAKE) img
	$(IMGTOL) w a:helloos.img

clean:
	DEL ipl.bin
	DEL ipl.lst


