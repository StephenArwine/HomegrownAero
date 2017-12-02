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
data = []
pageLocation = 0


sensor_sample = 0

ser.write(b'A')

logtype = ser.read(1)

if logtype == b'T':

    bytes_to_read = ser.read(2)
    databytes = bytes_to_read[0] + bytes_to_read[1]

    data = ser.read(databytes)

    for samples in range(0,10):

        if data[0] == 0x41:

            sensor_sample = data[pageLocation:pageLocation + 24]

            point1 = SensorPoint()
            point1.sampleTick = (sensor_sample[1] << 0) + (sensor_sample[2] << 8)
            point1.sampleTick = point1.sampleTick + (sensor_sample[3] << 16) + (sensor_sample[4] << 24)

            point1.heightCM = (sensor_sample[5] << 0) + (sensor_sample[6] << 8)
            point1.heightCM = point1.heightCM + (sensor_sample[7] << 16) + (sensor_sample[8] << 24)

            point1.accelX = twosComp.twosComplement(sensor_sample[9], sensor_sample[10]) #Accel X conv
            point1.accelY = twosComp.twosComplement(sensor_sample[11], sensor_sample[12]) #Accel Y conv
            point1.accelZ = twosComp.twosComplement(sensor_sample[13], sensor_sample[14]) #Accel Z conv

            pageLocation = pageLocation + 24

            pointList.append(point1)


ser.close()

for x in range(0,10):
    print('Sample tick: ',pointList[x].sampleTick,' Height CM: ',pointList[x].heightCM,' AccelX :',pointList[x].accelX,' AccelY :',pointList[x].accelY,' AccelZ :',pointList[x].accelZ)

