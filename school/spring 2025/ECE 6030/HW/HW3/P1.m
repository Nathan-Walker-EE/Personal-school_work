clear all
clc
close all

p = [inf, 4, 3, 2, 1, 0.8, 0.5, 0.2, eps];
x = -1:.00001:1;
colors = lines(length(p));
figure
hold on
for i = 1:length(p)
    y = nthroot(1 - abs(x).^p(i), p(i));
    plot(x,[y; -y], 'Color', colors(i, :));
end
saveas(gcf, 'norm_plot.png');
hold off