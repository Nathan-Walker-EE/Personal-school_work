function[Q,R] = GramSchmidt(B)
    [m,n] = size(B);
    Q = zeros(m,n);
    R = zeros(n,n);

    for i = 1:n
        v = B(:,i);
        
        for j = 1:i-1
            R(j,i) = Q(:,j)'*v/(Q(:,j)'*Q(:,j));
            v = v - R(j,i)*Q(:,j);
        end

        Q(:,i) = v;
        R(i,i) = Q(:,i)'*v/(Q(:,i)'*Q(:,i));
    end 
    R = R';

end