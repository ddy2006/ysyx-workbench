module adder (
    input wire n1, n2, n3,
    output wire overflow, s
);
    assign s = n1 ^ n2 ^ n3;
    assign overflow = (n1 & n2) | (n1 & n3) | (n2 & n3);
endmodule

// 4 位有符号加法器模块
module adder_4 (
    input signed [3:0] n1,
    input signed [3:0] n2,
    output wire overflow,
    output signed [3:0] s
);
    wire [4:0] carry;
    wire [3:0] sum;
    assign carry[0] = 1'b0;
    genvar i;
    generate
        for (i = 0; i < 4; i = i + 1) begin : adder_loop
            adder adder_inst (.n1(n1[i]), .n2(n2[i]), .n3(carry[i]), .overflow(carry[i+1]), .s(sum[i]));
        end
    endgenerate
    assign s = sum;
    assign overflow = carry[4];
endmodule

module neg_4 (
    input signed [3:0] n1,
    output signed [3:0] n2
);
    wire signed [3:0] n3;
    assign n3 = ~n1;
    wire cout;
    adder_4 adder_inst (.n1(n3), .n2(4'h1), .overflow(cout), .s(n2));
endmodule

module min_4 (
    input signed [3:0] n1,
    input signed [3:0] n2,
    output signed [3:0] n3
);
    wire signed [3:0] n4;
    wire cout;
    neg_4 neg_inst (.n1(n2), .n2(n4));
    adder_4 adder_inst (.n1(n1), .n2(n4), .overflow(cout), .s(n3));
endmodule

module ALU (
    input signed [3:0] n1,    // 操作数 A
    input signed [3:0] n2,    // 操作数 B
    input [2:0] sel,          // 功能选择
    output signed [3:0] y,    // 4 位结果
    output wire zero,         // 零标志
    output wire overflow,     // 溢出标志（加减法）
    output wire carry         // 进位标志（加减法）
);
    wire signed [3:0] add;
    wire add_carry;
    adder_4 adder_inst (.n1(n1), .n2(n2), .overflow(add_carry), .s(add));

    // 减法
    wire signed [3:0] min;
    wire sub_carry;
    min_4 min_inst (.n1(n1), .n2(n2), .n3(min));
    wire signed [3:0] neg_n2 = ~n2 + 4'h1;
    wire [4:0] sub_temp = {1'b0, n1} + {1'b0, neg_n2};
    assign sub_carry = sub_temp[4];

    // 取反
    wire signed [3:0] inverse;
    neg_4 neg_inst (.n1(n1), .n2(inverse));

    // 逻辑操作
    wire signed [3:0] and_result = n1 & n2;
    wire signed [3:0] or_result = n1 | n2;
    wire signed [3:0] xor_result = n1 ^ n2;
    wire signed [3:0] slt_result = (n1 < n2) ? 4'h1 : 4'h0;
    wire signed [3:0] eq_result = (n1 == n2) ? 4'h1 : 4'h0;
    // 结果选择
    reg signed [3:0] y_temp;
    always @(*) begin
        case (sel)
            3'b000: y_temp = add;        // 加法
            3'b001: y_temp = min;        // 减法
            3'b010: y_temp = inverse;    // 取反
            3'b011: y_temp = and_result; // 与
            3'b100: y_temp = or_result;  // 或
            3'b101: y_temp = xor_result; // 异或
            3'b110: y_temp = slt_result; // 比较大小
            3'b111: y_temp = eq_result;  // 判断相等
            default: y_temp = 4'h0;      // 默认
        endcase
    end
    assign y = y_temp;

    // 零标志
    assign zero = (y_temp == 4'h0);

    // 溢出标志（加减法）
    wire add_overflow = (n1[3] == n2[3]) && (add[3] != n1[3]);
    wire sub_overflow = (n1[3] != neg_n2[3]) && (min[3] != n1[3]);
    assign overflow = (sel == 3'b000) ? add_overflow :
                      (sel == 3'b001) ? sub_overflow : 1'b0;

    // 进位标志（加减法）
    assign carry = (sel == 3'b000) ? add_carry :
                   (sel == 3'b001) ? sub_carry : 1'b0;
endmodule

module top (
    input wire [15:0] sw,
    input wire [4:0] btn,
    output wire [15:0] ledr
);
    wire signed [3:0] n1 = sw[7:4];     // 操作数 A
    wire signed [3:0] n2 = sw[3:0];     // 操作数 B
    wire [2:0] sel = sw[15:13];         // 功能选择
    wire signed [3:0] y;                // 结果
    wire zero, overflow, carry;         // 标志

    ALU alu_inst (
        .n1(n1),
        .n2(n2),
        .sel(sel),
        .y(y),
        .zero(zero),
        .overflow(overflow),
        .carry(carry)
    );

    assign ledr[3:0] = y;           // 结果
    assign ledr[4] = carry;         // 进位标志
    assign ledr[5] = overflow;      // 溢出标志
    assign ledr[6] = zero;          // 零标志
    assign ledr[15:7] = 9'h0;       // 未使用 LED 置 0
endmodule