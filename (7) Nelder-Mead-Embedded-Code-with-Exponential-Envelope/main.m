clear all; clc
% Initialize serial communication with Arduino
portArd = serialportlist("available");
arduino = serialport(portArd(1), 9600, 'Timeout', 40);
configureCallback(arduino, "terminator", @callbackSerial);
configureTerminator(arduino, "LF");

% Initialize variables
parametersVectors = [];
timeVectors = [];
periodNumber = 0;
timeVectorIndex = 1;

global a_matrix freq_matrix;
a_matrix = [0:-0.05:-0.2];
freq_matrix = [0.1:0.1:0.5];

message = 'amp_400_per_100_freq_0.4_a_0';

global g_message
g_message = message;


global a_counter freq_counter;
a_counter = 1;
freq_counter = 1;

pause(1); % Allow time for the connection to be established
% Callback function for serial communication
callbackSerial(arduino); % Ensure the callback is invoked
