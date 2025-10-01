#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>

// String functions
size_t strlen(const char* str);
int strcmp(const char* str1, const char* str2);
int strncmp(const char* str1, const char* str2, size_t n);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
char* strcat(char* dest, const char* src);

// Memory functions
void* memset(void* ptr, int value, size_t num);
void* memcpy(void* dest, const void* src, size_t n);
int memcmp(const void* ptr1, const void* ptr2, size_t n);

// Conversion functions
int atoi(const char* str);
char* itoa(int value, char* str, int base);

// Character classification
int isspace(char c);
int isdigit(char c);
int isalpha(char c);
int isalnum(char c);

// String trimming
char* strtrim(char* str);

#endif // STRING_H
