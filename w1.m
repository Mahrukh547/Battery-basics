% Define the serial port properties
port = 'COM3';  % Change this to your Arduino's serial port name
baudrate = 115200;  % Match with Arduino's baud rate

% Create a serial object
s = serial(port, 'BaudRate', baudrate);

try
    % Open the serial port
    fopen(s);
    
    % Define the string to send
    datastring = '11.01,0.35,199.0';
    
    % Send the string to the Arduino
    fprintf(s, '%s\n', datastring);
    
    fprintf('Data sent to Arduino: %s\n', datastring);
    
    % Close the serial port
    fclose(s);
    
catch e
    % Display any error messages
    fprintf('Error: %s\n', e.message);
    
    % Close the serial port if it's open
    if strcmp(s.Status, 'open')
        fclose(s);
    end
end
try
    % Open the serial port
    fopen(s);
    
    % Send data to ESP32
    datastring = 'Hello ESP32';
    fprintf(s, '%s\n', datastring);
    
    fprintf('Data sent to ESP32: %s\n', datastring);
    
    % Read data from ESP32
    receivedString = fscanf(s, '%s');
    
    fprintf('Data received from ESP32: %s\n', receivedString);
    
    % Close the serial port
    fclose(s);
    
catch e
    % Display any error messages
    fprintf('Error: %s\n', e.message);
    
    % Close the serial port if it's open
    if strcmp(s.Status, 'open')
        fclose(s);
    end
end
% Define serial port properties
port = 'COM3';  % Replace with the correct COM port name for your ESP32
baudrate = 115200;  % Baud rate must match the ESP32's baud rate

% Create a serial object
s = serialport(port, baudrate);

% Open the serial port
configureTerminator(s, "LF");  % Set newline as the terminator
flush(s);  % Flush the serial buffer

try
    % Open the serial port
    fopen(s);
    
    % Send messages in a while loop
    while true
        % Get user input
        message = input('Enter message to send (or "exit" to quit): ', 's');
        
        % Check for exit command
        if strcmpi(message, 'exit')
            disp('Exiting...');
            break;
        end
        
        % Send message with newline character
        write(s, message);
        
        % Read and display the echoed message from ESP32
        echoedMessage = readline(s);
        disp(['Echoed message: ', echoedMessage]);
    end
    
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

