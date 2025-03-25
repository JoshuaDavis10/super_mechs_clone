#include "files.h"

#include "logger.h"
#include "asserts.h"

#include <string.h>
#include <stdlib.h>

u8 open_file(const char* path, file* file, const char* mode) {
	FILE* opened_file;
	opened_file = fopen(path, mode);
	file->handle = opened_file;
	file->mode = mode;
	if(opened_file == 0) {
		return false;
	}
	return true;
}

u8 close_file(file* file) {
	int result = fclose(file->handle);
	if(result == 0) {
		return true;
	}
    return false;
}

u8 read_line(file file, char* line_buffer, u32 line_buffer_size) {
	char temp_buffer[MAX_LINE_SIZE];
    u32 length;
	if(fgets(temp_buffer, MAX_LINE_SIZE, file.handle) != 0) {
        length = strlen(temp_buffer);
        _ASSERT(length <= line_buffer_size);
		strcpy(line_buffer, temp_buffer);
        line_buffer[strcspn(line_buffer, "\r\n")] = '\0';
        line_buffer[strcspn(line_buffer, "\n")] = '\0';
		return true;
	}
    i32 result = ferror(file.handle);
    if(result != 0) { LERROR("failed to read line."); }
    else { LTRACE("reached end of file."); }
	return false;
}

u8 write_line(file file, const char* line) {
    u32 line_len = strlen(line);
    _ASSERT(line_len <= MAX_LINE_SIZE);
    if(strcmp(file.mode, "w") != 0 && strcmp(file.mode, "r+") != 0) {
        LERROR("do not have permission to write to file.\n");
    }
	char line_to_write[MAX_LINE_SIZE];
	strcpy(line_to_write, line);
	strcat(line_to_write, "\n");
	if(fputs(line_to_write, file.handle) == EOF) {
        LERROR("failed to write line: '%s' to file.\n", line);
		return false;
	}
	return true;
}
