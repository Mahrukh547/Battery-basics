arduino = serialport("COM3",115200)
flush(arduino)
disp('Numbytes in serial is:')
arduino.NumBytesAvailable
writeline(arduino,"Hello From Matlab");
while(arduino.NumBytesAvailable == 0)

end

readline(arduino)

