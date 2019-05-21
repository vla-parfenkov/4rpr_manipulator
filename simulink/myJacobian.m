function J = myJacobian(L, angle)
cf = cos(angle)
sf = sin(angle)
cs = zeros(4)
sn = zeros(4)
for i = 1:4
    cs(i) = cos(L(i + 4))
    sn(i) = sin(L(i + 4))
end
[ b, a] = setgeomentry()
A = [ 1 0 -b(1)*sf
        1 0 -b(2)*sf
        1 0 -b(3)*sf
        1 0 -b(4)*sf
        0 1 -b(1)*cf
        0 1 -b(2)*cf
        0 1 -b(3)*cf
        0 1 -b(4)*cf ]
B = [ L(1)*sn(1) 0 0 0 cs(1) 0 0 0
        0 L(2)*sn(2) 0 0 0 cs(2) 0 0
        0 0 L(3)*sn(3) 0 0 0 cs(3) 0
        0 0 0 L(4)*sn(4) 0 0 0 cs(4)
        L(1)*cs(1) 0 0 0 sn(1) 0 0 0
        0 L(2)*cs(2) 0 0 0 sn(2) 0 0
        0 0 L(3)*cs(3) 0 0 0 sn(3) 0
        0 0 0 L(4)*cs(4) 0 0 0 sn(4) ]
J = pinv(A)*B     
end