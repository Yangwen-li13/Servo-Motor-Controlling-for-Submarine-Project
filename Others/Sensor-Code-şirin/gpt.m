% clear all;
% clc;
function main
    % seri port ayarları
    s = serialport('COM4', 115200);
    configureTerminator(s, "LF");
    s.Timeout = 10;

    % veri depolama 
    global recording data sensor_1 sensor_2;
    recording = false;
    data = [];
    sensor_1 = [];
    sensor_2 = [];


    configureCallback(s, "terminator", @callbackSerial);

    disp('Komut bekleniyor: "start" veya "stop"');

    while true
        % Komut penceresinden giriş bekle
        cmd = input('Komut girin: ', 's');

        if strcmp(cmd, 'start')
            disp('Veri kaydetmeye başladı...');
            recording = true;
        elseif strcmp(cmd, 'stop')
            disp('Veri kaydetme durduruldu.');
            recording = false;
            break;
        else
            disp('Geçersiz komut. "start" veya "stop" girin.');
        end
    end

    % Verileri plotlama
    if ~isempty(data)
        figure;
        subplot(3, 1, 1);
        plot(sensor_1(:, 1), 'r');
        hold on;
        plot(-1 .* sensor_2(:, 1), 'b');
        ylabel("x axis");
        xlabel("time");
        hold off;

        subplot(3, 1, 2);
        plot(sensor_1(:, 2), 'r');
        hold on;
        plot(-1 .* sensor_2(:, 2), 'b');
        ylabel("y axis");
        xlabel("time");
        hold off;

        subplot(3, 1, 3);
        plot(sensor_1(:, 3), 'r');
        hold on;
        plot(-1 .* sensor_2(:, 3), 'b');
        ylabel("z axis");
        xlabel("time");
        hold off;
    else
        disp('Kayıtlı veri yok.');
    end
end

function callbackSerial(src, ~)
    global recording data sensor_1 sensor_2;

    if ~recording
        return;
    end

    dataLine = readline(src);

    if isempty(dataLine)
        return;
    end

    values = sscanf(dataLine, '%f,%f,%f,%f');

    if isempty(values) || numel(values) ~= 4
        return;
    end

    % Verileri sakla
    data = [data; values'];
    disp(values');

    % Sensor verilerini ayırma
    if values(1) == 1
        sensor_1 = [sensor_1; values(2:4)'];
    elseif values(1) == 2
        sensor_2 = [sensor_2; values(2:4)'];
    end
end
