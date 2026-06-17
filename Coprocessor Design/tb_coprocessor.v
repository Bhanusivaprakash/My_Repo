
module tb_coprocessor;

reg clk;
reg signed [15:0] data_in;
wire signed [15:0] data_out;
reg write_enable, read_enable, rst, execute;
reg [4:0] operation;
reg address_enable;
reg [4:0] address;
wire done;
wire error;

coprocessor cp(
    .clk(clk),
    .data_in(data_in),
    .data_out(data_out),
    .error(error),
    .write_enable(write_enable),
    .read_enable(read_enable),
    .rst(rst),
    .operation(operation),
    .address_enable(address_enable),
    .address(address),
    .execute(execute),
    .done(done)
);

initial clk = 0;
always #5 clk = ~clk;

task write;
    input signed [15:0] value;
    begin
        @(negedge clk);
        write_enable = 1;
        data_in = value;

        @(posedge clk);   // DUT samples

        @(negedge clk);
        write_enable = 0;
    end
endtask

task load;
input [4:0] opcode;
input [4:0] addr;
begin
    @(negedge clk);
    operation      = opcode;
    address        = addr;
    address_enable = 1;

    @(posedge clk);

    @(negedge clk);
    address_enable = 0;
end
endtask

task exec;
begin
    @(negedge clk);
    execute = 1;

    wait(done == 1);

    @(negedge clk);
    execute = 0;

    @(posedge clk);
end
endtask

task read;
    begin
        @(negedge clk);
        read_enable = 1;

        @(posedge clk);

        @(negedge clk);
        read_enable = 0;

        $display("mem0=%0d mem1=%0d mem3=%0d",
                cp.memory[0],
                cp.memory[1],cp.memory[2]);

        $display("write_ptr=%0d execution_ptr=%0d",
                cp.write_ptr,
                cp.execution_ptr);

        $display("result=%0d",
                data_out);
    end
endtask

    integer j;

initial begin
    write_enable = 0;
    read_enable  = 0;
    execute      = 0;
    rst          = 1;
    data_in      = 0;
    operation    = 0;
    address      = 0;

    repeat(2) @(posedge clk);

    @(negedge clk);
    rst = 0;

    write(4);
    write(2);
    load(4'b0100, 0); //div
    exec();

    //load(4'b1111, 0); //store
    //exec();

    read();

    $display("\n---------------------------------------------------");
    $display("ADDR\tMEM\tOP_MEM");
    $display("---------------------------------------------------");

    for (j = 0; j < 6; j = j + 1) begin
        $display("[%0d]\t%0d\t\t%0d",
                j,
                cp.memory[j],
                cp.op_memory[j]);
    end

    $display("---------------------------------------------------\n");

    $display("Error = %d, Done = %d", cp.error, cp.done);

    #20;
    $finish;
end

endmodule
