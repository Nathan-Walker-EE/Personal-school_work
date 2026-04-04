function b = dectobin(n)

 if n == 0
        b = 0;
        return;
    end

    % Initialize variables
    b = [];
    
    % Convert decimal to binary
    while n > 0
        remainder = mod(n, 2);
        b = [remainder b];
        n = floor(n / 2);
    end
end