close all
clear all
x= [0,1,2,3,4,5,6,7,8,9];   % input x in the form [1,2,3,4,5]
h= [9,8,7,6,5,4,3,2,1,0];
m=length(x);
n=length(h);
X=[x,zeros(1,n)];           % padding of n zeros
H=[h,zeros(1,m)];           % padding of m zeros
for i=1:n+m-1
    Y(i)=0;
    for j=1:i
        Y(i)=Y(i)+X(j)*H(i-j+1);
    end
end
stem(Y);
ylabel('Y[n]');
xlabel('------------>n');
title('Convolution of two signals');