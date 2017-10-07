/*
 * error.c
 *
 * Created: 27/10/2015 18:04:03
 *  Author: Tom
 */

#include "error.h"
#include "pc.h"

void error_throw(ERR_CODE code)
{
	pc_tx_error(code);
}