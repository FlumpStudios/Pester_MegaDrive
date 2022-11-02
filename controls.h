#ifndef CONTROLS_H
#define CONTROLS_H

#include "common.h"
void handleInput(u16 joy, u16 changed, u16 state);
void CTR_set_locked_controls(bool locked);
#endif