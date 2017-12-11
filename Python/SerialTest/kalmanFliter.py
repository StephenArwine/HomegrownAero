from sensorPoint import SensorPointType
import math
import sys


def printf(format, *args):
    sys.stdout.write(format % args)


def filter_data_with_kalman(data):
    argc = 1
    argv = 0

    ALTITUDESIGMA = 5
    ACCELERATIONSIGMA = .2
    MODELSIGMA = .6

    altitude_variance = float(ALTITUDESIGMA * ALTITUDESIGMA)
    acceleration_variance = float(ACCELERATIONSIGMA * ACCELERATIONSIGMA)
    model_variance = float(MODELSIGMA * MODELSIGMA)

    alt_inovation = 0
    accel_inovation = 0
    time = 0
    pressure = 0
    accel = 0
    last_time = 0
    last_pressure = 0
    det = 0
    est = [0, 0, 0]
    estp = [0, 0, 0]
    pest = [[2, 0, 0], [0, 9, 0], [0, 0, 9]]

    pestp = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]

    phi = [[1, 0, 0], [0, 1, 0], [0, 0, 1.0]]

    phit = [[1, 0, 0], [0, 1, 0], [0, 0, 1.0]]

    kgain = [[0.01, 0.01], [0.01, 0.01], [0.01, 0.01]]

    lastkgain = [[0.1, 0.1], [0.1, 0.1], [0.1, 0.1]]

    term = pestp

    # print('time', point.sampleTick, 'accel', point.accelZ, 'alt', point.heightFeet)
    # dt = point.sampleTick - last_time
    # last_time = point.sampleTick
    dt = 0.008

    est[0] = data[0].heightFeet
    last_time = data[0].sampleTick

    # Fill in state transition matrix and its transpose
    phi[0][1] = dt
    phi[1][2] = dt
    phi[0][2] = dt * dt / 2.0
    phi[1][0] = dt
    phi[2][1] = dt
    phi[2][0] = dt * dt / 2.0

    for i in range(0, 3):
        for j in range(0, 2):
            lastkgain[i][j] = 0.001
            kgain[i][j] = 1.0

    k = 0

    while (1):

        # Propagate state covariance

        term[0][0] = phi[0][0] * pest[0][0] + phi[0][1] * pest[1][0] + phi[0][2] * pest[2][0]
        term[0][1] = phi[0][0] * pest[0][1] + phi[0][1] * pest[1][1] + phi[0][2] * pest[2][1]
        term[0][2] = phi[0][0] * pest[0][2] + phi[0][1] * pest[1][2] + phi[0][2] * pest[2][2]
        term[1][0] = phi[1][0] * pest[0][0] + phi[1][1] * pest[1][0] + phi[1][2] * pest[2][0]
        term[1][1] = phi[1][0] * pest[0][1] + phi[1][1] * pest[1][1] + phi[1][2] * pest[2][1]
        term[1][2] = phi[1][0] * pest[0][2] + phi[1][1] * pest[1][2] + phi[1][2] * pest[2][2]
        term[2][0] = phi[2][0] * pest[0][0] + phi[2][1] * pest[1][0] + phi[2][2] * pest[2][0]
        term[2][1] = phi[2][0] * pest[0][1] + phi[2][1] * pest[1][1] + phi[2][2] * pest[2][1]
        term[2][2] = phi[2][0] * pest[0][2] + phi[2][1] * pest[1][2] + phi[2][2] * pest[2][2]

        pestp[0][0] = term[0][0] * phit[0][0] + term[0][1] * phit[1][0] + term[0][2] * phit[2][0]
        pestp[0][1] = term[0][0] * phit[0][1] + term[0][1] * phit[1][1] + term[0][2] * phit[2][1]
        pestp[0][2] = term[0][0] * phit[0][2] + term[0][1] * phit[1][2] + term[0][2] * phit[2][2]
        pestp[1][0] = term[1][0] * phit[0][0] + term[1][1] * phit[1][0] + term[1][2] * phit[2][0]
        pestp[1][1] = term[1][0] * phit[0][1] + term[1][1] * phit[1][1] + term[1][2] * phit[2][1]
        pestp[1][2] = term[1][0] * phit[0][2] + term[1][1] * phit[1][2] + term[1][2] * phit[2][2]
        pestp[2][0] = term[2][0] * phit[0][0] + term[2][1] * phit[1][0] + term[2][2] * phit[2][0]
        pestp[2][1] = term[2][0] * phit[0][1] + term[2][1] * phit[1][1] + term[2][2] * phit[2][1]
        pestp[2][2] = term[2][0] * phit[0][2] + term[2][1] * phit[1][2] + term[2][2] * phit[2][2]

        pestp[2][2] = pestp[2][2] + model_variance

        # Calculate Kalman Gain

        det = (pestp[0][0] + altitude_variance) * (pestp[2][2] + acceleration_variance) - pestp[2][0] * pestp[0][2]

        kgain[0][0] = (pestp[0][0] * (pestp[2][2] + acceleration_variance) - pestp[0][2] * pestp[2][0]) / det

        kgain[0][1] = (pestp[0][0] * (-pestp[0][2]) + pestp[0][2] * (pestp[0][0] + altitude_variance)) / det

        kgain[1][0] = (pestp[1][0] * (pestp[2][2] + acceleration_variance) - pestp[1][2] * pestp[2][0]) / det

        kgain[1][1] = (pestp[1][0] * (-pestp[0][2]) + pestp[1][2] * (pestp[0][0] + altitude_variance)) / det

        kgain[2][0] = (pestp[2][0] * (pestp[2][2] + acceleration_variance) - pestp[2][2] * pestp[2][0]) / det

        kgain[2][1] = (pestp[2][0] * (-pestp[0][2]) + pestp[2][2] * (pestp[0][0] + altitude_variance)) / det

        pest[0][0] = pestp[0][0] * (1.0 - kgain[0][0]) - kgain[0][1] * pestp[2][0]
        pest[0][1] = pestp[0][1] * (1.0 - kgain[0][0]) - kgain[0][1] * pestp[2][1]
        pest[0][2] = pestp[0][2] * (1.0 - kgain[0][0]) - kgain[0][1] * pestp[2][2]
        pest[1][0] = pestp[0][0] * (-kgain[1][0]) + pestp[1][0] - kgain[1][1] * pestp[2][0]
        pest[1][1] = pestp[0][1] * (-kgain[1][0]) + pestp[1][1] - kgain[1][1] * pestp[2][1]
        pest[1][2] = pestp[0][2] * (-kgain[1][0]) + pestp[1][2] - kgain[1][1] * pestp[2][2]
        pest[2][0] = (1.0 - kgain[2][1]) * pestp[2][0] - kgain[2][0] * pestp[2][0]
        pest[2][1] = (1.0 - kgain[2][1]) * pestp[2][1] - kgain[2][0] * pestp[2][1]
        pest[2][2] = (1.0 - kgain[2][1]) * pestp[2][2] - kgain[2][0] * pestp[2][2]

        # Check for convergance.Criteria is less than .001 % change from last time through the mill.

        notDone = 0
        k += 1

        for i in range(0, 3):
            for j in range(0, 2):
                if ((kgain[i][j] - lastkgain[i][j]) / lastkgain[i][j]) > 0.000001:
                    notDone += 1
                lastkgain[i][j] = kgain[i][j]
        if notDone:
            continue
        else:
            break

    print('Input noise values used (standard deviation):')
    print('#Altitude feet', math.sqrt(altitude_variance))
    print('#Acceleration feet/sec/sec', math.sqrt(acceleration_variance))
    print('#Model noise feet/sec/sec', math.sqrt(model_variance))
    print('Kalman gains converged atfter', k, ' iterations.')
    for i in range(0, 3):
        print(kgain[i][0], kgain[i][1])
    print('#Estimated output first order statistics (standard deviation):')
    print('#Altitude feet', math.sqrt(pest[0][0]))
    print('#Acceleration feet/sec/sec', math.sqrt(pest[1][1]))
    print('#Model noise feet/sec/sec', math.sqrt(pest[2][2]))

    print('# Output from rkal32:\n# A third order Kalman filter using acceleration and pressure measurements')
    print('##    Time          Press. Alt.      Acceleration   Est Pos          Est Rate        Est Accel#')





    for point in data:
        accel = point.accelZ * 32.17417

        # Compute the innovations
        alt_inovation = point.heightFeet - estp[0]
        accel_inovation = accel - estp[2]

        # Propagate state
        estp[0] = phi[0][0] * est[0] + phi[0][1] * est[1] + phi[0][2] * est[2]
        estp[1] = phi[1][0] * est[0] + phi[1][1] * est[1] + phi[1][2] * est[2]
        estp[2] = phi[2][0] * est[0] + phi[2][1] * est[1] + phi[2][2] * est[2]

        # Update state
        est[0] = estp[0] + kgain[0][0] * alt_inovation + kgain[0][1] * accel_inovation
        est[1] = estp[1] + kgain[1][0] * alt_inovation + kgain[1][1] * accel_inovation
        est[2] = estp[2] + kgain[2][0] * alt_inovation + kgain[2][1] * accel_inovation

        # Output

        printf("%15f %15f %15f %15f %15f %15f\n", point.sampleTick, point.heightFeet, accel, est[0], est[1], est[2])
        last_time = point.sampleTick
