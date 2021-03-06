function [ u ] = method_liebmann( u,h,g,u_initial )

u(1,1)=u_initial(1)+g(1)*((u(2,1)-u_initial(1))/(h+g(1))); 
u(1,2)=u_initial(2)+g(2)*((u(2,2)-u_initial(2))/(h+g(2))); 
u(2,3)=u_initial(3)+g(3)*((u(3,3)-u_initial(3))/(h+g(3))); 
u(3,4)=u_initial(4)+(2.33-3)*((u(3,3)-u_initial(4))/(h+(2.33-3))); 
u(4,4)=u_initial(5)+(2.66-3)*((u(4,3)-u_initial(5))/(h+(2.66-3))); 
u(5,4)=u_initial(6)+g(4)*((u(5,3)-u_initial(6))/(h+g(4))); 
 
u(2,1)=1/4*(u(1,1)+2*u(2,2)+u(3,1));
u(2,2)=1/4*(u(1,2)+u(2,1)+u(2,3)+u(3,2));
 
u(3,1)=1/4*(u(2,1)+2*u(3,2)+u(4,1));
u(3,2)=1/4*(u(2,2)+u(3,1)+u(3,3)+u(4,2));
u(3,3)=1/4*(u(2,3)+u(3,2)+u(3,4)+u(4,3));
 
u(4,1)=1/4*(u(3,1)+2*u(4,2)+u(5,1));
u(4,2)=1/4*(u(3,2)+u(4,1)+u(4,3)+u(5,2));
u(4,3)=1/4*(u(3,3)+u(4,2)+u(4,4)+u(5,3));
 
u(5,1)=1/4*(4*u(4,2));
u(5,2)=1/4*(u(5,1)+2*u(4,2)+u(5,3));
u(5,3)=1/4*(u(5,2)+2*u(4,3)+u(5,4));
end
