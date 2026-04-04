clear
clc

  w= pi/3;
  t = -10:0.1:10;
  n = 10000;
  suma=0;
  nvec = [-n:-1:-1,1:n];
for k = -n:n;
    if k ~= 0
        x = -j * w * k;
        cn = ;
        suma = suma + (cn.*exp(-x * t));
    end
end
plot(t,suma)
grid