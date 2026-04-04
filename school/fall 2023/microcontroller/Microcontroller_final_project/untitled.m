clc
clear

clk = 16e6;
clkp = 1/clk;

note = 1567.98;
Prescaler = 1;

pwmf = note/2;

pwmp = 1/pwmf;

ARR =   (pwmp/clkp) - 1;

while any(ARR > 65535)
    Prescaler = Prescaler + 1;
    clk_t = clk/Prescaler;
    clkp_t = 1/clk_t;
    ARR = pwmp/clkp_t - 1;
end

if any(Prescaler > 65535)
    disp('invalid Prescaler')
end


CCR = .5 * (ARR +1);

ARR
CCR = ceil(CCR)
Prescaler