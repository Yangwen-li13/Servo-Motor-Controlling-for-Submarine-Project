clear; clc;

% Initialize serial communication with Arduino
portArd = serialportlist("available");
arduino = serialport(portArd(1), 9600);
configureCallback(arduino, "terminator", @callbackSerial);
configureTerminator(arduino, "LF");

% Initialize variables
timeVectors = [];
periodNumber = 0;
timeVectorIndex = 1;

% Callback function for serial communication
function callbackSerial(src, ~)
    persistent timeVectors periodNumber timeVectorIndex;
    
    if isempty(timeVectors)
        timeVectors = []; % Initialize if empty
    end
    
    data = readline(src);
    disp(data);
    
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
        %if size(timeVectors, 1) < periodNumber
            timeVectors(periodNumber, timeVectorIndex) = value;
        %end
        timeVectorIndex = timeVectorIndex + 1;
    elseif strcmp(strtrim(data), "Test has been done! Write new command")
        disp("Test has been done! Write new command.");
        saveTimeVectors(timeVectors); % Save the time vectors to a .mat file
        load("timeVectors.mat");
        %wave(timeVectors,4,400,25,0.5);
    end
end

% Function to save time vectors to a .mat file
function saveTimeVectors(timeVectors)
    save('timeVectors.mat', 'timeVectors');
    disp('Time vectors saved to timeVectors.mat');
end
