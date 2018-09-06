#ifndef FX2LAFW_INCLUDE_GPIF_ACQUISITION_H
#define FX2LAFW_INCLUDE_GPIF_ACQUISITION_H

#include <stdbool.h>
#include <command.h>

void gpif_init_la(void);
bool gpif_acquisition_start(const struct cmd_start_acquisition *cmd);
void gpif_poll(void);

#endif
