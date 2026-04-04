clear all
clc
close all

%% problem 2

imshow(imread('image.pgm'));
figure();
imshow(imread('problem_2.pgm'));

im = imread('image.pgm');
H = (1/81) * [1 2 3 2 1;2 4 6 4 2;3 6 9 6 3; 2 4 6 4 2; 1 2 3 2 1];
nim = conv2(im, H, 'same');
nim = uint8(nim);
filename = 'output.pgm';
imwrite(nim, filename, 'pgm');
figure();
imshow(imread(filename))

%% problem 3
close all
imshow(imread('image.pgm'));
figure();
imshow(imread('problem_3.pgm'));

im = imread('image.pgm');
H1 = [1 0 -1;2 0 -2;1 0 -1];
H2 = [-1 -2 -1;0 0 0;1 2 1];
nim1 = abs(conv2(im, H1, 'same'));
nim2 = abs(conv2(im, H2, 'same'));
nim = nim1 + nim2;
nim = uint8(nim);
filename = 'output_p3m.pgm';
imwrite(nim, filename, 'pgm');
figure();
imshow(imread(filename))
%% problem 4
close all
imshow(imread('image.pgm'));
figure();
imshow(imread('problem_4.pgm'));
im = imread('problem_4.pgm');
max(im, [], 'all')
