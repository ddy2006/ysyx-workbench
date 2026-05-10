#include <Vtop.h>
#include <nvboard.h>
#include <verilated.h>

void nvboard_bind_all_pins(Vtop* top);

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    Vtop* top = new Vtop; // 实例化顶层模块

    // 初始化 NVBoard
    nvboard_bind_all_pins(top);
    nvboard_init();

    // 主循环
    while (1) {
        nvboard_update(); // 更新 NVBoard 输入（如开关状态）
        top->eval();       // 运行 Verilog 逻辑
        nvboard_update(); // 更新 NVBoard 输出（如 LED 状态）
    }

    // 清理
    nvboard_quit();
    delete top;
    return 0;
}