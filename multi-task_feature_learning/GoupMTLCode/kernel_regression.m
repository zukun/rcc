function [w] = kernel_regression(K,y,gamma,x)
n = size(K,1);
a = inv(K+gamma*eye(n)) * y;
w = x*a;