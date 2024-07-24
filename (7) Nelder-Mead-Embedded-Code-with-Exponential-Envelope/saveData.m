function saveData(~)
    % Load parameters and time vectors
    parameter = load("parameters.mat");
    vector = load("timeVectors.mat");
    date = load("date.mat");
    
    
    % Extract loaded variables
    amp = parameter.amp;
    per = parameter.per;
    freq = parameter.freq;
    a = parameter.a;
    timeVectors = vector.timeVectors;
    periodNumber = size(timeVectors, 1); % Determine period number from time vectors
    % timeData = reshape(timeVectors', 1, []); % Reshape timeVectors to a row vector
    fileName = string(date.date) + '.mat';
 
    save(fileName, 'amp', 'per', 'freq', 'a', 'periodNumber', 'timeVectors');
end
