import serial
import twosComp

ser = serial.Serial()
ser.baudrate = 9600
ser.port = 'COM14'
ser.timeout = 5

if ser.is_open:
    print('already open')
    ser.close()
else:
    ser.open()


class SensorPoint:
    def __init__(self):
        self.sampleTick = 0
        self.heightCM = 0
        self.accelX = 0
        self.accelY = 0
        self.accelZ = 0
        self.gyroX = 0
        self.gyroY = 0
        self.gyroZ = 0
        self.analogRaw = 0


pointList = []
pageLocation = 0

data = []

sensor_sample = 0

ser.write(b'A')

logtype = ser.read(1)

if logtype == b'T':
    pages_to_read = ser.read(1)
    pages = int.from_bytes(pages_to_read,byteorder='big')
    print('pages to read: ',pages)

    for page in range(0,pages):

        PageOfData = []
        PageOfData = ser.read(255)

        data.append(PageOfData)


#for i in range(0,255):
    #print('{0:3d} {1:4X} {2:4X} {3:4X} {4:4X} {5:4X}'.format( i, data[0][i], data[1][i], data[2][i], data[3][i], data[4][i]))

LocationInPage = 0
NextPage = 0

for page2 in range(0,pages):
    NextPage = 0

    while (NextPage < 1):

        if data[page2][LocationInPage] == 0x41:
            sensor_sample = data[page2][pageLocation:pageLocation + 24]

            point = SensorPoint()
            point.sampleTick = int.from_bytes(sensor_sample[1:4],byteorder='little')
            point.heightCM = int.from_bytes(sensor_sample[5:8], byteorder='little')
            point.accelX = twosComp.twosComplement(sensor_sample[9], sensor_sample[10]) * 0.0078125 #Accel X conv
            point.accelY = twosComp.twosComplement(sensor_sample[11], sensor_sample[12]) * 0.0078125 #Accel Y conv
            point.accelZ = twosComp.twosComplement(sensor_sample[13], sensor_sample[14]) * 0.0078125 #Accel Z conv

            if (pageLocation + 24) > 255:
                NextPage = 1
                pageLocation = (pageLocation + 24) - 255

            else:
                pageLocation = pageLocation + 24


            print(point.sampleTick,' ',point.heightCM,' ',point.accelX,' ',point.accelZ)
            pointList.append(point)



ser.close()

print('done')
   # for x in range(0,10):
    # print('Sample tick: ',pointList[x].sampleTick,' Height CM: ',pointList[x].heightCM,' AccelX :',pointList[x].accelX,' AccelY :',pointList[x].accelY,' AccelZ :',pointList[x].accelZ)

