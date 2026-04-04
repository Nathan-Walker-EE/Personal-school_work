% Define the numerator and denominator coefficients
clear all
clc
format compact

b = [1 -5 -6];       % Numerator coefficients (1 - 5z^(-1) - 6z^(-2))
a = [1 -1.5 0.56];   % Denominator coefficients (1 - 1.5z^(-1) + 0.56z^(-2))

% Initialize quotient and remainder
quotient = [];
remainder = b;
temp = 1;
b_ld = b;
sum = 0;
% Perform long division
for n = 1:50000
    for i = 1:3
        sum = sum + b_ld(i);
    end
    if(a(1) ~= b_ld(1))
        temp = b_ld(1);
    end
    temp_v = b_ld - a*temp;
    
    if(temp_v(1) == 0)
        b_ld = circshift(temp_v, -1);
    end
end
display(sum)

% To verify, use residuez to compute K automatically
[R, P, K_residuez] = residuez(b, a);
disp('Constant term K from residuez:');
disp(K_residuez);

% Define the numerator and denominator coefficients
b = [1 -5 -6];       % Numerator coefficients
a = [1 -1.5 0.56];   % Denominator coefficients

% Use deconv to perform polynomial long division
[quotient, remainder] = deconv(b, a);

% The constant term K is the first term of the quotient
K = quotient;

% Display results
disp('Quotient (Direct Terms):');
disp(quotient);
disp('Remainder:');
disp(remainder);
disp('Constant term K (from long division):');
disp(K);

% Verify using residuez
[R, P, K_residuez] = residuez(b, a);
disp('Constant term K (from residuez):');
disp(K_residuez);

