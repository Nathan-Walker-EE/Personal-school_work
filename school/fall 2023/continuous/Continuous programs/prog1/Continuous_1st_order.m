x = linspace(0,10,100)
tiledlayout(2,1)

load approximation
plot(approximation(:,1),approximation(:,2))
title('approximation')

%hold on
%t = 0:.0001:10;
%eqn = 2 * exp(-3.*t);
%plot(t,eqn);
%hold off

legend('approximation')