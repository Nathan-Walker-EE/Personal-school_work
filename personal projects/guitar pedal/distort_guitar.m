clear all; clc; close all;

[input, Fs] = audioread("12acguit - 12 String D4..wav");
output = distort_audio(input, 5);

n = 0:length(input)-1;
n = n/Fs;

plot(n, input)
hold on
plot(n, output/max(output))
hold off
xlabel('time');
ylabel('amplitude')
legend('input', 'output')

