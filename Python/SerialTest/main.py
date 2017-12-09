import serial
import twosComp
import time
import matplotlib.pyplot as plt
import os

ser = serial.Serial()
ser.baudrate = 19200
ser.port = 'COM14'
ser.timeout = 5


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


class FlightPoint:
    def __init__(self):
        self.FlightNumb = 0
        self.bufferTick = 0
        self.groundOffset = 0
        self.groundTemperature = 0
        self.groundAccel = 0


pointList = []
flight = FlightPoint()

data = []

sensor_sample = 0
sensor_sample_part = 0

input(' Wait for startup beep and press ENTER')

ser.open()

ser.write(b'H')

handshake = ser.read(1)

if handshake == b'H':
    print('')
    print(' Altimeter connected, what would you like to do?')
    print('     P    Print sensors')
    print('     L    Download flight log')
    print('     E    Chip Erase')
    print('')

    option = input(' ').upper()

    print('')


if option == 'E':
    ser.write(b'E')
    print('')
    print(' Please wait.')

    time.sleep(15)
    isWriteDone = ser.read(1)
    if isWriteDone == b'E':
        print('')
        print('Chip erased')
        input(' Wait for startup beep and press ENTER')

if option == 'L':

    ser.write(b'L')

    print(' Which flight log would you like to download?')

    allFlightsFound = 1
    while allFlightsFound:
        flightNumb = ser.read(1)

        if flightNumb == b'\x0F':
            break
        else:
            print('    ',flightNumb.decode('utf-8'))

    print('')
    flightToRead = input(' ')
    print('')
    ser.write(flightToRead.encode('utf-8'))

    StartTime = time.clock()

    pages_to_read = ser.read(1)
    pages = int.from_bytes(pages_to_read, byteorder='big')
    print('Reading', ((pages * 256)/1000), 'kilobytes of data.')

    for page in range(0, pages):
        PageOfData = []
        PageOfData = ser.read(256)

        data.append(PageOfData)

    ser.close()

CurrentPage = 0
LocationInPage = 0
ProcessLog = True

samplenum = 0

while ProcessLog:

    if CurrentPage > pages:
        ProcessLog = False
        break

    # Flight point decode
    if data[CurrentPage][LocationInPage] == 0x46:
        # print('F found, location', LocationInPage)

        if (LocationInPage + 17) >= 255:

            if (CurrentPage + 1) >= pages:
                ProcessLog = False
                break

            sensor_sample_part = data[CurrentPage][LocationInPage:256]
            sensor_sample = sensor_sample_part + data[CurrentPage + 1][0:(16 - (255 - LocationInPage))]

            point = FlightPoint()
            point.FlightNumb = sensor_sample[1]
            point.bufferTick = int.from_bytes(sensor_sample[2:5], byteorder='little')
            point.groundOffset = int.from_bytes(sensor_sample[6:9], byteorder='little')
            point.groundTemperature = int.from_bytes(sensor_sample[10:13], byteorder='little')
            point.groundAccel = twosComp.twosComplement(sensor_sample[14], sensor_sample[15]) * 0.0078125

            CurrentPage += 1
            LocationInPage -= 240  # rollover + 16

        else:
            sensor_sample = data[CurrentPage][LocationInPage:LocationInPage + 16]

            point = FlightPoint()
            point.FlightNumb = sensor_sample[1]
            point.bufferTick = int.from_bytes(sensor_sample[2:5], byteorder='little')
            point.groundOffset = int.from_bytes(sensor_sample[6:9], byteorder='little')
            point.groundTemperature = int.from_bytes(sensor_sample[10:13], byteorder='little')
            point.groundAccel = twosComp.twosComplement(sensor_sample[14], sensor_sample[15]) * 0.0078125

            LocationInPage += 17

        flight = point

    # Sensor point decoding
    if data[CurrentPage][LocationInPage] == 0x41:
        # print('A found, location', LocationInPage, 'Sample number:', samplenum)
        samplenum += 1

        if (LocationInPage + 23) > 255:
            if (CurrentPage + 1) >= pages:
                ProcessLog = False
                break

            sensor_sample_part = data[CurrentPage][LocationInPage:255]
            sensor_sample = sensor_sample_part + data[CurrentPage + 1][0:(23 - (256 - LocationInPage))]

            point = SensorPoint()
            point.sampleTick = int.from_bytes(sensor_sample[1:4], byteorder='little')
            point.heightCM = int.from_bytes(sensor_sample[5:8], byteorder='little')
            point.accelX = twosComp.twosComplement(sensor_sample[9], sensor_sample[10]) * 0.0078125  # Accel X conv
            point.accelY = twosComp.twosComplement(sensor_sample[11], sensor_sample[12]) * 0.0078125  # Accel Y conv
            point.accelZ = twosComp.twosComplement(sensor_sample[13], sensor_sample[14]) * 0.0078125  # Accel Z conv

            point.gyroX = twosComp.twosComplement(sensor_sample[15], sensor_sample[16]) * 0.0078125  # Accel Z conv
            point.gyroY = twosComp.twosComplement(sensor_sample[17], sensor_sample[18]) * 0.0078125  # Accel Z conv
            point.gyroZ = twosComp.twosComplement(sensor_sample[19], sensor_sample[20]) * 0.0078125  # Accel Z conv

            CurrentPage += 1
            LocationInPage -= 232  # rollover + 24

            # print('Reading page ', CurrentPage, ' starting Location ', LocationInPage, 'point',
            #      hex(data[CurrentPage][LocationInPage]))

        else:
            sensor_sample = data[CurrentPage][LocationInPage:LocationInPage + 23]

            point = SensorPoint()
            point.sampleTick = int.from_bytes(sensor_sample[1:4], byteorder='little')
            point.heightCM = int.from_bytes(sensor_sample[5:8], byteorder='little')
            point.accelX = twosComp.twosComplement(sensor_sample[9], sensor_sample[10]) * 0.0078125  # Accel X conv
            point.accelY = twosComp.twosComplement(sensor_sample[11], sensor_sample[12]) * 0.0078125  # Accel Y conv
            point.accelZ = twosComp.twosComplement(sensor_sample[13], sensor_sample[14]) * 0.0078125  # Accel Z conv

            point.gyroX = twosComp.twosComplement(sensor_sample[15], sensor_sample[16]) * 0.0078125  # Accel Z conv
            point.gyroY = twosComp.twosComplement(sensor_sample[17], sensor_sample[18]) * 0.0078125  # Accel Z conv
            point.gyroZ = twosComp.twosComplement(sensor_sample[19], sensor_sample[20]) * 0.0078125  # Accel Z conv

            LocationInPage += 24

        pointList.append(point)

