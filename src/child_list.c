/**
 * Implémente les opérations nécessaires pour gérer les processus 
 *   de traitement du server.
 */
 
#include <stdlib.h>
#include <stdio.h>
#include "child_list.h"

/*
 * La structure de liste chainée permettant de garder en mémoire 
 *   les associations : pid du client ==> tube
 */
typedef struct CHILD_LIST {
	pid_t client;
	int pipe;
	struct CHILD_LIST* next;
} *child_list_t;

/*
 * La liste des associations.
 * On ajoute / supprime les éléments de façon à garder une 
 *   liste triée sur les pid des clients.
 */
static child_list_t child_list = NULL;
/*
 * La liste de associations supprimées.
 * 'removeChild' pouvant être utilisée dans un gestionnaire de signaux
 *   on ne peut utiliser supprimer un élément de la mémoire 
 *   (free n'est pas signal-safe).
 * On modifie donc le chainage de la liste pour transferer l'élément à
 *   supprimer de 'child_list' vers 'deleted'.
 */
static child_list_t deleted = NULL;

void addChild(pid_t clientPid, int pipe) {
	if (pipe != -1) {
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
}

int childExist(pid_t clientPid) {
	child_list_t elmnt = child_list;
	while ((elmnt != NULL) && (elmnt->client < clientPid)) {
		elmnt = elmnt->next;
	}
	return (elmnt != NULL) && (elmnt->client == clientPid);
}

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

void childsClean() {
	while (child_list != NULL) {
		child_list_t next = child_list->next;
		if (close(child_list->pipe) == -1) {
			perror("close");
		}
		free(child_list);
		child_list = next;
	}
	while (deleted != NULL) {
		child_list_t next = deleted->next;
		free(child_list);
		deleted = next;
	}
}
