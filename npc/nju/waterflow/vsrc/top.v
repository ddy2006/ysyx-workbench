module top (
    input clk,
    input rst,
    output reg [15:0] ledr
);
    reg [31:0] count;
    always @(posedge clk) begin
        if (rst) begin
            ledr <= 16'b1; // 复位时 LED 初始为 1
            count <= 32'b0;
        end else begin
            if (count == 32'd0) begin
                ledr <= {ledr[14:0], ledr[15]}; // 左移实现流水效果
            end
            count <= (count >= 32'd5000000 ? 32'b0 : count + 1); // 计数到 5000000 后清零
        end
    end
endmodule