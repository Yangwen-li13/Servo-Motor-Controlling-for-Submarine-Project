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
    fileName = 'referenceWave_' + 'amp_' + string(amp) + '_f_' + string(freq) + '_per_' + string(per) + '_a_' + string(a) +  '_' + string(date.date) + '.mat';
 
    save(fileName, 'amp', 'per', 'freq', 'a', 'periodNumber', 'timeVectors');
end
