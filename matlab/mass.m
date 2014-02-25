function dvdt= mass (t,y)
m = 80;
g = 9.81;
dvdt = -g+4/15*y^2/m;
end
