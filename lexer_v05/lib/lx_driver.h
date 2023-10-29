#ifndef LX_DRIVER_H
#define LX_DRIVER_H

#include "lx_file.h"
#include "lx_memory.h"
#include "lx_scanner.h"
#include "lx_token.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	char          str[64];
	TokenIdentity kind;

} TokenPrint;

void driver_start_process(char *path);

#endif
