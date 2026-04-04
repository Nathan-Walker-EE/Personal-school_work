---
title: 'Homework 8'
author: 'Nathan Walker'
geometry: "left=3cm, right=3cm, top=2cm, bottom=2cm"
...

1. This problem explores the design a symmetric real-valued linear phase FIR bandpass filter that meets the following specifications.

|filter design|Length|Order| group delay| type | window parameters|
| :---:|:---:|:---:|:---:| :---:| :---:|
|Window| 62| 61| 30.5| II| hann|
|Kaiser|46|45|22.5| II| $\beta$ = 3.3953|
|equiripple|30|29|14.5|II|equiripple|

## Window Design

![impulse response of the windowed and non windowed filter](p1_impulse.png){ width=70%}

![Linear magnitude response of the windowed filter](p1_magresponse.png){ width=70%}

![Logrithmic magnitude response of the windowed filter](p1_maglogresponse.png){ width=70%}

![Phase response of the windowed filter](p1_phaseresponse.png){ width=70%}

![Pole zero plot of the windowed filter](p1_zplane.png){ width=70%}

## Kaiser

![impulse response of the windowed and non windowed filter](p1_impulse_k.png){ width=70%}

![Linear magnitude response of the windowed filter](p1_magresponse_k.png){ width=70%}

![Logrithmic magnitude response of the windowed filter](p1_maglogresponse_k.png){ width=70%}

![Phase response of the windowed filter](p1_phaseresponse_k.png){ width=70%}

![Pole zero plot of the windowed filter](p1_zplane_k.png){ width=70%}

## equiripple

![impulse response of the equiripple filter](p1_impulse_eq.png){ width=70%}

![Linear magnitude response of the equiripple filter](p1_magresponse_eq.png){ width=70%}

![Logrithmic magnitude response of the equiripple filter](p1_maglogresponse_eq.png){ width=70%}

![Phase response of the equiripple filter](p1_phaseresponse_eq.png){ width=70%}

![Pole zero plot of the equiripple filter](p1_zplane_eq.png){ width=70%}

![Pole zero plot of the equiripple filter about the unit circle](p1_zplane_eq_zoomed.png){ width=70%}

2. This problem explores designing a lowpass filter with a very narrow transition band that achieves the following specifications A~p~ = 0.1 db ripple in the passband (0 $\le$ |f| $\le$ 0.24) and A~s~ = 60dB suppression in the stopband (0.26 $\le$ |f| $\le$ 0.5).

|filter design|Length|Order| group delay| type | window parameters|
| :---:|:---:|:---:|:---:| :---:| :---:|
|Window| 275| 274| 137 | I| Blackman|
|Kaiser|183|182|91| I| $\beta$ = 5.6533|
|equiripple|137|136|68|I|equiripple|

## Window Design

![impulse response of the windowed and non windowed filter](p2_impulse.png){ width=70%}

![Linear magnitude response of the windowed filter](p2_magresponse.png){ width=70%}

![Logrithmic magnitude response of the windowed filter](p2_maglogresponse.png){ width=70%}

![Phase response of the windowed filter](p2_phaseresponse.png){ width=70%}

![Pole zero plot of the windowed filter](p2_zplane.png){ width=70%}

![Pole zero plot of the windowed filter about the unit circle](p2_zplane_zoomed.png){ width=70%}

## Kaiser

![impulse response of the windowed and non windowed filter](p2_impulse_k.png){ width=70%}

![Linear magnitude response of the windowed filter](p2_magresponse_k.png){ width=70%}

![Logrithmic magnitude response of the windowed filter](p2_maglogresponse_k.png){ width=70%}

![Phase response of the windowed filter](p2_phaseresponse_k.png){ width=70%}

![Pole zero plot of the windowed filter](p2_zplane_k.png){ width=70%}

## equiripple

![impulse response of the equiripple filter](p2_impulse_eq.png){ width=70%}

![Linear magnitude response of the equiripple filter](p2_magresponse_eq.png){ width=70%}

![Logrithmic magnitude response of the equiripple filter](p2_maglogresponse_eq.png){ width=70%}

![Phase response of the equiripple filter](p2_phaseresponse_eq.png){ width=70%}

![Pole zero plot of the equiripple filter](p2_zplane_eq.png){ width=70%}

