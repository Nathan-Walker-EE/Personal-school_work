function [T, Ainv, B] = update_transformation(Ainv_prev, B_prev, s1, s2)
    v = s1;
    z = Ainv_prev * v;
    denom = 1 + (v' * z);
    Ainv = Ainv_prev - (z * z') / denom;

    B = B_prev + s2 * s1';

    T = B * Ainv;
end
