module top (
    input wire [15:0] sw,       // NVBoard 的 16 位开关输入
    output wire [15:0] ledr,    // NVBoard 的 16 位 LED 输出
    output wire [7:0] seg0,     // 七段数码管 0
    output wire [7:0] seg1,     // 七段数码管 1
    output wire [7:0] seg2,     // 七段数码管 2
    output wire [7:0] seg3,     // 七段数码管 3
    output wire [7:0] seg4,     // 七段数码管 4
    output wire [7:0] seg5,     // 七段数码管 5
    output wire [7:0] seg6,     // 七段数码管 6
    output wire [7:0] seg7      // 七段数码管 7
);

    // 信号定义
    wire [7:0] x;    // 8 位输入
    wire en;         // 使能信号
    wire [2:0] y;    // 3 位编码输出
    wire [7:0] seg;  // 七段数码管段码

    // 信号映射
    assign x = sw[7:0];         // SW0 到 SW7 作为输入 x[7:0]
    assign en = sw[8];          // SW8 作为使能信号
    assign ledr[2:0] = y;       // LED0 到 LED2 显示 y[2:0]
    assign ledr[15:3] = 13'b0;  // 未使用的 LED 置为 0
    assign seg0 = seg;          // 七段数码管 0 显示结果
    assign seg1 = 8'hFF;        // 未使用的七段数码管关闭
    assign seg2 = 8'hFF;
    assign seg3 = 8'hFF;
    assign seg4 = 8'hFF;
    assign seg5 = 8'hFF;
    assign seg6 = 8'hFF;
    assign seg7 = 8'hFF;

    encode83 encoder (
        .x(x),
        .en(en),
        .y(y)
    );

    bcd7seg seg_dec (
        .b(y),
        .h(seg)
    );

endmodule

module encode83 (
    input wire [7:0] x,
    input wire en,
    output reg [2:0] y
);
    always @(*) begin
        if (!en) begin
            y = 3'b000; // 使能关闭时输出 0
        end else begin
            casez (x)
                8'b1???????: y = 3'b111; // x[7] = 1
                8'b01??????: y = 3'b110; // x[6] = 1, x[7] = 0
                8'b001?????: y = 3'b101; // x[5] = 1, x[7:6] = 0
                8'b0001????: y = 3'b100; // x[4] = 1, x[7:5] = 0
                8'b00001???: y = 3'b011; // x[3] = 1, x[7:4] = 0
                8'b000001??: y = 3'b010; // x[2] = 1, x[7:3] = 0
                8'b0000001?: y = 3'b001; // x[1] = 1, x[7:2] = 0
                8'b00000001: y = 3'b000; // x[0] = 1, x[7:1] = 0
                default:     y = 3'b000; // x = 0
            endcase
        end
    end
endmodule

module bcd7seg (
    input wire [2:0] b,    // 3 位二进制输入
    output reg [7:0] h     // 七段数码管段码
);
    always @(*) begin
        case (b)
            3'b000: h = 8'h02; // 显示 0
            3'b001: h = 8'h9e; // 显示 1
            3'b010: h = 8'h24; // 显示 2
            3'b011: h = 8'h0c; // 显示 3
            3'b100: h = 8'h98; // 显示 4
            3'b101: h = 8'h48; // 显示 5
            3'b110: h = 8'h40; // 显示 6
            3'b111: h = 8'h1f; // 显示 7
            default: h = 8'hff; // 默认关闭
        endcase
    end
endmodule