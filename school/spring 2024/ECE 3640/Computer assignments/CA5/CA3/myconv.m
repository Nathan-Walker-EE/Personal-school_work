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
