`timescale 1ns/1ps

module testbench ();
   
   // DECLARE SIGNALS
   reg clk;     // "reg" type signals are  controlled
   reg [7:0] a;  // by the testbench
   reg [7:0] b;  // by the testbench
   reg 	     rst;
   reg 	     up;
   reg 	     dn;

   wire [7:0] q;
   
   integer    done = 0;
   
   integer clk_count = 0;
   integer rst_count = 0;
   
   
   // INITIAL SIGNAL CONFIGURATION:
   initial begin
      clk = 0;
      rst = 1;
      up  = 1;
      dn  = 0;
      a   = 128;
      b   = 65;
   end

   // GENERATE CLOCK:
   initial forever #10 clk = ~clk;

   always @(posedge clk) begin
      if(rst == 1) begin
	 rst_count <= rst_count + 1;
      end
      if(rst_count == 2)begin
	 rst_count <= 0;
	 rst <= 0;
	 
      end
      
   end
   
   // CREATE STIMULI:
   always @(posedge clk) begin
      if(done != 1) begin
	 up <= ~clk_count[3];
	 dn <= ~up;
      end
      
      if(dn == 1 && done != 1) begin
	 a <= 17;
	 b <= 3;
	 rst <= 1;
	 done = done + 1;
	 
      end
   end

   up_down_counter DUT (.clk(clk), .a(a), .b(b), .up(up), .dn(dn), .rst(rst), .q(q));
   

   // WRITE OUTPUT TO CONSOLE:
   integer fid;
   initial fid = $fopen("test_result.txt", "w");
   
   always @(posedge clk) begin
      $write("clk:  %d", clk_count);      
      $write("\ta:  %b", a);
      $write("\tb:  %b", b);
      $write("\tup: %b", up);      
      $write("\tdn: %b", dn);
      $write("\trst: %b", rst);
      $write("\tq: %b", q);
      $write("\n");
      
      $fwrite(fid,"clk:  %d", clk_count);      
      $fwrite(fid,"\ta:  %b", a);
      $fwrite(fid,"\tb:  %b", b);
      $fwrite(fid, "\tup: %b", up);
      $fwrite(fid, "\tdn: %b", dn);
      $fwrite(fid, "\trst: %b", rst);
      $fwrite(fid, "\tq: %b", q);
      $fwrite(fid,"\n");
   end

   // DEFINE WHEN TO TERMINATE SIMULATION:
   always @(posedge clk) begin
      clk_count <= clk_count + 1;
      if (clk_count == 28) begin
	 $fclose(fid);
	 $finish;
      end
   end

   
endmodule // testbench
