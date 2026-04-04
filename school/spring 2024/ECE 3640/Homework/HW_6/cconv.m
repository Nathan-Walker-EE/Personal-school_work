function y = cconv(x,h,N)
    X = fft(x,N);
    H = fft(h,N);
    y = ifft(X.*H);
end