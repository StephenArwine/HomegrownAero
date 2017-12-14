import twosComp
from twosComp import twos_complement

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
        self.heightCM = 0
        self.heightFeet = 0
        self.accelX = 0
        self.accelY = 0
        self.accelZ = 0
        self.accelZFract = 0
        self.gyroX = 0
        self.gyroY = 0
        self.gyroZ = 0
        self.analogRaw = 0


def build_flight_point(data, currentPage, pages, locationInPage):

	if (LocationInPage + 17) >= 255:

		if (CurrentPage + 1) >= pages:
			ProcessLog = False
			break

		sensor_sample_part = data[CurrentPage][LocationInPage:255]
		sensor_sample = sensor_sample_part + data[CurrentPage + 1][0:(16 - (256 - LocationInPage))]

		point = FlightPoint()
		point.FlightNumb = sensor_sample[1]
		point.bufferTick = int.from_bytes(sensor_sample[2:5], byteorder='little')
		point.groundOffset = int.from_bytes(sensor_sample[6:9], byteorder='little')
		point.groundTemperature = int.from_bytes(sensor_sample[10:13], byteorder='little')
		point.groundAccel = twosComp.twos_complement(sensor_sample[14], sensor_sample[15]) * 0.0078125

		CurrentPage += 1
		LocationInPage -= 240  # rollover + 16

	else:
		sensor_sample = data[CurrentPage][LocationInPage:LocationInPage + 16]

		point = FlightPoint()
		point.FlightNumb = sensor_sample[1]
		point.bufferTick = int.from_bytes(sensor_sample[2:5], byteorder='little')
		point.groundOffset = int.from_bytes(sensor_sample[6:9], byteorder='little')
		point.groundTemperature = int.from_bytes(sensor_sample[10:13], byteorder='little')
		point.groundAccel = twosComp.twos_complement(sensor_sample[14], sensor_sample[15]) * 0.0078125

		LocationInPage += 17
		
	return point	

def build_sensor_point(data, currentPage, pages, locationInPage, lastTick):

    point = SensorPointType()

    if (locationInPage + 24) > 255:

        if (currentPage + 1) >= pages:
            ProcessLog = False
            return 0, currentPage + 1, 0

        sensor_sample_part = data[currentPage][locationInPage:255]
        sensor_sample = sensor_sample_part + data[currentPage + 1][0:(23 - (256 - locationInPage))]

        point.sampleTick = int.from_bytes(sensor_sample[1:4], byteorder='little')
        point.Dt = point.sampleTick - lastTick
        point.heightFeet = int.from_bytes(sensor_sample[5:8], byteorder='little')

        point.accelX = twos_complement(sensor_sample[9], sensor_sample[10]) * 0.0078125  # Accel X conv
        point.accelY = twos_complement(sensor_sample[11], sensor_sample[12]) * 0.0078125  # Accel Y conv
        point.accelZFract = twos_complement(sensor_sample[13], sensor_sample[14])
        #point.accelZFract = int.from_bytes(sensor_sample[13:14], byteorder='little', signed=True)
        point.accelZFract = point.accelZFract / 1000
        point.accelZ = twos_complement(sensor_sample[15], sensor_sample[16])
        point.accelZ = point.accelZ + point.accelZFract


        point.gyroY = twos_complement(sensor_sample[17], sensor_sample[18]) * 0.0078125  # Gyro Y conv
        point.gyroZ = twos_complement(sensor_sample[19], sensor_sample[20]) * 0.0078125  # Gyro Z conv

        currentPage += 1
        locationInPage -= 232  # rollover + 24

        #print('Reading page ', currentPage, ' starting Location ', locationInPage, 'point',
        #      hex(data[currentPage][locationInPage]))

    else:
        sensor_sample = data[currentPage][locationInPage:locationInPage + 23]

        point.sampleTick = int.from_bytes(sensor_sample[1:4], byteorder='little')
        point.Dt = point.sampleTick - lastTick

        point.heightFeet = int.from_bytes(sensor_sample[5:8], byteorder='little')

        point.accelX = twos_complement(sensor_sample[9], sensor_sample[10]) * 0.0078125  # Accel X conv
        point.accelY = twos_complement(sensor_sample[11], sensor_sample[12]) * 0.0078125  # Accel Y conv
        point.accelZFract = twos_complement(sensor_sample[13], sensor_sample[14])
        #point.accelZFract = int.from_bytes(sensor_sample[13:14], byteorder='little', signed=True)
        point.accelZFract = point.accelZFract / 1000
        point.accelZ = twos_complement(sensor_sample[15], sensor_sample[16])
        point.accelZ = point.accelZ + point.accelZFract


        point.gyroY = twos_complement(sensor_sample[17], sensor_sample[18]) * 0.0078125  # Accel Z conv
        point.gyroZ = twos_complement(sensor_sample[19], sensor_sample[20]) * 0.0078125  # Accel Z conv

        locationInPage += 24

    return point, currentPage, locationInPage

	
	
def processDataLog(data, pages):
	
	CurrentPage = 0
	LocationInPage = 0
	ProcessLog = True

	samplenum = 0

	while ProcessLog:

		if CurrentPage >= pages:
			ProcessLog = False
			break

		# Flight point decode
		if data[CurrentPage][LocationInPage] == 0x46:
			# print('F found, location', LocationInPage)
			flightPoint = build_flight_point(data, CurrentPage, pages, LocationInPage)
			

		# Sensor point decoding
		if data[CurrentPage][LocationInPage] == 0x41:
			#print('A found, page', CurrentPage, ' ,Location', LocationInPage)
			samplenum += 1

			if pointList.__len__() > 0:
				lastTick = pointList[pointList.__len__() - 1].sampleTick
			else:
				lastTick = 0

			point, CurrentPage, LocationInPage = sensorPoint.build_sensor_point(data, CurrentPage, pages, LocationInPage,
                                                                            lastTick)
			if isinstance(point, SensorPointType): pointList.append(point)



		elapsedTime = (time.clock() - StartTime)
		print('Took:', elapsedTime, 'seconds to read and process', pointList.__len__(), 'data points')
		
	return pointList
