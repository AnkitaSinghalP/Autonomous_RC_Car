/**
 * @file
 * @brief Contains the terminal handler function declarations.
 */
#ifndef FREE_RUN_HPP_
#define FREE_RUN_HPP_

#include <stdint.h>
#include "io.hpp"
#include "stdio.h"
#include "timers.h"
#include "utilities.h"
#include "file_logger.h"
#include "can_init.h"
#include "lpc_timers.h"
#include "string.h"

extern bool free_run_init;
void free_run_func(void);

#endif /* HANDLERS_HPP_ */
