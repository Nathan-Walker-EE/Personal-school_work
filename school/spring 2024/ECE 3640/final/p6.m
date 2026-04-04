clear 
clc
close all

Fs = 300;

f = 1000/Fs;

while(f > .5)
    f = f - 1;
end

f