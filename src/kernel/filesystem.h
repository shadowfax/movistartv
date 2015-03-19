#ifndef MOVISTARTV_KERNEL_FILESYSTEM_H
#define MOVISTARTV_KERNEL_FILESYSTEM_H

int file_exists(char *filename);
int is_dir(char *filename);
int is_file(char *filename);

#ifdef _WIN32
int mkdir(const char *filename, unsigned short mode);
#endif

#endif