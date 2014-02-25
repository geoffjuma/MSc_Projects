function dy = mySolver(x,y)
dy = [0,1];
dy = ((cos(x)/(x+1))-0.5*y.^2);
end