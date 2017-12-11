import twosComp
from twosComp import twos_complement


class SensorPointType:
    def __init__(self):
        self.sampleTick = 0
        self.Dt = 0
        self.heightCM = 0
        self.heightFeet = 0
        self.accelX = 0
        self.accelY = 0
        self.accelZ = 0
        self.gyroX = 0
        self.gyroY = 0
        self.gyroZ = 0
        self.analogRaw = 0


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
        point.accelZ = int.from_bytes(sensor_sample(13:14), byteorder='little', signed=True)
        #point.accelZ = twos_complement(sensor_sample[13], sensor_sample[14])  # Accel Z conv

        point.gyroX = twos_complement(sensor_sample[15], sensor_sample[16]) * 0.0078125  # Gyro X conv
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
        point.accelZ = int.from_bytes(sensor_sample(13:14), byteorder='little', signed=True)
        #point.accelZ = twos_complement(sensor_sample[13], sensor_sample[14])  # Accel Z conv

        point.gyroX = twos_complement(sensor_sample[15], sensor_sample[16]) * 0.0078125  # Accel Z conv
        point.gyroY = twos_complement(sensor_sample[17], sensor_sample[18]) * 0.0078125  # Accel Z conv
        point.gyroZ = twos_complement(sensor_sample[19], sensor_sample[20]) * 0.0078125  # Accel Z conv

        locationInPage += 24

    return point, currentPage, locationInPage
