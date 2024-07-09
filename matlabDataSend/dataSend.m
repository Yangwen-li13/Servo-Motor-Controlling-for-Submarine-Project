clear all
portArd = serialportlist("available");
arduino = serialport(portArd, 9600, "Timeout", 20);
configureTerminator(arduino, "LF");
pause(2);
%{
sendMessage(arduino, 'stop', 20)   : Stops motors
sendMessage(arduino, 'start',20)   : Starts motors
sendMessage(arduino, 'test', 20)   : Changes variables to test motors
%}

dataLine = readline(arduino);
disp(dataLine);
