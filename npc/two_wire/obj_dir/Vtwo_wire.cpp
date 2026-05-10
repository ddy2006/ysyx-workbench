// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vtwo_wire.h"
#include "Vtwo_wire__Syms.h"
#include "verilated_vcd_c.h"

//============================================================
// Constructors

Vtwo_wire::Vtwo_wire(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vtwo_wire__Syms(contextp(), _vcname__, this)}
    , a{vlSymsp->TOP.a}
    , b{vlSymsp->TOP.b}
    , f{vlSymsp->TOP.f}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vtwo_wire::Vtwo_wire(const char* _vcname__)
    : Vtwo_wire(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vtwo_wire::~Vtwo_wire() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vtwo_wire___024root___eval_debug_assertions(Vtwo_wire___024root* vlSelf);
#endif  // VL_DEBUG
void Vtwo_wire___024root___eval_static(Vtwo_wire___024root* vlSelf);
void Vtwo_wire___024root___eval_initial(Vtwo_wire___024root* vlSelf);
void Vtwo_wire___024root___eval_settle(Vtwo_wire___024root* vlSelf);
void Vtwo_wire___024root___eval(Vtwo_wire___024root* vlSelf);

void Vtwo_wire::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vtwo_wire::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vtwo_wire___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_activity = true;
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vtwo_wire___024root___eval_static(&(vlSymsp->TOP));
        Vtwo_wire___024root___eval_initial(&(vlSymsp->TOP));
        Vtwo_wire___024root___eval_settle(&(vlSymsp->TOP));
    }
    // MTask 0 start
    VL_DEBUG_IF(VL_DBG_MSGF("MTask0 starting\n"););
    Verilated::mtaskId(0);
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vtwo_wire___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfThreadMTask(vlSymsp->__Vm_evalMsgQp);
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vtwo_wire::eventsPending() { return false; }

uint64_t Vtwo_wire::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "%Error: No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vtwo_wire::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vtwo_wire___024root___eval_final(Vtwo_wire___024root* vlSelf);

VL_ATTR_COLD void Vtwo_wire::final() {
    Vtwo_wire___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vtwo_wire::hierName() const { return vlSymsp->name(); }
const char* Vtwo_wire::modelName() const { return "Vtwo_wire"; }
unsigned Vtwo_wire::threads() const { return 1; }
std::unique_ptr<VerilatedTraceConfig> Vtwo_wire::traceConfig() const {
    return std::unique_ptr<VerilatedTraceConfig>{new VerilatedTraceConfig{false, false, false}};
};

//============================================================
// Trace configuration

void Vtwo_wire___024root__trace_init_top(Vtwo_wire___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD static void trace_init(void* voidSelf, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    Vtwo_wire___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtwo_wire___024root*>(voidSelf);
    Vtwo_wire__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (!vlSymsp->_vm_contextp__->calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
            "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->scopeEscape(' ');
    tracep->pushNamePrefix(std::string{vlSymsp->name()} + ' ');
    Vtwo_wire___024root__trace_init_top(vlSelf, tracep);
    tracep->popNamePrefix();
    tracep->scopeEscape('.');
}

VL_ATTR_COLD void Vtwo_wire___024root__trace_register(Vtwo_wire___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD void Vtwo_wire::trace(VerilatedVcdC* tfp, int levels, int options) {
    if (tfp->isOpen()) {
        vl_fatal(__FILE__, __LINE__, __FILE__,"'Vtwo_wire::trace()' shall not be called after 'VerilatedVcdC::open()'.");
    }
    if (false && levels && options) {}  // Prevent unused
    tfp->spTrace()->addModel(this);
    tfp->spTrace()->addInitCb(&trace_init, &(vlSymsp->TOP));
    Vtwo_wire___024root__trace_register(&(vlSymsp->TOP), tfp->spTrace());
}
