module coprocessor(

    input wire clk,
    input wire signed [15:0] data_in,
    output reg signed [15:0] data_out,
    output reg error,
    input wire write_enable,
    input wire read_enable,
    input wire rst,
    input wire [4:0] operation,
    input wire address_enable,
    input wire [4:0] address,
    input wire execute,
    output reg done

);

reg signed [15:0] memory [0:31];
reg signed [15:0] op_memory [0:31];
    
reg signed [31:0] acc = 0;
reg signed [31:0] result = 0;

reg [4:0] write_ptr = 5'd0;
//reg [4:0] read_ptr  = 5'd0;
reg [4:0] execution_ptr = 5'd0;
reg [4:0] store_ptr = 5'd0;
reg [4:0] scalar_ptr = 5'd0;

reg read_lock = 0;
reg write_lock = 0;

integer i;
integer sum_acc = 0, prod_acc = 1;

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
        sum_acc <= memory[execution_ptr];
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
        prod_acc <= prod_acc * memory[execution_ptr];
        acc <= prod_acc * memory[execution_ptr];
        sum_acc <= prod_acc * memory[execution_ptr];
        result <= prod_acc * memory[execution_ptr];
        execution_ptr <= execution_ptr + 1;
    end
end
endtask

task DIV;
begin
    if(execution_ptr == 4'b0) begin
        prod_acc <= memory[execution_ptr];
        execution_ptr <= 1;
    end
    else if(execution_ptr < write_ptr && memory[execution_ptr] != 0) begin
        prod_acc <= prod_acc / memory[execution_ptr];
        result  <= prod_acc / memory[execution_ptr];
        acc <= prod_acc / memory[execution_ptr];
        execution_ptr <= execution_ptr + 1;
        error <= 0;
    end
    else if (memory[execution_ptr] == 0) begin
        error <= 1;
    end
    $display("DIV: ptr=%0d val=%0d",
         execution_ptr,
         memory[execution_ptr]);
end
endtask

task SQUARE;
begin
    result <= (memory[address] * memory[address]);
end
endtask

task STORE;
begin
    memory[address] <= result;    
end
endtask

task SWAP;
reg signed [15:0] temp;
begin
    temp = memory[address];

    memory[address]   <= memory[address + 1];
    memory[address+1] <= temp;
end
endtask

task ABS;
begin
    result <= (memory[address][15]) ? -memory[address] : memory[address];
end
endtask

task CLEAR;
begin
    result <= 0;
    acc <= 0;
  //  op_memory[address] <= 0;
    sum_acc <= 0;
    prod_acc <= 1;
end
endtask

always @(posedge clk) begin

    if(rst) begin
        write_ptr <= 0;
        execution_ptr <= 0;
        data_out <= 0;
        acc <= 0;
        sum_acc <= 0;
        prod_acc <= 1;
        done <= 0;
        
        for(i = 0; i < 32; i = i + 1) begin
            memory[i] <= 16'h00;
            op_memory[i] <= 16'h00;
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
             data_in);
            memory[write_ptr] <= data_in;
            op_memory[write_ptr] <= data_in;
            write_ptr         <= write_ptr + 1;
            write_lock        <= 1;
        end
        else if(!write_enable) begin
            write_lock <= 0;
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

                4'b0100:    begin   //DIVISION;
                    DIV;
                    done <= (write_ptr != 0) && (execution_ptr == write_ptr - 1);
                end

                4'b0101:    begin   //SQUARE;
                    SQUARE;
                    //done <= (write_ptr != 0) && (scalar_ptr == write_ptr - 1);
                    done <= 1;
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

                default: begin
                    done <= 0;
                end
            endcase

        end

        // READ SECTION
        if(read_enable && !read_lock) begin
            //data_out <= memory[read_ptr];
            data_out <= result;
            //read_ptr <= read_ptr + 1;
            read_lock <= 1; 
        end
        else if(!read_enable) begin
            read_lock <= 0;
        end
    end
    
end

endmodule