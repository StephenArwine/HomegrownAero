function [ comp ] = twosComplement( data1,data2 )

    comp = data1 + bitshift(data2,8);
    
    
    if (bitshift(data2,-7) == 1);
        compl = bitcmp(data1,'uint8');
        compH = bitcmp(data2,'uint8') ;
        compH = bitshift(compH,8,'uint16') - 1;
        
        accelzand = compH + compl;
        comp = bitshift(accelzand,-4,'int16');
        comp = comp * -0.0078125;
    else
        
        comp = bitshift(comp,-4);
        comp = comp * 0.0078125;
    end


end

