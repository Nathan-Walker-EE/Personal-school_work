`timescale 1ns/1ps

module keypadmem
    (
        input clk,
        input rst,
        input [3:0] row,
        input done,
        output [3:0] col,
        output reg [15:0] prev_keys
    );

    wire [15:0] keys;
    reg [15:0] keys_changed;
    integer timer;

    wire rst_l;
    assign rst_l = ~rst;

    initial begin
        keys_changed <= 0;
        timer <= 0;
        prev_keys <= 0;
    end

    keypad #(.N(300_000)) kypd
        (
            .clk(clk),
            .rst_l(rst_l),
            .row(row),
            .col(col),
            .keys(keys)
        );

    always @(posedge clk) begin
        if (done) begin
            prev_keys <= 16'b0000000000000000; // Reset prev_keys when reset is active
            keys_changed <= 16'b0000000000000000; // Reset keys_changed when reset is active
        end else begin
            // Detect changes in keys
            if(keys != prev_keys)
                keys_changed <= keys;
            else    
                keys_changed <= 0;
            
            //keys_changed <= (keys != prev_keys) ? keys : 16'b0000000000000000;
            // Update prev_keys only if there's a change in keys

            if(keys_changed != 0) begin
                prev_keys <= keys_changed;
                keys_changed <= 0;
            end else
                prev_keys <= prev_keys;

            //prev_keys <= (keys_changed != 16'b0000_0000_0000_0000) ? keys_changed : prev_keys;
        end
    end


endmodule