#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "obj_dir/Vtwo_wire.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <nvboard.h>

int main(){
    Vtwo_wire* top = new Vtwo_wire;
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("wave.vcd");
    // nvboard_bind_all_pins(&dut);
    // nvboard_init();
    for(int i=0;i<100;i++) {
        int a = rand() & 1;
        int b = rand() & 1;
        top->a = a;
        top->b = b;
        top->eval();
        tfp->dump(i);
        // nvboard_update();
        printf("a = %d, b = %d, f = %d\n", a, b, top->f);
        assert(top->f == (a ^ b));
    }
    tfp->close();
    // nvboard_quit();
    delete top;
    return 0;
}