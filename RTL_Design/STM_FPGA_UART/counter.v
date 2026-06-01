module test_seq (
    input  wire clk,
    output reg  [7:0] out
);

reg [23:0] prescaler = 0; 
reg [7:0]  counter = 0;

always @(posedge clk) begin
    prescaler <= prescaler + 1;

    // prescaled to match the STM's speed.    
    if (prescaler == 24'd0) begin
        counter <= counter + 1'b1;
        out     <= counter + 1'b1; 
    end
end

endmodule