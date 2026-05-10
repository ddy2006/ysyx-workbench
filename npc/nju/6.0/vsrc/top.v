module shift_register (
    input wire clk,
    input wire rst_n,
    input wire [2:0] ctrl,
    input wire [7:0] data_in,
    input wire serial_in,
    output reg [7:0] data_out
);

    reg [7:0] reg_data;    

    always @(negedge rst_n) begin
//    always @(posedge clk or negedge rst_n) begin
//       if (rst_n) begin
//           reg_data <= 8'b0;
//       end else begin
            case (ctrl)
//                3'b000: reg_data <= 8'b0;                    // 清0
                3'b001: reg_data <= data_in;                 // 置数
                3'b010: reg_data <= {1'b0, reg_data[7:1]};   // 逻辑右移
                3'b011: reg_data <= {reg_data[6:0], 1'b0};   // 逻辑左移
                3'b100: reg_data <= {reg_data[7], reg_data[7:1]}; // 算术右移
                3'b101: reg_data <= {reg_data[6:0], serial_in}; // 串行输入
                3'b110: reg_data <= {reg_data[0], reg_data[7:1]}; // 循环右移
                3'b111: reg_data <= {reg_data[6:0], reg_data[7]}; // 循环左移
                default: reg_data <= reg_data;
            endcase
//        end
//        $display("clk = %b", clk);
    end

    always @(*) begin
        data_out = reg_data;
//        data_out = 8'b11100011;
    end 

endmodule

module top(
    input wire clk,
    input wire [15:0] sw,
    input wire [4:0] btn,
    output wire [15:0] ledr
    );

    shift_register shift_regist(
        .clk(clk),
        .rst_n(btn[2]),
        .ctrl(sw[15:13]),
        .data_in(sw[7:0]),
        .serial_in(sw[8]),
        .data_out(ledr[7:0])
    );
    assign ledr[15:8]=8'b0;
endmodule
