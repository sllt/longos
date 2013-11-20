/* 函数声明，函数体在 nas 中 */

void io_hlt(void);

/**
 *作用：调用 汇编中的_io_hlt()函数
 *
**/
void HariMain(void)
{

fin:
    io_hlt();
    goto fin;
}
