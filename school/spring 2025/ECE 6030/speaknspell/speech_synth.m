function y = speech_synth(a, f)
    Fs = 8000;
    N = ceil(Fs/f);
    imp = zeros(1, Fs*1.5);
    for i = 1:length(imp)
        if mod(i,N) == 0
            imp(i) = 1;
        end
    end
    y = filter(1, a, imp);
    y = y/max(y);
end