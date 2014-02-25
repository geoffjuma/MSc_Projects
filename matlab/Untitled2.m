% Laboratory 3

format shortE;
 
A = [1 1.2 2 0.5; 1.2 1 0.4 1.2; 2 0.4 2 1.5; 0.5 1.2 1.5 1];

B = randi([-1 1], 10,10);
C = randi([-10 10], 100,100);
D = randi([-100 100], 1000,1000);

disp(['Matrix A: ' mat2str(A)]);
disp(['Matrix B: ' mat2str(B)]);
% disp(['Matrix C: ' mat2str(C)]);
% disp(['Matrix D: ' mat2str(D)]);

B(1,:) = B(1,:)*10^-14;
B(end,:) = B(end,:)*10^-14;
B(:,2) = B(:,2)*10^-14;
B(:, end-1) = B(:,end-1)*10^-14;

disp(['multiply Matrix B: ' mat2str(B)]);

disp(['Rank of Matrix A: ' num2str(rank(A))]);
disp(['Rank of Matrix B: ' num2str(rank(B))]);
disp(['Rank of Matrix C: ' num2str(rank(C))]);
disp(['Rank of Matrix D: ' num2str(rank(D))]);

[U1,S1,V1] = svd(A);
[U2,S2,V2] = svd(B);
[U3,S3,V3] = svd(C);
[U4,S4,V4] = svd(D);

disp(['Singular Value of Matrix A: ' mat2str(S1)]);
disp(['Singular Value of Matrix B: ' mat2str(S2)]);
% disp(['Singular Value of Matrix C: ' mat2str(S3)]);
% disp(['Singular Value of Matrix D: ' mat2str(S4)]);

[R_A,D_A] = eig(A);
[R_B,D_B] = eig(B);
[R_C,D_C] = eig(C);
[R_D,D_D] = eig(D);

disp(['Singular Right Vector of Matrix A: ' mat2str(R_A)]);
disp(['Singular Right Vector of Matrix B: ' mat2str(R_B)]);
% disp(['Singular Right Vector of Matrix C: ' mat2str(R_C)]);
% disp(['Singular Right Vector of Matrix D: ' mat2str(R_D)]);

disp(['Polynomial coefficients of Matrix A: ' mat2str(poly(A))]);
disp(['Polynomial coefficients of Matrix B: ' mat2str(poly(B))]);
% disp(['Polynomial coefficients of Matrix C: ' mat2str(poly(C))]);
% disp(['Polynomial coefficients of Matrix D: ' mat2str(poly(D))]);

disp(['Conditionality value of Matrix A: ' num2str(cond(A))]);
disp(['Conditionality value of Matrix B: ' num2str(cond(B))]);
disp(['Conditionality value of Matrix C: ' num2str(cond(C))]);
disp(['Conditionality value of Matrix D: ' num2str(cond(D))]);

disp(['Determinant of Matrix A: ' num2str(det(A))]);
disp(['Determinant of Matrix B: ' num2str(det(B))]);
disp(['Determinant of Matrix C: ' num2str(det(C))]);
disp(['Determinant of Matrix D: ' num2str(det(D))]);

disp(['Conditionality measure of Matrix A: ' num2str(rcond(A))]);
disp(['Conditionality measure of Matrix B: ' num2str(rcond(B))]);
disp(['Conditionality measure of Matrix C: ' num2str(rcond(C))]);
disp(['Conditionality measure of Matrix D: ' num2str(rcond(D))]);