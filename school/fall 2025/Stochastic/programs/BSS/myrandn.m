function x = myrandn()
    u1 = rand(1,1);
    u2 = rand(1,1);
    x = sqrt(-2*log(u1))*cos(2*pi*u2);
end