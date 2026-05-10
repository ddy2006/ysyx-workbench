module top (
    input wire [15:0] sw,        // 开关输入
    input wire [4:0] btn,        // 按键输入
    output wire [15:0] ledr      // LED 输出
);

parameter [3:0] S0 = 4'd0, S1 = 4'd1, S2 = 4'd2, S3 = 4'd3,
                S4 = 4'd4, S5 = 4'd5, S6 = 4'd6, S7 = 4'd7, S8 = 4'd8;

reg [3:0] current_state, next_state;
reg ledr0;                   // 输出 z
reg [3:0] ledr7_ledr4;       // 状态显示

always @(posedge btn[0] or negedge sw[0]) begin
    if (!sw[0]) // 低电平有效复位
        current_state <= S0;
    else
        current_state <= next_state;
end

always @(*) begin
    case (current_state)
        S0: next_state = sw[1] ? S5 : S1;
        S1: next_state = sw[1] ? S5 : S2;
        S2: next_state = sw[1] ? S5 : S3;
        S3: next_state = sw[1] ? S5 : S4;
        S4: next_state = sw[1] ? S5 : S4;
        S5: next_state = sw[1] ? S6 : S1;
        S6: next_state = sw[1] ? S7 : S1;
        S7: next_state = sw[1] ? S8 : S1;
        S8: next_state = sw[1] ? S8 : S1;
        default: next_state = S0;
    endcase
end

// 输出逻辑
always @(*) begin
    case (current_state)
        S4, S8: ledr0 = 1'b1;
        default: ledr0 = 1'b0;
    endcase
end

// 状态显示
assign ledr7_ledr4 = current_state;

// 顶层输出映射
assign ledr[0] = ledr0;
assign ledr[7:4] = ledr7_ledr4;
assign ledr[3:1] = 3'b0;
assign ledr[15:8] = 8'b0;

endmodule