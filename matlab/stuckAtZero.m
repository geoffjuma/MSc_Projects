function [d,e,f,g,h,i] = stuckAtZero()
% This is a script that tests the stuc-at-zero error in integrated digital
% circuits. We generate random bits for the inpuom vectors [x,y,z]. Then we
% manipulate the bits according to the combinational circuit

x = rand (1,1)>0.3;
y = rand (1,1)>0.3;
z = rand (1,1)>0.3;

%assign the random input bit vectors

a=x;
b=y;
c=z;

%components within the circuitry with correct results
d = ~b;
e = (a|d);
f = e;
g = f;
h = ~(g|c);
i = (f|h);
%ouput the results
fprintf('input bit vector %d %d %d\n',x,y,z);

fprintf('ouput bit vector %d %d %d %d %d %d\n',d,e,f,g,h,i);
end
