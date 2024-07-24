clear; clc;

% Initialize serial communication with Arduino
portArd = serialportlist("available");
arduino = serialport(portArd(1), 9600,'Timeout',40);
configureCallback(arduino, "terminator", @callbackSerial);
configureTerminator(arduino, "LF");

% Initialize variables
parametersVectors = [];
timeVectors = [];
periodNumber = 0;
timeVectorIndex = 1;

% Callback function for serial communication
callbackSerial(arduino);
