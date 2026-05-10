// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table implementation internals

#include "Vtwo_wire__Syms.h"
#include "Vtwo_wire.h"
#include "Vtwo_wire___024root.h"

// FUNCTIONS
Vtwo_wire__Syms::~Vtwo_wire__Syms()
{
}

Vtwo_wire__Syms::Vtwo_wire__Syms(VerilatedContext* contextp, const char* namep, Vtwo_wire* modelp)
    : VerilatedSyms{contextp}
    // Setup internal state of the Syms class
    , __Vm_modelp{modelp}
    // Setup module instances
    , TOP{this, namep}
{
    // Configure time unit / time precision
    _vm_contextp__->timeunit(-12);
    _vm_contextp__->timeprecision(-12);
    // Setup each module's pointers to their submodules
    // Setup each module's pointer back to symbol table (for public functions)
    TOP.__Vconfigure(true);
}
