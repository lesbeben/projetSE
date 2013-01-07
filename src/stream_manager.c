#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <errno.h>
#include <dlfcn.h>

#include "stream_manager.h"

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

/*
 * Structure des listes de bibliotheques dynamique chargées.
 *   -
 */
typedef struct DL_LIST {
	operation_t op;
	void* handler;
	struct DL_LIST* next;
} *dl_list_t;

/*
 * La liste des bibliotheques chargées.
 */
dl_list_t dl_list = NULL;

void manager_init() {
	const char* dirname= "plugin/";
	DIR* dir = opendir(dirname);
	if (dir == NULL) {
		perror("opendir");
		raise(SIGTERM);
	}
	char name[256 + 7];
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
			operation_t (*func)();
			func = dlsym(hndl, "getop");
			if (func == NULL) {
				fprintf(stderr, "%s n'est pas un type de connection valide : %s\n", name, dlerror()); 
				dlclose(hndl);
				ent = readdir(dir);
				continue;
			}
			operation_t op = func();
			element = malloc(sizeof(struct DL_LIST));
			element->op = op;
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

stream_t manager_getstream(const char* streamName) {
	dl_list_t elmnt = dl_list;
	operation_t op;
	while (elmnt != NULL) {
		if (strcmp(streamName, elmnt->op.name) == 0) {
			op = elmnt->op;
			break;
		}
		elmnt = elmnt->next;
	}
	char buffer[512];
	sprintf(buffer, "Le flux %s n'existe pas\n", streamName);
	check_error(elmnt == NULL, buffer);

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

void manager_close() {
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

