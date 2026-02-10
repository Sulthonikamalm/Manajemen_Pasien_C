#include "string_utils.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// Trim whitespace dari awal dan akhir string
void strTrim(char *str) {
    if (str == NULL) return;
    
    // Trim leading whitespace
    int start = 0;
    while (str[start] && isspace((unsigned char)str[start])) {
        start++;
    }
    
    // Trim trailing whitespace
    int end = strlen(str) - 1;
    while (end >= start && isspace((unsigned char)str[end])) {
        end--;
    }
    
    // Move trimmed string to beginning
    int i;
    for (i = 0; i <= end - start; i++) {
        str[i] = str[start + i];
    }
    str[i] = '\0';
}

// Convert string to lowercase
void strToLower(char *str) {
    if (str == NULL) return;
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Convert string to uppercase
void strToUpper(char *str) {
    if (str == NULL) return;
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

// Capitalize first letter of each word
void strCapitalize(char *str) {
    if (str == NULL) return;
    
    bool newWord = true;
    for (int i = 0; str[i]; i++) {
        if (isspace((unsigned char)str[i])) {
            newWord = true;
        } else if (newWord) {
            str[i] = toupper((unsigned char)str[i]);
            newWord = false;
        } else {
            str[i] = tolower((unsigned char)str[i]);
        }
    }
}

// Check if string contains substring
bool strContains(const char *haystack, const char *needle) {
    if (haystack == NULL || needle == NULL) return false;
    return strstr(haystack, needle) != NULL;
}

// Check if string starts with prefix
bool strStartsWith(const char *str, const char *prefix) {
    if (str == NULL || prefix == NULL) return false;
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

// Check if string ends with suffix
bool strEndsWith(const char *str, const char *suffix) {
    if (str == NULL || suffix == NULL) return false;
    
    int strLen = strlen(str);
    int suffixLen = strlen(suffix);
    
    if (suffixLen > strLen) return false;
    
    return strcmp(str + strLen - suffixLen, suffix) == 0;
}

// Split string by delimiter
int strSplit(char *str, char delimiter, char **tokens, int maxTokens) {
    if (str == NULL || tokens == NULL) return 0;
    
    int count = 0;
    char *token = strtok(str, &delimiter);
    
    while (token != NULL && count < maxTokens) {
        tokens[count++] = token;
        token = strtok(NULL, &delimiter);
    }
    
    return count;
}

// Replace character in string
void strReplace(char *str, char find, char replace) {
    if (str == NULL) return;
    
    for (int i = 0; str[i]; i++) {
        if (str[i] == find) {
            str[i] = replace;
        }
    }
}

// Safe string copy
void strCopy(char *dest, const char *src, int maxLen) {
    if (dest == NULL || src == NULL) return;
    
    strncpy(dest, src, maxLen - 1);
    dest[maxLen - 1] = '\0';
}

// Case-insensitive string comparison
int strCompareIgnoreCase(const char *str1, const char *str2) {
    if (str1 == NULL || str2 == NULL) return -1;
    
    while (*str1 && *str2) {
        char c1 = tolower((unsigned char)*str1);
        char c2 = tolower((unsigned char)*str2);
        
        if (c1 != c2) {
            return c1 - c2;
        }
        
        str1++;
        str2++;
    }
    
    return tolower((unsigned char)*str1) - tolower((unsigned char)*str2);
}
