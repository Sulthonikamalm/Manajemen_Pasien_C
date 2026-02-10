#include "validator.h"
#include "date_utils.h"
#include <string.h>
#include <ctype.h>
#include <time.h>

// Validate nama (min 3 char, max 50 char, tidak boleh kosong)
bool validateNama(const char *nama) {
    if (nama == NULL || strlen(nama) < 3 || strlen(nama) > 50) {
        return false;
    }
    return true;
}

// Validate NIK (harus 16 digit angka)
bool validateNIK(const char *nik) {
    if (nik == NULL || strlen(nik) != 16) {
        return false;
    }
    
    for (int i = 0; i < 16; i++) {
        if (!isdigit((unsigned char)nik[i])) {
            return false;
        }
    }
    
    return true;
}

// Validate telepon (10-13 digit, harus angka)
bool validateTelepon(const char *telepon) {
    if (telepon == NULL) return false;
    
    int len = strlen(telepon);
    if (len < 10 || len > 13) return false;
    
    // Skip +62 prefix if exists
    int start = 0;
    if (telepon[0] == '+') {
        if (len < 12 || len > 15) return false;
        start = 1;
    }
    
    for (int i = start; telepon[i]; i++) {
        if (!isdigit((unsigned char)telepon[i])) {
            return false;
        }
    }
    
    return true;
}

// Validate tanggal (format DD/MM/YYYY, tidak boleh masa depan)
bool validateTanggal(const char *tanggal) {
    int day, month, year;
    
    if (!parseDate(tanggal, &day, &month, &year)) {
        return false;
    }
    
    if (!isValidDate(day, month, year)) {
        return false;
    }
    
    // Check if not in future
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    
    int currentYear = t->tm_year + 1900;
    int currentMonth = t->tm_mon + 1;
    int currentDay = t->tm_mday;
    
    if (year > currentYear) return false;
    if (year == currentYear && month > currentMonth) return false;
    if (year == currentYear && month == currentMonth && day > currentDay) return false;
    
    return true;
}

// Validate jenis kelamin
bool validateJenisKelamin(const char *jenisKelamin) {
    if (jenisKelamin == NULL) return false;
    
    return (strcmp(jenisKelamin, "Laki-laki") == 0 || 
            strcmp(jenisKelamin, "Perempuan") == 0);
}

// Validate usia (0-150)
bool validateUsia(int usia) {
    return (usia >= 0 && usia <= 150);
}

// Validate not empty
bool validateNotEmpty(const char *str) {
    if (str == NULL) return false;
    
    // Check if string has at least one non-whitespace character
    for (int i = 0; str[i]; i++) {
        if (!isspace((unsigned char)str[i])) {
            return true;
        }
    }
    
    return false;
}

// Sanitize input (remove dangerous characters)
void sanitizeInput(char *str) {
    if (str == NULL) return;
    
    // Remove newlines and carriage returns
    for (int i = 0; str[i]; i++) {
        if (str[i] == '\n' || str[i] == '\r') {
            str[i] = ' ';
        }
    }
}

// Trim whitespace
void trimWhitespace(char *str) {
    if (str == NULL) return;
    
    // Trim leading
    int start = 0;
    while (str[start] && isspace((unsigned char)str[start])) {
        start++;
    }
    
    // Trim trailing
    int end = strlen(str) - 1;
    while (end >= start && isspace((unsigned char)str[end])) {
        end--;
    }
    
    // Move trimmed string
    int i;
    for (i = 0; i <= end - start; i++) {
        str[i] = str[start + i];
    }
    str[i] = '\0';
}

// Convert to lowercase
void toLowerCase(char *str) {
    if (str == NULL) return;
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Convert to uppercase
void toUpperCase(char *str) {
    if (str == NULL) return;
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

// Check if string is numeric
bool isNumeric(const char *str) {
    if (str == NULL || *str == '\0') return false;
    
    for (int i = 0; str[i]; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return false;
        }
    }
    
    return true;
}

// Check if string is alphabetic
bool isAlpha(const char *str) {
    if (str == NULL || *str == '\0') return false;
    
    for (int i = 0; str[i]; i++) {
        if (!isalpha((unsigned char)str[i]) && !isspace((unsigned char)str[i])) {
            return false;
        }
    }
    
    return true;
}

// Check if string is alphanumeric
bool isAlphaNumeric(const char *str) {
    if (str == NULL || *str == '\0') return false;
    
    for (int i = 0; str[i]; i++) {
        if (!isalnum((unsigned char)str[i]) && !isspace((unsigned char)str[i])) {
            return false;
        }
    }
    
    return true;
}
