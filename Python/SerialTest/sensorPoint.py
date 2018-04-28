import twosComp
from twosComp import twos_complement
import time

sensorPointLength = 35
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
        self.altitudeMSL = 0
        self.kalmanAltitude = 0
        self.velocity = 0
        self.velocityfract = 0
        self.accelX = 0
        self.accelY = 0
        self.accelZ = 0
        self.kalmanAccel = 0
        self.kalmanAccelFract = 0
        self.kalmanAccelRaw = 0
        self.kalmanAccelRawFract = 0
        self.gyroX = 0
        self.gyroY = 0
        self.gyroZ = 0
        self.analogRaw = 0
        self.analogAccel = 0
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

    if (locationInPage + sensorPointLength) >= 256:

        if (currentPage + 1) >= pages:
            ProcessLog = False
            return 0, currentPage + 1, 0

        sensor_sample_part = data[currentPage][locationInPage:256]
        sensor_sample = sensor_sample_part + data[currentPage + 1][0:(sensorPointLength - (256 - locationInPage))]
        currentPage += 1
        locationInPage -= (0xff - sensorPointLength + 1 )

    else:
        sensor_sample = data[currentPage][locationInPage:locationInPage + sensorPointLength]
        locationInPage += sensorPointLength

    point.sampleTick = int.from_bytes(sensor_sample[1:4], byteorder='little')
    point.Dt = point.sampleTick - lastTick
    point.kalmanAltitude = int.from_bytes(sensor_sample[5:8], byteorder='little', signed=True)

    point.kalmanAccel = twosComp.twos_complement(sensor_sample[9], sensor_sample[10])
    point.kalmanAccelraw = twosComp.twos_complement(sensor_sample[9], sensor_sample[10])
    point.kalmanAccelRawFract = twosComp.twos_complement(sensor_sample[11], sensor_sample[12])
    point.kalmanAccelFract = point.kalmanAccelRawFract / 1000
    point.kalmanAccel = (point.kalmanAccel + point.kalmanAccelFract + 32.17417) / 32.17417

    point.velocity = twosComp.twos_complement(sensor_sample[13], sensor_sample[14])

    point.velocityfract = twosComp.twos_complement(sensor_sample[15], sensor_sample[16])
    point.velocityfract = point.velocityfract / 1000
    point.velocity = point.velocity + point.velocityfract

    point.altitudeMSL = int.from_bytes(sensor_sample[17:20], byteorder='little', signed=True)

    point.accelX = (twos_complement(sensor_sample[21],sensor_sample[22])) * 0.0078125
    point.accelY = (twos_complement(sensor_sample[23],sensor_sample[24])) * 0.0078125
    point.accelZ = (twos_complement(sensor_sample[25], sensor_sample[26])) * 0.0078125

    point.gyroX = (twos_complement(sensor_sample[27], sensor_sample[28]) >> 4) * 0.0078125
    point.gyroY = (twos_complement(sensor_sample[29], sensor_sample[30]) >> 4) * 0.0078125
    point.gyroZ = (twos_complement(sensor_sample[31], sensor_sample[32]) >> 4) * 0.0078125

    point.analogRaw = sensor_sample[33] + (sensor_sample[34] << 8)

    point.analogAccel = (point.analogRaw - 48695) * 0.00487

    return point, currentPage, locationInPage


def build_event_point(data,currentPage, pages, locationInPage):

    event = EventPointType()

    if (locationInPage + eventSampleLength) >= 256:

        if (currentPage + 1) >= pages:
            ProcessLog = False
            return 0, currentPage + 1, 0

        sensor_sample_part = data[currentPage][locationInPage:256]
        eventSample = sensor_sample_part + data[currentPage + 1][0:(eventSampleLength - (256 - locationInPage))]
        currentPage += 1
        locationInPage -= (0xff - eventSampleLength + 1 )

    else:
        eventSample = data[currentPage][locationInPage:locationInPage + eventSampleLength]
        locationInPage += eventSampleLength


    event.eventType = eventSample[1]
    event.sampleTick = int.from_bytes(eventSample[2:5], byteorder='little')
    event.altitude = int.from_bytes(eventSample[6:9], byteorder='little', signed=True)

    return currentPage, locationInPage, event


def processDataLog(data, pages, StartTime):
    pointList = []
    eventList = []
    CurrentPage = 0
    LocationInPage = 0
    ProcessLog = True

    samplenum = 0

    print('Pages: ', pages)

    while ProcessLog:

        if CurrentPage >= pages:
            ProcessLog = False
            break

        #print(chr(data[CurrentPage][LocationInPage]), CurrentPage, LocationInPage)

        # Flight point decode
        if chr(data[CurrentPage][LocationInPage]) == 'F':
            #print('F found, location', LocationInPage)
            flightPoint, CurrentPage, LocationInPage = build_flight_point(data, CurrentPage, pages, LocationInPage)
            continue

        # Sensor point decoding
        if chr(data[CurrentPage][LocationInPage]) == 'S':
            #print('S found, page', CurrentPage, ' ,Location', LocationInPage)
            samplenum += 1

            if pointList.__len__() > 0:
                lastTick = pointList[pointList.__len__() - 1].sampleTick
            else:
                lastTick = 0

            point, CurrentPage, LocationInPage = build_sensor_point(data, CurrentPage, pages, LocationInPage,
                                                                    lastTick)
            if isinstance(point, SensorPointType):
                pointList.append(point)

                #print('tick:', point.sampleTick, 'Sample DT:', point.Dt, 'Height Feet:',
                #    point.heightFeet, 'Velocity:', point.velocity, 'AccelX:', point.accelX,
                #    'AccelZ:', point.accelZ, 'AccelY:', point.accelY, 'Analog Accel', point.analogAccel, ' ', CurrentPage,' ',LocationInPage)

            continue

        if chr(data[CurrentPage][LocationInPage]) == 'E':

            if (CurrentPage + 1) >= pages:
                ProcessLog = False
                return 0, CurrentPage + 1, 0
                #print('event')

            CurrentPage, LocationInPage, event = build_event_point(data, CurrentPage, pages, LocationInPage)

            eventList.append(event)
            continue


    elapsedTime = (time.clock() - StartTime)
    print('Took:', elapsedTime, 'seconds to read and process', pointList.__len__(), 'data points')

    return pointList, flightPoint, eventList
