clear all
serialportlist("available");
arduino = serialport("COM4", 9600, "Timeout", 10);
configureTerminator(arduino, "LF");
pause(2);
message = "d"
writeline(arduino,message);
response = readline(arduino);
disp(response);
response = readline(arduino);
disp(response);

