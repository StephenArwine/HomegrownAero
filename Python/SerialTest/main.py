import serial
import twosComp
import sensorPoint
import time
import matplotlib.pyplot as plt
from kalmanFliter import filter_data_with_kalman
from flightPlotter import PlotFlight
import numpy
from sensorPoint import processDataLog
import csv
from os import walk


# from sensorPoint import build_sensor_point
# import sensorPoint

from sensorPoint import SensorPointType
from sensorPoint import FlightPointType

ser = serial.Serial()
ser.baudrate = 115200
ser.port = 'COM3'
ser.timeout = 5


def make_patch_spines_invisible(ax):
    ax.set_frame_on(True)
    ax.patch.set_visible(False)
    for sp in ax.spines.values():
        sp.set_visible(False)


#pointList = []
#eventList = []
#flight = FlightPointType()

data = []

sensor_sample = 0
sensor_sample_part = 0


def saveFlightData(data):
    f = []
    for (dirpath, dirnames, filenames) in walk('logs/'):
        f.extend(filenames)
        break

    print(filenames)

    myFlile = open(str('logs/flightLog_' + time.strftime('%b%d%Y_%H%M', time.localtime()) + '.csv'), 'w', newline='')

    with myFlile:
        writer = csv.writer(myFlile)

        for page in data:
            writer.writerow(page)

    print('done')


def DownloadFlightData():
    ser.write(b'L')

    print(' Which flight log would you like to download?')

    allFlightsNotFound = 1
    while allFlightsNotFound:
        flightNumb = ser.read(1)

        if flightNumb == b'\x0F':
            break
        else:
            print('    ', flightNumb.decode('utf-8'))

    print('')
    flightToRead = input(' ')
    print('')
    ser.write(flightToRead.encode('utf-8'))

    StartTime = time.clock()

    pages_to_read = ser.read(2)
    pages = (pages_to_read[0] << 0) + (pages_to_read[1] << 8)
    print('Reading', ((pages * 256) / 1000), 'kilobytes of data.')

    flightEndingAddressbytes = ser.read(3)
    flightEndingAddress = (flightEndingAddressbytes[0] << 0) + (flightEndingAddressbytes[1] << 8) + (
        flightEndingAddressbytes[2] << 16)

    memoryPercent = (flightEndingAddress / 1048575) * 100
    print('Memory is {0:2.2f}% full.'.format(memoryPercent))

    for page in range(0, pages):
        PageOfData = []
        PageOfData = ser.read(256)
        data.append(PageOfData)

    ser.close()

    pointList, flight, eventList = processDataLog(data, pages, StartTime)
    PlotFlight(pointList, flight, eventList)

    print('Would you like to save this flight data? [Y/N]')

    savechoice = input(' ').upper()

    if savechoice == 'Y':
        saveFlightData(data)


def EraseChip():
    ser.write(b'E')
    print('')
    print(' Please wait.')

    ser.timeout = None
    isWriteDone = ser.read(1)
    if isWriteDone == b'E':
        print('')
        print('Chip erased')
    ser.close()


def LoadSavedData():

    print(' loading data')
    data = []

    with open('logs/flightLog.csv', newline='') as myFile:

        dataIn = csv.reader(myFile)

        for row in dataIn:
            pointLocation = 0
            for point in row:
                row[pointLocation] = int(point)
                pointLocation += 1

            data.append(row)

    StartTime = time.clock()

    pointList, flight, eventList = processDataLog(data, data.__len__(), StartTime)
    PlotFlight(pointList, flight, eventList)



def resetAltimeter():
    print(' Attempting reset')
    ser.open()
    ser.write(b'R')
    ser.close()


def connectToAltimeter():
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
            EraseChip()

        if option == 'L':
            DownloadFlightData()


running = 1
while running:

    data.clear()

    print(' ')
    print(' What would you like to do?')
    print('     L    Load saved Flight Data.')
    print('     R    Reset Altimeter.')
    print('     C    Connect to Altimeter.')
    print(' ')

    option = input(' ').upper()
    print(' ')

    if option == 'L':
        LoadSavedData()

    if option == 'R':
        resetAltimeter()

    if option == 'C':
        connectToAltimeter()
