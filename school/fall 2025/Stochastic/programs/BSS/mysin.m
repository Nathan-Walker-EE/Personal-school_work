function Y = mysin()
    b = pi;
    a = -pi;
    X = rand(1,1);
    X =  X*(b-a) + a;
    Y = sin(X);
end