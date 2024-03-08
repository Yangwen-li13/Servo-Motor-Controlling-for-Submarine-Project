arduino = serialport("COM7",9600)
configureTerminator(arduino,"CR/LF");
flush(arduino);
arduino.UserData = struct("Data",[],"Count",1)
configureCallback(arduino,"terminator",@readSineWaveData);
readSineWaveData(arduino)