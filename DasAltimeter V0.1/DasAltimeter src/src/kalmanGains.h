
#include <util.h>

double	phi[3][3]     = { 1, 0, 0,
                              0, 1, 0,
                              0, 0, 1.0
                           };


void computeKalmanGains(kalmanFilter *my_kalmanFilter);


void computeKalmanStates(Altimeter *my_altimeter);
