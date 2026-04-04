clear 
clc
close all

[h, Fs] = audioread('Corner bucket basement impulse.wav');

dt = 1/Fs;
h2 = size(size(h) - 82000);
h2 = h(82000:length(h));
t = 0:dt:(length(h2)*dt)-dt;

plot(t,h2)
title('Impulse response')
xlabel('time')
ylabel('amplidtude')
saveas(gcf,'Impulse response','png');
shg

[H, Fs2] = freqz(h2, 1, 2^14, 1);

figure;
plot(Fs2,abs(H));
title('Magnitude Response')
xlabel('Frequency Hz')
ylabel('|h2|');
saveas(gcf,'Magnitdue Response','png');
shg

figure;
plot(Fs2,20*log10(abs(H)));
title('Magnitude Response(dB)');
xlabel('Frequency Hz');
ylabel('20*log|h2|')
saveas(gcf,'Magnitude Response(dB)','png');
shg


[x, Fsx] = audioread("Nathan-Guitar-chamber.wav");

dt = 1/Fsx;
t = 0:dt:(length(x)*dt)-dt;

plot(t,x)
title('Input x[n]')
xlabel('time')
ylabel('amplidtude')
saveas(gcf,'Input Guitar','png');
shg

NFFT = 2^14;
figure();
[S,F,T] = spectrogram(x,hamming(NFFT),round(0.9*NFFT),NFFT,Fsx);
imagesc(T,F,20*log10(abs(S)),[0 50]);
colorbar;
set(gca,'YDir','normal');
ylim([0 1500]);
xlabel('Time [seconds]');
ylabel('Frequency [Hertz]');
title('Spectrogram of the Input x[n]');
saveas(gcf,'guitar_spectrogram','png');
grid on;

y = myconv(x,h2);
t2 = 0:dt:(length(y) * dt)-dt;
plot(t2, y);
title("Convolved guitar")
xlabel("time")
ylabel("amplitude")
saveas(gcf,'Output Guitar','png');

y = y/40;
plot(t2, y);
title("Reduced Convolved Guitar")
xlabel("time")
ylabel("amplitude")
saveas(gcf,'Reduced Output Guitar','png');

NFFT = 2^14;
figure();
[S,F,T] = spectrogram(y,hamming(NFFT),round(0.9*NFFT),NFFT,Fsx);
imagesc(T,F,20*log10(abs(S)),[0 50]);
colorbar;
set(gca,'YDir','normal');
ylim([0 1500]);
xlabel('Time [seconds]');
ylabel('Frequency [Hertz]');
title('Spectrogram of the Output y[n]');
saveas(gcf,'Output_guitar_spectrogram','png');
grid on;


audiowrite('Convolved_guitar.wav', y, Fsx);

[x2, Fsx] = audioread("Nate_speech.wav");
x2 = x2(:,1);

dt = 1/Fsx;
t = 0:dt:(length(x2)*dt)-dt;

plot(t,x2)
title('Input x[n]')
xlabel('time')
ylabel('amplidtude')
saveas(gcf,'Input Speech','png');
shg

NFFT = 2^14;
figure();
[S,F,T] = spectrogram(x2,hamming(NFFT),round(0.9*NFFT),NFFT,Fsx);
imagesc(T,F,20*log10(abs(S)),[0 50]);
colorbar;
set(gca,'YDir','normal');
ylim([0 1500]);
xlabel('Time [seconds]');
ylabel('Frequency [Hertz]');
title('Spectrogram of the Input x[n]');
saveas(gcf,'speech_spectrogram','png');
grid on;

y2 = myconv(x2,h2);
y2 = y2/27;
t3 = 0:dt:(length(y2) * dt)-dt;
plot(t3, y2);
title("Convolved speech")
xlabel("time")
ylabel("amplitude")
saveas(gcf,'Output speech','png');


plot(t3, y2);
title("Reduced Convolved speech")
xlabel("time")
ylabel("amplitude")
saveas(gcf,'Reduced Output Speech','png');

NFFT = 2^14;
figure();
[S,F,T] = spectrogram(y2,hamming(NFFT),round(0.9*NFFT),NFFT,Fsx);
imagesc(T,F,20*log10(abs(S)),[0 50]);
colorbar;
set(gca,'YDir','normal');
ylim([0 1500]);
xlabel('Time [seconds]');
ylabel('Frequency [Hertz]');
title('Spectrogram of the output y[n]');
saveas(gcf,'Output_speech_spectrogram','png');
grid on;

audiowrite('Convolved_speech.wav', y2, Fsx);