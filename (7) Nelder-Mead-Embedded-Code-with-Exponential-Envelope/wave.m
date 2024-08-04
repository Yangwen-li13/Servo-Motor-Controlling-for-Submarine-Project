function wave(timeVector, periodNumber, amplitude, percentage, frequency, a, startingAmplitude)
    % Convert time vector to relative time in seconds
    timeInitial = timeVector - timeVector(1, 1);
    time = timeInitial / 1000;

    % Starting time for sine wave calculation
    TimeSin = time(1, 1);

    % Create a figure for plotting
    fig = figure('Name', 'Reference Wave', 'NumberTitle', 'off');
    hold on; % Allows multiple plots on the same figure

    % Loop through each motor
    for motorNumber = 0:11
        phase = (motorNumber * pi) / 24;
        L = motorNumber + 1;
        exponential_constant = exp(a * (13-L));

        % Loop through each period
        for i = 1:periodNumber
            Time1 = time(i, 1);
            Time2 = time(i, 2);
            Time3 = time(i, 3);
            Time4 = time(i, 4);

            if i ~= periodNumber
                Time5 = time(i + 1, 1);
                StartingTime = Time1:0.001:Time2;
                ContinuingTime = Time2:0.001:Time3;
                StoppingTime = Time3:0.001:Time4;
                WaitingTime = Time4:Time5;
                


                % Plot the starting, continuing, and waiting times
                plot(StartingTime, ((startingAmplitude + amplitude * i * percentage / 100) * sin((StartingTime - Time1) .* (pi/2) ./ (Time2 - Time1))) .* exponential_constant .* sin((TimeSin + StartingTime) .* (2 * pi * frequency) + phase), 'g');
                plot(StoppingTime, ((startingAmplitude + amplitude * i * percentage / 100) * sin((StoppingTime - Time4) .* (pi/2) ./ (Time3 - Time4))) .* exponential_constant .* sin((TimeSin + StoppingTime) .* (2 * pi * frequency) + phase), 'm');
                plot(ContinuingTime, (startingAmplitude + amplitude * i * percentage / 100) * exponential_constant * sin((TimeSin + ContinuingTime) * (2 * pi * frequency) + phase), 'b');
                plot(WaitingTime, zeros(size(WaitingTime)), 'r');
            else
                StartingTime = Time1:0.001:Time2;
                ContinuingTime = Time2:0.001:Time3;
                StoppingTime = Time3:0.001:Time4;

                % Plot the starting, continuing, and stopping times for the final period
                plot(StartingTime, ((startingAmplitude + amplitude * i * percentage / 100) * sin((StartingTime - Time1) .* (pi/2) ./ (Time2 - Time1))) .* exponential_constant .* sin((TimeSin + StartingTime) .* (2 * pi * frequency) + phase), 'g');
                plot(StoppingTime, ((startingAmplitude + amplitude * i * percentage / 100) * sin((StoppingTime - Time4) .* (pi/2) ./ (Time3 - Time4))) .* exponential_constant .* sin((TimeSin + StoppingTime) .* (2 * pi * frequency) + phase), 'm');
                plot(ContinuingTime, (startingAmplitude + amplitude * i * percentage / 100) * exponential_constant * sin((TimeSin + ContinuingTime) * (2 * pi * frequency) + phase), 'b');
            end
        end
    end

    % Labels and title
    xlabel('Time (s)');
    ylabel('Length (without DC Offset)');
    title('Motor Length vs Time (without DC Offset)');
    legend('Starting', 'Stopping', 'Active', 'Waiting', 'Location', 'Best');

    % Save the figure
    if ishandle(fig)
        saveas(fig, 'referenceWavePlot.fig');
        try
            saveas(fig, 'referenceWavePlot.eps');
        catch ME
            disp(['Failed to save EPS: ' ME.message]);
        end
    else
        error('Figure handle is not valid.');
    end

    hold off;
end
