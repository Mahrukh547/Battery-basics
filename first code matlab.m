try
    % Define serial port properties
    port = 'COM3';  % Replace with the correct COM port name for your Arduino
    baudrate = 9600;

    % Create a serial object
    x = serial(port, 'BaudRate', baudrate);

    % Open the serial port
    fopen(x);

    % Send test data to Arduino
    dataString = 'A';
    fprintf(x, '%s\n', dataString);  % Note: Added newline character

    fprintf('Data sent to Arduino: %s\n', dataString);

    % Wait for data with a timeout
    timeout = 10;  % Timeout in seconds
    tic;  % Start a timer
    while x.BytesAvailable == 0 && toc < timeout
        % Wait
    end

    % Check if data is available
    if x.BytesAvailable > 0
        % Read test data from Arduino
        receivedString = fscanf(x, '%s');
        fprintf('Data received from Arduino: %s\n', receivedString);
    else
        fprintf('Timeout: No data received from Arduino\n');
    end

    % Close the serial port
    fclose(x);

catch e
    % Display any error messages
    fprintf('Error: %s\n', e.message);
    fprintf('Error Identifier: %s\n', e.identifier);

    % Close the serial port if it's open
    if strcmp(x.Status, 'open')
        fclose(x);
    end
end
