function [xhat, P] = kalman(y,xtt,Ptt, A,C,Q,R)
Kt = Ptt*C.'/(C * Ptt * C.' + R);
xhatt = xtt + Kt*(y-C*xtt);
Pt = (eye(size(Kt*C)) - Kt*C)*Ptt;
xhat = A*xhatt;
P = A*Pt*A.' + Q;
end