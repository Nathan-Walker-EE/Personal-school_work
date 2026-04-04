function [ Mu ] = CalcMu (i , j , B , Q)
    v = B(: , i );
    u = Q(: , j );
    Mu = v' * u /(u'* u );
end
