#ifndef CONST_H
#define CONST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "se_fifo.h"
#include "se_shm.h"
#include "se_mq.h"
#include "stream.h"
#include "error.h"

stream_t getStream(const char* streamName) {
	operation_t reg_op[] = {
		fifo_getOp(), mq_getOp(), shm_getOp()
	};
	
	operation_t* op = NULL;
	for (int i = 0; i < sizeof(reg_op); i++) {
		if (strcmp(streamName, reg_op[i].name) == 0) {
			op = &reg_op[i];
			break;
		}
	}
	char buffer[512];
	sprintf(buffer, "Le flux %s n'existe pas\n", streamName);
	check_error((op == NULL), "buffer");
    
	stream_t stream = {op, NULL};
	return stream;
}

#endif
