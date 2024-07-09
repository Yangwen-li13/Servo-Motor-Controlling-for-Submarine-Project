clear all
portArd = serialportlist("available");
arduino = serialport(portArd, 9600, "Timeout", 10);
configureTerminator(arduino, "LF");
pause(2);
%{
sendMessage(arduino, 'stop', 10)   : Stops motors
sendMessage(arduino, 'start', 10)   : Starts motors
sendMessage(arduino, 'test', 10)   : Changes variables to test motors
%}

dataLine = readline(arduino);
disp(dataLine);
