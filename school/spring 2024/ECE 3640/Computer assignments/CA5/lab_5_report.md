---
title: 'Lab 5 Part One Report'
author: 'Nathan Walker'
geometry: "left=3cm, right=3cm, top=2cm, bottom=2cm"
...

1. Measure the time required to compute the convolution from CA3 using convolution method 1 based on for loops
    For method 1 the elapsed time was 253.531 seconds or 4.226 minutes.
2. Measure the time required to compute the convolution from CA3 using convolution method 2 based on frequency domain convolution
    For method 2 the elapsed time was .257337 seconds on a 2^20 or 1048576 point transform.
3. verify that the outputs are the same for both methods. 
    Taking the difference of the two methods and then using the matlab function norm. The result ended up being 2.12e-12, this shows that the two different methods result in the same output. As well as a subjective listening test the two outputs are equal as the sound was the same for both.

|          | Sample Rate      | Length of x | Length of h | Length of y | FFT size | Execution times(s) |
| :---:    |  :---:           |  :---:      |  :---:      |     :---:   |  :---:   |      :---:      |
| Method 1 |    44100         |     896043  |   55633     |     1033674 | N/A      | 253.531         |
| Method 2 |    44100         |   896043    |   55633     |     1048576 | 1048576  | .257337         |

\pagebreak
 
# Appendix

## myconv.m

```
function y = myconv(x, h)
    N = length(x);
    L = length(h);

    K = N + 2*(L-1);
    xzp = zeros(K,1);
    xzp(L:L+N-1) = x;

    M = N+L-1;
    y = zeros(M,1);
    for n=1:M
        for k = 1:L
            y(n) = y(n) + xzp(n - k + L) * h(k);
        end
    end
end
```

## fastconv.m

```
function y = fastconv(x,h,N)
    X = fft(x,N);
    H = fft(h,N);
    y = ifft(X.*H);
end

```

## main.m

```

clear
clc
close all

[h, Fs] = audioread('Corner bucket basement impulse.wav');

h2 = size(size(h) - 82000);
h2 = h(82000:length(h));

[x, Fsx] = audioread("Nathan-Guitar-chamber.wav");

tic
y1 = myconv(x,h2);
toc

N = nextpow2(length(h2)+length(x)-1)
N = 2^N;

tic 
y2 = fastconv(x,h2,N);
toc

```