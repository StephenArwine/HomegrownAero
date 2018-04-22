import matplotlib.pyplot as plt
from sensorPoint import SensorPointType
from sensorPoint import EventPointType
import numpy


def PlotFlight(pointList, flight, eventList):
    print('Flight Numb:', flight.FlightNumb, 'Buffer Time:', flight.bufferTick, 'Ground Offset:', flight.groundOffset,
          'Ground Temperature', flight.groundTemperature)

    x = 0

    accelplot = []
    heightplot = []
    velocityplot = []
    tick = []
    stdheight = []
    stdaccel = []

    previousPoint = SensorPointType()

    for pointToPlot in pointList:

        if x == 0:

            print('Sample 0 tick:', pointToPlot.sampleTick, 'Height Feet:', 'Velocity:', pointToPlot.velocity,
                  'AccelZ:', pointToPlot.accelZ)

        elif x > 0:

            print('Sample', x, 'tick:', pointToPlot.sampleTick, 'Height Feet:',
                  pointToPlot.heightFeet, 'Velocity:', pointToPlot.velocity, 'AccelX:', pointToPlot.accelX,
                  'AccelZ:', pointToPlot.accelZ, 'AccelY:', pointToPlot.accelY, 'Analog Accel', pointToPlot.analogAccel)
        tick.append(pointToPlot.sampleTick / 1000)
        accelplot.append(pointToPlot.accelX)
        heightplot.append(pointToPlot.heightFeet)
        velocityplot.append(pointToPlot.velocity)
        x = x + 1

        # stdaccel.append(pointToPlot.accelZraw / 32.17417)
        # stdheight.append(pointToPlot.rawFeet)

    # print('accel StdDev ', numpy.std(stdaccel))
    # print('height StdDev ', numpy.std(stdheight))

    fig, ax1 = plt.subplots()
    ax2 = ax1.twinx()
    ax3 = ax2.twinx()

    p1, = ax1.plot(tick, heightplot, color='b')
    ax1.set_xlabel('time (s)')
    ax1.set_ylabel('Height (ft)')
    ax2.set_ylabel('Accel (g)')
    ax3.set_ylabel('Velocity (ft/s)')

    ax2.spines["right"].set_position(("axes", 1))

    ax3.spines["right"].set_position(("axes", 1.1))

    p2, = ax2.plot(tick, accelplot, color='r')
    p3, = ax3.plot(tick, velocityplot, color='g')

    for event in eventList:
        print('event type:', chr(event.eventType), 'event tick:', event.sampleTick, 'event altitude;', event.altitude)
        ax1.axvline(x=(event.sampleTick / 1000))

    ax3.axhline(y=0, color='g', linestyle='--', linewidth=0.5)
    ax2.axhline(y=0, color='r', linestyle='--', linewidth=0.5)

    # make_patch_spines_invisible(ax1)
    # make_patch_spines_invisible(ax2)

    ax1.spines["left"].set_visible(True)
    ax1.yaxis.set_label_position('left')
    ax1.yaxis.set_ticks_position('left')
    ax1.grid(True)

    # ax2.spines["left"].set_visible(True)
    ax2.yaxis.set_label_position('right')
    ax2.yaxis.set_ticks_position('right')
    ax2.set_ylim(-2, 4)

    ax3.yaxis.set_label_position('right')
    ax3.yaxis.set_ticks_position('right')
    ax3.set_ylim(-32, 32)

    tkw = dict(size=4, width=1.5)
    ax1.tick_params(axis='y', colors=p1.get_color(), **tkw)
    ax2.tick_params(axis='y', colors=p2.get_color(), **tkw)
    ax3.tick_params(axis='y', colors=p3.get_color(), **tkw)

    ax1.set_zorder(1)
    ax2.set_zorder(2)
    ax3.set_zorder(3)

    fig.tight_layout()
    plt.show()
