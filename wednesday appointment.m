%send data to ESP-32
%Open "COM-3"
ESP32 = serialport("COM3",115200);






%wait for response from micro-controller
%request read available data
%parse the data 
