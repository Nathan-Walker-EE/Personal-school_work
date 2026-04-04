function a = wavtocoeffs(vowel, N)

    [signal, Fs] = audioread("wav_files/" + vowel +  ".wav");
    lena = length(signal.');
    raac = xcorr(signal, 'unbiased');
    r = raac(lena:lena+N);
    R = toeplitz(r(1:N));
    b = r(2:N+1);
    a = -R\b;
    a = [1 a.'];
end