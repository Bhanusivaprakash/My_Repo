module coprocessor(

    input wire clk,
    inout wire [15:0] data,
    output reg error,
    input wire write_enable_async,
    input wire read_enable_async,
    input wire rst,
    input wire [4:0] operation,
    input wire address_enable_async,
    input wire [4:0] address,
    input wire execute_async,
    output reg done,
    output reg ack

);

reg signed [31:0] memory [0:31];

reg signed [15:0] data_in;
reg signed [15:0] data_out;
    
reg signed [31:0] acc = 0;
reg signed [31:0] result = 0;

reg word_select = 0;
reg [4:0] write_ptr = 5'd0;
//reg [4:0] read_ptr  = 5'd0;
reg [4:0] execution_ptr = 5'd0;
reg [4:0] store_ptr = 5'd0;
reg [4:0] scalar_ptr = 5'd0;

reg read_lock = 0;
reg write_lock = 0;

integer i;
reg signed [31:0] sum_acc;
reg signed [31:0] prod_acc = 32'h00010000;
reg signed [63:0] temp_res;

task ADD;
begin
    $display("EXEC=%d", execution_ptr);
  if(execution_ptr < write_ptr) begin
        sum_acc <= sum_acc + memory[execution_ptr];
        acc <= sum_acc + memory[execution_ptr];
        prod_acc <= sum_acc + memory[execution_ptr];
        result <= sum_acc + memory[execution_ptr];
        execution_ptr <= execution_ptr + 1; 
    end  
end
endtask

