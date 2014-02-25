function [i,j] = diagonal(n)
i=0; j =0;
for i = 1:n
    for j = 1:n
        i=i+1;
        j=j+1;
    end
end