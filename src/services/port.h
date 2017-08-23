#ifndef PORT_H_
#define PORT_H_

#include "src/util.h"


#define INPUT 0
#define OUTPUT 1
#define OUTSTRONG 3

static inline PortGroup* getPort(
       const u8_t gpioPin) {
u8_t port_index = (gpio_pin / 128);
ut_t group_index = (gpio_pin / 32);
