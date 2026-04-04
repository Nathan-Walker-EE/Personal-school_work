format compact
clear all
clc
1 
T1 = zeros(5,4); T1(1,4) = 1;
T2 = zeros(5,4); T2(1,3) = 1; T2(2, 4) = 1;
T3 = zeros(5,4); T3(1,2) = 1; T3(2, 3) = 1; T3(3, 4) = 1;
T4 = eye(4); T4 = [eye(4); 0, 0, 0, 0];
T5 = [0,0,0,0;eye(4)];
T7 = fliplr(flip(T2));
T6 = fliplr(flip(T3));
T8 = fliplr(flip(T1));
n1 = trace(T1 * T1');
n2 = trace(T2 * T2');
n3 = trace(T3 * T3');
n4 = trace(T4 * T4');
n5 = trace(T5 * T5');
n6 = trace(T6 * T6');
n7 = trace(T7 * T7');
n8 = trace(T8 * T8');
T1 = T1/n1;
T2 = T2/n2;
T3 = T3/n3;
T4 = T4/n4;
T5 = T5/n5;
T6 = T6/n6;
T7 = T7/n7;
T8 = T8/n8;

x = [1 3 2 4; 2 2 2 2; 4 7 5 3; 5 4 3 2; 2 3 4 5];
c1 = trace(x*T1')
c2 = trace(x*T2')
c3 = trace(x*T3')
c4 = trace(x*T4')
c5 = trace(x*T5')
c6 = trace(x*T6')
c7 = trace(x*T7')
c8 = trace(x*T8')