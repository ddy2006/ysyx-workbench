接口需要一一对应，top.nxdc需要在开头增加top=module_name
引脚接口一一对应需要几个引脚用几个
top用来引脚对应相联系
csrc用来储存c语言程序(测试)
vsrc用来储存verilog程序(可以将主程序与函数分开写)
它会自动生成autobind.cpp
top.nxdc输出只能识别LD,LED会出问题导致auto bind无法正常生成
casex可以在条件中加入x忽略x类似于正则表达式.
七段数码管
0:h = 8'h02;
1:h = 8'h9e;
2:h = 8'h24;
3:h = 8'h0c;
4:h = 8'h98;
5:h = 8'h48;
6:h = 8'h40;
7:h = 8'h1f;
8:h = 8'h00;
9:h = 9'h08;