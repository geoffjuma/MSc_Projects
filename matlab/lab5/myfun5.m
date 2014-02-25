% (5)   sin(x)+0.5-y=1;cos(y-2)+x=0
%  (6)  cos(x+0.5)+y=0.8; sin(y)-2x=1.6
function F5 = myfun5(x)
F5 = [sin(x(1))+0.5 - x(2) - 1; cos(x(2)-2)+x(1)
    ];
