#include <unistd.h>
#include "stream_manager.h"
#include "se_fifo.h"
#include "se_shm.h"
#include "se_mq.h"

/*
 * Structure de liste de flux contenant : 
 *   - stream : le flux à gérer
 *   - stream_name : le nom du flux 
 *   - ownerpid : le pid du processus responsable de la durée 
 *                  de vie du flux
 *   - next : élément suivant de la liste
 */
typedef struct STREAM_LIST {
	stream_t* stream;
	char* stream_name;
	pid_t ownerpid;
	struct STREAM_LIST* next;
} *stream_list_t;

/*
 * La liste des flux à gérer
 */
stream_list_t stream_list = NULL;

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
	stream_list_t element = malloc(sizeof(struct STREAM_LIST));
	element->stream_name = malloc(strlen(name) + 1);
	strcpy(element->stream_name, name);
	element->stream = stream;
	element->ownerpid = getpid();
	element->next = stream_list;
	stream_list = element;
}

void manager_removestream(stream_t* stream) {
	stream_list_t prev = NULL;
	stream_list_t element = NULL;
	stream_list_t next = stream_list;
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
			stream_list = next;
		} else {
			prev->next = next;
		}
		free(element);
	}
}

void manager_clean() {
	stream_list_t element = NULL;
	stream_list_t next = stream_list;
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
	stream_list = NULL;
}



