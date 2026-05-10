// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VTWO_WIRE__SYMS_H_
#define VERILATED_VTWO_WIRE__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vtwo_wire.h"

// INCLUDE MODULE CLASSES
#include "Vtwo_wire___024root.h"

// SYMS CLASS (contains all model state)
class Vtwo_wire__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vtwo_wire* const __Vm_modelp;
    bool __Vm_activity = false;  ///< Used by trace routines to determine change occurred
    uint32_t __Vm_baseCode = 0;  ///< Used by trace routines when tracing multiple models
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vtwo_wire___024root            TOP;

    // CONSTRUCTORS
    Vtwo_wire__Syms(VerilatedContext* contextp, const char* namep, Vtwo_wire* modelp);
    ~Vtwo_wire__Syms();

    // METHODS
    const char* name() { return TOP.name(); }
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

#endif  // guard
