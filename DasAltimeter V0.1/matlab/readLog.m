clc;
clear all;
close all;
numSec=1;
clearvars;



s1 = serial('COM14');    % define serial port
s1.BaudRate=9600;               % define baud rate
%% Connect the serial port to Arduino
s1.InputBufferSize = 1; % read only one byte every time
try
    fopen(s1);
catch err
    fclose(instrfind);
    error('Make sure you select the correct COM Port where the Arduino is connected.');
end

tic % Start timer
Tmax = 20;


while fread(s1) ~= 'T'
end

fwrite(s1,'A');

while toc <= Tmax
    
    
    logType = 0;
    dataLog = 0;
    sampleTick = 0;
    
    logType = fread(s1);
    
    if logType == 'A';
        
        dataLog(1) = fread(s1); %% time byte
        dataLog(2) = fread(s1); %% time byte
        dataLog(3) = fread(s1); %% time byte
        dataLog(4) = fread(s1); %% time byte
        
        dataLog(5) = fread(s1); %% height byte
        dataLog(6) = fread(s1); %% height byte
        dataLog(7) = fread(s1); %% height byte
        dataLog(8) = fread(s1); %% height byte
        
         dataLog(9) = fread(s1); % accel X
         dataLog(10) = fread(s1); % accel X
         
         dataLog(11) = fread(s1); % accel Y
         dataLog(12) = fread(s1); % accel Y
         
         dataLog(13) = fread(s1); % accel Z
         dataLog(14) = fread(s1); % accel Z
         
         dataLog(15) = fread(s1); % gyro X
         dataLog(16) = fread(s1); % gyro X
         
         dataLog(17) = fread(s1); % gyro Y
         dataLog(18) = fread(s1); % gyro Y
         
         dataLog(19) = fread(s1); % gyro Z
         dataLog(20) = fread(s1); % gyro Z
         
         dataLog(21) = fread(s1); % analog X
         dataLog(22) = fread(s1); % analog X
         
         sampleTick =  bitshift(dataLog(1),0,'uint32') + bitshift(dataLog(2),8,'uint32') + ...
             bitshift(dataLog(3),16,'uint32') + bitshift(dataLog(4),24,'uint32');
         
         heightCm =  bitshift(dataLog(5),0,'uint32') + bitshift(dataLog(6),8,'uint32') + ...
             bitshift(dataLog(7),16,'uint32') + bitshift(dataLog(8),24,'uint32');
         
         heightFt = heightCm * 0.03281;
         
         accelX = twosComplement(dataLog(9),dataLog(10));
         accelY = twosComplement(dataLog(11),dataLog(12));
         accelZ = twosComplement(dataLog(13),dataLog(14));
         
         gyroX = twosComplement(dataLog(15),dataLog(16));
         gyroY = twosComplement(dataLog(17),dataLog(18));
         gyroZ = twosComplement(dataLog(19),dataLog(20));
         
         analogAccel = dataLog(21) + bitshift(dataLog(22),8,'uint16');
         
         analogAccel = (analogAccel - 3900) * -0.0154;
         
         break;
    end
end
fclose(s1);



