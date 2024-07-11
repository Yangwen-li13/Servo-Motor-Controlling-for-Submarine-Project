clear; clc;
portArd = serialportlist("available");
arduino = serialport(portArd(1), 9600);
configureCallback(arduino, "terminator", @callbackSerial);
configureTerminator(arduino, "LF");

%{
To send message use:  
writeline(arduino, 'Any message is here')

To begin test, send the message as 'test' to the arduino:
writeline(arduino, 'test')
%}

function callbackSerial(src, ~)
    data = readline(src);
    disp(data);
end
