#pragma once

#include "../defines.h"

#include <stdio.h>

#define MAX_LINE_SIZE 32000

typedef struct file {
	FILE* handle;
	const char* mode;
} file;

u8 open_file(const char* path, file* file, const char* mode);
u8 close_file(file* file);
u8 read_line(file file, char* line_buffer, u32 line_buffer_size);
u8 write_line(file file, const char* line);
