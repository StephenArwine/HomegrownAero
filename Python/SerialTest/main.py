import os
import serial
import serial.tools.list_ports
import twosComp
import sensorPoint
import time
import matplotlib.pyplot as plt
import sys
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QPushButton
from kalmanFliter import filter_data_with_kalman
from flightPlotter import PlotFlight
import numpy
from sensorPoint import processDataLog
import csv
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
import random


from os import walk


# from sensorPoint import build_sensor_point
# import sensorPoint

from sensorPoint import SensorPointType
from sensorPoint import FlightPointType

ser = serial.Serial()
ser.baudrate = 115200
ser.port = 'COM9'
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


    myFlile = open(str('logs/log_' + time.strftime('%b%d%Y_%H%M', time.localtime()) + '.csv'), 'w', newline='')

    with myFlile:
        writer = csv.writer(myFlile)

        for page in data:
            writer.writerow(page)

    print('done')

def setPort():
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        print(p)


def DownloadFlightData():
    ser.write(b'L')

    print(' Which flight log would you like to download?')

    allFlightsNotFound = 1
    while allFlightsNotFound:
        flightNumb = ser.read(1)

        if flightNumb == b'\xFF':
            break
        else:
            print('    ', int.from_bytes(flightNumb, byteorder= 'big'))

    print('')
    flightToRead = input(' ')
    print('')
    ser.write(int(flightToRead).to_bytes(1, byteorder='big'))

    StartTime = time.time()

    pages_to_read = ser.read(2)
    pages = (pages_to_read[0] << 0) + (pages_to_read[1] << 8)
    print('Reading', ((pages * 256) / 1000), 'kilobytes of data.')

    flightEndingAddressbytes = ser.read(3)
    flightEndingAddress = (flightEndingAddressbytes[0] << 0) + (flightEndingAddressbytes[1] << 8) + (
        flightEndingAddressbytes[2] << 16)

    memoryPercent = (flightEndingAddress / 2097151) * 100
    print('Memory is {0:2.2f}% full.'.format(memoryPercent))

    for page in range(0, pages):
        PageOfData = []
        PageOfData = ser.read(256)
        data.append(PageOfData)
        #print(PageOfData)

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


    print('\n')
    print(' Which file would you like to Load?')
    data = []

    f = []
    for (dirpath, dirnames, filenames) in walk('logs/'):
        f.extend(filenames)
        break

    for names in filenames:
        print(names)

    toLoad = input('Load : ')

    try:
        with open(('logs/' + toLoad), newline='') as myFile:

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

    except IOError:
        os.system('cls' if os.name == 'nt' else 'clear')
        print('\n')
        print('File not found, check file name')
        LoadSavedData()
    except:
        print('generic error')


def resetAltimeter():
    print(' Attempting reset')
    try:
        ser.open()
        ser.write(b'R')
        ser.close()
    except serial.SerialException as e:
        print('Serial port could not be opened, check connection')

def steamSensorData()
    acknowledge = ser.read(1)

    if acknowledge == b'R':
        


def connectToAltimeter():
    input(' Wait for startup beep and press ENTER')

    ser.open()

    ser.write(b'H')

    handshake = ser.read(1)

    if handshake == b'H':
        print('')
        print(' Altimeter connected, what would you like to do?')
        print('     P    Print/stream sensor data')
        print('     L    Download & Plot flight log')
        print('     S    Change Altimeter settings')
        print('     E    Chip Erase')
        print('')

        option = input('Input : ').upper()

        print('')

        if option == 'E':
            EraseChip()

        if option == 'L':
            DownloadFlightData()

        if option == 'P':
            steamSensorData()


running = 1
while running:

    data.clear()

    print(' ')
    print(' What would you like to do?')
    print('     L    Load saved Flight Data.')
    print('     R    Reset Altimeter.')
    print('     C    Connect to Altimeter.')
    print('     S    Change Serial Port.')
    print(' ')

    option = input('Input : ').upper()
    print(' ')

    if option == 'L':
        os.system('cls' if os.name == 'nt' else 'clear')
        LoadSavedData()

    if option == 'R':
        resetAltimeter()

    if option == 'C':
        os.system('cls' if os.name == 'nt' else 'clear')
        connectToAltimeter()

    if option == 'S':
        os.system('cls' if os.name == 'nt' else 'clear')
        setPort()
