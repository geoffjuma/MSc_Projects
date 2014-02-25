function [F, J] = big_matr_jac(in_x)
out = getappdata(0, 'out');
x = getappdata(0, 'x');
jac=getappdata(0, 'jac');
F = [subs(out, x, in_x)];
J =[subs(jac,x,in_x)]; 
end