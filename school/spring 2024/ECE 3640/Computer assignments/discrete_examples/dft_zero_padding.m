NFFT = 2^4;
freq = [0:NFFT-1]/NFFT;
NFFTzp = 2^17;
freqzp = [0:NFFTzp-1]/NFFTzp;


n = [0:NFFT-1];
f0 = 4/NFFT;
x = exp(1j*2*pi*f0*n);

subplot(211);
stem([0:NFFT-1],abs(fft(x,NFFT)),'b');
xlabel('DFT Bin Index','FontSize',16);
xlim([0 NFFT]);
set(gca,'FontSize',16);

subplot(212);
plot(freqzp,abs(fft(x,NFFTzp)),'r');
hold on;
stem(freq,abs(fft(x,NFFT)),'b');
hold off;
legend('DTFT','DFT');
xlabel('Normalized Frequency [cycles/sample]','FontSize',16);
set(gca,'FontSize',16);


