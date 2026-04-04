
N = 5       %size of array/# of w_n terms
r = randn(1,N + 1) %generate arbitrary correlation values
R = toeplitz(r(1:N)); %create toeplitz
b = r(2:N+1).'; %create b vector
w = zeros(length(b),1); % create storage vector
w(1) = b(1)/R(1) % base case w1 = r1/ro
wk = w(1);%make first case wk vector
for i = 2:length(b)
    rk = R(1,2:i).'; %build vector up as it iterates
    j = flipud(eye(length((rk))));%resizes J matrix
    alpha = b(i) - rk.' * (j * wk);
    alpha = alpha / (R(1,1) - rk.' * conj(wk));%computes alpha from pg 402
    zk = wk - alpha * j * conj(wk);% scales zk by alpha
    wk = [zk;alpha]; %computes new wk   
    %on last iteration wk = solved wn values
end
wk
R\b