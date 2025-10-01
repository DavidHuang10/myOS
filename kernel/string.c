#include "include/string.h"

// Get string length
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Compare two strings
int strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

// Compare first n characters of two strings
int strncmp(const char* str1, const char* str2, size_t n) {
    while (n && *str1 && (*str1 == *str2)) {
        str1++;
        str2++;
        n--;
    }
    if (n == 0) {
        return 0;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

// Copy string
char* strcpy(char* dest, const char* src) {
    char* orig_dest = dest;
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return orig_dest;
}

// Copy up to n characters
char* strncpy(char* dest, const char* src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

// Concatenate strings
char* strcat(char* dest, const char* src) {
    char* orig_dest = dest;
    // Find end of dest
    while (*dest != '\0') {
        dest++;
    }
    // Copy src to end
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return orig_dest;
}

// Set memory to value
void* memset(void* ptr, int value, size_t num) {
    unsigned char* p = (unsigned char*)ptr;
    while (num > 0) {
        *p = (unsigned char)value;
        p++;
        num--;
    }
    return ptr;
}

// Copy memory
void* memcpy(void* dest, const void* src, size_t n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    while (n > 0) {
        *d = *s;
        d++;
        s++;
        n--;
    }
    return dest;
}

// Compare memory
int memcmp(const void* ptr1, const void* ptr2, size_t n) {
    const unsigned char* p1 = (const unsigned char*)ptr1;
    const unsigned char* p2 = (const unsigned char*)ptr2;
    while (n > 0) {
        if (*p1 != *p2) {
            return *p1 - *p2;
        }
        p1++;
        p2++;
        n--;
    }
    return 0;
}

// Check if character is whitespace
int isspace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

// Check if character is digit
int isdigit(char c) {
    return c >= '0' && c <= '9';
}

// Check if character is letter
int isalpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// Check if character is alphanumeric
int isalnum(char c) {
    return isalpha(c) || isdigit(c);
}

// Convert string to integer
int atoi(const char* str) {
    int result = 0;
    int sign = 1;
    int i = 0;

    // Skip leading whitespace
    while (isspace(str[i])) {
        i++;
    }

    // Handle sign
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    // Convert digits
    while (isdigit(str[i])) {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return sign * result;
}

// Convert integer to string
char* itoa(int value, char* str, int base) {
    // Validate base
    if (base < 2 || base > 36) {
        *str = '\0';
        return str;
    }

    char* ptr = str;
    char* ptr1 = str;
    char tmp_char;
    int tmp_value;

    // Handle negative numbers for base 10
    if (value < 0 && base == 10) {
        value = -value;
        *ptr++ = '-';
        ptr1++;
    }

    // Convert to string (reversed)
    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[tmp_value - value * base];
    } while (value);

    // Null terminate
    *ptr-- = '\0';

    // Reverse string
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }

    return str;
}

// Trim leading and trailing whitespace (in-place)
char* strtrim(char* str) {
    if (str == 0) {
        return str;
    }

    char* end;

    // Trim leading space
    while (isspace(*str)) {
        str++;
    }

    // All spaces?
    if (*str == '\0') {
        return str;
    }

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) {
        end--;
    }

    // Write new null terminator
    *(end + 1) = '\0';

    return str;
}
