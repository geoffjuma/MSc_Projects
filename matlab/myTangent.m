function F = myTangent(x)
F = [tan(x(1)*x(2)+0.1)-x(1)^2;
    x(1)^2+2*x(2)^2-1];
end
