#ifndef FX2LAFW_INCLUDE_FX2LAFW_H
#define FX2LAFW_INCLUDE_FX2LAFW_H

#include <autovector.h>

#define SYNCDELAY() SYNCDELAY4

/*
 * Major and minor fx2lafw firmware version numbers.
 * These can be queried by the host via CMD_GET_FW_VERSION.
 *
 * The minor version number must be increased every time there are
 * backwards-compatible changes (which do not change the API).
 *
 * The major version number must be increased every time there are API
 * changes or functional changes which require adaptations in the host
 * (libsigrok) drivers, i.e. changes where old libsigrok versions would no
 * longer (properly) work with the new fx2lafw firmware.
 */
#define FX2LAFW_VERSION_MAJOR	0
#define FX2LAFW_VERSION_MINOR	99

#endif
