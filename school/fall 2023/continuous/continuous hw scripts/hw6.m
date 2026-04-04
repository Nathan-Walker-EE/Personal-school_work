%problem 3.13
s = -3;

a = (s^2 + 4*s);
b = (9*s^2 + 62 * s + 131);
c = 3*s^3 + 36*s^2 + 131*s + 144;
d = 2*s + 4;

e = ((a * b)-(c * d))/ a^2;

display(e);

%problem 3.14 a
s = 0;
num = s^2 + 17*s + 20;
den = (s+3)*(s+2);
a = num/den;
display(a);
s = -3;
num = s^2 + 17*s + 20;
den = s*(s+2);
b = num/den;
display(b);
s = -2;
num = s^2 + 17*s + 20;
den = s*(s+3);
c = num/den;
display(c);
