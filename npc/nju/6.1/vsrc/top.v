module barrel_shifter (
    input wire rst_n,
    input wire update_en,
    input wire [1:0] ctrl,
    input wire [2:0] shamt,
    input wire [7:0] data_in,
    output reg [7:0] data_out
);
    // reg [7:0] stored_data;
    always @(posedge rst_n or negedge update_en) begin
        if (rst_n) begin
            // stored_data <= data_in;
            data_out <= data_in;
        end else begin
            case (ctrl)
                2'b00: data_out <= data_out >> shamt;    //逻辑右移
                2'b10: data_out <= data_out << shamt;    //逻辑左移
                2'b01: data_out <= $signed(data_out) >>> shamt;   //算术右移
                2'b11: data_out <= data_out << shamt;    //算数左移
                default: data_out <= data_out;
            endcase
        end
    end
endmodule

module top (
    input wire [15:0] sw,
    input wire [4:0] btn,
    output wire [15:0] ledr
);
    barrel_shifter shift_regist (
        .rst_n(btn[2]),
        .update_en(btn[4]),
        .ctrl(sw[15:14]),
        .shamt(sw[13:11]),
        .data_in(sw[7:0]),
        .data_out(ledr[7:0])
    );

    assign ledr[15:8] = 8'b0;
endmodule