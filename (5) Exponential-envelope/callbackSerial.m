function callbackSerial(src, ~)
    persistent timeVectors periodNumber timeVectorIndex parametersVectors;
    
    if isempty(timeVectors)
        timeVectors = []; % Initialize if empty
    end
    if isempty(parametersVectors)
        parametersVectors = [];
    end

    
    data = readline(src);
    if isempty(data)
        return;
    end
    disp(data);
    

    if startsWith(data, "Amplitude:")
        values = sscanf(data, 'Amplitude: %d, Period: %d, Frequency: %f');
        amp = values(1);
        per = values(2);
        freq = values(3);
        save('parameters.mat', 'amp', 'per', 'freq');
        disp('Parameters saved to parameters.mat');

    end

    % Check for test period message
    if startsWith(data, "Test Period is:")
        periodNumber = str2double(readline(src));
        disp(['Period Number: ', num2str(periodNumber)]);
        timeVectorIndex = 1; % Reset time vector index for the new test period
    elseif contains(data, "timeVector[")
        % Extract the time vector value
        value = str2double(readline(src));
        disp(['Time Vector Value: ', num2str(value)]);
        
        % Ensure the timeVectors matrix can accommodate the new period and index

            timeVectors(periodNumber, timeVectorIndex) = value;

        timeVectorIndex = timeVectorIndex + 1;
    elseif strcmp(strtrim(data), "Test has been done! Write new command")
        save('timeVectors.mat', 'timeVectors');
        disp('Time vectors saved to timeVectors.mat');
        plotReferenceWave();
    end
end
