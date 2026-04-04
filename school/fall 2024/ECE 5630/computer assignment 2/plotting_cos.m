clear all
clc
close all

n = 1:47900;

filename = 'outputM_cos_4.bin';
ofilename = 'outputM_cos_4.png';
y = plot_dft(1/4, filename, n, ofilename, 'trad 1/4');

filename = 'outputM_cos_8.bin';
ofilename = 'outputM_cos_8.png';
y = plot_dft(1/8, filename, n, ofilename, 'trad 1/8');

filename = 'outputM_cos_16.bin';
ofilename = 'outputM_cos_16.png';
y = plot_dft(1/16, filename, n, ofilename, 'trad 1/16');

filename = 'outputM_cos_4_p5.bin';
ofilename = 'outputM_cos_4_p5.png';
y = plot_dft(1/4, filename, n, ofilename, 'poly 1/4');

filename = 'outputM_cos_8_p5.bin';
ofilename = 'outputM_cos_8_p5.png';
y = plot_dft(1/8, filename, n, ofilename, 'poly 1/8');

filename = 'outputM_cos_16_p5.bin';
ofilename = 'outputM_cos_16_p5.png';
y = plot_dft(1/16, filename, n, ofilename, 'poly 1/16');

