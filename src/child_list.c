#include <stdlib.h>
#include "child_list.h"


typedef struct CHILD_LIST {
	pid_t client;
	int pipe;
	struct CHILD_LIST* next;
} *child_list_t;

static child_list_t child_list = NULL;
static child_list_t deleted = NULL;

void addChild(pid_t clientPid, int pipe) {
	if (!childExist(clientPid)) {
		child_list_t elmnt = malloc(sizeof(struct CHILD_LIST));
		child_list_t l = child_list;
		if ((l != NULL) && (l->client < clientPid)) {
			while ((l->next != NULL) && (l->next->client < clientPid)) {
				l = l->next;
			}
			elmnt->next = l->next;
			elmnt->client = clientPid;
			elmnt->pipe = pipe;
			l->next = elmnt;
		} else {
			elmnt->next = l;
			elmnt->client = clientPid;
			elmnt->pipe = pipe;
			child_list = elmnt;
		}
	}
}

int childExist(pid_t clientPid) {
	child_list_t elmnt = child_list;
	while ((elmnt != NULL) && (elmnt->client < clientPid)) {
		elmnt = elmnt->next;
	}
	return (elmnt != NULL) && (elmnt->client == clientPid);
}

/**
 * Renvoie le pipe associé à un client.
 * renvoie -1 si le client n'existe pas;
 */
int getPipe(pid_t clientPid) {
	child_list_t elmnt = child_list;
	while ((elmnt != NULL) && (elmnt->client < clientPid)) {
		elmnt = elmnt->next;
	}
	if ((elmnt != NULL) && (elmnt->client == clientPid)) {
		return elmnt->pipe;
	}
	return -1;
}

/**
 * Enleve un processus de traitement la liste des processus.
 */
void removeChild(pid_t clientPid) {
	child_list_t l = child_list;
	if ((l != NULL) && (l->client != clientPid)) {
		while ((l->next != NULL) && (l->next->client < clientPid)) {
			l = l->next;
		}
		if ((l->next != NULL) && (l->next->client == clientPid)) {
			child_list_t elmnt = l->next;
			l->next = elmnt->next;
			elmnt->next = deleted;
			deleted = elmnt;
			if (close(elmnt->pipe) == -1) {
				perror("close");
			}
		}		
	} else if ((l != NULL) && (l->client == clientPid)) {
		child_list_t elmnt = l;
		child_list = elmnt->next;
		elmnt->next = deleted;
		deleted = elmnt;
		if (close(elmnt->pipe) == -1) {
			perror("close");
		}
	}
}

/**
 * Enleve tous les processus de traitement de la liste. 
 */
void childsClean() {
	while (child_list != NULL) {
		child_list_t next = child_list->next;
		free(child_list);
		child_list = next;
	}
	while (deleted != NULL) {
		child_list_t next = deleted->next;
		free(child_list);
		deleted = next;
	}
}
