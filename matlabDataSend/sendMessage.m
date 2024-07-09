function sendMessage(arduino, message, timeout)
    % Send a message to the Arduino
    writeline(arduino, message);

    % Start a timer to stop the loop after the specified timeout
    tic;
    
    % Continuously read and display data from the Arduino
    try
        while toc < timeout
            if arduino.NumBytesAvailable > 0
                dataLine = readline(arduino);
                disp(dataLine);
            end
            pause(0.1); % Adjust the pause duration as needed
        end
    catch ME
        disp('Error occurred:');
        disp(ME.message);
    end
end
