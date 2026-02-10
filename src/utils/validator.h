#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <stdbool.h>

// Validation functions
bool validateNama(const char *nama);
bool validateNIK(const char *nik);
bool validateTelepon(const char *telepon);
bool validateTanggal(const char *tanggal);
bool validateJenisKelamin(const char *jenisKelamin);
bool validateUsia(int usia);
bool validateNotEmpty(const char *str);

// Sanitization functions
void sanitizeInput(char *str);
void trimWhitespace(char *str);
void toLowerCase(char *str);
void toUpperCase(char *str);

// Format validation
bool isNumeric(const char *str);
bool isAlpha(const char *str);
bool isAlphaNumeric(const char *str);

#endif
