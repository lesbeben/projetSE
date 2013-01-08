#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <errno.h>
#include <dlfcn.h>
#include <string.h>

#include "client_request.h"
#include "client_command.h"

/*
 * Structure des listes de bibliotheques dynamique chargées.
 *   -
 */
typedef struct DL_LIST {
	request_cmd_t cmd;
	void* handler;
	struct DL_LIST* next;
} *dl_list_t;

/*
 * La liste des bibliotheques chargées.
 */
static dl_list_t dl_list = NULL;

void request_manager_init() {
	const char* dirname= "plugin/clientcmd/";
	DIR* dir = opendir(dirname);
	if (dir == NULL) {
		perror("opendir");
		raise(SIGTERM);
	}
	char name[256 + 17];
	dl_list_t element = NULL;
	
	struct dirent* ent = readdir(dir);
	while (ent != NULL) {
		if (ent->d_name[0] != '.') {
			sprintf(name, "%s%s", dirname, ent->d_name);
			void *hndl = dlopen(name, RTLD_LAZY);
			if(hndl == NULL) {
				fprintf(stderr, "Impossible d'ouvrir %s : %s\n", name, dlerror());
				ent = readdir(dir);
				continue;
			}
			request_cmd_t (*func)();
			func = dlsym(hndl, "getCommand");
			if (func == NULL) {
				fprintf(stderr, "%s n'est pas un type de requete valide : %s\n", name, dlerror()); 
				dlclose(hndl);
				ent = readdir(dir);
				continue;
			}
			element = malloc(sizeof(struct DL_LIST));
			element->cmd = func();
			element->handler = hndl;
			element->next = dl_list;
			dl_list = element;
		}
		ent = readdir(dir);
	}
	if (errno != 0) {
		perror("readdir");
	}
	if (closedir(dir) == -1) {
		perror("closedir");
	}
}

request_t* create_request(const char* cmd) {
	dl_list_t elmnt = dl_list;
	while (elmnt != NULL) {
		if (strncmp(cmd, elmnt->cmd.cmd_name, 3) == 0) {
			return elmnt->cmd.request_cmd_func(cmd);
		}
		elmnt = elmnt->next;
	}
	fprintf(stderr, "Commande %s inconnue\n", cmd);
	return NULL;
}

void delete_request(request_t* req) {
	free(req);
}

void request_manager_helpAll() {
	dl_list_t elmnt = dl_list;
	while (elmnt != NULL) {
		elmnt->cmd.help();
		elmnt = elmnt->next;
	}
}

/**
 * Affiche l'aide de la commande d'une commande.
 */
void request_manager_help(const char* cmd) {
	dl_list_t elmnt = dl_list;
	while (elmnt != NULL) {
		if (strncmp(cmd, elmnt->cmd.cmd_name, 3) == 0) {
			elmnt->cmd.help();
		}
		elmnt = elmnt->next;
	}
	fprintf(stderr, "Commande %s inconnue\n", cmd);
}


void request_manager_close() {
	dl_list_t elmnt = dl_list;
	while (elmnt != NULL) {
		if (dlclose(elmnt->handler) == -1) {
			fprintf(stderr, "dlclose : %s\n", dlerror());
			raise(SIGTERM);
		}
		dl_list_t next = elmnt->next;
		free(elmnt);
		elmnt = next;
	}
}
