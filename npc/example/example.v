module top(
    input btn,
    output led
);
    assign led = btn; // 简单直连示例
endmodule