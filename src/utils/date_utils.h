#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include <time.h>
#include <stdbool.h>

// Date utility functions
void getCurrentDate(char *buffer, int size);
void formatDate(char *buffer, int size, int day, int month, int year);
bool parseDate(const char *dateStr, int *day, int *month, int *year);
int calculateAge(const char *birthDate);
bool isValidDate(int day, int month, int year);
bool isLeapYear(int year);
int getDaysInMonth(int month, int year);
int compareDates(const char *date1, const char *date2);
bool isDateInCurrentMonth(const char *date);
void addDays(char *buffer, int size, const char *date, int days);

#endif
