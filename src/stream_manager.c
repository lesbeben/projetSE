#include <unistd.h>
#include "stream_manager.h"
#include "se_fifo.h"
#include "se_shm.h"
#include "se_mq.h"

typedef struct STREAM_LISTE {
	char* stream_name;
	stream_t* stream;
	pid_t ownerpid;
	struct STREAM_LISTE* next;
} *stream_liste_t;

stream_liste_t stream_liste = NULL;

stream_t manager_getstream(const char* streamName) {
	operation_t reg_op[] = {
		fifo_getOp(), mq_getOp(), shm_getOp()
	};

	operation_t op;
	unsigned int i = 0;
	for (i = 0; i < sizeof(reg_op); i++) {
		if (strcmp(streamName, reg_op[i].name) == 0) {
			op = reg_op[i];
			break;
		}
	}
	char buffer[512];
	sprintf(buffer, "Le flux %s n'existe pas\n", streamName);
	check_error(i == sizeof(reg_op), buffer);

	stream_t stream;
	stream.op = op;
	stream.sd.data = NULL;
	return stream;
}

void manager_addstream(stream_t* stream, const char* name) {
	stream_liste_t element = malloc(sizeof(struct STREAM_LISTE));
	element->stream_name = malloc(strlen(name) + 1);
	strcpy(element->stream_name, name);
	element->stream = stream;
	element->ownerpid = getpid();
	element->next = stream_liste;
	stream_liste = element;
}

void manager_removestream(stream_t* stream) {
	stream_liste_t prev = NULL;
	stream_liste_t element = NULL;
	stream_liste_t next = stream_liste;
	while (next != NULL) {
		if (element != NULL) {
			prev = element;
		}
		element = next;
		next = element->next;
		if (element->stream == stream) {
			break;
		}
	}
	if (next != NULL) {
		if (prev == NULL) {
			stream_liste = next;
		} else {
			prev->next = next;
		}
		free(element);
	}
}

void manager_clean() {
	stream_liste_t element = NULL;
	stream_liste_t next = stream_liste;
	pid_t pid = getpid();
	while (next != NULL) {
		element = next;
		next = element->next;
		if (element->ownerpid == pid) {
			stream_unlink(element->stream, element->stream_name);
		} else {
			manager_removestream(element->stream);
		}
	}
	stream_liste = NULL;
}



