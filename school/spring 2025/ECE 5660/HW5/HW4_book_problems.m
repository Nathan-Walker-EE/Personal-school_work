
% Define signal constellation points as a column vector (complex numbers)
constellation = [-3 + 3j; -1 + 3j; 1 + 3j; 3 + 3j; -3 + j; -1 + j; 1 + j; 3 + j; -3 - j; -1 - j; 1 - j; 3 - j; -3 - 3j; -1 - 3j; 1 - 3j; 3 - 3j];

% Convert complex numbers to Cartesian coordinates
X = [real(constellation), imag(constellation)];

% Compute the pairwise Euclidean distances
D = pdist2(X, X);

% Display the distance matrix
disp(D);
