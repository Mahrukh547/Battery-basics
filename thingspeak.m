% Create TCP/IP client
client = tcpclient('192.168.1.184', 1884); % Replace with Arduino's IP address and port

try
    % Connect to Arduino
    connect(client);

    % Send data to Arduino
    write(client, 'Hello Arduino');

    % Read data from Arduino
    data = read(client);
    disp(['Received from Arduino: ', data]);
    
    % Close the client
    close(client);
    
catch e
    % Display any errors
    disp(['Error: ', e.message]);
    
    % Close the client if not closed
    if isvalid(client)
        close(client);
    end
end
