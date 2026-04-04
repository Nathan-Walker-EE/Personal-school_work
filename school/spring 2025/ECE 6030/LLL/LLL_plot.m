clear all
clc

% Define basis vectors as columns of a matrix
b1 = [10, 2].';
b2 = [12, 5].';
B = [b1, b2];  % Example basis vectors

% Define the grid size (how many points in each direction)
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

xlim([0,40]);
ylim([0 20]);
xlabel('x');
ylabel('y');
title('2D Lattice Plot');
grid on;
hold off;
