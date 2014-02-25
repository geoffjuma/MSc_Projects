function [A out] = generate_data(N)
x = sym(zeros(1, N));
for i=1:N
    x(i) = sym(sprintf('x%d', i));  
end

A = sym(zeros(N, N + 1));
for row=1:N
    for col=1:N
        if row == col
            A(row, col) = randn()* sym(x(col)^3);
        elseif col == N
            A(row, col) = randn() * sym(sprintf('cos(x%d)', N));
        elseif col == row - 1
            A(row, col) = randn() * sym(x(col)^2);
        else
            A(row, col) = randn() * sym(x(col));
        end
    end
end
for row=1:N
    A(row, N + 1) = rand();
end
A(1, 2) = rand() * x(2)^2;
out = sym(zeros(N, 1));
jac = sym(zeros(N, N));
for row=1:N
    for col=1:N+1
        out(row) = out(row) + A(row, col);
    end
end
for row=1:N
    for col=1:N
        jac(row,col)=diff(out(row),x(col));
    end
end
%df1/x1 df1/x2
setappdata(0, 'x', x);
setappdata(0, 'out', out);
setappdata(0,'jac',jac);
end