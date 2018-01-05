import twosComp
from twosComp import twos_complement
import time

sensorPointLength = 24
eventSampleLength = 12
flightPointLength = 16

class FlightPointType:
    def __init__(self):
        self.FlightNumb = 0
        self.bufferTick = 0
        self.groundOffset = 0
        self.groundTemperature = 0
        self.groundAccel = 0


class SensorPointType:
    def __init__(self):
        self.sampleTick = 0
        self.Dt = 0
        self.heightFeet = 0
        self.velocity = 0
        self.velocityfract = 0
        self.accelX = 0
        self.accelY = 0
        self.accelZ = 0
        self.accelZFract = 0
        self.accelZraw = 0
        self.accelZrawFract = 0
        self.gyroX = 0
        self.gyroY = 0
        self.gyroZ = 0
        self.analogRaw = 0
        self.rawFeet = 0

class EventPointType:
    def __init__(self):
        self.eventType = 0
        self.sampleTick = 0
        self.altitude = 0
        self.voltage = 0


def build_flight_point(data, CurrentPage, pages, LocationInPage):
    flight = FlightPointType()

    if (LocationInPage + flightPointLength + 1) >= 255:

        sensor_sample_part = data[CurrentPage][LocationInPage:255]
        sensor_sample = sensor_sample_part + data[CurrentPage + 1][0:(flightPointLength - (256 - LocationInPage))]

        point = FlightPointType()
        point.FlightNumb = sensor_sample[1]
        point.bufferTick = int.from_bytes(sensor_sample[2:5], byteorder='little')
        point.groundOffset = int.from_bytes(sensor_sample[6:9], byteorder='little')
        point.groundTemperature = int.from_bytes(sensor_sample[10:13], byteorder='little')
        point.groundAccel = twosComp.twos_complement(sensor_sample[14], sensor_sample[15]) * 0.0078125

        CurrentPage += 1
        LocationInPage -= 240  # rollover + 16

    else:
        sensor_sample = data[CurrentPage][LocationInPage:LocationInPage + flightPointLength]

        point = FlightPointType()
        point.FlightNumb = sensor_sample[1]
        point.bufferTick = int.from_bytes(sensor_sample[2:5], byteorder='little')
        point.groundOffset = int.from_bytes(sensor_sample[6:9], byteorder='little')
        point.groundTemperature = int.from_bytes(sensor_sample[10:13], byteorder='little')
        point.groundAccel = twosComp.twos_complement(sensor_sample[14], sensor_sample[15]) * 0.0078125

        LocationInPage += flightPointLength

    return point, CurrentPage, LocationInPage


def build_sensor_point(data, currentPage, pages, locationInPage, lastTick):
    point = SensorPointType()

    if (locationInPage + sensorPointLength + 1) >= 255:

        if (currentPage + 1) >= pages:
            ProcessLog = False
            return 0, currentPage + 1, 0

        sensor_sample_part = data[currentPage][locationInPage:256]
        sensor_sample = sensor_sample_part + data[currentPage + 1][0:(sensorPointLength - (256 - locationInPage))]

        point.sampleTick = int.from_bytes(sensor_sample[1:4], byteorder='little')
        point.Dt = point.sampleTick - lastTick
        point.heightFeet = int.from_bytes(sensor_sample[5:8], byteorder='little')

        point.accelZ = twos_complement(sensor_sample[9], sensor_sample[10])
        point.accelZFract = twos_complement(sensor_sample[11], sensor_sample[12])
        point.accelZFract = point.accelZFract / 1000
        point.accelZ = point.accelZ + point.accelZFract

        point.velocity = twos_complement(sensor_sample[13], sensor_sample[14])
        point.velocityfract = twos_complement(sensor_sample[15], sensor_sample[16])
        point.velocityfract = point.velocityfract / 1000
        point.velocity = point.velocity + point.velocityfract

        point.rawFeet = int.from_bytes(sensor_sample[17:20], byteorder='little')

        point.accelZraw = twos_complement(sensor_sample[21], sensor_sample[22])
        point.accelZrawFract = sensor_sample[23] / 256
        point.accelZraw = point.accelZraw + point.accelZrawFract

        #point.gyroY = twos_complement(sensor_sample[17], sensor_sample[18]) * 0.0078125  # Gyro Y conv
        #point.gyroZ = twos_complement(sensor_sample[19], sensor_sample[20]) * 0.0078125  # Gyro Z conv

        currentPage += 1
        locationInPage -= (0xff - sensorPointLength + 1 )

        # print('Reading page ', currentPage, ' starting Location ', locationInPage, 'point',
        #      hex(data[currentPage][locationInPage]))

    else:
        sensor_sample = data[currentPage][locationInPage:locationInPage + sensorPointLength]

        point.sampleTick = int.from_bytes(sensor_sample[1:4], byteorder='little')
        point.Dt = point.sampleTick - lastTick
        point.heightFeet = int.from_bytes(sensor_sample[5:8], byteorder='little')

        point.accelZ = twos_complement(sensor_sample[9], sensor_sample[10])
        point.accelZFract = twos_complement(sensor_sample[11], sensor_sample[12])
        point.accelZFract = point.accelZFract / 1000
        point.accelZ = point.accelZ + point.accelZFract

        point.velocity = twos_complement(sensor_sample[13], sensor_sample[14])
        point.velocityfract = twos_complement(sensor_sample[15], sensor_sample[16])
        point.velocityfract = point.velocityfract / 1000
        point.velocity = point.velocityfract + point.velocityfract

        point.rawFeet = int.from_bytes(sensor_sample[17:20], byteorder='little')

        point.accelZraw = twos_complement(sensor_sample[21], sensor_sample[22])
        point.accelZrawFract = sensor_sample[23] / 256
        point.accelZraw = point.accelZraw + point.accelZrawFract



        #point.gyroY = twos_complement(sensor_sample[17], sensor_sample[18]) * 0.0078125  # Accel Z conv
        #point.gyroZ = twos_complement(sensor_sample[19], sensor_sample[20]) * 0.0078125  # Accel Z conv

        locationInPage += sensorPointLength

    return point, currentPage, locationInPage


