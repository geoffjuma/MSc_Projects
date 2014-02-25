function A = jacobbi(N)
x =randn()
A = randi(N,N);
for row = 1:N
    for col=1:N
        if row == col
            A(row,col) =randn()*col^3;
        elseif col == N
            A(row,col) = randn()*sin(x);
        elseif col == (row-1)
            A(row,col) = randn()*col^2;
        else
            %A(row,col) = randn()*col;
        end
    end
end
