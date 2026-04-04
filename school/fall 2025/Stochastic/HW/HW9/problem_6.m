clear all; close all; clc; format compact;

n = 0:4;
c = 5*(0.3).^(n+1);
RXY = c.';
c = 5*(0.3).^(n);
c(1) = c(1) + 1;
RY = toeplitz(c);

h = RY\RXY

a = 0.3;

% Polynomial numerator and denominator of S_y(z)
% S_y(z) = (a z^2 + (4a^2 - 6) z + a) / (a z^2 - (a^2 + 1) z + a)
Num = [a, 4*a^2 - 6, a];        % a z^2 + (4a^2 - 6) z + a
Den = [a, -(a^2 + 1), a];       % a z^2 - (a^2 + 1) z + a

% Zeros and poles of S_y(z)
z_zeros = roots(Num);
z_poles = roots(Den);

% --- Build MINIMUM-PHASE spectral factor S_y^-(z) ---

% Keep zeros inside unit circle for the numerator
z_zeros_in = z_zeros(abs(z_zeros) < 1 - 1e-8);

% Keep poles inside unit circle for the denominator
z_poles_in = z_poles(abs(z_poles) < 1 - 1e-8);

% Minimum-phase numerator and denominator (up to gain)
Num_min = poly(z_zeros_in);   % product (z - z_i), |z_i|<1
Den_min = poly(z_poles_in);

% Now normalize the gain so that:
% S_y(z) ≈ S_y^-(z) * S_y^-(z^{-1}) on the unit circle.
% A simple way: match |S_y(e^{j0})| at z = 1
Sy_at_1      = polyval(Num, 1) / polyval(Den, 1);
Syminus_at_1 = polyval(Num_min, 1) / polyval(Den_min, 1);

gain = sqrt(Sy_at_1 / Syminus_at_1);

Num_minus = gain * Num_min;
Den_minus = Den_min;

% Now S_y^-(z) = Num_minus(z) / Den_minus(z)

