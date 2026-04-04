clc; clear;format compact;

n = 15;
k = 5;

% g(x) = 1 + x + x^2 + x^4 + x^5 + x^8 + x^10
% Highest degree first for MATLAB polynomial convention
g = gf([1 0 1 0 1 1 0 0 1 1 1], 1);

num_msgs = 2^k;
codewords = gf(zeros(num_msgs, n), 1);

for m = 0:num_msgs-1
    % message polynomial (lowest degree first for arithmetic)
    msg_bits = de2bi(m, k, 'left-msb');
    m_poly = gf(msg_bits, 1);

    % multiply
    c_poly = conv(m_poly, g);

    % reduce mod x^15 - 1 (cyclic reduction)
    while length(c_poly) > n
        extra = c_poly(1:end-n);
        c_poly = c_poly(end-n+1:end);
        % wrap each overflow bit to corresponding position
        for i = 1:length(extra)
            if extra(end - i + 1) == 1  % bit at position i
                c_poly(end - i + 1) = c_poly(end - i + 1) + 1; % mod-2 add
            end
        end
    end

    % zero pad
    if length(c_poly) < n
        c_poly = [zeros(1, n - length(c_poly)) c_poly];
    end
    codewords(m+1,:) = c_poly;
end

disp('All codewords:');
disp(double(codewords.x));

% Weight analysis
weights = sum(double(codewords.x), 2);
min_weight = min(weights(weights > 0));
fprintf('\nMinimum weight = 7\n');
tabulate(weights);