3. This problem explores designing a lowpass filter for sample rate conversion using the system depicted in Fig.6.47(c) in the textbook(page 379) in which the upsampling factor is U = 3 and the downsampling factor is D = 4. The maximum frequency in the signal x[n] is f~max,x~ = 1/3. design the lowpass filter to have A~p~ = 0.1 dB of ripple in the pass band and A~s~ = 60 dB of attenuation in the stopband. Use your knowledge of upsampling and downsampling to determine the passband and stopband edge frequencies, f~p~ and f~s~. What are f~p~ and f~s~.

f~p~ = 1/9
f~s~ = 2/9

|filter design|Length|Order| group delay| type | window parameters|
| :---:       |:---: |:---:|:---:       | :---:| :---:|
|Window| 50| 49| 24.5| II| Blackman|
|Kaiser|34|33|16.5| II| $\beta$ = 5.6533|
|equiripple|25|24|12|I|equiripple|

## Window Design

![impulse response of the windowed and non windowed filter](p3_impulse.png){ width=70%}

![Linear magnitude response of the windowed filter](p3_magresponse.png){ width=70%}

![Logrithmic magnitude response of the windowed filter](p3_maglogresponse.png){ width=70%}

![Phase response of the windowed filter](p3_phaseresponse.png){ width=70%}

![Pole zero plot of the windowed filter](p3_zplane.png){ width=70%}

## Kaiser

![impulse response of the windowed and non windowed filter](p3_impulse_k.png){ width=70%}

![Linear magnitude response of the windowed filter](p3_magresponse_k.png){ width=70%}

![Logrithmic magnitude response of the windowed filter](p3_maglogresponse_k.png){ width=70%}

![Phase response of the windowed filter](p3_phaseresponse_k.png){ width=70%}

![Pole zero plot of the windowed filter](p3_zplane_k.png){ width=70%}

## equiripple

![impulse response of the equiripple filter](p3_impulse_eq.png){ width=70%}

![Linear magnitude response of the equiripple filter](p3_magresponse_eq.png){ width=70%}

![Logrithmic magnitude response of the equiripple filter](p3_maglogresponse_eq.png){ width=70%}

![Phase response of the equiripple filter](p3_phaseresponse_eq.png){ width=70%}

![Pole zero plot of the equiripple filter](p3_zplane_eq.png){ width=70%}

4. Suppose you would like to boost by a factor of 4 the low frequencies of an audio signal between the range of 0 $\le$ |F| $\le$ 150 Hz and leave the scale of frequencies above 300 Hz unchanged. This type of filter has two passbands and no stopband. is is called a shelving filter. The transtion band extends from 150 Hz to 300 Hz. You plan ot perfomr this processing using the system depected in Fig6.1 of the textbook (page 331) using a sample rate of 1/T = F~s~ = 20k samples/second. Convert this information to a specification for a discrete-time filter and design a linear phase filter to meet this spec. Require your filter to limit deviations from the desired response to $\delta$ = 0.01 in both pass bands. 

|filter design|Length|Order| group delay| type | window parameters|
| :---:|:---:|:---:|:---:| :---:| :---:|
|Window| 441| 440| 220| I| Hamming|
|Kaiser|411|410|205| I| $\beta$ = 4.7717|
|equiripple|353|352|176|I|equiripple|

## Window Design

![impulse response of the windowed and non windowed filter](p4_impulse.png){ width=70%}

![Linear magnitude response of the windowed filter](p4_magresponse.png){ width=70%}

![Logrithmic magnitude response of the windowed filter](p4_maglogresponse.png){ width=70%}

![Phase response of the windowed filter](p4_phaseresponse.png){ width=70%}

![Pole zero plot of the windowed filter](p4_zplane.png){ width=70%}

## Kaiser

![impulse response of the windowed and non windowed filter](p4_impulse_k.png){ width=70%}

![Linear magnitude response of the windowed filter](p4_magresponse_k.png){ width=70%}

![Logrithmic magnitude response of the windowed filter](p4_maglogresponse_k.png){ width=70%}

![Phase response of the windowed filter](p4_phaseresponse_k.png){ width=70%}

![Pole zero plot of the windowed filter](p4_zplane_k.png){ width=70%}

## equiripple

![impulse response of the equiripple filter](p4_impulse_eq.png){ width=70%}

