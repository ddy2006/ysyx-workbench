// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtwo_wire.h for the primary calling header

#include "verilated.h"

#include "Vtwo_wire__Syms.h"
#include "Vtwo_wire___024root.h"

void Vtwo_wire___024root___ctor_var_reset(Vtwo_wire___024root* vlSelf);

Vtwo_wire___024root::Vtwo_wire___024root(Vtwo_wire__Syms* symsp, const char* v__name)
    : VerilatedModule{v__name}
    , vlSymsp{symsp}
 {
    // Reset structure values
    Vtwo_wire___024root___ctor_var_reset(this);
}

void Vtwo_wire___024root::__Vconfigure(bool first) {
    if (false && first) {}  // Prevent unused
}

Vtwo_wire___024root::~Vtwo_wire___024root() {
}
