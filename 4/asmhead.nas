; haribote-os boot asm
;------------------------------------------------------------------
; @sllt
; 该程序首先为进入保护模式做准备
; 然后正式进入保护模式
; 再重新安排数据在内存中的分布
; 最终跳入C语言程序
;------------------------------------------------------------------
BOTPAK  EQU     0x00280000      ; 内核代码段基址
DSKCAC  EQU     0x00100000      ; ipl10的512B将被移动到该地址处 (1M)
DSKCAC0 EQU     0x00008000      ; 软盘中的数据将被加载到的内存起始地址(事实上0x8000~0x81ff这512B没有用到
                                ; 将软盘中的内容全部加载到这部分，这里512B的内容完全是多余的,第一扇区的内容还是在7c00处
                                ; 不过从0x8200开始确实是软盘的第二扇区的内容)

; BOOT_INFO
CYLS    EQU     0x0ff0          ; 0xff0中存放着在ipl10中读入的柱面数
LEDS    EQU     0x0ff1
VMODE   EQU     0x0ff2          ; 该地址处存放关于颜色数目的信息。颜色的位数
SCRNX   EQU     0x0ff4          ; 该地址处存放分辨率的X
SCRNY   EQU     0x0ff6          ; 该地址处存放分辨率的Y
VRAM    EQU     0x0ff8          ; 该地址处存放图像缓冲区的开始地址

        ORG     0xc200          

; 保存启动信息

        MOV     AL,0x13         ; VGA显卡，320*100*8位彩色
        MOV     AH,0x00
        INT     0x10
        MOV     BYTE [VMODE],8  ; 记录画面模式
        MOV     WORD [SCRNX],320
        MOV     WORD [SCRNY],200
        MOV     DWORD [VRAM],0x000a0000

; 用BIOS取得键盘上各种LED指示灯的状态

        MOV     AH,0x02
        INT     0x16            ; keyboard BIOS
        MOV     [LEDS],AL

; 下面的代码是为进入保护模式做准备

;---------------------------------------------------------------------- 
;   可以认为该段代码执行之后  CPU将不再响应任何中断
;   可参考8259A的相关资料     
        MOV     AL,0xff         
        OUT     0x21,AL         ; 屏蔽主pic的所有中断
        NOP                     
        OUT     0xa1,AL         ; 屏蔽从pic的所有中断   

        CLI                     ; 关闭所有可屏蔽中断
;---------------------------------------------------------------------- 
; 打开A20地址总线
        CALL    waitkbdout      ; 清空i8042的输入缓冲区
        MOV     AL,0xd1
        OUT     0x64,AL         
        CALL    waitkbdout
        MOV     AL,0xdf         ; enable A20
        OUT     0x60,AL
        CALL    waitkbdout
;---------------------------------------------------------------------- 

[INSTRSET "i486p"]              

        LGDT    [GDTR0]         ; 加载GDTR寄存器
        MOV     EAX,CR0     
        AND     EAX,0x7fffffff  ; 禁止分页机制
        OR      EAX,0x00000001  ; 开启保护模式
        MOV     CR0,EAX
        JMP     pipelineflush   ; 必须跟一个跳转指令
;---------------------------------------------------------------------- 
; 此后的代码运行于保护模式下
pipelineflush:
        MOV     AX,1*8          ; 重新设置段寄存器
        MOV     DS,AX           ; DS, ES, FS, GS, SS都指向GDT中的内核数据段描述符
        MOV     ES,AX
        MOV     FS,AX
        MOV     GS,AX
        MOV     SS,AX

;---------------------------------------------------------------------- 
; 将DS:ESI指向的512KB的内容复制到ES:EDI指向的内存单元
; DS:ESI = 8:该文件随后的位置
; ES:EDI = 8:0x00280000 (内核代码段的基地址是0x00280000)
; 所以这段代码可以认为是该文件之后的512KB内容(内核代码)复制到0x00280000处
; 实际上内核代码并没有512KB那么大
        MOV     ESI,bootpack    
        MOV     EDI,BOTPAK      
        MOV     ECX,512*1024/4
        CALL    memcpy
;---------------------------------------------------------------------- 
; 将启动扇区的代码复制到0x00100000处 (1M)
        MOV     ESI,0x7c00      
        MOV     EDI,DSKCAC      
        MOV     ECX,512/4
        CALL    memcpy

; 将从软盘中读入的数据从0x8200处复制到0x00100200处
        MOV     ESI,DSKCAC0+512 ; ESI = 0x8200
        MOV     EDI,DSKCAC+512  ; EDI = 0x00100200
        MOV     ECX,0
        MOV     CL,BYTE [CYLS]  ; CL = 共读入的柱面数
        IMUL    ECX,512*18*2/4  ; 一共要复制的次数
        SUB     ECX,512/4       ; 减去(启动扇区的字节数/4)
        CALL    memcpy

; bootpack的启动
        MOV     EBX,BOTPAK      ; EBX = 0x00280000
        MOV     ECX,[EBX+16]    ; ECX = 0x11a8
        ADD     ECX,3           ; ECX += 3;
        SHR     ECX,2           ; ECX /= 4;
        JZ      skip            ; 没有要传送的数据时
        MOV     ESI,[EBX+20]    ; 复制的源地址(相对于bootoack.hrb头部的偏移) ESI = 0x10c8
        ADD     ESI,EBX         ; 换算出要复制的数据的实际物理地址
        MOV     EDI,[EBX+12]    ; 复制的目的地址   EDI = 0x310000
        CALL    memcpy
skip:
        MOV     ESP,[EBX+12]    ; 栈初始值  ESP = 0x310000
        JMP     DWORD 2*8:0x0000001b

; 该子程序与void wait_KBC_sendready(void)函数是一样的 只不过一个用汇编写 一个用C语言写
waitkbdout:                     
        IN       AL,0x64        ; 从0x64端口读取数据
        AND      AL,0x02        ; 测试i8042输入缓冲区是否为空
        JNZ     waitkbdout      ; 若不为空 则继续读取 直到输入缓冲区为空为止
        RET                     ; 为空 则返回

;---------------------------------------------------------------------- 
; 该函数将DS:ESI指向的内存单元的内容复制到ES:EDI指向的内存单元
; 每次复制4个字节
memcpy:
        MOV     EAX,[ESI]       
        ADD     ESI,4
        MOV     [EDI],EAX
        ADD     EDI,4
        SUB     ECX,1
        JNZ     memcpy          
        RET
;---------------------------------------------------------------------- 
;   全局描述符表以及GDTR寄存器的内容
        ALIGNB  16              ; 内存以16字节对齐 (个人猜想 不过内存对齐是肯定的)
GDT0:
        RESB    8               ; 空描述符
        DW      0xffff,0x0000,0x9200,0x00cf ;   32位可读写数据段描述符 段限长 4G-1    段基址为0   
                                            ;   也就是说该段可寻址0~4G-1    DPL = 0 内核数据段
        DW      0xffff,0x0000,0x9a28,0x0047 ;   32位可读可执行代码段描述符  段限长512KB        段基址为0x280000
                                            ;   DPL = 0 内核代码段

        DW      0
GDTR0:                                      
        DW      8*3-1                       ; GDT限长
        DD      GDT0                        ; GDT基址

        ALIGNB  16
bootpack:
