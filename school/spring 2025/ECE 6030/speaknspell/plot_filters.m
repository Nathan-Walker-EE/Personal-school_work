function a = plot_filters(a, vowel)
% Create a figure
figure;

% Plot pole-zero plot in the first subplot
subplot(2, 1, 1); % First subplot (1 row, 2 columns, first position)
zplane(1, a);     % Pole-zero plot of the IIR filter
title('Pole-Zero Plot of ' + vowel);

% Plot frequency response in the second subplot
subplot(2, 1, 2); % Second subplot (1 row, 2 columns, second position)
[H, w] = freqz(1, a, 1024, 'whole'); % Frequency response of the filter
w = w - pi; % Shift frequency axis to [-pi, pi]
plot(w, fftshift(abs(H))); % Plot magnitude response
xlabel('Frequency (-\pi to \pi)');
ylabel('|H(w)|');
title('Frequency Response of ' + vowel);
grid on;

saveas(gcf, "pngs/" + vowel + ".png")
end