![Linear magnitude response of the equiripple filter](p4_magresponse_eq.png){ width=70%}

![Logrithmic magnitude response of the equiripple filter](p4_maglogresponse_eq.png){ width=70%}

![Phase response of the equiripple filter](p4_phaseresponse_eq.png){ width=70%}

![Pole zero plot of the equiripple filter](p4_zplane_eq.png){ width=70%}

5. After working the previous 4 problems, summarize your observations about the best filter design method to use if the goal is to minimize the filter length.

Using the windowing technique to satisfy the parameters leads to over designing. The filters fufill the requirements but have higher order counts. Kaiser consistently has a 20 to 30 orders less than the window method. Equiripple has half to a third of the windowing order. Thus far equiripple design method is the best for minimizing order degree while fulfilling requirements.

6. Design a differentiator/delay filter pair to be used in FM demodulation. Let the received signal be x[n]. Design type III derivative and delay filters using a Hamming window with length L = 21. Design type IIII derivative and delay filters using a Hamming window with length L = 22. In each case, plot the impulse response and the frequency response over -1/2 $\le$ f $\le$ 1/2.

## Type 3

![Impulse response of the type 3 difference filter.](p6_impulse_response_type3.png){width=70%}

![Frequency response of the type 3 difference filter.](p6_freq_response_type3.png){width=70%}

## Type 4

![Impulse response of the type 4 difference filter.](p6_impulse_response_type4.png){width=70%}

![Frequency response of the type 4 difference filter.](p6_freq_response_type4.png){width=70%}

\newpage

# Appendix

