load approximation
%plot(approximation(:,1),approximation(:,2))
%title('approximation vs analytical')

%hold on
t = 0:.0001:10;
eqn = 2.062.*exp(-.05.*t) + (-.031 + .2576*i).*exp((-.2-6*i).*t) + (-.031 - .2576*i).*exp((-.2+6*i).*t) ;
plot(t,eqn);
%hold off
legend('actual');

