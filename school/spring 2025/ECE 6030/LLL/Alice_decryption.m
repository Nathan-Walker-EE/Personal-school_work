clear all
clc
close all

B = eye(8);
B(8,1) = -906;
B(8,2) = -879;
B(8,3) = -817;
B(8,4) = -260;
B(8,5) = -594;
B(8,6) = -22;
B(8,7) = -425;
B(8,8) = 3287;
B_reduced = LLLReduce(B, 3/4);
disp('unreduced Basis');
disp(B);
disp('Reduced Basis:');
disp(B_reduced);