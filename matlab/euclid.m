function k = euclid(a,b)
% script to find the gcd of a and b by the Euclidean algorithm, and to
% report how many steps were required
% This implementation requires a>0, b>0, so replace, if necessary,
a=abs(a);
b=abs(b);
if(a<b)  % switch a and b, if necessary, so that b<a
    c=a; % hang onto the value of a
    a=b; % even while replacing a with b
    b=c; % now replace b with a
end
count=0; % initialize counter
while(b>0)
    q=floor(a/b);
    r=a-q*b;
    a=b;
    b=r;
    count=count+1;
end
a
count