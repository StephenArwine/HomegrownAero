% Communications MatLab <--> Arduino
% Matlab file 1 for use with Arduino file 1
clc;
clear all;
close all;
numSec=1;
t=[];
v=[];

s1 = serial('COM5');    % define serial port
s1.BaudRate=9600;               % define baud rate
%% Connect the serial port to Arduino
s1.InputBufferSize = 1; % read only one byte every time
try
    fopen(s1);
catch err
    fclose(instrfind);
    error('Make sure you select the correct COM Port where the Arduino is connected.');
end
%% Create a figure window to monitor the live data
Tmax = 60; % Total time for data collection (s)
figure,
grid on,
xlabel ('Time (s)'), ylabel('Data (8-bit)'),
axis([0 Tmax+1 -2 1]),
%% Read and plot the data from Arduino
Ts = 0.03; % Sampling time (s)
i = 0;
data = 0;
accelZ = 0;
t = 0;
tic % Start timer

time = 0;
currentTic = 0;
firstTic = 0;


while toc <= Tmax
    i = i + 1;
    % Read buffer data
    while fread(s1) ~= 'A'
    end
    time(i,4) = fread(s1);
    time(i,3) = fread(s1);
    time(i,2) = fread(s1);
    time(i,1) = fread(s1);
    
    time(i,1) = bitshift(time(i,1),24,'uint32');
    time(i,2) = bitshift(time(i,2),16,'uint32');
    time(i,3) = bitshift(time(i,3),8,'uint32');
    
    if firstTic == 0;
        firstTic = time(i,1)+time(i,2)+time(i,3)+time(i,4); 
    end
    
    currentTic(i,1) = time(i,1)+time(i,2)+time(i,3)+time(i,4);
    currentTic(i,1) = currentTic(i,1) - firstTic
    
    
    
    
    
    
    
    
    data(i,1) = fread(s1);
    data(i,2) = fread(s1);
    
    accelZ(i) = data(i,1) + bitshift(data(i,2),8);
    
    
    if (bitshift(data(i,2),-7) == 1);
        compl = bitcmp(data(i,1),'uint8');
        compH = bitcmp(data(i,2),'uint8') ;
        compH = bitshift(compH,8,'uint16') - 1;
        
        accelzand = compH + compl;
        accelZ(i) = bitshift(accelzand,-4,'int16');
        accelZ(i) = accelZ(i) * -0.0078125;
    else
        
        accelZ(i) = bitshift(accelZ(i),-4);
        accelZ(i) = accelZ(i) * 0.0078125;
    end
    
    
    
    
    %% Read time stamp
    % If reading faster than sampling rate, force sampling time.
    % If reading slower than sampling rate, nothing can be done. Consider
    % decreasing the set sampling time Ts
    t(i) = currentTic(i)/1000;
    if i > 1
        T = toc - t(i-1);
        while T < Ts
            T = toc - t(i-1);
        end
    end
    %t(i) = toc;
    %% Plot live data
    if i > 1
        line([t(i-1) t(i)],[accelZ(i-1) accelZ(i)])
        drawnow
    end
end


fclose(s1);
