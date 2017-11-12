
//flight.c

#ifndef FLIGHT_H_
#define FLIGHT_H_



enum flight_state {

    flightStatrup = 0,
    flightIdle = 1,
    flightPad = 2,
    flightBoost = 3,
    flightFast = 4,
    flightCoast = 5,
    flightDrogue = 6,
    flightMain = 7,
    flightLanded = 8,
    flightTest = 10

};

extern enum flight_state my_flight_state;

void flight();


#endif
