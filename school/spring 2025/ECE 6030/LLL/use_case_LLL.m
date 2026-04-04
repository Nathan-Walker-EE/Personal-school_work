clear all
clc
close all

B = [12 13; 2 4]; % Example basis matrix
B_reduced = LLLReduce(B, 3/4);
disp('unreduced Basis');
disp(B);
disp('Reduced Basis:');
disp(B_reduced);
B = B_reduced;
N = 15;  % Number of lattice points in each direction

% Generate lattice points
vertices = [0 0; B(:,1)'; B(:,1)' + B(:,2)'; B(:,2)'];
[X, Y] = meshgrid(-N:N, -N:N);  % Generate integer coefficients
lattice_points = B * [X(:) Y(:)]';  % Compute lattice positions

% Plot lattice points
figure(1); hold on; axis equal;
plot(lattice_points(1, :), lattice_points(2, :), 'ko', 'MarkerFaceColor', 'k');

% Plot basis vectors
quiver(0, 0, B(1,1), B(2,1), 0, 'r', 'LineWidth', 2, 'MaxHeadSize', 0.5);
quiver(0, 0, B(1,2), B(2,2), 0, 'b', 'LineWidth', 2, 'MaxHeadSize', 0.5);
fill(vertices(:,1), vertices(:,2), 'cyan', 'FaceAlpha', 0.5, 'EdgeColor', 'k');

xlim([-20,20]);
ylim([-20 20]);
xlabel('x');
ylabel('y');
title('2D Lattice Plot');
grid on;
hold off;


B1 = [1;1;1];
B2 = [-1; 0; 2];
B3 = [3;5;6];
B = [B1, B2, B3];
B_reduced = LLLReduce(B, 3/4);
disp('unreduced Basis');
disp(B);
disp('Reduced Basis:');
disp(B_reduced);

B = eye(7);
B(7,1) = -366;
B(7,2) = -385;
B(7,3) = -392;
B(7,4) = -401;
B(7,5) = -422;
B(7,6) = -437;
B(7,7) = 1215;
B_reduced = LLLReduce(B, 3/4);
disp('unreduced Basis');
disp(B);
disp('Reduced Basis:');
disp(B_reduced);