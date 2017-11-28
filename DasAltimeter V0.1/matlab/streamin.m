function streamin

clc;
clear all;
close all;
numSec=1;
clearvars;



s1 = serial('COM14');    % define serial port
s1.BaudRate=9600;               % define baud rate
%% Connect the serial port to Arduino
%%s1.InputBufferSize = 1; % read only one byte every time
try
    fopen(s1);
catch err
    fclose(instrfind);
    error('Make sure you select the correct COM Port where the Arduino is connected.');
end

tic % Start timer
Tmax = 10;

data = 0;
count = 1;

while toc <= Tmax
    
    
    data = fread(s1,255);
  
    %%  page = page + 1
    
    
 %%   if data(1) ~= 0;
 %%       newdata = 1
       %% break;
 %%   end
    
        
    end
fclose(s1);

end
