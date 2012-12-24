#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>
#include "se_shm.h"
#include "error.h"

typedef struct {
	int maxSize;
	int sizeToRead;
} shm_t;

typedef struct {
	int shm;
	shm_t* shmdata;
	sem_t* read;
	sem_t* write;
} data_t;

/*
 * Renvoie la taille d'un segment de mémoire partagé à partir de la taille 
 *   des données à écrire
 */
static size_t _shm_size(size_t size) {
	return (2 * sizeof(int) + size * sizeof(char));
}

/*
 *
 */
static int _shm_create(streamd_t* sd, const char* name, size_t size) {
	int realnamelength = strlen(name) + 2;
	char realname[realnamelength];
	char readSemName[realnamelength + 4];
	char writeSemName[realnamelength + 5];
    sprintf(realname, "/%s", name);
    sprintf(readSemName, "%sread", realname);
    sprintf(writeSemName, "%swrite", realname);
    
	int shm = shm_open(realname, O_CREAT | O_EXCL | O_RDWR
						, S_IRUSR | S_IWUSR);
	if (shm == -1) {
		perror("shm_open");
		return -1;
	}
    /*
     * Notre segment de mémoire partagé contiendra 
     *   La taille maximale des données à écrire
     *   Une semaphore si l'on peut accéder aux données
     *   Un entier pour la lecture des données
     *   Un entier pour l'écriture des données
     *   size octets pour recevoir les données
     */
    size_t shmsize = _shm_size(size);
    if (ftruncate(shm, shmsize) == -1) {
		perror("ftruncate");
		return -1;
	}
	
	void* p = mmap(NULL, shmsize, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
	if ((p == MAP_FAILED) || (p == NULL)) {
		perror("mmap");
		return -1;
	}
	shm_t* s = (shm_t*) p;
	s->maxSize = size;
	
	sem_t* sem = sem_open(readSemName, O_CREAT | O_EXCL | O_RDWR
						, S_IRUSR | S_IWUSR, 0);
	if (sem == NULL) {
		perror("sem_open");
		return -1;
	}
	if (sem_close(sem) == -1) {
		perror("sem_close");
		return -1;
	}
	sem = sem_open(writeSemName, O_CREAT | O_EXCL | O_RDWR
						, S_IRUSR | S_IWUSR, 1);
	if (sem == NULL) {
		perror("sem_open");
		return -1;
	}
	if (sem_close(sem) == -1) {
		perror("sem_close");
		return -1;
	}
	s->sizeToRead = 0;
	if (munmap(p, shmsize) == -1) {
		perror("munmap");
		return -1;
	}
	
	if (close(shm) == -1) {
		perror("close");
		return -1;
	}
	return 0;
}

/*
 * 
 */
static int _shm_open(streamd_t* sd, const char* name, int oflag) {
	sd->data = malloc(sizeof(data_t));
	data_t* data = (data_t*) sd->data;
	int realnamelength = strlen(name) + 2;
	char realname[realnamelength];
	char readSemName[realnamelength + 4];
	char writeSemName[realnamelength + 5];
    sprintf(realname, "/%s", name);
    sprintf(readSemName, "%sread", realname);
    sprintf(writeSemName, "%swrite", realname);
	
	// On ouvre le segment de mémoire
	int shm = shm_open(realname, O_RDWR, 0);
	if (shm == -1) {
		perror("shm_open");
		return -1;
	}
	// On projette le segment en mémoire pour récuperer la taille du segment
	void* p = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
	if ((p == MAP_FAILED) || (p == NULL)) {
		perror("mmap");
		return -1;
	}
	int size = *((int*) p);
	if (munmap(p, sizeof(int)) == -1) {
		perror("munmap");
		return -1;
	}
	// On projette le segment entier en mémoire
	size_t shmsize = _shm_size(size);
	p = mmap(NULL, shmsize, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
	if ((p == MAP_FAILED) || (p == NULL)) {
		perror("mmap");
		return -1;
	}
	data->shm = shm;
	data->shmdata = (shm_t*) p;
	data->read = sem_open(readSemName, O_RDWR);
	if (data->read == NULL) {
		perror("sem_open");
		return -1;
	}
	data->write = sem_open(writeSemName, O_RDWR);
	if (data->write == NULL) {
		perror("sem_open");
		return -1;
	}
	return shm;
}

/*
 * Ferme le segment de mémoire partagé
 */
static int _shm_close(streamd_t* sd) {
	data_t* data = (data_t*) sd->data;
	size_t shmsize = _shm_size(data->shmdata->maxSize);
	
	if (sem_close(data->read) == -1) {
		perror("sem_close");
		return -1;
	}
	if (sem_close(data->write) == -1) {
		perror("sem_close");
		return -1;
	}
	if (munmap(data->shmdata, shmsize) == -1) {
		perror("munmap");
		return -1;
	}
	if (close(data->shm) == -1) {
		perror("close");
		return -1;
	}
	free(sd->data);
	return 0;
}

/*
 * 
 */
static int _shm_read(streamd_t* sd, void* buffer, size_t size) {
	data_t* data = (data_t*) sd->data;
	shm_t* s = data->shmdata;
	int read = 0;
	
	if (sem_wait(data->read) == -1) {
		perror("sem_wait");
		return -1;
	}
	if (s->sizeToRead > 0) {
		if (size < s->sizeToRead) {
			fprintf(stderr, "La taille du buffer n'est pas assez grande\n");
			return -2;
		}
		if (memcpy(buffer, s + 1, s->sizeToRead) == NULL) {
			perror("memcpy");
			return -1;
		}
		read = s->sizeToRead;
		s->sizeToRead = 0;
	}
	if (sem_post(data->write) == -1) {
		perror("sem_post");
		return -1;
	}
	return read;
}

/*
 * 
 */
static int _shm_write(streamd_t* sd, void* buffer, size_t size) {
	data_t* data = (data_t*) sd->data;
	shm_t* s = data->shmdata;
	int writed = 0;
	
	if (size > s->maxSize) {
		fprintf(stderr, "La taille du buffer à envoyer est trop grande\n");
		return -2;
	}
	
	if (sem_wait(data->write) == -1) {
		return -1;
	}
	if (s->sizeToRead == 0) {
		if (memcpy(s + 1, buffer, size) == NULL) {
			perror("memcpy");
			return -1;
		}
		writed = size;
		s->sizeToRead = size;
	}
	if (sem_post(data->read) == -1) {
		perror("sem_post");
		return -1;
	}
	return writed;
}

/*
 *
 */
static int _shm_unlink(const char* name) {
	int realnamelength = strlen(name) + 2;
	char realname[realnamelength];
	char readSemName[realnamelength + 4];
	char writeSemName[realnamelength + 5];
    sprintf(realname, "/%s", name);
    sprintf(readSemName, "%sread", realname);
    sprintf(writeSemName, "%swrite", realname);
    
    if (sem_unlink(readSemName) == -1) {
		perror("sem_unlink");
		return -1;
	}
    if (sem_unlink(writeSemName) == -1) {
		perror("sem_unlink");
		return -1;
	}
	if (shm_unlink(realname) == -1) {
		perror("shm_unlink");
		return -1;
	}
	return 0;
}

/*
 *
 */
const operation_t _shm_op = {
	_shm_create, _shm_open, _shm_close
	, _shm_read, _shm_write, _shm_unlink, "shm"
};

operation_t shm_getOp() {
	return _shm_op;
}
