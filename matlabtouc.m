% Create serial object
s = serial('COM3', 'BaudRate', 9600);

% Open serial port
fopen(s);

% Send integer value to Arduino
valueToSend = 5;  % Change this value as needed
fprintf(s, '%d', valueToSend);

% Close serial port
fclose(s);
delete(s);
clear s;
