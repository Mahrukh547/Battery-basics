% Create a serial port object
arduino = serialport("COM3", 115200);

try
    % Open the serial port
    fopen(arduino);
    % Flush the serial port
    flush(arduino);
    
    % Display the number of bytes available in the serial buffer
    disp('Numbytes in serial is:');
    disp(arduino.NumBytesAvailable);
    
    % Send a message to Arduino
    writeline(arduino, "Hello From Matlab");
    
    % Wait until data is available
    while arduino.NumBytesAvailable == 0
       
    end
end

    % Declare the received string for testing
receivedString = "11.05,0.35,199.0";

try
    % Parse the received string to extract three floating-point numbers
    parsedData = sscanf(receivedString, '%f,%f,%f');
    
    % Check if three numbers were successfully parsed
    if length(parsedData) == 3
        % Extract the parsed numbers
        number1 = parsedData(1);
        number2 = parsedData(2);
        number3 = parsedData(3);
        
        % Display the extracted numbers
        fprintf('Extracted numbers: %f, %f, %f\n', number1, number2, number3);
        
        % Create a serial port object
        arduino = serialport("COM3", 115200);
        
        % Open the serial port
        fopen(arduino);
        
        % Send the parsed numbers to Arduino
        fprintf(arduino, '%f %f %f\n', number1, number2, number3);
        
        disp('Data sent to Arduino');
        
        % Close the serial port
        fclose(arduino);
    else
        fprintf('Error: Failed to parse three floating-point numbers from the received string.\n');
    end
    
catch e
    % Display any error messages
    fprintf('Error: %s\n', e.message);
end
