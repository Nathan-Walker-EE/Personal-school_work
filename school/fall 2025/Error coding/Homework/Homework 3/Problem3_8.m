clc; close all; clear all; format compact

G = [1, 0, 1, 0, 1, 1;
     0, 1, 1, 1, 0, 1;
     0, 1, 1, 0, 1, 0
     ];
H = [1, 1, 0, 1, 1, 0;
     1, 0, 1, 0, 1, 1;
     0, 1, 0, 0, 1, 1
     ];

T = mod(G*H', 2);
disp(['result of G*HT = ', mat2str(T)]);
c = zeros(8,6);

for i = 0:7
    binary = dec2bin(i,3);
    x = [str2num(binary(3)), str2num(binary(2)), ...
        str2num(binary(1))];
    c(i+1,:) = mod(x*H,2);
    disp(['i = ', num2str(i), ' dual code word is = ', mat2str(c(i+1,:))]);
end

for i = 0:7
    binary = dec2bin(i,3);
    x = [str2num(binary(3)), str2num(binary(2)), ...
        str2num(binary(1))];
    c(i+1,:) = mod(x*G,2);
    disp(['i = ', num2str(i), ' code word is = ', mat2str(c(i+1,:))]);
end

codewords = string(c) + "";  % convert numeric to string
codewords = join(codewords, "", 2)'; % join row-wise
for i = 0:5
    error = dec2bin(bitshift(1,5-i),6) - '0';
    cosets = [];
    for j = 2:8
        sum = error + c(j,:);
        sum = mod(sum,2);
        sum = join(string(sum),"");
        cosets = [cosets, sum];
    end
    cosets = [join(string(error),""), cosets];
    codewords = [codewords;cosets];
end

for i = 0:(2^6-1)
    if ismember(i,bin2dec(codewords)) == 0
        error = dec2bin(i,6) - '0';
        cosets = [];
        for j = 2:8
            sum = error + c(j,:);
            sum = mod(sum,2);
            sum = join(string(sum),"");
            cosets = [cosets,sum];
        end
        cosets = [join(string(error),""), cosets];
        codewords = [codewords;cosets];
    end
end
disp('standard array');
disp(codewords);

errors = codewords(:,1);
syndromes = [errors];
temp_s = [];
for i = 1:8
    error = bin2dec(errors(i));
    error = dec2bin(error,6) - '0';
    syn = mod(error * H',2);
    temp_s = [temp_s; join(string(syn),"")];
end
syndromes = [syndromes, temp_s];
disp('Syndrome Table');
disp(syndromes)

%probabilities of errors
A0 = 1;
A3 = 4;
A4 = 3;

f = @(n, j, p) p^j * (1-p)^(n-j);
Pu = A3*f(6,3,0.01) + A4 * f(6,4,0.01);
Pd = 1 - (1-(0.01))^6 - Pu;
PE = 0;
coset_wt = [1 6 1];
for i = 0:2
    PE = PE + coset_wt(i+1)*f(6, i, 0.01);
end
PE = 1 - PE;
PEb = 0;

Aweights = [0 0 4 3 0 0];
for j = 3:4
    PL_sum = 0;
    for L = 0:1
        sum = PLj(6, j, L, 0.01);
        PL_sum = PL_sum + sum;
    end
    PEb = PEb + Aweights(j) * PL_sum;
end

sum = 0;
for j = 0:1
    prod = nchoosek(6,j) * 0.01^j * (1-0.01)^(6-j);
    sum = sum + prod;
end
PF = 1 - sum - PE;
disp(['Pu = ', num2str(Pu), ' Pd = ', num2str(Pd), ' PE = ', num2str(PE) ...
    , ' bounded PE = ', num2str(PEb), ' PF = ', num2str(PF)]);
