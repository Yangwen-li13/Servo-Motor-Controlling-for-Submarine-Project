clear all;
s = serialport("COM4", 115200); 
configureTerminator(s, "LF");
num_samples = 270; 
data = zeros(num_samples,4);
sensor_1 = zeros(1,3)
sensor_2 = zeros(1,3)
tic
for i = 1:num_samples
    line = readline(s); % Seri porttan bir satır oku
    values = sscanf(line, '%f,%f,%f,%f'); % Satırı sayı dizisine dönüştür
    if numel(values) == 4
        data(i, :) = values'; % Matrise ekle
    end
        %sensor dataları ayırma
        if data(i,1)==1
            sensor_1 = [sensor_1; data(i,2:4)]
        elseif data(i,1)==2
            sensor_2 = [sensor_2; data(i,2:4)]
        else
        end

    

  toc 
end

% Seri portu kapatma
clear s;
disp(data);


    % sensor_1 = data(data(i,1)==1, 2:4);
    % sensor_2 = data(data(i,1)==2, 2:4);
    %plotlama
    subplot(3, 1, 1);
    plot(sensor_1(:, 1), 'r');
    hold on;
    plot(sensor_2(:, 1), 'b');
    ylabel("x ekseni");
    xlabel("zaman");
    hold off;

     subplot(3, 1, 2);
     plot(sensor_1(:, 2), 'r');
     hold on;
     plot(sensor_2(:, 2), 'b');
     ylabel("y ekseni");
     xlabel("zaman");
     hold off;

     subplot(3, 1, 3);
     plot(sensor_1(:, 3), 'r');
     hold on;
     plot(sensor_2(:, 3), 'b');
     ylabel("z ekseni");
     xlabel("zaman");
     hold off;
