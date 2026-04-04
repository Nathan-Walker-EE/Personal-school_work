clear all; clc; close all;

Fs = 8000;
n = 0:Fs*2;
input = sin(2*pi/25.*n);
g = 1;
iterations = 8;
outputs = [];
gs = [];
for i = 1:iterations
    gs = [gs;g];
    output = distort_audio(input, g);
    outputs = [outputs;output];
    g = g * 1.2;
end

legend_entries = cell(iterations,1);
figure(1)
for i = 1:iterations
    legend_entries{i} = sprintf('g = %.3f', gs(i));
    plot(n/Fs, outputs(i,:));
    hold on;
end
hold off;
legend(legend_entries);

