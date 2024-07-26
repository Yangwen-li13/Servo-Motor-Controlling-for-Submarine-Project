function callbackSerial(src, ~)
    persistent timeVectors periodNumber timeVectorIndex parametersVectors data;
    global g_message perX;
    % Initialize variables if they are empty
    if isempty(timeVectors)
        timeVectors = [];
    end
    if isempty(parametersVectors)
        parametersVectors = [];
    end

    % Read data from the serial port
    data = readline(src);
    if isempty(data)
        return;
    end
    disp(data);


    % Handle message from user
    if strcmp(strtrim(data), "For example: 'amp_400_per_25_freq_0.4_a_0'")
        pause(1);
        writeline(src, g_message); % Send the user-defined message
    end
    % Handle test start
    if strcmp(strtrim(data), "Test began")
        text = fopen('command.txt', 'w');
        fprintf(text, 'Start'); % Write 'Start' to the text file
        fclose(text);
    end

    % Handle saving parameters
    if startsWith(data, "Amplitude:")
        values = sscanf(data, 'Amplitude: %d, Period: %d, Frequency: %f, Envelope constant: %f');
        if numel(values) == 4
            amp = values(1);
            per = values(2);
            freq = values(3);
            a = values(4);
            perX = per;
            date = string(datetime('now', 'Format', 'yyyy-MM-dd_HH-mm-ss'));
            save('date.mat', 'date');
            save('parameters.mat', 'amp', 'per', 'freq', 'a');
            disp('Parameters saved to parameters.mat');
        else
            disp('Error parsing parameters from data.');
        end
    end

    % Handle test period and time vector values
    if startsWith(data, "Test Period is:")
        periodNumber = str2double(readline(src));
        if isnumeric(periodNumber) && ~isnan(periodNumber)
            disp(['Period Number: ', num2str(periodNumber)]);
            timeVectorIndex = 1; % Reset time vector index for the new test period  
            disp(['Amplitude is: ', num2str(400 * perX * periodNumber/100)])
        else
            disp('Error parsing test period number.');
        end
    elseif contains(data, "timeVector[")
        % Extract the time vector value
        value = str2double(readline(src));
        if isnumeric(value) && ~isnan(value)
            disp(['Time Vector Value: ', num2str(value)]);
            if size(timeVectors, 1) < periodNumber
                timeVectors(periodNumber, :) = NaN(1, size(timeVectors, 2)); % Preallocate if necessary
            end
            timeVectors(periodNumber, timeVectorIndex) = value;
            timeVectorIndex = timeVectorIndex + 1;
        else
            disp('Error parsing time vector value.');
        end
    elseif strcmp(strtrim(data), "Test has been done! Write new command")
        text = fopen('command.txt', 'w');
        fprintf(text, 'Stop'); % Write 'Stop' to the text file
        fclose(text);
        save('timeVectors.mat', 'timeVectors');
        disp('Time vectors saved to timeVectors.mat');
        referenceWave(); % Call referenceWave function (make sure it's defined elsewhere)
    end
end
