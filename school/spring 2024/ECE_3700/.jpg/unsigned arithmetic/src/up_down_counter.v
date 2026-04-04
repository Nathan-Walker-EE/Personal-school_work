`timescale 1ns/1ps

module up_down_counter
  (
   input clk,
   input [7:0] a,
   input [7:0] b,
   input up,
   input dn,
   input rst,
   output reg [7:0] q
   );

   wire [8:0] 	    test;
   assign test = q;
   
   always @(posedge clk) begin
      
      if (rst)
	q <= a;
      else begin
	 if(up && (test + b) < 255)
	   q <= q + b;

	 else if (dn && (test - b) < 255)
	   q <= q - b;
      end
      
   end // always @ (posedge clk)
endmodule // up_down_counter
