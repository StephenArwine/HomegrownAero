import matplotlib.pyplot as plt
from sensorPoint import SensorPointType


def PlotFlight(pointList, flight):
    print('Flight Numb:', flight.FlightNumb, 'Buffer Time:', flight.bufferTick, 'Ground Offset:', flight.groundOffset,
          'Ground Temperature', flight.groundTemperature)

    x = 0

    accelplot = []
    heightplot = []
    tick = []

    previousPoint = SensorPointType()

    for pointToPlot in pointList:

        pointToPlot.sampleTick = pointToPlot.sampleTick - flight.bufferTick

        if x == 0:

            print('Sample 0 tick:', pointToPlot.sampleTick, 'Height Feet:', pointToPlot.heightFeet, 'AccelX:',
                  pointToPlot.accelX, 'AccelY:', pointToPlot.accelY, 'AccelZ:', pointToPlot.accelZ)

        elif x > 0:

            dt = pointToPlot.sampleTick - previousPoint.sampleTick
            print('Sample', x, 'tick:', pointToPlot.sampleTick, 'Sample DT:', pointToPlot.Dt, 'Height Feet:',
                  pointToPlot.heightFeet,
                  'AccelX:',
                  pointToPlot.accelX, 'AccelY:', pointToPlot.accelY, 'AccelZ:', pointToPlot.accelZ)

        tick.append(pointToPlot.sampleTick / 1000)
        accelplot.append(pointToPlot.accelZ / 32.17417)
        heightplot.append(pointToPlot.heightFeet)

        previousPoint = pointToPlot
        x += 1

        #stdaccel.append(pointToPlot.accelZ)
        #stdheight.append(pointToPlot.heightFeet)

    #print('accel StdDev ', numpy.std(stdaccel))
    #print('height StdDev ', numpy.std(stdheight))

    fig, ax1 = plt.subplots()
    ax2 = ax1.twinx()

    p1, = ax1.plot(tick, heightplot, color='b')
    ax1.set_xlabel('time (s)')
    ax1.set_ylabel('Height (ft)')
    ax2.set_ylabel('Accel (g)')

    ax2.spines["left"].set_position(("axes", -0.1))
    # ax1.spines["left"].set_position(("axes", -0.2))


    p2, = ax2.plot(tick, accelplot, color='r')

    # make_patch_spines_invisible(ax1)
    # make_patch_spines_invisible(ax2)

    ax1.spines["left"].set_visible(True)
    ax1.yaxis.set_label_position('left')
    ax1.yaxis.set_ticks_position('left')

    # ax2.spines["left"].set_visible(True)
    ax2.yaxis.set_label_position('left')
    ax2.yaxis.set_ticks_position('left')
    ax2.set_ylim(-.5, 2)

    tkw = dict(size=4, width=1.5)
    ax1.tick_params(axis='y', colors=p1.get_color(), **tkw)
    ax2.tick_params(axis='y', colors=p2.get_color(), **tkw)

    ax1.set_zorder(1)
    ax2.set_zorder(2)

    fig.tight_layout()
    plt.show()
    print('')
    input('press ENTER to close')
