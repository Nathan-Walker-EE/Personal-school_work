function [y, B] = bss(x, B0, mu, func)
    I = eye(size(B0));
    y = zeros(size(x));
    xsz = size(x); 
    for t = 1:xsz(2)
        y(:,t) = B0 * x(:,t);
        if func == 1
            B = B0 + mu*(I + -y(:,t).^3 * y(:,t)')*B0;
        end
        if func == 2
            B = B0 + mu*(I + -2*tanh(y(:,t)) * y(:,t)') * B0;
        end
        if func == 3
            B = B0 + mu*(I + (tanh(y(:,t)) - y(:,t)) * y(:,t)') * B0;
        end
        B0 = B;
    end
end