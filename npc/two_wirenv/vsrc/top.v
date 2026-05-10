module top (
    input switch0,   // 输入：开关 0
    input switch1,   // 输入：开关 1
    output led0     // 输出：LED 0
);
    assign led0 = switch0^switch1;    // LED 1 反映开关 1 的状态
endmodule