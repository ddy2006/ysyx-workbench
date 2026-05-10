module top (
    input wire [15:0] sw,   // NVBoard 的 16 位开关输入
    output wire [15:0] ledr  // NVBoard 的 16 位 LED 输出
);

    // 二位四选一选择器
    wire [7:0] a;    // 4 个 2 位输入，a[7:0]
    wire [1:0] s;    // 2 位选择信号
    wire [1:0] y;    // 2 位输出

    // 信号映射
    assign s = sw[1:0];      // 使用 SW8 和 SW9 作为选择信号 s[1:0]
    assign a = sw[9:2];      // 使用 SW0 到 SW7 作为输入 a[7:0]
    assign ledr[1:0] = y;    // 使用 LED0 和 LED1 显示输出 y[1:0]
    assign ledr[15:2] = 14'b0; // 未使用的 LED 置为 0

    // 实例化两个 1 位四选一选择器
    mux41b mux_low (
        .a({a[6], a[4], a[2], a[0]}), // 低位输入：a[0], a[2], a[4], a[6]
        .s(s),                        // 选择信号
        .y(y[0])                      // 输出低位
    );

    mux41b mux_high (
        .a({a[7], a[5], a[3], a[1]}), // 高位输入：a[1], a[3], a[5], a[7]
        .s(s),                        // 选择信号
        .y(y[1])                      // 输出高位
    );

endmodule

module mux41b(a, s, y);
    input  [3:0] a;
    input  [1:0] s;
    output y;
    MuxKeyWithDefault #(4, 2, 1) i0 (y, s, 1'b0, {
        2'b00, a[0],
        2'b01, a[1],
        2'b10, a[2],
        2'b11, a[3]
    });
endmodule

module MuxKeyInternal #(NR_KEY = 2, KEY_LEN = 1, DATA_LEN = 1, HAS_DEFAULT = 0) (
    output reg [DATA_LEN-1:0] out,
    input [KEY_LEN-1:0] key,
    input [DATA_LEN-1:0] default_out,
    input [NR_KEY*(KEY_LEN + DATA_LEN)-1:0] lut
);
    localparam PAIR_LEN = KEY_LEN + DATA_LEN;
    wire [PAIR_LEN-1:0] pair_list [NR_KEY-1:0];
    wire [KEY_LEN-1:0] key_list [NR_KEY-1:0];
    wire [DATA_LEN-1:0] data_list [NR_KEY-1:0];

    generate
        for (genvar n = 0; n < NR_KEY; n = n + 1) begin
            assign pair_list[n] = lut[PAIR_LEN*(n+1)-1 : PAIR_LEN*n];
            assign data_list[n] = pair_list[n][DATA_LEN-1:0];
            assign key_list[n]  = pair_list[n][PAIR_LEN-1:DATA_LEN];
        end
    endgenerate

    reg [DATA_LEN-1 : 0] lut_out;
    reg hit;
    integer i;
    always @(*) begin
        lut_out = 0;
        hit = 0;
        for (i = 0; i < NR_KEY; i = i + 1) begin
            lut_out = lut_out | ({DATA_LEN{key == key_list[i]}} & data_list[i]);
            hit = hit | (key == key_list[i]);
        end
        if (!HAS_DEFAULT) out = lut_out;
        else out = (hit ? lut_out : default_out);
    end
endmodule

module MuxKey #(NR_KEY = 2, KEY_LEN = 1, DATA_LEN = 1) (
    output [DATA_LEN-1:0] out,
    input [KEY_LEN-1:0] key,
    input [NR_KEY*(KEY_LEN + DATA_LEN)-1:0] lut
);
    MuxKeyInternal #(NR_KEY, KEY_LEN, DATA_LEN, 0) i0 (out, key, {DATA_LEN{1'b0}}, lut);
endmodule

module MuxKeyWithDefault #(NR_KEY = 2, KEY_LEN = 1, DATA_LEN = 1) (
    output [DATA_LEN-1:0] out,
    input [KEY_LEN-1:0] key,
    input [DATA_LEN-1:0] default_out,
    input [NR_KEY*(KEY_LEN + DATA_LEN)-1:0] lut
);
    MuxKeyInternal #(NR_KEY, KEY_LEN, DATA_LEN, 1) i0 (out, key, default_out, lut);
endmodule