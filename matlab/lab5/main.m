% ��������� x^2
% ��������� x^3 (�������� �� x^2)

x0 = [0;0];           % Make a starting guess at the solution
options=optimset('Display','iter');   % Option to display output
[x,fval,exitflag] = fsolve(@myfun5,x0,options)  % Call solver


x0 = [0;0];           % Make a starting guess at the solution
options=optimset('Display','iter');   % Option to display output
[x,fval,exitflag] = fsolve(@big_matr,x0,options)  % Call solver