def build_event_point(data,currentPage, pages, locationInPage):

    event = EventPointType()

    if (locationInPage + eventSampleLength + 1) > 255:

        eventSamplePart = data[currentPage][locationInPage:255]
        eventSample = eventSamplePart + data[currentPage + 1][0:(eventSampleLength - (256 - locationInPage))]
        if (currentPage + 1) >= pages:
            ProcessLog = False
            return 0, currentPage + 1, 0

        event.eventType = eventSample[1]
        event.sampleTick = int.from_bytes(eventSample[2:5], byteorder='little')


        currentPage += 1
        locationInPage -= (0xff - eventSampleLength)

    else:
        eventSample = data[currentPage][locationInPage:locationInPage + eventSampleLength]

        event.eventType = eventSample[1]
        event.sampleTick = int.from_bytes(eventSample[2:5], byteorder='little')



        locationInPage += eventSampleLength

    return currentPage, locationInPage, event


def processDataLog(data, pages, StartTime):
    pointList = []
    eventList = []
    CurrentPage = 0
    LocationInPage = 0
    ProcessLog = True

    samplenum = 0

    while ProcessLog:

        if CurrentPage >= pages:
            ProcessLog = False
            break

        #print(chr(data[CurrentPage][LocationInPage]), CurrentPage, LocationInPage)

        # Flight point decode
        if chr(data[CurrentPage][LocationInPage]) == 'F':
            # print('F found, location', LocationInPage)
            flightPoint, CurrentPage, LocationInPage = build_flight_point(data, CurrentPage, pages, LocationInPage)
            continue

        # Sensor point decoding
        if chr(data[CurrentPage][LocationInPage]) == 'S':
            # print('S found, page', CurrentPage, ' ,Location', LocationInPage)
            samplenum += 1

            if pointList.__len__() > 0:
                lastTick = pointList[pointList.__len__() - 1].sampleTick
            else:
                lastTick = 0

            point, CurrentPage, LocationInPage = build_sensor_point(data, CurrentPage, pages, LocationInPage,
                                                                    lastTick)
            if isinstance(point, SensorPointType): pointList.append(point)

            continue

        if chr(data[CurrentPage][LocationInPage]) == 'E':

            if (CurrentPage + 1) >= pages:
                ProcessLog = False
                return 0, CurrentPage + 1, 0
                print('event')

            CurrentPage, LocationInPage, event = build_event_point(data, CurrentPage, pages, LocationInPage)

            eventList.append(event)
            continue


    elapsedTime = (time.clock() - StartTime)
    print('Took:', elapsedTime, 'seconds to read and process', pointList.__len__(), 'data points')

    return pointList, flightPoint, eventList
