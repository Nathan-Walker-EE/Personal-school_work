x = linspace(0,10,100)
tiledlayout(2,1)

load approximation
plot(approximation(:,1),approximation(:,2))
title('approximation')

hold on
t = 0:.0001:10;
eqn = 5.2134 .* exp(-0.2128 .* t) - 0.2134 .* exp(-195.53 .* t);
plot(t,eqn);

legend('approximation', 'actual');

hold off