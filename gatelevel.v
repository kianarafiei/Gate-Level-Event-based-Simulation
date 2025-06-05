`timescale 1ns/1ns

module Comparator(input[1:0] A,B, output gt, lt, eq);
wire [0:15] w;

//and gates
and#3 and1(w[4],w[0],B[1]);
and#3 and2(w[5],w[1],A[1]);
and#3 and3(w[6],w[2],B[0]);
and#3 and4(w[7],w[3],A[0]);
and#3 and5(w[10],w[9],w[6]);
and#3 and6(w[11],w[7],w[9]);

//or gates
or#3 or1(w[8],w[4],w[5]);
or#3 or2(w[12],w[10],w[4]);
or#3 or3(w[13],w[11],w[5]);
or#3 or4(w[14],w[12],w[13]);

//not gates
not#1 not1(w[0],A[1]);
not#1 not2(w[1],B[1]);
not#1 not3(w[2],A[0]);
not#1 not4(w[3],B[0]);
not#1 not5(w[9],w[8]);
not#1 not6(w[15],w[14]);


assign eq = w[15];
assign gt = w[13];
assign lt = w[12];

endmodule
