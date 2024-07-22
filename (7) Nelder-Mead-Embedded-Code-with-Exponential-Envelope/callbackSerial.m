function callbackSerial(src, ~)
    persistent timeVectors periodNumber timeVectorIndex parametersVectors;
    
    % Initialize variables if they are empty
    
    if isempty(timeVectors)
        timeVectors = []; 
    end
    if isempty(parametersVectors)
        parametersVectors = [];
    end

    % Read data if it is not empty
    data = readline(src);
    if isempty(data)
        return;
    end
    disp(data);
    
    % When test begin, Create a text file
    if strcmp(strtrim(data), "Test began")
    text = fopen('command.txt','w');
    fprintf(text, '%c', 'Start');
    fclose(text);
    end

    % Save parameters
    if startsWith(data, "Amplitude:")
        values = sscanf(data, 'Amplitude: %d, Period: %d, Frequency: %f, Envelope constant: %f');
        amp = values(1);
        per = values(2);
        freq = values(3);
        a = values(4);
        save('parameters.mat', 'amp', 'per', 'freq', 'a');
        disp('Parameters saved to parameters.mat');
    end

    % Check for test period message, save vectors and change text file
    if startsWith(data, "Test Period is:")
        periodNumber = str2double(readline(src));
        disp(['Period Number: ', num2str(periodNumber)]);
        timeVectorIndex = 1; % Reset time vector index for the new test period
    elseif contains(data, "timeVector[")
        % Extract the time vector value
        value = str2double(readline(src));
        disp(['Time Vector Value: ', num2str(value)]);
        timeVectors(periodNumber, timeVectorIndex) = value;
        timeVectorIndex = timeVectorIndex + 1;
    elseif strcmp(strtrim(data), "Test has been done! Write new command")
        text = fopen('command.txt','w');
        fprintf(text, '%c', 'Stop');
        fclose(text);
        save('timeVectors.mat', 'timeVectors');
        disp('Time vectors saved to timeVectors.mat');
        plotReferenceWave();
    end
end
