S1 = 20; % ���������� �������� �� ������� ����
S2 = 15;
Q = 3;% ���������� �������� �� ������ ���� (���������� ������� ����)
P = zeros(20); % ������� �������, ���������� ���������� � ������
% ������� ����� ���� � �������������� ����������� ����
net=newff(minmax(P),[20,15,3],{'purelin' 'purelin' 'tansig'},'traingdx');