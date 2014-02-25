function [y,n] = sigMult(x1,n1,x2,n2)
% implements y(n) = xl(n)+x2(n)
%-----------------------------
% [y,n] = sigadd(xl,nl,x2,n2)
% = sum sequence over n, vhich includes nl and n2
% xl = first sequence over nl
% x2 = second sequence over n2 (n2 can be different from nl)
%
n = min(min(n1) ,min(n2)) :max(max(n1) ,max(n2)) ; % duration of y(n)
y1 = zeros(1,length(n)); y2 = y1; % initialization
y1(find((n>=min(n1))&(n<=max(n1))==1))=x1; % x1 vith duration of y
y2(find((n>=min(n2))&(n<=max(n2))==1))=x2; % x2 vith duration of y
y = y1+y2; %siganl addition
end
