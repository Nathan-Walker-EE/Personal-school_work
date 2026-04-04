% Sample data (replace with your own values)
x = [2 2.5 3 5 9]; % X-values
y = [-4.2 -5 2 1 24.3]; % Y-values

% Construct matrix X and vector Y
X = [ones(length(x), 1), x(:)]; % Design matrix (first column = 1 for intercept)
Y = y(:); % Convert to column vector

weights = ones(length(x), 1);
weights(1) = 10; % Higher weight for first point
weights(end) = 10; % Higher weight for last point
W = diag(weights);

% Compute least-squares solution using normal equations
beta = (X' * X) \ (X' * Y); 
beta2 = (X' * W * X) \ (X' * W * Y); 

% Extract slope (m) and intercept (b)
b = beta(1); % Intercept
m = beta(2); % Slope
b2 = beta2(1); % Intercept
m2 = beta2(2); % Slope

% Display results
fprintf('Best-fit line: y = %.4f x + %.4f\n', m, b);

% Plot data and fitted line
figure;
scatter(x, y, 'bo', 'filled'); % Plot data points
hold on;
x_fit = linspace(min(x), max(x), 100);
y_fit = m * x_fit + b;
plot(x_fit, y_fit, 'r-', 'LineWidth', 2); % Plot fitted line
xlabel('x');
ylabel('y');
y_fit2 = m2* x_fit + b2;
plot(x_fit, y_fit2);
title('Least-Squares Line Fit');
grid on;
legend('Data points', 'Best-fit line', 'weighted Best-fit line', 'Location', 'best');
hold off;
