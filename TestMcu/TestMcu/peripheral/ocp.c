/*
 * ocp.c
 *
 * Created: 02/11/2015 18:42:16
 *  Author: Tom
 */

#include <hal/capture.h>
#include "pc.h"

void ocp_process()
{
	uint32_t capture = capture_read();
	if (capture)
	{
		pc_tx_ocp(capture);
	}
}

void ocp_reset()
{
	capture_reset();
}