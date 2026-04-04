clear all
clc

pertubations = [];
counter = 0;
trial_counter = 0;
t = 0;

while counter < 120
    trial_counter = trial_counter + 1;
    x = randperm(5)';
    sz = size(pertubations);
    for i = 1:sz(2)
        if x == pertubations(:,i); t = 1;end
    end
    if t == 0; pertubations = [pertubations, x]; counter = counter + 1; end
    t = 0;
end
disp(['Number of trials = ', num2str(trial_counter)]);