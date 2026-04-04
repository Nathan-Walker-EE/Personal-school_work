% Define time-related parameters
startTime = 0;
endTime = 5;
timeStep = 0.001;

% Generate time vector
timeVector = startTime:timeStep:endTime;

% Function definitions
h = @(t) -0.3339 * exp(2.6038 * -t) + 0.4202 * exp(-1.1981 * t) .* cos(2.0787 * t - deg2rad(-37.386));
f = @(t) sin(3 * pi * t);

% Evaluate functions at each time point
hValues = arrayfun(h, timeVector);
fValues = arrayfun(f, timeVector);

% Write results to text files with space as delimiter and newline character
dlmwrite('output.txt', hValues, 'precision', 16, 'delimiter', ' ', 'newline', 'pc');
dlmwrite('output2.txt', fValues, 'precision', 16, 'delimiter', ' ', 'newline', 'pc');


