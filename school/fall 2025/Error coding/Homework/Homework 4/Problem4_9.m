clear all;clc; close all;format compact

n = 15; k = 11;
check_poly = zeros(1,16);
check_poly(1) = 1; check_poly(16) = 1;

gx = [1 0 0 1 1];
[hx, r] = deconv(check_poly, gx);
hx = mod(hx,2)
r = mod(r,2);

G = zeros(k,n);
for i = 0:k-1
    row = [zeros(1,i) gx zeros(1,n-length(gx)-i)];
    G(i+1,:) = row;
end
G = mod(G,2)

H = zeros(n-k,n);
for i = 0:(n-k-1)
    row = [zeros(1,i) hx zeros(1,n-length(hx)-i)];
    H(i+1,:) = row;
end
H = mod(H,2)

Gsys = mod(rref(G), 2)
P = Gsys(:,12:15);

H = [eye(n-k), P']

mx1 = [1 1 1 0];
mx1(end+1:k) = 0;
cx1 = mod(conv(gx,mx1),2)

xnk = zeros(1,n-k+1);
xnk(1) = 1;
mxnk = mod(conv(xnk, mx1),2);
[q, r] = deconv(mxnk,gx);
r = mod(r,2);
r(end+1:n-k+1) = 0;
cx2 = r + mxnk

cxfg = [0 1 0 1 1 1 0 0 0 1 1 1 0 1 1];
mxfg = deconv(cxfg,gx);
mxf = mod(mxfg,2)
mxg = cxfg(1:11)

rx = [1 0 0 0 1 0 0 0 0 1 0 1 0 0 0];
synh = mod(conv(rx, hx), 2);
synh = deconv(synh, check_poly);
synh = mod(synh,2)
