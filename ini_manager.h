#ifndef INI_MANAGER_H
#define INI_MANAGER_H

#include <string>
#include <map>
#include <cstdio>
#include <ctime>
#include <dirent.h>
#include <cstring>
#include <map>

const int TRUE = 1;
const int FALSE = 0;

using namespace std;

// Declaração das funções
int center_text(const char* text, int width);
int check_directory(const char *directory);
int change_key_value(map<string, string>& ini_map);
int add_key_value(map<string, string>& ini_map);
int delete_key(map<string, string>& ini_map);
void extract_key_value_from_ini(const char* ini_file, char* key_buff, char* value_buff, map<string, string>& ini_map);
void clear_buffers(char* key_buff, char* value_buff);
void add_char_to_buffer(char c, char* buffer);
void get_date(char *date);
void print_key_value(map<string, string>& ini_map);
void read_input_trimmed(char* buffer, size_t size);
void write_ini_file(FILE *p_txt_file, map<string, string>& ini_map);

#endif // INI_MANAGER_H