task SUB;
begin
    if(execution_ptr == 4'b0) begin
        sum_acc = memory[execution_ptr];
        execution_ptr <= 1;
    end
    else if(execution_ptr < write_ptr) begin
        sum_acc <= sum_acc - memory[execution_ptr];
        result  <= sum_acc - memory[execution_ptr];
        acc <= sum_acc - memory[execution_ptr];
        execution_ptr <= execution_ptr + 1;
    end    
end
endtask

task MUL;
begin
    if(execution_ptr < write_ptr) begin
        temp_res = prod_acc * memory[execution_ptr];
        prod_acc <= temp_res >>> 16;
        acc <= temp_res >>> 16;
        sum_acc <= temp_res >>> 16;
        result <= temp_res >>> 16;
        execution_ptr <= execution_ptr + 1;
    end
end
endtask

task MAC;
begin
    if(execution_ptr < write_ptr) begin
        if(2*execution_ptr+1 < write_ptr) begin
            temp_res = acc + ((memory[2*execution_ptr] * memory[2*execution_ptr + 1]) >>> 16);
        end
        else begin
            temp_res = acc + (memory[2*execution_ptr]);
        end
        acc <= temp_res;
        result <= temp_res;    
        execution_ptr <= execution_ptr + 1;
    end
end
endtask

task STORE;
begin
    memory[address] <= result;    
end
endtask

task SWAP;
reg signed [31:0] temp;
begin
    temp = memory[address];

    memory[address]   <= memory[address + 1];
    memory[address+1] <= temp;
end
endtask

task ABS;
begin
    result <= (memory[address][31]) ? -memory[address] : memory[address];
end
endtask

task CLEAR;
begin
    sum_acc <= 0;
    prod_acc <= 32'h00010000; // NOT 1;
end
endtask

reg write_ff1, write_ff2;   // Sync flipflops
always @(posedge clk) begin
    write_ff1 <= write_enable_async;
    write_ff2 <= write_ff1;
end

wire write_enable = write_ff2;

reg read_ff1, read_ff2;   // Sync flipflops
always @(posedge clk) begin
    read_ff1 <= read_enable_async;
    read_ff2 <= read_ff1;
end

wire read_enable = read_ff2;
assign data = read_enable ? data_out : 16'bz;

reg address_ff1, address_ff2;   // Sync flipflops
always @(posedge clk) begin
    address_ff1 <= address_enable_async;
    address_ff2 <= address_ff1;
end

wire address_enable = address_ff2;

reg exec_ff1, exec_ff2;   // Sync flipflops
always @(posedge clk) begin
    exec_ff1 <= execute_async;
    exec_ff2 <= exec_ff1;
end

wire execute = exec_ff2;

always @(posedge clk) begin
    
    if(rst) begin
        write_ptr <= 0;
        execution_ptr <= 0;
        data_out <= 0;
        acc <= 0;
        sum_acc <= 0;
        prod_acc <= 32'h00010000; // NOT 1;
        done <= 0;
        word_select <= 0;
        write_lock <= 0;
        read_lock <= 0;
        error <= 0;
        result <= 0;;
        ack <= 0;
        
        for(i = 0; i < 32; i = i + 1) begin
            memory[i] <= 32'h00;
        end
    end

    else begin
        // WRITE SECTION
        if(address_enable) begin
            execution_ptr <= address;
        end

        if(write_enable && !write_lock) begin
            $display("WRITE: ptr=%0d data=%0d",
             write_ptr,
             data);
            if(word_select == 0) begin
                memory[write_ptr][15:0] <= data;
                write_lock        <= 1;
                ack <= 1;
                word_select <= 1;
            end
            else if(word_select == 1) begin
                memory[write_ptr][31:16] <= data;
                write_ptr         <= write_ptr + 1;
                write_lock        <= 1;
                ack <= 1;
                word_select <= 0;
            end
            
        end
        else if(!write_enable) begin
            write_lock <= 0;
            ack <= 0;
        end

        if(!execute) begin
           // error <= 0;
            done <= 0;
        end
        
        if(execute) begin
            case (operation)
                4'b0000:    begin
                    CLEAR;
                    done <= 1;
                end

                4'b0001:    begin   //ADDITION
                    ADD;
                    done <= (write_ptr != 0) && (execution_ptr == write_ptr - 1);
                end

                4'b0010:    begin   //SUBTRACTION;
                    SUB;
                    done <= (write_ptr != 0) && (execution_ptr == write_ptr - 1);
                end

                4'b0011:    begin   //PRODUCT;
                    MUL;
                    done <= (write_ptr != 0) && (execution_ptr == write_ptr - 1);
                end

                4'b0110:    begin   //ABS;
                    ABS;
                    //done <= (write_ptr != 0) && (execution_ptr == write_ptr - 1);
                    done <= 1;
                end
                    
                4'b1110:    begin
                    SWAP;
                    //done <= (write_ptr != 0) && (store_ptr == execution_ptr - 1);
                    done <= 1;
                end

                4'b1111:    begin
                    STORE;
                    //done <= (write_ptr != 0) && (store_ptr == execution_ptr - 1);
                    done <= 1;
                end

                5'b10000:   begin
                    MAC;
                    done <= (write_ptr != 0) && (execution_ptr == write_ptr - 1);
                end

                default: begin
                    done <= 0;
                end
            endcase

        end

        // READ SECTION
        if(read_enable && !read_lock) begin
            //data_out <= memory[read_ptr];
            if(word_select == 0) begin
                data_out <= result[15:0];
                //read_ptr <= read_ptr + 1;
                read_lock <= 1; 
                ack <= 1;
                word_select <= 1;
            end
            else if (word_select == 1) begin
                data_out <= result[31:16];
                read_lock <= 1;
                ack <= 1;
                word_select <= 0;
            end
            
        end
        else if(!read_enable) begin
            read_lock <= 0;
            ack <= 0;
        end
    end
    
end

ila_0 ila_dbg (
	.clk(clk), // input wire clk
	.probe0(result), // input wire [31:0] probe0
	.probe1(memory[0]),
	.probe2(operation),
	.probe3(address),
	.probe4(rst),
	.probe5(write_enable),
	.probe6(read_enable),
	.probe7(address_enable),
	.probe8(execute),
	.probe9(done),
	.probe10(ack)
);

endmodule

