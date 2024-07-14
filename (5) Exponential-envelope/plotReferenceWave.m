function plotReferenceWave(~)
    % Load parameters and time vectors
    parameter = load("parameters.mat");
    vector = load("timeVectors.mat");
    
    % Extract loaded variables
    amp = parameter.amp;
    per = parameter.per;
    freq = parameter.freq;
    timeVectors = vector.timeVectors;
    periodNumber = size(timeVectors, 1); % Determine period number from time vectors

    % Call wave function with loaded parameters
    wave(timeVectors, periodNumber, amp, per, freq);
end