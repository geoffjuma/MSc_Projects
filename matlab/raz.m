function [y] = raz(p)
p = 10^(-p);
a = [ -0.79  0.12 -0.34 -0.16;
       0.34 -1.08  0.17 -0.18;
       0.16  0.34 -0.85 -0.31;
      -0.79-0.79*p  0.12+0.12*p -0.34-0.34*p -0.16-0.16*p];
b = [0.64; -1.42; 0.42; 0.64+0.64*p];

% determ = det(a);
% 
% for i = 1 : 4
%     for j = 1 : 4
%         c = a;
%         c(i,:)=[];
%         c(:,j)=[];
%         det_i(i) = det(c);
%     end
% end
% 
% y = zeros(1,4);
% y1 = det_i / determ;


det(a)

y = a\b;

for i = 1 : 4
    c(i,1) = a(i,:) * y;
end

err = c - b;

disp(max(err));

end