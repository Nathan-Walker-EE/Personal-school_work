function f = fact(n)

if n==1
    f = 1;
else
    f = n*fact(n-1);
end