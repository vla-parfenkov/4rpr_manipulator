function L  = inverseKinematic(x)
[b, A] = setgeomentry();
cf = cos(x(3));
sf = sin(x(3));
L(1) = ((x(1) - A(1) + b(1)*cf).^2 + (x(2)-A(2) + b(1)*sf).^2).^(1/2);
L(2) = ((x(1) - A(3) + b(2)*cf).^2 + (x(2)-A(4) + b(2)*sf).^2).^(1/2); 
L(3) = ((x(1) - A(5) + b(3)*cf).^2 + (x(2)-A(6) + b(3)*sf).^2).^(1/2);
L(4) = ((x(1) - A(7) + b(4)*cf).^2 + (x(2)-A(8) + b(4)*sf).^2).^(1/2);
%L(5) = atan((x(2)-A(2) + b(1)*sf)/ (x(1) - A(1) + b(1)*cf))
%L(6) = atan((x(2)-A(4) + b(2)*sf)/ (x(1) - A(3) + b(2)*cf))
%L(7) = atan((x(2)-A(6) + b(3)*sf) /(x(1) - A(5) + b(3)*cf))
%L(8) = atan((x(2)-A(8) + b(4)*sf) / (x(1) - A(7) + b(4)*cf))
end