```
%% Problem 1
Ap = .87;
As = 40.424;
df = 0.05;
fc1 = .15;
fc2 = 0.35;

L = 62;
M = 61;
n = (0:M) - M/2;
N = 2^14;
win = hanning(L);
h = ((sin(2 * pi * fc2 .* n)./(pi .* n)) - (sin(2 * pi * fc1 .* n)./(pi .* n))).';
hwin = h .* win;
plot(hwin);
hold on;
plot(h);
title('Bandpass impulse response');
xlabel('samples');
ylabel('Amplitude');
legend('hwin', 'h');
hold off
saveas(gcf, 'p1_impulse.png');
Hwin = fft(hwin,N);
plot((0:N-1)/N - .5,abs(fftshift(Hwin)));
title('Bandpass Magnitude response');
xlabel('Frequency');
ylabel('Magnitude')
saveas(gcf,'p1_magresponse.png');
plot((0:N-1)/N - .5,20*log10(abs(fftshift(Hwin))));
title('Bandpass Magnitude response(dB)')
xlabel('Frequency');
ylabel('Magnitude(dB)');
saveas(gcf,'p1_maglogresponse.png');
plot((0:N-1)/N - .5,rad2deg(unwrap(angle(fftshift(Hwin)))));
title('Bandpass Phase response');
xlabel('frequency');
ylabel('Phase(degrees)')
saveas(gcf,'p1_phaseresponse.png');
zplane(hwin.');
saveas(gcf,'p1_zplane.png');

beta = 0.5842 * (40-21)^0.4 + 0.07886 * (40 - 21);
M = ceil((40-8)/(2.285 * 2 * pi * .05));
L = M + 1;
n = (0:M) - M/2;
h = ((sin(2 * pi * fc2 .* n)./(pi .* n)) - (sin(2 * pi * fc1 .* n)./(pi .* n))).';
win = kaiser(L,beta);
hwin = h .* win;

plot(hwin);
hold on;
plot(h);
title('Kaiser Bandpass impulse response');
xlabel('samples');
ylabel('Amplitude');
legend('hwin', 'h');
hold off
saveas(gcf, 'p1_impulse_k.png');
Hwin = fft(hwin,N);
plot((0:N-1)/N - .5,abs(fftshift(Hwin)));
title('Kaiser Bandpass Magnitude response');
xlabel('Frequency');
ylabel('Magnitude')
saveas(gcf,'p1_magresponse_k.png');
plot((0:N-1)/N - .5,20*log10(abs(fftshift(Hwin))));
title('Kaiser Bandpass Magnitude response(dB)')
xlabel('Frequency');
ylabel('Magnitude(dB)');
saveas(gcf,'p1_maglogresponse_k.png');
plot((0:N-1)/N - .5,rad2deg(unwrap(angle(fftshift(Hwin)))));
title('Kaiser Bandpass Phase response');
xlabel('frequency');
ylabel('Phase(degrees)')
saveas(gcf,'p1_phaseresponse_k.png');
zplane(hwin.');
saveas(gcf,'p1_zplane_k.png');

%equiripple
h = load('p1_equiripple.fcf');
plot(h);
title('Equiripple Bandpass impulse response');
xlabel('samples');
ylabel('Amplitude');
legend('hwin', 'h');
hold off
saveas(gcf, 'p1_impulse_eq.png');
Hwin = fft(h,N);
plot((0:N-1)/N - .5,abs(fftshift(Hwin)));
title('Equiripple Bandpass Magnitude response');
xlabel('Frequency');
ylabel('Magnitude')
saveas(gcf,'p1_magresponse_eq.png');
plot((0:N-1)/N - .5,20*log10(abs(fftshift(Hwin))));
title('Equiripple Bandpass Magnitude response(dB)')
xlabel('Frequency');
ylabel('Magnitude(dB)');
saveas(gcf,'p1_maglogresponse_eq.png');
plot((0:N-1)/N - .5,rad2deg(unwrap(angle(fftshift(Hwin)))));
title('Equiripple Bandpass Phase response');
xlabel('frequency');
ylabel('Phase(degrees)')
saveas(gcf,'p1_phaseresponse_eq.png');
zplane(h.');
saveas(gcf,'p1_zplane_eq.png');
xlim([-1.5 1.5]);
saveas(gcf,'p1_zplane_eq_zoomed.png');

%% problem 2
Ap = .1;
As = 60;
gamma = 10^(Ap/20);
deltap = (gamma-1)/(gamma + 1);
deltas = 10^(-As/20);
delta = min([deltap deltas]);
A = 20 * log10(delta);
fp = .24;
fs = .26;
df = fs-fp;
fc = .5*(fs + fp);

%blackman
L = ceil((11)/(2 * df));
M = L - 1;
N = 2^15;
win = blackman(L).';
n = (0:M) - M/2;
h = (2 * fc)*sinc(2 * fc .* n);
hwin = h .* win;

plot(hwin);
hold on;
plot(h);
title('lowpass impulse response');
xlabel('samples');
ylabel('Amplitude');
legend('hwin', 'h');
hold off
saveas(gcf, 'p2_impulse.png');
Hwin = fft(hwin,N);
plot((0:N-1)/N - .5,abs(fftshift(Hwin)));
title('lowpass Magnitude response');
xlabel('Frequency');
ylabel('Magnitude')
saveas(gcf,'p2_magresponse.png');
plot((0:N-1)/N - .5,20*log10(abs(fftshift(Hwin))));
title('lowpass Magnitude response(dB)')
xlabel('Frequency');
ylabel('Magnitude(dB)');
saveas(gcf,'p2_maglogresponse.png');
plot((0:N-1)/N - .5,rad2deg(unwrap(angle(fftshift(Hwin)))));
title('lowpass Phase response');
xlabel('frequency');
ylabel('Phase(degrees)')
saveas(gcf,'p2_phaseresponse.png');
zplane(hwin);
saveas(gcf,'p2_zplane.png');
xlim([-1.5 1.5]);
saveas(gcf,'p2_zplane_zoomed.png');

%Kaiser
gamma = 10^(Ap/20)
dp = (gamma - 1)/(gamma + 1);
ds = 10^(-As/20);
delta = min([dp ds]);
A = -20*log10(delta);

beta = 0.1102 * (A - 8.7);
M = ceil((A-8)/(2.285 * 2 * pi * df))
L = M + 1;
n = (0:M) - M/2;
h = (2 * fc)*sinc(2 * fc .* n);
win = kaiser(L,beta).';
hwin = h .* win;

plot(hwin);
hold on;
plot(h);
title('Kaiser lowpass impulse response');
xlabel('samples');
ylabel('Amplitude');
legend('hwin', 'h');
hold off
saveas(gcf, 'p2_impulse_k.png');
Hwin = fft(hwin,N);
plot((0:N-1)/N - .5,abs(fftshift(Hwin)));
title('Kaiser lowpass Magnitude response');
xlabel('Frequency');
ylabel('Magnitude')
saveas(gcf,'p2_magresponse_k.png');
plot((0:N-1)/N - .5,20*log10(abs(fftshift(Hwin))));
title('Kaiser lowpass Magnitude response(dB)')
xlabel('Frequency');
ylabel('Magnitude(dB)');
saveas(gcf,'p2_maglogresponse_k.png');
plot((0:N-1)/N - .5,rad2deg(unwrap(angle(fftshift(Hwin)))));
title('Kaiser lowpass Phase response');
xlabel('frequency');
ylabel('Phase(degrees)')
saveas(gcf,'p2_phaseresponse_k.png');
zplane(hwin);
saveas(gcf,'p2_zplane_k.png');

%equiripple
h = load('p2_equiripple.fcf');
plot(h);
title('Equiripple lowpass impulse response');
xlabel('samples');
ylabel('Amplitude');
legend('hwin', 'h');
hold off
saveas(gcf, 'p2_impulse_eq.png');
Hwin = fft(h,N);
plot((0:N-1)/N - .5,abs(fftshift(Hwin)));
title('Equiripple lowpass Magnitude response');
xlabel('Frequency');
ylabel('Magnitude')
saveas(gcf,'p2_magresponse_eq.png');
plot((0:N-1)/N - .5,20*log10(abs(fftshift(Hwin))));
title('Equiripple lowpass Magnitude response(dB)')
xlabel('Frequency');
ylabel('Magnitude(dB)');
saveas(gcf,'p2_maglogresponse_eq.png');
plot((0:N-1)/N - .5,rad2deg(unwrap(angle(fftshift(Hwin)))));
title('Equiripple lowpass Phase response');
xlabel('frequency');
ylabel('Phase(degrees)')
saveas(gcf,'p2_phaseresponse_eq.png');
zplane(h.');
saveas(gcf,'p2_zplane_eq.png');
xlim([-1.5 1.5]);
saveas(gcf,'p2_zplane_eq_zoomed.png');

%% Problem 3
Ap = .1;
As = 60;
fp = 1/9;
fs = 2/9;
df = fs-fp;
fc = .5*(fs + fp);

%blackman
L = ceil((11 * pi)/(2 * pi * df));
M = L - 1;
N = 2^15;
win = blackman(L).';
n = (0:M) - M/2;
h = (2 * fc)*sinc(fc * 2 .* n);
hwin = h .* win;

plot(hwin);
hold on;
plot(h);
title('lowpass impulse response');
xlabel('samples');
ylabel('Amplitude');
legend('hwin', 'h');
hold off
saveas(gcf, 'p3_impulse.png');
Hwin = fft(hwin,N);
plot((0:N-1)/N - .5,abs(fftshift(Hwin)));
title('lowpass Magnitude response');
xlabel('Frequency');
ylabel('Magnitude')
saveas(gcf,'p3_magresponse.png');
plot((0:N-1)/N - .5,20*log10(abs(fftshift(Hwin))));
title('lowpass Magnitude response(dB)')
xlabel('Frequency');
ylabel('Magnitude(dB)');
saveas(gcf,'p3_maglogresponse.png');
plot((0:N-1)/N - .5,rad2deg(unwrap(angle(fftshift(Hwin)))));
title('lowpass Phase response');
xlabel('frequency');
ylabel('Phase(degrees)')
saveas(gcf,'p3_phaseresponse.png');
zplane(hwin);
saveas(gcf,'p3_zplane.png');
xlim([-1.5 1.5]);
saveas(gcf,'p3_zplane_zoomed.png');

gamma = 10^(Ap/20)
dp = (gamma - 1)/(gamma + 1);
ds = 10^(-As/20);
delta = min([dp ds]);
A = -20*log10(delta);

beta = 0.1102 * (A - 8.7);
M = ceil((A-8)/(2.285 * 2 * pi * df));
L = M + 1;
n = (0:M) - M/2;
h = (2 * fc)*sinc(fc * 2 .* n);
win = kaiser(L,beta).';
hwin = h .* win;

plot(hwin);
hold on;
plot(h);
title('Kaiser lowpass impulse response');
xlabel('samples');
ylabel('Amplitude');
legend('hwin', 'h');
hold off
saveas(gcf, 'p3_impulse_k.png');
Hwin = fft(hwin,N);
plot((0:N-1)/N - .5,abs(fftshift(Hwin)));
title('Kaiser lowpass Magnitude response');
xlabel('Frequency');
ylabel('Magnitude')
saveas(gcf,'p3_magresponse_k.png');
plot((0:N-1)/N - .5,20*log10(abs(fftshift(Hwin))));
title('Kaiser lowpass Magnitude response(dB)')
xlabel('Frequency');
ylabel('Magnitude(dB)');
saveas(gcf,'p3_maglogresponse_k.png');
plot((0:N-1)/N - .5,rad2deg(unwrap(angle(fftshift(Hwin)))));
title('Kaiser lowpass Phase response');
xlabel('frequency');
ylabel('Phase(degrees)')
saveas(gcf,'p3_phaseresponse_k.png');
zplane(hwin);
saveas(gcf,'p3_zplane_k.png');

%equiripple
h = load('p3_equiripple.fcf');
plot(h);
title('Equiripple lowpass impulse response');
xlabel('samples');
ylabel('Amplitude');
legend('hwin', 'h');
hold off
saveas(gcf, 'p3_impulse_eq.png');
Hwin = fft(h,N);
plot((0:N-1)/N - .5,abs(fftshift(Hwin)));
title('Equiripple lowpass Magnitude response');
xlabel('Frequency');
ylabel('Magnitude')
saveas(gcf,'p3_magresponse_eq.png');
plot((0:N-1)/N - .5,20*log10(abs(fftshift(Hwin))));
title('Equiripple lowpass Magnitude response(dB)')
xlabel('Frequency');
ylabel('Magnitude(dB)');
saveas(gcf,'p3_maglogresponse_eq.png');
plot((0:N-1)/N - .5,rad2deg(unwrap(angle(fftshift(Hwin)))));
title('Equiripple lowpass Phase response');
xlabel('frequency');
ylabel('Phase(degrees)')
saveas(gcf,'p3_phaseresponse_eq.png');
zplane(h.');
saveas(gcf,'p3_zplane_eq.png');
xlim([-1.5 1.5]);
saveas(gcf,'p3_zplane_eq_zoomed.png');

%% Problem 4
delta = 0.01;
Ap = 20*log10((1 + delta/3)/(1-delta/3));
As = 20 * log10((1 + delta/3)/(delta/3));

Fs = 20e3;
fp = 150/Fs;
fs = 300/Fs;
df = fs-fp;
fc = .5*(fs + fp);

%blackman
L = ceil((6.6 * pi)/(2 * pi * df))+1;
M = L - 1;
N = 2^15;
win = hamming(L).';
n = (0:M) - M/2;
h1 = 3*(2 * fc)*sinc(fc * 2 .* n);
h2 = sinc(n);
h = h1 + h2;
hwin = h .* win;

plot(hwin);
hold on;
plot(h);
title('shelving impulse response');
xlabel('samples');
ylabel('Amplitude');
legend('hwin', 'h');
hold off
saveas(gcf, 'p4_impulse.png');
Hwin = fft(hwin,N);
plot((0:N-1)/N - .5,abs(fftshift(Hwin)));
title('shelving Magnitude response');
xlabel('Frequency');
ylabel('Magnitude')
saveas(gcf,'p4_magresponse.png');
plot((0:N-1)/N - .5,20*log10(abs(fftshift(Hwin))));
title('shelving Magnitude response(dB)')
xlabel('Frequency');
ylabel('Magnitude(dB)');
saveas(gcf,'p4_maglogresponse.png');
plot((0:N-1)/N - .5,rad2deg(unwrap(angle(fftshift(Hwin)))));
title('shelving Phase response');
xlabel('frequency');
ylabel('Phase(degrees)')
saveas(gcf,'p4_phaseresponse.png');
zplane(hwin);
saveas(gcf,'p4_zplane.png');

%Kaiser
gamma = 10^(Ap/20)
dp = (gamma - 1)/(gamma + 1);
ds = 10^(-As/20);
delta = min([dp ds]);
A = 52;

%beta = 0.5842*(A-21)^0.4 + 0.07886*(A-21);
beta = 0.1102* (A - 8.7);
M = ceil((A-8)/(2.285 * 2 * pi * df)) + 1;
L = M + 1;
n = (0:M) - M/2;

h1 = 3*(2 * fc)*sinc(fc * 2 .* n);
%Change delay filter to a sinc function
h2 = sinc(n);
h = h1 + h2;
win = kaiser(L,beta).';
hwin = h .* win;

plot(hwin);
hold on;
plot(h);
title('Kaiser lowpass impulse response');
xlabel('samples');
ylabel('Amplitude');
legend('hwin', 'h');
hold off
saveas(gcf, 'p4_impulse_k.png');
Hwin = fft(hwin,N);
plot((0:N-1)/N - .5,abs(fftshift(Hwin)));
title('Kaiser lowpass Magnitude response');
xlabel('Frequency');
ylabel('Magnitude')
saveas(gcf,'p4_magresponse_k.png');
plot((0:N-1)/N - .5,20*log10(abs(fftshift(Hwin))));
title('Kaiser lowpass Magnitude response(dB)')
xlabel('Frequency');
ylabel('Magnitude(dB)');
saveas(gcf,'p4_maglogresponse_k.png');
plot((0:N-1)/N - .5,rad2deg(unwrap(angle(fftshift(Hwin)))));
title('Kaiser lowpass Phase response');
xlabel('frequency');
ylabel('Phase(degrees)')
saveas(gcf,'p4_phaseresponse_k.png');
zplane(hwin);
saveas(gcf,'p4_zplane_k.png');

%equiripple
h = 3 * load('p4_equiripple.fcf').';
M = length(h) - 1;
L = length(h);
h(M/2+1) = h(M/2 + 1) + 1;

plot(h);
title('Equiripple lowpass impulse response');
xlabel('samples');
ylabel('Amplitude');
hold off
saveas(gcf, 'p4_impulse_eq.png');
Hwin = fft(h,N);
plot((0:N-1)/N - .5,abs(fftshift(Hwin)));
title('Equiripple lowpass Magnitude response');
xlabel('Frequency');
ylabel('Magnitude')
saveas(gcf,'p4_magresponse_eq.png');
plot((0:N-1)/N - .5,20*log10(abs(fftshift(Hwin))));
title('Equiripple lowpass Magnitude response(dB)')
xlabel('Frequency');
ylabel('Magnitude(dB)');
saveas(gcf,'p4_maglogresponse_eq.png');
plot((0:N-1)/N - .5,rad2deg(unwrap(angle(fftshift(Hwin)))));
title('Equiripple lowpass Phase response');
xlabel('frequency');
ylabel('Phase(degrees)')
saveas(gcf,'p4_phaseresponse_eq.png');
zplane(h);
saveas(gcf,'p4_zplane_eq.png');
xlim([-1.5 1.5]);
saveas(gcf,'p4_zplane_eq_zoomed.png');

%% Problem 6
%type 3 filter
L = 21;
M = 20;
n = (0:M) - M/2;
h = cos(pi .* n)./(pi .* n) - sin(pi .* n)./(pi .* n.^2);
h(M/2 + 1) = 0;

w = hamming(L).';
d = h.*w;

NFFT = 2^16;
f = [-NFFT/2:NFFT/2-1]/NFFT;
H = fftshift(abs(fft(h,NFFT)));
D = fftshift(abs(fft(d,NFFT)));

plot(f, D,'LineWidth',2);
title('frequency response of diff filter');
xlabel('frequency');
ylabel('amplitude');
saveas(gcf,'p6_freq_response_type3.png');

plot(h);
title('impulse response');
xlabel('samples');
ylabel('amplitude');
saveas(gcf,'p6_impulse_response_type3.png')

%type 4 filter
L = 22;
M = 21;
n = (0:M) - M/2;
h = cos(pi .* n)./(pi .* n) - sin(pi .* n)./(pi .* n.^2);

w = hamming(L).';
d = h.*w;

NFFT = 2^16;
f = [-NFFT/2:NFFT/2-1]/NFFT;
H = fftshift(abs(fft(h,NFFT)));
D = fftshift(abs(fft(d,NFFT)));

plot(f, D,'LineWidth',2);
title('frequency response of diff filter');
xlabel('frequency');
ylabel('amplitude');
saveas(gcf,'p6_freq_response_type4.png');

plot(h);
title('impulse response');
xlabel('samples');
ylabel('amplitude');
saveas(gcf,'p6_impulse_response_type4.png')

```