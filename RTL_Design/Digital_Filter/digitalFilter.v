module lowPassFilter (
    input  wire clk,
    input  wire signed [7:0] in,
    output reg  signed [7:0] out,
    input  wire mode
);

    // ---------------- PARAMETERS ----------------
    localparam N = 16;

    reg signed [7:0] x [0:N-1];
    reg signed [7:0] h [0:N-1];

    reg signed [31:0] acc;
    integer i, j;

    // ---------------- INIT COEFF ----------------
    initial begin
        $readmemb("fir_coeff.mem", h);

        for (j = 0; j < N; j = j + 1)
            x[j] = 0;
    end

    // ---------------- SHIFT REGISTER ----------------
    always @(posedge clk) begin
        for (i = N-1; i > 0; i = i - 1)
            x[i] <= x[i-1];

        x[0] <= in;
    end

    // ---------------- FIR MAC ----------------
    always @(posedge clk) begin
        acc = 0;

        for (i = 0; i < N; i = i + 1)
            acc = acc + (x[i] * h[i]);
    end

    // ---------------- SCALING ----------------
    wire signed [31:0] scaled;
    assign scaled = acc >>> 7;   // 8-bit coeff normalization

    // ---------------- SATURATION ----------------
    wire signed [7:0] fir_out;

    assign fir_out =
        (scaled > 127)  ? 127 :
        (scaled < -128) ? -128 :
        scaled[7:0];

    // ---------------- OUTPUT ----------------
    always @(posedge clk) begin
        if (mode == 0)
            out <= fir_out;
        else
            out <= in - fir_out;  // high-pass option
    end

endmodule


module tb_lpf;

reg clk;
reg mode;
reg signed [7:0] in;
wire signed [7:0] out;

integer t;

lowPassFilter lpf(
    .clk(clk),
    .in(in),
    .out(out),
    .mode(mode)
);

initial clk = 0;
always #5 clk = ~clk;

initial begin

    mode = 0;

    for(t = 0; t < 400; t = t + 1) begin

        @(posedge clk);

        // LOW frequency component
        in = 50 * $sin(2.0 * 3.14159 * t / 80.0)

        // HIGH frequency noise
           + 20 * $sin(2.0 * 3.14159 * t / 4.0);

    end

    $finish;

end

initial begin
    $monitor("t=%0d in=%0d out=%0d", t, in, out);
end

endmodule