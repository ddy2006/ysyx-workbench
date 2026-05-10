module random_number (
    input wire clk,        
    input wire rst_n,
    output reg [7:0] data_out
);
    wire feedback;

    assign feedback = data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0];

    always @(posedge clk or negedge rst_n) begin
        // $display("clk = %b", clk);
        if (!rst_n) begin
            data_out <= 8'h01;
        end else begin
            if (data_out == 8'h00) begin
                data_out <= 8'h01;
            end else begin
                data_out <= {feedback, data_out[7:1]};
            end
        end
    end
endmodule

module bcd7seg (
    input wire [3:0] b,
    output reg [7:0] h     // 七段数码管段码
);
    always @(*) begin
        case (b)
            4'b0000: h = 8'h02; // 显示 0
            4'b0001: h = 8'h9e; // 显示 1
            4'b0010: h = 8'h24; // 显示 2
            4'b0011: h = 8'h0c; // 显示 3
            4'b0100: h = 8'h98; // 显示 4
            4'b0101: h = 8'h48; // 显示 5
            4'b0110: h = 8'h40; // 显示 6
            4'b0111: h = 8'h1f; // 显示 7
            4'b1000: h = 8'h00; // 显示 8
            4'b1001: h = 8'h08; // 显示 9
            4'b1010: h = 8'h11; // 显示 A
            4'b1011: h = 8'h01; // 显示 B
            4'b1100: h = 8'h47; // 显示 C
            4'b1101: h = 8'h03; // 显示 D
            4'b1110: h = 8'h61; // 显示 E
            4'b1111: h = 8'h71; // 显示 F
            default: h = 8'hff; // 默认关闭
        endcase
    end
endmodule
module top (
    input wire clk,
    input wire [4:0] btn,
    output wire [7:0] seg0,
    output wire [7:0] seg1,
    output wire [7:0] seg2,
    output wire [7:0] seg3,
    output wire [7:0] seg4,
    output wire [7:0] seg5,
    output wire [7:0] seg6,
    output wire [7:0] seg7
);
    // output declaration of module random_number
    reg [7:0] data_out;
    
    random_number u_random_number(
        .clk      	(clk       ),
        .rst_n    	(btn[2]    ),
        .data_out 	(data_out  )
    );

    bcd7seg u_bcd7seg(
        .b 	(data_out[3:0]),
        .h 	(seg0)
    );

    bcd7seg u_bcd7seg1(
        .b 	(data_out[7:4]),
        .h 	(seg1)
    );
    assign seg2 = 8'hFF;
    assign seg3 = 8'hFF;
    assign seg4 = 8'hFF;
    assign seg5 = 8'hFF;
    assign seg6 = 8'hFF;
    assign seg7 = 8'hFF;
endmodule