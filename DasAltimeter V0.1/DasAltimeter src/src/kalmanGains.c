

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <util.h>

/* default values */

#define  ALTITUDESIGMA	       5.0
#define  ACCELERATIONSIGMA      2.0
#define  MODELSIGMA		0.6

double	phi[3][3]     = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1.0}
};

double altitude_variance = ALTITUDESIGMA*ALTITUDESIGMA;
double acceleration_variance = ACCELERATIONSIGMA*ACCELERATIONSIGMA;
double model_variance  = MODELSIGMA*MODELSIGMA;

void computeKalmanGains(kalmanFilter *my_kalmanFilter) {
    int     i, j, k, notdone;

    double	pest[3][3]    = {
        {2, 0, 0},
        {0, 9, 0},
        {0, 0, 9}
    };
    double  pestp[3][3]   = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    double  phit[3][3]    = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1.0}
    };
    double  kgain[3][2], lastkgain[3][2];
    double  dt, det;
    double	term[3][3];

    k = 0;

    /* Initialize */

    dt = 0.008;

    /*
     Fill in state transition matrix and its transpose
    */
    phi[0][1]    = dt;
    phi[1][2]    = dt;
    phi[0][2]    = dt*dt/2.0;
    phit[1][0]    = dt;
    phit[2][1]    = dt;
    phit[2][0]    = dt*dt/2.0;



    for( i = 0; i <= 2; i++)
        for( j = 0; j <=1; j++) {
            lastkgain[i][j] = .001;
            kgain[i][j] = 1.0;
        }
    k = 0;


    /* Compute the Kalman gain matrix. */
    while(1) {
        /* Propagate state covariance */

        term[0][0] = phi[0][0] * pest[0][0] + phi[0][1] * pest[1][0] + phi[0][2] * pest[2][0];
        term[0][1] = phi[0][0] * pest[0][1] + phi[0][1] * pest[1][1] + phi[0][2] * pest[2][1];
        term[0][2] = phi[0][0] * pest[0][2] + phi[0][1] * pest[1][2] + phi[0][2] * pest[2][2];
        term[1][0] = phi[1][0] * pest[0][0] + phi[1][1] * pest[1][0] + phi[1][2] * pest[2][0];
        term[1][1] = phi[1][0] * pest[0][1] + phi[1][1] * pest[1][1] + phi[1][2] * pest[2][1];
        term[1][2] = phi[1][0] * pest[0][2] + phi[1][1] * pest[1][2] + phi[1][2] * pest[2][2];
        term[2][0] = phi[2][0] * pest[0][0] + phi[2][1] * pest[1][0] + phi[2][2] * pest[2][0];
        term[2][1] = phi[2][0] * pest[0][1] + phi[2][1] * pest[1][1] + phi[2][2] * pest[2][1];
        term[2][2] = phi[2][0] * pest[0][2] + phi[2][1] * pest[1][2] + phi[2][2] * pest[2][2];

        pestp[0][0] = term[0][0] * phit[0][0] + term[0][1] * phit[1][0] + term[0][2] * phit[2][0];
        pestp[0][1] = term[0][0] * phit[0][1] + term[0][1] * phit[1][1] + term[0][2] * phit[2][1];
        pestp[0][2] = term[0][0] * phit[0][2] + term[0][1] * phit[1][2] + term[0][2] * phit[2][2];
        pestp[1][0] = term[1][0] * phit[0][0] + term[1][1] * phit[1][0] + term[1][2] * phit[2][0];
        pestp[1][1] = term[1][0] * phit[0][1] + term[1][1] * phit[1][1] + term[1][2] * phit[2][1];
        pestp[1][2] = term[1][0] * phit[0][2] + term[1][1] * phit[1][2] + term[1][2] * phit[2][2];
        pestp[2][0] = term[2][0] * phit[0][0] + term[2][1] * phit[1][0] + term[2][2] * phit[2][0];
        pestp[2][1] = term[2][0] * phit[0][1] + term[2][1] * phit[1][1] + term[2][2] * phit[2][1];
        pestp[2][2] = term[2][0] * phit[0][2] + term[2][1] * phit[1][2] + term[2][2] * phit[2][2];

        pestp[2][2] = pestp[2][2] + model_variance;
        /*
          Calculate Kalman Gain
        */
        det = (pestp[0][0]+altitude_variance)*(pestp[2][2] + acceleration_variance) - pestp[2][0] * pestp[0][2];


        kgain[0][0] = (pestp[0][0] * (pestp[2][2] + acceleration_variance) - pestp[0][2] * pestp[2][0])/det;

        kgain[0][1] = (pestp[0][0] * (-pestp[0][2]) + pestp[0][2] * (pestp[0][0] + altitude_variance))/det;

        kgain[1][0] = (pestp[1][0] * (pestp[2][2] + acceleration_variance) - pestp[1][2] * pestp[2][0])/det;

        kgain[1][1] = (pestp[1][0] * (-pestp[0][2]) + pestp[1][2] * (pestp[0][0] + altitude_variance))/det;

        kgain[2][0] = (pestp[2][0] * (pestp[2][2] + acceleration_variance) - pestp[2][2] * pestp[2][0])/det;

        kgain[2][1] = (pestp[2][0] * (-pestp[0][2]) + pestp[2][2] * (pestp[0][0] + altitude_variance))/det;

        pest[0][0] = pestp[0][0] * (1.0 - kgain[0][0]) - kgain[0][1]*pestp[2][0];
        pest[0][1] = pestp[0][1] * (1.0 - kgain[0][0]) - kgain[0][1]*pestp[2][1];
        pest[0][2] = pestp[0][2] * (1.0 - kgain[0][0]) - kgain[0][1]*pestp[2][2];
        pest[1][0] = pestp[0][0] * (-kgain[1][0]) + pestp[1][0] - kgain[1][1]*pestp[2][0];
        pest[1][1] = pestp[0][1] * (-kgain[1][0]) + pestp[1][1] - kgain[1][1]*pestp[2][1];
        pest[1][2] = pestp[0][2] * (-kgain[1][0]) + pestp[1][2] - kgain[1][1]*pestp[2][2];
        pest[2][0] = (1.0 - kgain[2][1]) * pestp[2][0] - kgain[2][0] * pestp[2][0];
        pest[2][1] = (1.0 - kgain[2][1]) * pestp[2][1] - kgain[2][0] * pestp[2][1];
        pest[2][2] = (1.0 - kgain[2][1]) * pestp[2][2] - kgain[2][0] * pestp[2][2];

        /* Check for convergance. Criteria is less than .001% change from last
         * time through the mill. Because I have noticed very early convergance
         * in a few cases (< 20 iterations) I am delaying this for at least
         * 100 iterations.
         */
        notdone = 0;
        k++;
        for( i = 0; i <= 2; i++)
            for( j = 0; j <= 1; j++) {
                if( (kgain[i][j] - lastkgain[i][j])/lastkgain[i][j] > 0.00001)
                    notdone++;
                lastkgain[i][j] = kgain[i][j];
            }
        if( notdone || k < 100)
            continue;
        else
            break;
    }

    for( i = 0; i <= 2; i++) {
        for( j = 0; j <=1; j++) {
            my_kalmanFilter->kgain[i][j] = kgain[i][j];
        }
    }
}

