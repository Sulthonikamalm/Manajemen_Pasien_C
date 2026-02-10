#ifndef CSV_HANDLER_H
#define CSV_HANDLER_H

#include "../models/pasien.h"
#include "../models/rekam_medis.h"
#include "../models/resep.h"

// CSV File paths
#define CSV_PASIEN "data/pasien.csv"
#define CSV_REKAM_MEDIS "data/rekam_medis.csv"
#define CSV_RESEP "data/resep.csv"
#define CSV_CONFIG "data/config.csv"

// Function declarations untuk Pasien
int savePasienToCSV(Pasien *pasien);
int loadPasienFromCSV(Pasien **pasienList);
int updatePasienInCSV(Pasien *pasien, int id);
int deletePasienFromCSV(int id);

// Function declarations untuk Rekam Medis
int saveRekamMedisToCSV(RekamMedis *rekamMedis);
int loadRekamMedisFromCSV(RekamMedis **rekamMedisList);
int updateRekamMedisInCSV(RekamMedis *rekamMedis, int id);
int deleteRekamMedisFromCSV(int id);

// Function declarations untuk Resep
int saveResepToCSV(Resep *resep);
int loadResepFromCSV(Resep **resepList);
int updateResepInCSV(Resep *resep, int id);
int deleteResepFromCSV(int id);

// Function declarations untuk Config
int saveConfig(const char *key, const char *value);
char* loadConfig(const char *key);

// Utility functions
int getNextId(const char *csvFile);
void escapeCSV(char *dest, const char *src, int maxLen);
void unescapeCSV(char *dest, const char *src, int maxLen);

#endif
