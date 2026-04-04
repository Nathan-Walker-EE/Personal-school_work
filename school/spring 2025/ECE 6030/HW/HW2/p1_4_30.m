close all
format compact
y = [0.32, 0.25, 0.1, -0.0222, 0.0006, -0.0012, 0.0005, -0.0001];
A = zeros(5,3);
b = zeros(5,1);
for i = 1:5
    A(i,:) = fliplr(y(i:i+2));
    b(i) = y(i+3);
end
a = -A\b;
a = [1 a.'];
g = roots(a);
Ac = [1, 1, 1;
      g(1), g(2), g(3);
      g(1)^2, g(2)^2, g(3)^2
      ];
bc = [y(1); y(2); y(3)];
c = Ac\bc;
y = [0.32, 0.25, 0.1, -0.0222, 0.0006, -0.0012, 0.0005, -0.0001];
noise_var = 0.01;
noisy_y = zeros(10, length(y));
for i = 1:10
    noise = sqrt(noise_var) * randn(size(y));
    noisy_y = y + noise;
    An = zeros(5,3);
    bn = zeros(5,1);
    for j = 1:5
        An(j,:) = noisy_y(j:2+j);
        bn(j) = noisy_y(j+3);
    end
    an = -An\bn;
    g = roots([1 an.']).'
end
