function [sum] = PLj(n, j, L, p)
sum = 0;
prod = 0;
for r = 0:L
    prod = nchoosek(j,L-r);
    if r ~= 0
        prod = prod * nchoosek(n-j,r);
    end
    prod = prod * p^(j-L+2*r)*(1-p)^(n+L-j-2*r);
    sum = sum + prod;
end
