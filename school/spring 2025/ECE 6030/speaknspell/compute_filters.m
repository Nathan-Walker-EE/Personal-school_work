clear all
close all
clc

f = 160;
Fs = 8000;
N = 12;

Haa = wavtocoeffs("aa", 12);
% Haa = plot_filters(Haa, "aa");
yaa = speech_synth(Haa, 150);

Ha = wavtocoeffs("a", 12);
% Ha = plot_filters(Ha, "a");
ya = speech_synth(Ha, 150);

Hah = wavtocoeffs("ah", 12);
% Hah = plot_filters(Hah, "ah");
yah = speech_synth(Hah, 150);

Hee = wavtocoeffs("ee", 12);
% Hee = plot_filters(Hee, "ee");
yee = speech_synth(Hee, 150);

He = wavtocoeffs("e", 12);
% He = plot_filters(He, "e");
ye = speech_synth(He, 150);

Hii = wavtocoeffs("II", 12);
% Hii = plot_filters(Hii, "II");
yii = speech_synth(Hii, 150);

Hi = wavtocoeffs("i", 12);
% Hi = plot_filters(Hi, "i");
yi = speech_synth(Hi, 150);

Hoo = wavtocoeffs("oo", 12);
% Hoo = plot_filters(Hoo, "oo");
yoo = speech_synth(Hoo, 150);

Ho = wavtocoeffs("o", 12);
% Ho = plot_filters(Ho, "o");
yo = speech_synth(Ho, 150);

Huu = wavtocoeffs("uu", 12);
% Huu = plot_filters(Huu, "uu");
yuu = speech_synth(Huu, 150);

Hu = wavtocoeffs("u", 12);
% Hu = plot_filters(Hu, "u");
yu = speech_synth(Hu, 150);

soundsc([yaa, ya, yah, yee, ye, yii, yi, yoo, yo, yuu, yu], Fs)