d = 3;
epsilon = 1e-3;
Ainv = eye(d) / epsilon;  % Ainv_0
B = zeros(d, d);          % B_0
T = zeros(d, d);          % T_0

N = 10000;
errors = zeros(1, N);
S1 = zeros(d, N);
S2 = zeros(d, N);

for i = 1:N
    s1 = randn(d, 1);
    s2 = 2 * s1 + 0.1 * randn(d, 1); 
    S1(:, i) = s1;
    S2(:, i) = s2;
    [T, Ainv, B] = update_transformation(Ainv, B, s1, s2);
    diff = T * S1(:, 1:i) - S2(:, 1:i);
    errors(i) = mean(sum(diff.^2, 1)); 
end
figure;
plot(1:N, errors, 'LineWidth', 2);
xlabel('Iteration');
ylabel('Mean Squared Error');
title('Error between T * s1 and s2');
grid on;
