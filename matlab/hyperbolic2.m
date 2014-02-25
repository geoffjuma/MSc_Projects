function hyperbolic2

% Klein-Gordon Eqn (dispersive hyperbolic PDE)
% u_{tt} = c^2*u_{xx} - d*u  u(0,t)=0 u(1,t)=0 u(x,0)=3-2x u_t(x,0)=500*exp(-100*(x-.7)^2)

c = 30;
d = 10000;
h = .01;
r = .5;
k = h*r/c;
x = 0:h:1;
n = length(x);

figure(2)

u1 = exp(-1000*(x-.7).^2);
plot(x,u1),axis([0 1 -1 1]),title('2nd order wave equation with true dispersion: u_{tt} = c^2 *u_{xx} - d*u')
pause
u2 = u1;
plot(x,u2),axis([0 1 -1 1]),title('2nd order wave equation with true dispersion: u_{tt} = c^2 *u_{xx} - d*u')
pause(.05)

for j=2:600
   uj(1) = 0;
   uj(n) = 0;
   for i=2:n-1
      uj(i) = (2-2*r^2-d*k^2)*u2(i) + r^2*(u2(i+1)+u2(i-1)) - u1(i);
   end
   plot(x,uj),axis([0 1 -1 1]),title('2nd order wave equation with true dispersion: u_{tt} = c^2 *u_{xx} - d*u')
   pause(.05)
   u1 = u2;
   u2 = uj;
end

plot(x,uj),axis([0 1 -1 1])