#pragma once

#include <pthread.h>

#define COMN_DAT_LEN 32

#define HALT_STATE 1
#define RUN_STATE 0

extern char halt_system;
extern int comn_dat[COMN_DAT_LEN];

extern pthread_mutex_t comn_mutex;