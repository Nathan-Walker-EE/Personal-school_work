function[LLLReducedBasis] =  LLLReduce(B, delta)
    [Q, R] = GramSchmidt(B);

    n = length(B(1,:));
    k = 2;

    while k <= n
        for j = k-1:-1:1
            if (abs(R(k,j)) > 0.5)
                B(:,k) = B(:,k) - round(R(k,j))*B(:,j);
                for t = 1:k-1
                    R(k,t) = CalcMu(k, t, B, Q);
                end
            end
        end

        if Q(:,k)'*Q(:,k) >= (delta - R(k,k-1)^2)*Q(:,k-1)'*Q(:,k-1)
            k =  k+1;
        else
            temp = B(:, k);
            B(:, k) = B(:, k-1);
            B(:, k-1) = temp;
            %[B(:, k ), B(:,k -1)] = swap(B(:, k), B(:,k -1));
            [Q,R] = GramSchmidt(B);
            k = max(2,k-1);
        end
    end
    LLLReducedBasis = B;
end