function F = big_matr(in_x)
out = getappdata(0, 'out');
x = getappdata(0, 'x');
F = [subs(out, x, in_x)];
end