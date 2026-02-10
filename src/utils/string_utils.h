#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdbool.h>

// String utility functions
void strTrim(char *str);
void strToLower(char *str);
void strToUpper(char *str);
void strCapitalize(char *str);
bool strContains(const char *haystack, const char *needle);
bool strStartsWith(const char *str, const char *prefix);
bool strEndsWith(const char *str, const char *suffix);
int strSplit(char *str, char delimiter, char **tokens, int maxTokens);
void strReplace(char *str, char find, char replace);
void strCopy(char *dest, const char *src, int maxLen);
int strCompareIgnoreCase(const char *str1, const char *str2);

#endif
