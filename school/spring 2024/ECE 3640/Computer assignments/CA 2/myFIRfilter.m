function y = myFIRfilter(b,x)
persistent z L; % Persistent internal filter memory
if isempty(z)
    z = zeros(size(b)); % Initialize to zero
    L = length(b);
end

    y = 0;
    for n = L:-1:2
        z(n) = z(n-1);
        y = y + b(n) * z(n);
    end
    z(1) = x;
    y = y + z(1)*b(1);
end