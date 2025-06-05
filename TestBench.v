`timescale 1ns/1ns

module Comparator_tb;
    // Inputs
    reg [1:0] A;
    reg [1:0] B;
    
    // Outputs
    wire gt;
    wire lt;
    wire eq;
    
  
    Comparator uut (
        .A(A), 
        .B(B), 
        .gt(gt), 
        .lt(lt), 
        .eq(eq)
    );
    
    initial begin
        // Initialize Inputs
        A = 2'b00;
        B = 2'b00;
        
        
        #100;
        
        
        // Test case 1
        A = 2'b00; B = 2'b00; #20;
        
        // Test case 2
        A = 2'b00; B = 2'b01; #20;
        
        // Test case 3
        A = 2'b00; B = 2'b10; #20;
        
        // Test case 4
        A = 2'b00; B = 2'b11; #20;
        
        // Test case 5
        A = 2'b01; B = 2'b00; #20;
        
        // Test case 6
        A = 2'b01; B = 2'b01; #20;
        
        // Test case 7
        A = 2'b01; B = 2'b10; #20;
        
        // Test case 8
        A = 2'b01; B = 2'b11; #20;
        
        // Test case 9
        A = 2'b10; B = 2'b00; #20;
        
        // Test case 10
        A = 2'b10; B = 2'b01; #20;
        
        // Test case 11
        A = 2'b10; B = 2'b10; #20;
        
        // Test case 12
        A = 2'b10; B = 2'b11; #20;
        
        // Test case 13
        A = 2'b11; B = 2'b00; #20;
        
        // Test case 14
        A = 2'b11; B = 2'b01; #20;
        
        // Test case 15
        A = 2'b11; B = 2'b10; #20;
        
        // Test case 16
        A = 2'b11; B = 2'b11; #20;
        
        
        #20 $stop;
    end
endmodule
