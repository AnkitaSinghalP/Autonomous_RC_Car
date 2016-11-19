/*
 * heartbeat.h
 *
 *  Created on: Nov 15, 2016
 *      Author: ankita
 */

#ifndef L5_APPLICATION_HEARTBEAT_HPP_
#define L5_APPLICATION_HEARTBEAT_HPP_

#include <stdint.h>
#include "io.hpp"
#include "stdio.h"
#include "timers.h"
#include "utilities.h"
#include "file_logger.h"
#include "can_init.h"
#include "lpc_timers.h"
#include "string.h"

void heartbeat_rx();

#endif /* L5_APPLICATION_HEARTBEAT_HPP_ */
