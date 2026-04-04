syms i1 i2 i3 s

eqns = [6*i1 - 4*i2 - 2*i3 == (44/s),-4*i1 + ((10/s)+10)*i2 - 6*i3 == 0, -2*i1 - 6*i2 + i3*4*s + 8*i3 == 0];
S = solve(eqns,[i1,i2,i3]);
sol = [S.i3];
display(sol);