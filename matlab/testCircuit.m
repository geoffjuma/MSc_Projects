function [d,e,f,g,h,i] = testCircuit(a,b,c)
d = ~(b);
e = (a|d);
g = e;
h = ~(c | g);
f = e;
i = (f|h);
end

