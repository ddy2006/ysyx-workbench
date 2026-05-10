接口需要一一对应，top.nxdc需要在开头增加top=module_name
引脚接口一一对应需要几个引脚用几个
top用来引脚对应相联系
csrc用来储存c语言程序(测试)
vsrc用来储存verilog程序(可以将主程序与函数分开写)
它会自动生成autobind.cpp
top.nxdc输出只能识别LD,LED会出问题导致auto bind无法正常生成

# VGA_CLK is set to clk in this example
VGA_VSYNC VGA_VSYNC
VGA_HSYNC VGA_HSYNC
VGA_BLANK_N VGA_BLANK_N
VGA_R (VGA_R7, VGA_R6, VGA_R5, VGA_R4, VGA_R3, VGA_R2, VGA_R1, VGA_R0)
VGA_G (VGA_G7, VGA_G6, VGA_G5, VGA_G4, VGA_G3, VGA_G2, VGA_G1, VGA_G0)
VGA_B (VGA_B7, VGA_B6, VGA_B5, VGA_B4, VGA_B3, VGA_B2, VGA_B1, VGA_B0)

ledr (LD15, LD14, LD13, LD12, LD11, LD10, LD9, LD8, LD7, LD6, LD5, LD4, LD3, LD2, LD1, LD0)
sw (SW7, SW6, SW5, SW4, SW3, SW2, SW1, SW0)
btn (BTNL, BTNU, BTNC, BTND, BTNR)
seg0 (SEG0A, SEG0B, SEG0C, SEG0D, SEG0E, SEG0F, SEG0G, DEC0P)
seg1 (SEG1A, SEG1B, SEG1C, SEG1D, SEG1E, SEG1F, SEG1G, DEC1P)
seg2 (SEG2A, SEG2B, SEG2C, SEG2D, SEG2E, SEG2F, SEG2G, DEC2P)
seg3 (SEG3A, SEG3B, SEG3C, SEG3D, SEG3E, SEG3F, SEG3G, DEC3P)
seg4 (SEG4A, SEG4B, SEG4C, SEG4D, SEG4E, SEG4F, SEG4G, DEC4P)
seg5 (SEG5A, SEG5B, SEG5C, SEG5D, SEG5E, SEG5F, SEG5G, DEC5P)
seg6 (SEG6A, SEG6B, SEG6C, SEG6D, SEG6E, SEG6F, SEG6G, DEC6P)
seg7 (SEG7A, SEG7B, SEG7C, SEG7D, SEG7E, SEG7F, SEG7G, DEC7P)

ps2_clk PS2_CLK
ps2_data PS2_DAT

uart_tx UART_TX
uart_rx UART_RX

muxkey的三个参数
NR_KEY 支持几个选项
KEY_LEN 键s的位宽。
DATA_LEN 输出y和输入的位宽。

输入位是后八位,
从右往左第一位控制移动方向，
第二位控制算数移位或者逻辑移位
第三到五位控制移位位数