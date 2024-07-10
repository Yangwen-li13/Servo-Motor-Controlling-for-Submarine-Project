clear all; clc;
portArd = serialportlist("available");
arduino = serialport(portArd(1), 9600);
configureCallback(arduino, "terminator", @callbackSerial);
configureTerminator(arduino, "LF");



function callbackSerial(src, ~)
    data = readline(src);
    disp(data);
end
