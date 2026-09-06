#ifndef NGD_FILE_H
#define NGD_FILE_H
//
#include "ngd_file.h"
//
typedef struct ngd_file_t ngd_file_t;
//
void ngd_file_init(ngd_file_t *f);
void ngd_file_close(ngd_file_t *f);
int ngd_file_open(ngd_file_t *f, const char *path);
int ngd_file_open_temp(ngd_file_t *f);
int ngd_file_read(ngd_file_t *f, u_char *buf, size_t len, size_t *bytes_read);
int ngd_file_write_all(ngd_file_t *f, u_char *buf, size_t len);
//
#endif
