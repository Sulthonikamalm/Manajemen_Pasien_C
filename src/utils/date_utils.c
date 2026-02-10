#include "date_utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Get current date in DD/MM/YYYY format
void getCurrentDate(char *buffer, int size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(buffer, size, "%02d/%02d/%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
}

// Format date to DD/MM/YYYY
void formatDate(char *buffer, int size, int day, int month, int year) {
    snprintf(buffer, size, "%02d/%02d/%04d", day, month, year);
}

// Parse date string DD/MM/YYYY
bool parseDate(const char *dateStr, int *day, int *month, int *year) {
    if (dateStr == NULL) return false;
    return sscanf(dateStr, "%d/%d/%d", day, month, year) == 3;
}

// Check if leap year
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Get days in month
int getDaysInMonth(int month, int year) {
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (month < 1 || month > 12) return 0;
    
    if (month == 2 && isLeapYear(year)) {
        return 29;
    }
    
    return days[month - 1];
}

// Validate date
bool isValidDate(int day, int month, int year) {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > getDaysInMonth(month, year)) return false;
    return true;
}

// Calculate age from birth date
int calculateAge(const char *birthDate) {
    int day, month, year;
    if (!parseDate(birthDate, &day, &month, &year)) {
        return -1;
    }
    
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    
    int currentYear = t->tm_year + 1900;
    int currentMonth = t->tm_mon + 1;
    int currentDay = t->tm_mday;
    
    int age = currentYear - year;
    
    if (currentMonth < month || (currentMonth == month && currentDay < day)) {
        age--;
    }
    
    return age;
}

// Compare two dates (returns -1 if date1 < date2, 0 if equal, 1 if date1 > date2)
int compareDates(const char *date1, const char *date2) {
    int d1, m1, y1, d2, m2, y2;
    
    if (!parseDate(date1, &d1, &m1, &y1) || !parseDate(date2, &d2, &m2, &y2)) {
        return 0;
    }
    
    if (y1 != y2) return (y1 > y2) ? 1 : -1;
    if (m1 != m2) return (m1 > m2) ? 1 : -1;
    if (d1 != d2) return (d1 > d2) ? 1 : -1;
    
    return 0;
}

// Check if date is in current month
bool isDateInCurrentMonth(const char *date) {
    int day, month, year;
    if (!parseDate(date, &day, &month, &year)) {
        return false;
    }
    
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    
    int currentYear = t->tm_year + 1900;
    int currentMonth = t->tm_mon + 1;
    
    return (year == currentYear && month == currentMonth);
}

// Add days to a date
void addDays(char *buffer, int size, const char *date, int days) {
    int d, m, y;
    if (!parseDate(date, &d, &m, &y)) {
        strncpy(buffer, date, size);
        return;
    }
    
    d += days;
    
    while (d > getDaysInMonth(m, y)) {
        d -= getDaysInMonth(m, y);
        m++;
        if (m > 12) {
            m = 1;
            y++;
        }
    }
    
    formatDate(buffer, size, d, m, y);
}
