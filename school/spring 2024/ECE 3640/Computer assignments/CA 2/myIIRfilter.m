function y = myIIRfilter(b,a,x)
persistent zx zy Lx Ly; % Persistent internal filter memory
if isempty(zx)
    zx = zeros(size(b)); % Initialize input buffer to zero
    zy = zeros(size(a)); % Initialize output buffer zero
    Lx = length(b);
    Ly = length(a);
end

y = 0;
    for n = Lx:-1:2
        zx(n) = zx(n-1);
        y = y + b(n) * zx(n);
    end
zx(1) = x;
y = y + zx(1)*b(1);

    for n = Ly:-1:2
        zy(n) = zy(n-1);
        y = y - a(n)*zy(n);
    end
zy(1) = y;
end