runningAverage = 0
averageX = 0
averageY = 0
averageZ = 0

elapsedTime = (time.clock() - StartTime)
print('Took:', elapsedTime, 'seconds to read and process', pointList.__len__(), 'data points')

print('Flight Numb:', flight.FlightNumb, 'Buffer Time:', flight.bufferTick, 'Ground Offset:', flight.groundOffset,
      'Ground Temperature', flight.groundTemperature)

plt.figure()

ax1 = plt.subplot(2, 1, 1)
ax2 = plt.subplot(2, 1, 2)

for x in range(0, pointList.__len__()):
    if x == 0:
        runningAverage = pointList[x].heightCM
        averageX = pointList[x].accelX
        averageY = pointList[x].accelY
        averageZ = pointList[x].accelZ
        print('Sample 0 tick:', pointList[x].sampleTick, 'Height CM:', pointList[x].heightCM, 'AccelX:',
              pointList[x].accelX, 'AccelY:', pointList[x].accelY, 'AccelZ:', pointList[x].accelZ)

    elif x > 0:
        runningAverage = runningAverage * 0.9 + pointList[x].heightCM * 0.1
        averageX = averageX * 0.6 + pointList[x].accelX * 0.4
        averageY = averageY * 0.6 + pointList[x].accelY * 0.4
        averageZ = averageZ * 0.6 + pointList[x].accelZ * 0.4

        dt = pointList[x].sampleTick - pointList[x - 1].sampleTick
        print('Sample', x, 'tick:', pointList[x].sampleTick, 'Sample DT:', dt, 'Height CM:', pointList[x].heightCM,
              'AccelX:',
              pointList[x].accelX, 'AccelY:', pointList[x].accelY, 'AccelZ:', pointList[x].accelZ)

    ax1.plot(pointList[x].sampleTick, averageX, 'r.')
    ax1.plot(pointList[x].sampleTick, averageY, 'g.')
    ax1.plot(pointList[x].sampleTick, averageZ, 'b.')

    ax2.plot(pointList[x].sampleTick, runningAverage, 'r.')
    # plt.plot(pointList[x].sampleTick, pointList[x].gyroX, 'r.')
    #ax2.plot(pointList[x].sampleTick, pointList[x].heightCM, 'g.')
    # plt.plot(pointList[x].sampleTick, pointList[x].gyroZ, 'b.')

plt.show()
print('')
input('press ENTER to close')
