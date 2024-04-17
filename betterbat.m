% Define serial port properties
port = 'COM3';  % Replace with the correct COM port name for your ESP32
baudrate = 115200;  % Baud rate must match the ESP32's baud rate

% Create a serial object
s = serialport(port, baudrate);

try
    % Open the serial port
    fopen(s);
    
    % Send battery parameters to Arduino
    fprintf(s, 'Q=%.2f,Vnom=%.2f,Ri=%.4f,AH=%.2f,V1=%.2f,AH1=%.2f,AH0=%.2f,SOC0=%.2f,Ts=%.2f,Cdc=%.4f,Vcdc0=%.2f\n', ...
            Q, Vnom, Ri, AH, V1, AH1, AH0, SOC0, Ts, Cdc, Vcdc0);
    
    % Close the serial port
    fclose(s);
    delete(s);
    clear s;
    
catch ME
    % Display error message
    disp(['Error: ', ME.message]);
    
    % Close and delete the serial port object
    if isvalid(s)
        fclose(s);
        delete(s);
        clear s;
    end
end
