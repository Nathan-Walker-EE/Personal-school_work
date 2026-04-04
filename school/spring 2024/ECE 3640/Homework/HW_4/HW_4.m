clear
clc
close all

%% 3.6

% 
% f = 0:.01:1;
% x = 2 * exp(j * -4 * pi .* f) .* sin(4 * pi .* f);
% 
% figure()
% plot(f, abs(x));
% title('my attempt');
% xlabel('frequency')
% ylabel('|H(f)|')
% saveas(gcf,'Mag_P6','png');
% 
% figure()
% plot(f, rad2deg(angle(x)));
% title('Phase')
% xlabel('frequency');
% ylabel('Phase (degrees)');
% saveas(gcf,'Phase_P6','png');



%% 3.2

F = 0:.01:1;
H = sin(3*pi.*F)./sin(pi.*F);
H = H .* exp(-j*2*pi.*F);

figure(1)
hold on
plot(F,abs(H),'linewidth',2);
xline(0,'linewidth',2)
xline(1/6,'linewidth',2)
xline(1/4,'linewidth',2)
xline(1/3,'linewidth',2)
xline(1/2,'linewidth',2)
title('Magnitude')
xlabel('frequency(radians per second)')
ylabel('|H(w)|')
saveas(gcf,'Mag_P2','png');
hold off

figure(2)
plot(F,rad2deg(angle(H)));
hold on
xline(0,'linewidth',2)
xline(1/6,'linewidth',2)
xline(1/4,'linewidth',2)
xline(1/3,'linewidth',.1)
xline(1/2,'linewidth',2)
title('Phase')
xlabel('frequency(radians per second)')
ylabel('Phase(degrees)')
saveas(gcf,'Phase_P2','png');
hold off

%% 3.15
% close all
% w = 0:.01:2*pi;
% H = 1 - 2*exp(j.*w) + exp(j*2 .*w);
% 
% plot(w,abs(H));
% title('Magnitude');
% xlabel('frequency');
% ylabel('|H(w)|')
% saveas(gcf,'Mag_P15','png');
% 
% figure()
% plot(w,rad2deg(angle(H)));
% title('Phase');
% xlabel('frequency');
% ylabel('Phase(degrees)')
% saveas(gcf,'Phase_P15','png');

%% 3.41
% 
% close all
% 
% w = -5:.01:5;
% x = sin(pi.*w)./(pi.*w);
% 
% plot(w,abs(x));