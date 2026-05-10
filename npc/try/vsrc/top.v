module top (
    input switch0,   // 输入：开关 0
    input switch1,   // 输入：开关 1
    output led0,     // 输出：LED 0
    output led1      // 输出：LED 1
);
    assign led0 = switch0;  // LED 0 反映开关 0 的状态
    assign led1 = switch1;  // LED 1 反映开关 1 的状态
endmodule