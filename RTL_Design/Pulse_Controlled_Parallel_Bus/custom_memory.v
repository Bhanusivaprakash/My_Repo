`timescale 1ns / 1ps

module custom_memory(
    input wire write_enable,
    input wire read_enable,
    inout wire [7:0] data,
    input wire rst,
    input wire clk
);
    reg [7:0] memory [0:31];
    reg [7:0] data_out;
    
    reg [7:0] acc = 0;

    reg [4:0] write_ptr = 5'd0;
    reg [4:0] read_ptr  = 5'd0;

    reg read_lock = 0;
    reg write_lock = 0;
    
    integer i;

    always @(posedge clk) begin
    
        if(rst) begin
            write_ptr <= 0;
            read_ptr <= 0;
            data_out <= 0;
            acc <= 0;
            
            for(i = 0; i < 32; i = i + 1)
                memory[i] <= 8'h00;
        end

        else begin
            // WRITE SECTION
            if(write_enable && !write_lock) begin
                memory[write_ptr] <= data;
                write_ptr         <= write_ptr + 1;
                write_lock        <= 1;
            end
            else if(!write_enable) begin
                write_lock <= 0;
            end
            
            // READ SECTION
            if(read_enable && !read_lock) begin
                //data_out <= memory[read_ptr];
                acc <= acc + memory[read_ptr];
                read_ptr <= read_ptr + 1;
                read_lock <= 1; 
            end
            else if(!read_enable) begin
                read_lock <= 0;
            end
        end
        
    end

    assign data = (read_enable) ? data_out : 8'bz;


ila_0 ila_dbg (
    .clk(clk),

    .probe0(data),
    .probe1(write_ptr),
    .probe2(write_enable),
    .probe3(acc),
    .probe4(read_enable),
    .probe5(read_ptr),
    .probe6(rst)
);

endmodule