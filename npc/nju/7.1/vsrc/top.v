module ps2_keyboard (
    input clk,
    input resetn,
    input ps2_clk,
    input ps2_data,
    output reg [9:0] buffer,
    output reg [3:0] count
);
    reg [2:0] ps2_clk_sync;

    always @(posedge clk) begin
        ps2_clk_sync <= {ps2_clk_sync[1:0], ps2_clk};
    end

    wire sampling = ps2_clk_sync[2] & ~ps2_clk_sync[1];

    always @(posedge clk) begin
        if (resetn == 0) begin
            count <= 0;
            buffer <= 0;
        end
        else begin
            if (sampling) begin
                if (count == 4'd10) begin
                    if ((buffer[0] == 0) && (ps2_data) && (^buffer[9:1])) begin
                    end
                    count <= 0;
                end else begin
                    buffer[count] <= ps2_data;
                    count <= count + 1;
                end
            end
        end
    end
endmodule

module seven_segment_display (
    input clk,
    input resetn,
    input ps2_clk,
    input ps2_data,
    output [7:0] seg0,
    output [7:0] seg1,
    output [7:0] seg2,
    output [7:0] seg3,
    output [7:0] seg4,
    output [7:0] seg5
);
    wire [9:0] buffer;
    wire [3:0] count;
    reg [7:0] last_keycode;
    reg key_pressed;
    reg [7:0] press_count;

    ps2_keyboard ps2_inst (
        .clk(clk),
        .resetn(resetn),
        .ps2_clk(ps2_clk),
        .ps2_data(ps2_data),
        .buffer(buffer),
        .count(count)
    );

    reg [7:0] rom [0:255];
    initial begin
        integer i;
        for (i = 0; i < 256; i = i + 1) rom[i] = 8'h00;
        rom[8'h1C] = 8'h61;
        rom[8'h32] = 8'h62;
        rom[8'h21] = 8'h63;
        rom[8'h23] = 8'h64;
        rom[8'h24] = 8'h65;
        rom[8'h2B] = 8'h66;
        rom[8'h34] = 8'h67;
        rom[8'h33] = 8'h68;
        rom[8'h43] = 8'h69;
        rom[8'h3B] = 8'h6A;
        rom[8'h42] = 8'h6B;
        rom[8'h4B] = 8'h6C;
        rom[8'h3A] = 8'h6D;
        rom[8'h31] = 8'h6E;
        rom[8'h44] = 8'h6F;
        rom[8'h4D] = 8'h70;
        rom[8'h15] = 8'h71;
        rom[8'h2D] = 8'h72;
        rom[8'h1B] = 8'h73;
        rom[8'h2C] = 8'h74;
        rom[8'h3C] = 8'h75;
        rom[8'h2A] = 8'h76;
        rom[8'h1D] = 8'h77;
        rom[8'h22] = 8'h78;
        rom[8'h35] = 8'h79;
        rom[8'h1A] = 8'h7A;
        rom[8'h45] = 8'h30;
        rom[8'h16] = 8'h31;
        rom[8'h1E] = 8'h32;
        rom[8'h26] = 8'h33;
        rom[8'h25] = 8'h34;
        rom[8'h2E] = 8'h35;
        rom[8'h36] = 8'h36;
        rom[8'h3D] = 8'h37;
        rom[8'h3E] = 8'h38;
        rom[8'h46] = 8'h39;
    end

    always @(posedge clk or negedge resetn) begin
        if (!resetn) begin
            key_pressed <= 0;
            press_count <= 0;
            last_keycode <= 0;
        end else begin
            if (count == 4'd10 && buffer[0] == 0 && ps2_data && (^buffer[9:1])) begin
                if (buffer[8:1] == 8'hF0) begin
                    key_pressed <= 0;
                end else if (!key_pressed && buffer[8:1] != last_keycode) begin
                    key_pressed <= 1;
                    press_count <= press_count + 1;
                    last_keycode <= buffer[8:1];
                end
            end
        end
    end

    function [7:0] hex_to_seg;
        input [3:0] hex;
        case (hex)
            4'b0000: hex_to_seg = 8'h02;
            4'b0001: hex_to_seg = 8'h9e;
            4'b0010: hex_to_seg = 8'h24;
            4'b0011: hex_to_seg = 8'h0c;
            4'b0100: hex_to_seg = 8'h98;
            4'b0101: hex_to_seg = 8'h48;
            4'b0110: hex_to_seg = 8'h40;
            4'b0111: hex_to_seg = 8'h1f;
            4'b1000: hex_to_seg = 8'h00;
            4'b1001: hex_to_seg = 8'h08;
            4'b1010: hex_to_seg = 8'h10;
            4'b1011: hex_to_seg = 8'hc0;
            4'b1100: hex_to_seg = 8'h62;
            4'b1101: hex_to_seg = 8'h84;
            4'b1110: hex_to_seg = 8'h60;
            4'b1111: hex_to_seg = 8'h70;
            default: hex_to_seg = 8'hff;
        endcase
    endfunction

    wire [7:0] ascii_code = rom[buffer[8:1]];
    assign seg0 = key_pressed ? hex_to_seg(buffer[8:1][3:0]) : 8'hff;
    assign seg1 = key_pressed ? hex_to_seg(buffer[8:1][7:4]) : 8'hff;
    assign seg2 = key_pressed ? hex_to_seg(ascii_code[3:0]) : 8'hff;
    assign seg3 = key_pressed ? hex_to_seg(ascii_code[7:4]) : 8'hff;
    assign seg4 = hex_to_seg(press_count[3:0]);
    assign seg5 = hex_to_seg(press_count[7:4]);
endmodule

module top (
    input clk,
    input [4:0] btn,
    input rst,
    input ps2_clk,
    input ps2_data,
    output [7:0] seg0,
    output [7:0] seg1,
    output [7:0] seg2,
    output [7:0] seg3,
    output [7:0] seg4,
    output [7:0] seg5,
    output [7:0] seg6,
    output [7:0] seg7
);
    seven_segment_display display_inst (
        .clk(clk),
        .resetn(btn[2]),
        .ps2_clk(ps2_clk),
        .ps2_data(ps2_data),
        .seg0(seg0),
        .seg1(seg1),
        .seg2(seg2),
        .seg3(seg3),
        .seg4(seg4),
        .seg5(seg5)
    );

    assign seg6 = 8'hFF;
    assign seg7 = 8'hFF;
endmodule