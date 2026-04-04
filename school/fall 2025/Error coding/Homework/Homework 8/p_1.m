clear all; close all; clc;

R = .5;

dfree = 6;
bdfree = 4;
k = 1;

SNRvals = 6:13;
punc = [];
pub = [];
plb = [];
pap = [];
pawgnub = [];
pawgnlb = [];


for snr = SNRvals
    Eb_N0 = 10^(snr/10);
    Ec_N0 = Eb_N0*R;
    punc_t = qfunc(sqrt(2*Eb_N0));
    punc = [punc, punc_t];
    pc = qfunc(sqrt(2*Ec_N0));

    pdfree = nchoosek(dfree,dfree/2)*(1-pc)*(dfree/2);
    for i = ((dfree/2)+1):dfree
        pdfree = pdfree + (nchoosek(dfree,i)*(pc^i)*((1-pc)^(dfree-1)));
    end

    D = sqrt(4*pc*(1-pc));
    pub_t = (D^6*(4-2*D^2+2*D^6-2*D^4))/((1-5*D^2+2*D^6)^2)/k;
    plb_t = bdfree*pdfree/k;
    pap_t = bdfree*D^dfree/k;

    pub = [pub, pub_t];
    plb = [plb, plb_t];
    pap = [pap, pap_t];

    pl_t = qfunc(sqrt(2*dfree*Ec_N0))*exp(dfree*Ec_N0)/k;
    D = exp(-Ec_N0);
    pawgnub_t = pl_t*(D^6*(4-2*D^2 + 2*D^6 - 3*D^4))/((1 - 5*D^2 + 2*D^6)^2);
    pawgnlb_t = bdfree*qfunc(sqrt(2*dfree*Ec_N0));

    pawgnub = [pawgnub, pawgnub_t];
    pawgnlb = [pawgnlb, pawgnlb_t];

end

clf;
semilogy(SNRvals, punc);
hold on;
semilogy(SNRvals, pub);
semilogy(SNRvals, plb);
semilogy(SNRvals, pap);

semilogy(SNRvals, pawgnub);
semilogy(SNRvals, pawgnlb);

legend('uncoded', 'BSC UB', 'BSC LB', 'BSC app', 'AWGN ub', 'AWGN lb');
grid on;