void computeKalmanStates(Altimeter *my_altimeter) {

    double alt_inovation, accel_inovation;


    double accel = (my_altimeter->myIMU.accelZ - my_altimeter->myIMU.gravityOffset) * 32.17417;
    double pressure = my_altimeter->myBarometer.altitudefeet;

    if (my_altimeter->myKalmanFilter.est[0] == 0) {
        my_altimeter->myKalmanFilter.est[0] = pressure;
    }

    /* Compute the innovations */
    alt_inovation = pressure - my_altimeter->myKalmanFilter.estp[0];
    accel_inovation = accel - my_altimeter->myKalmanFilter.estp[2];


    /* Propagate state */
    my_altimeter->myKalmanFilter.estp[0] = phi[0][0] * my_altimeter->myKalmanFilter.est[0] + phi[0][1] * my_altimeter->myKalmanFilter.est[1] + phi[0][2] * my_altimeter->myKalmanFilter.est[2];
    my_altimeter->myKalmanFilter.estp[1] = phi[1][0] * my_altimeter->myKalmanFilter.est[0] + phi[1][1] * my_altimeter->myKalmanFilter.est[1] + phi[1][2] * my_altimeter->myKalmanFilter.est[2];
    my_altimeter->myKalmanFilter.estp[2] = phi[2][0] * my_altimeter->myKalmanFilter.est[0] + phi[2][1] * my_altimeter->myKalmanFilter.est[1] + phi[2][2] * my_altimeter->myKalmanFilter.est[2];

    /*
    Update state
    */
    my_altimeter->myKalmanFilter.est[0] = my_altimeter->myKalmanFilter.estp[0] + my_altimeter->myKalmanFilter.kgain[0][0] * alt_inovation + my_altimeter->myKalmanFilter.kgain[0][1] * accel_inovation;
    my_altimeter->myKalmanFilter.est[1] = my_altimeter->myKalmanFilter.estp[1] + my_altimeter->myKalmanFilter.kgain[1][0] * alt_inovation + my_altimeter->myKalmanFilter.kgain[1][1] * accel_inovation;
    my_altimeter->myKalmanFilter.est[2] = my_altimeter->myKalmanFilter.estp[2] + my_altimeter->myKalmanFilter.kgain[2][0] * alt_inovation + my_altimeter->myKalmanFilter.kgain[2][1] * accel_inovation;

    my_altimeter->Acceleration = my_altimeter->myKalmanFilter.est[2];
    my_altimeter->Velocity = my_altimeter->myKalmanFilter.est[1];
    my_altimeter->Altitude = my_altimeter->myKalmanFilter.est[0];


}

