#include "csv_handler.h"
#include "string_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Escape special characters untuk CSV
void escapeCSV(char *dest, const char *src, int maxLen) {
    int j = 0;
    for (int i = 0; src[i] != '\0' && j < maxLen - 1; i++) {
        if (src[i] == ',' || src[i] == '"' || src[i] == '\n') {
            dest[j++] = '\\';
        }
        dest[j++] = src[i];
    }
    dest[j] = '\0';
}

// Unescape special characters dari CSV
void unescapeCSV(char *dest, const char *src, int maxLen) {
    int j = 0;
    for (int i = 0; src[i] != '\0' && j < maxLen - 1; i++) {
        if (src[i] == '\\' && (src[i+1] == ',' || src[i+1] == '"' || src[i+1] == '\n')) {
            i++;
        }
        dest[j++] = src[i];
    }
    dest[j] = '\0';
}

// Get next ID dari CSV file
int getNextId(const char *csvFile) {
    FILE *file = fopen(csvFile, "r");
    if (file == NULL) {
        return 1;  // First ID
    }
    
    int maxId = 0;
    char line[2048];
    
    while (fgets(line, sizeof(line), file)) {
        int id;
        if (sscanf(line, "%d,", &id) == 1) {
            if (id > maxId) {
                maxId = id;
            }
        }
    }
    
    fclose(file);
    return maxId + 1;
}

// ==================== PASIEN CSV FUNCTIONS ====================

int savePasienToCSV(Pasien *pasien) {
    FILE *file = fopen(CSV_PASIEN, "a");
    if (file == NULL) {
        return -1;
    }
    
    char nama[MAX_NAMA * 2], alamat[MAX_ALAMAT * 2], riwayat[MAX_RIWAYAT * 2];
    escapeCSV(nama, pasien->nama, sizeof(nama));
    escapeCSV(alamat, pasien->alamat, sizeof(alamat));
    escapeCSV(riwayat, pasien->riwayatPenyakit, sizeof(riwayat));
    
    fprintf(file, "%d,%s,%s,%s,%s,%d,%s,%s,%s,%s,%s\n",
            pasien->id, nama, pasien->nik, pasien->jenisKelamin,
            pasien->tanggalLahir, pasien->usia, alamat, pasien->telepon,
            riwayat, pasien->tanggalRegistrasi, pasien->status);
    
    fclose(file);
    return 0;
}

int loadPasienFromCSV(Pasien **pasienList) {
    FILE *file = fopen(CSV_PASIEN, "r");
    if (file == NULL) {
        *pasienList = NULL;
        return 0;
    }
    
    int count = 0;
    int capacity = 10;
    *pasienList = (Pasien*)malloc(capacity * sizeof(Pasien));
    
    char line[2048];
    while (fgets(line, sizeof(line), file)) {
        if (count >= capacity) {
            capacity *= 2;
            *pasienList = (Pasien*)realloc(*pasienList, capacity * sizeof(Pasien));
        }
        
        Pasien *p = &(*pasienList)[count];
        char nama[MAX_NAMA * 2], alamat[MAX_ALAMAT * 2], riwayat[MAX_RIWAYAT * 2];
        
        if (sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%d,%[^,],%[^,],%[^,],%[^,],%[^\n]",
                   &p->id, nama, p->nik, p->jenisKelamin, p->tanggalLahir,
                   &p->usia, alamat, p->telepon, riwayat,
                   p->tanggalRegistrasi, p->status) == 11) {
            
            unescapeCSV(p->nama, nama, MAX_NAMA);
            unescapeCSV(p->alamat, alamat, MAX_ALAMAT);
            unescapeCSV(p->riwayatPenyakit, riwayat, MAX_RIWAYAT);
            count++;
        }
    }
    
    fclose(file);
    return count;
}

int updatePasienInCSV(Pasien *pasien, int id) {
    Pasien *pasienList;
    int count = loadPasienFromCSV(&pasienList);
    
    FILE *file = fopen(CSV_PASIEN, "w");
    if (file == NULL) {
        free(pasienList);
        return -1;
    }
    
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (pasienList[i].id == id) {
            pasienList[i] = *pasien;
            found = 1;
        }
        
        char nama[MAX_NAMA * 2], alamat[MAX_ALAMAT * 2], riwayat[MAX_RIWAYAT * 2];
        escapeCSV(nama, pasienList[i].nama, sizeof(nama));
        escapeCSV(alamat, pasienList[i].alamat, sizeof(alamat));
        escapeCSV(riwayat, pasienList[i].riwayatPenyakit, sizeof(riwayat));
        
        fprintf(file, "%d,%s,%s,%s,%s,%d,%s,%s,%s,%s,%s\n",
                pasienList[i].id, nama, pasienList[i].nik, pasienList[i].jenisKelamin,
                pasienList[i].tanggalLahir, pasienList[i].usia, alamat,
                pasienList[i].telepon, riwayat, pasienList[i].tanggalRegistrasi,
                pasienList[i].status);
    }
    
    fclose(file);
    free(pasienList);
    return found ? 0 : -1;
}

int deletePasienFromCSV(int id) {
    Pasien *pasienList;
    int count = loadPasienFromCSV(&pasienList);
    
    FILE *file = fopen(CSV_PASIEN, "w");
    if (file == NULL) {
        free(pasienList);
        return -1;
    }
    
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (pasienList[i].id == id) {
            found = 1;
            continue;  // Skip this record
        }
        
        char nama[MAX_NAMA * 2], alamat[MAX_ALAMAT * 2], riwayat[MAX_RIWAYAT * 2];
        escapeCSV(nama, pasienList[i].nama, sizeof(nama));
        escapeCSV(alamat, pasienList[i].alamat, sizeof(alamat));
        escapeCSV(riwayat, pasienList[i].riwayatPenyakit, sizeof(riwayat));
        
        fprintf(file, "%d,%s,%s,%s,%s,%d,%s,%s,%s,%s,%s\n",
                pasienList[i].id, nama, pasienList[i].nik, pasienList[i].jenisKelamin,
                pasienList[i].tanggalLahir, pasienList[i].usia, alamat,
                pasienList[i].telepon, riwayat, pasienList[i].tanggalRegistrasi,
                pasienList[i].status);
    }
    
    fclose(file);
    free(pasienList);
    return found ? 0 : -1;
}

// ==================== REKAM MEDIS CSV FUNCTIONS ====================

int saveRekamMedisToCSV(RekamMedis *rekamMedis) {
    FILE *file = fopen(CSV_REKAM_MEDIS, "a");
    if (file == NULL) {
        return -1;
    }
    
    char keluhan[MAX_KELUHAN * 2], diagnosis[MAX_DIAGNOSIS * 2];
    char catatan[MAX_CATATAN * 2], tindakan[MAX_TINDAKAN * 2];
    
    escapeCSV(keluhan, rekamMedis->keluhan, sizeof(keluhan));
    escapeCSV(diagnosis, rekamMedis->diagnosis, sizeof(diagnosis));
    escapeCSV(catatan, rekamMedis->catatanDokter, sizeof(catatan));
    escapeCSV(tindakan, rekamMedis->tindakan, sizeof(tindakan));
    
    fprintf(file, "%d,%d,%s,%s,%s,%s,%s,%s\n",
            rekamMedis->id, rekamMedis->idPasien, rekamMedis->tanggalKunjungan,
            keluhan, diagnosis, catatan, tindakan, rekamMedis->tanggalKontrol);
    
    fclose(file);
    return 0;
}

int loadRekamMedisFromCSV(RekamMedis **rekamMedisList) {
    FILE *file = fopen(CSV_REKAM_MEDIS, "r");
    if (file == NULL) {
        *rekamMedisList = NULL;
        return 0;
    }
    
    int count = 0;
    int capacity = 10;
    *rekamMedisList = (RekamMedis*)malloc(capacity * sizeof(RekamMedis));
    
    char line[4096];
    while (fgets(line, sizeof(line), file)) {
        if (count >= capacity) {
            capacity *= 2;
            *rekamMedisList = (RekamMedis*)realloc(*rekamMedisList, capacity * sizeof(RekamMedis));
        }
        
        RekamMedis *rm = &(*rekamMedisList)[count];
        char keluhan[MAX_KELUHAN * 2], diagnosis[MAX_DIAGNOSIS * 2];
        char catatan[MAX_CATATAN * 2], tindakan[MAX_TINDAKAN * 2];
        
        if (sscanf(line, "%d,%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",
                   &rm->id, &rm->idPasien, rm->tanggalKunjungan,
                   keluhan, diagnosis, catatan, tindakan, rm->tanggalKontrol) == 8) {
            
            unescapeCSV(rm->keluhan, keluhan, MAX_KELUHAN);
            unescapeCSV(rm->diagnosis, diagnosis, MAX_DIAGNOSIS);
            unescapeCSV(rm->catatanDokter, catatan, MAX_CATATAN);
            unescapeCSV(rm->tindakan, tindakan, MAX_TINDAKAN);
            count++;
        }
    }
    
    fclose(file);
    return count;
}

// ==================== RESEP CSV FUNCTIONS ====================

int saveResepToCSV(Resep *resep) {
    FILE *file = fopen(CSV_RESEP, "a");
    if (file == NULL) {
        return -1;
    }
    
    char namaObat[MAX_NAMA_OBAT * 2], catatan[MAX_CATATAN_RESEP * 2];
    escapeCSV(namaObat, resep->namaObat, sizeof(namaObat));
    escapeCSV(catatan, resep->catatan, sizeof(catatan));
    
    fprintf(file, "%d,%d,%d,%s,%s,%s,%s,%s,%s,%s\n",
            resep->id, resep->idPasien, resep->idRekamMedis, resep->tanggalResep,
            namaObat, resep->dosis, resep->frekuensi, resep->durasi,
            resep->aturanPakai, catatan);
    
    fclose(file);
    return 0;
}

int loadResepFromCSV(Resep **resepList) {
    FILE *file = fopen(CSV_RESEP, "r");
    if (file == NULL) {
        *resepList = NULL;
        return 0;
    }
    
    int count = 0;
    int capacity = 10;
    *resepList = (Resep*)malloc(capacity * sizeof(Resep));
    
    char line[2048];
    while (fgets(line, sizeof(line), file)) {
        if (count >= capacity) {
            capacity *= 2;
            *resepList = (Resep*)realloc(*resepList, capacity * sizeof(Resep));
        }
        
        Resep *r = &(*resepList)[count];
        char namaObat[MAX_NAMA_OBAT * 2], catatan[MAX_CATATAN_RESEP * 2];
        
        if (sscanf(line, "%d,%d,%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",
                   &r->id, &r->idPasien, &r->idRekamMedis, r->tanggalResep,
                   namaObat, r->dosis, r->frekuensi, r->durasi,
                   r->aturanPakai, catatan) == 10) {
            
            unescapeCSV(r->namaObat, namaObat, MAX_NAMA_OBAT);
            unescapeCSV(r->catatan, catatan, MAX_CATATAN_RESEP);
            count++;
        }
    }
    
    fclose(file);
    return count;
}

// ==================== CONFIG FUNCTIONS ====================

int saveConfig(const char *key, const char *value) {
    // Load existing config
    FILE *file = fopen(CSV_CONFIG, "r");
    char lines[100][256];
    int count = 0;
    int found = 0;
    
    if (file != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), file) && count < 100) {
            char existingKey[128];
            if (sscanf(line, "%[^,]", existingKey) == 1) {
                if (strcmp(existingKey, key) == 0) {
                    snprintf(lines[count], sizeof(lines[count]), "%s,%s\n", key, value);
                    found = 1;
                } else {
                    strncpy(lines[count], line, sizeof(lines[count]));
                }
                count++;
            }
        }
        fclose(file);
    }
    
    // Write back
    file = fopen(CSV_CONFIG, "w");
    if (file == NULL) {
        return -1;
    }
    
    for (int i = 0; i < count; i++) {
        fputs(lines[i], file);
    }
    
    if (!found) {
        fprintf(file, "%s,%s\n", key, value);
    }
    
    fclose(file);
    return 0;
}

char* loadConfig(const char *key) {
    FILE *file = fopen(CSV_CONFIG, "r");
    if (file == NULL) {
        return NULL;
    }
    
    char line[256];
    static char value[128];
    
    while (fgets(line, sizeof(line), file)) {
        char existingKey[128];
        if (sscanf(line, "%[^,],%[^\n]", existingKey, value) == 2) {
            if (strcmp(existingKey, key) == 0) {
                fclose(file);
                return value;
            }
        }
    }
    
    fclose(file);
    return NULL;
}


// Update rekam medis di CSV
int updateRekamMedisInCSV(RekamMedis *rekamMedis, int id) {
    RekamMedis *daftarRekamMedis;
    int count = loadRekamMedisFromCSV(&daftarRekamMedis);
    
    FILE *file = fopen(CSV_REKAM_MEDIS, "w");
    if (file == NULL) {
        free(daftarRekamMedis);
        return -1;
    }
    
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (daftarRekamMedis[i].id == id) {
            daftarRekamMedis[i] = *rekamMedis;
            found = 1;
        }
        
        char keluhan[MAX_KELUHAN * 2], diagnosis[MAX_DIAGNOSIS * 2];
        char catatan[MAX_CATATAN * 2], tindakan[MAX_TINDAKAN * 2];
        
        escapeCSV(keluhan, daftarRekamMedis[i].keluhan, sizeof(keluhan));
        escapeCSV(diagnosis, daftarRekamMedis[i].diagnosis, sizeof(diagnosis));
        escapeCSV(catatan, daftarRekamMedis[i].catatanDokter, sizeof(catatan));
        escapeCSV(tindakan, daftarRekamMedis[i].tindakan, sizeof(tindakan));
        
        fprintf(file, "%d,%d,%s,%s,%s,%s,%s,%s\n",
                daftarRekamMedis[i].id, daftarRekamMedis[i].idPasien,
                daftarRekamMedis[i].tanggalKunjungan, keluhan, diagnosis,
                catatan, tindakan, daftarRekamMedis[i].tanggalKontrol);
    }
    
    fclose(file);
    free(daftarRekamMedis);
    return found ? 0 : -1;
}

// Delete rekam medis dari CSV
int deleteRekamMedisFromCSV(int id) {
    RekamMedis *daftarRekamMedis;
    int count = loadRekamMedisFromCSV(&daftarRekamMedis);
    
    FILE *file = fopen(CSV_REKAM_MEDIS, "w");
    if (file == NULL) {
        free(daftarRekamMedis);
        return -1;
    }
    
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (daftarRekamMedis[i].id == id) {
            found = 1;
            continue;
        }
        
        char keluhan[MAX_KELUHAN * 2], diagnosis[MAX_DIAGNOSIS * 2];
        char catatan[MAX_CATATAN * 2], tindakan[MAX_TINDAKAN * 2];
        
        escapeCSV(keluhan, daftarRekamMedis[i].keluhan, sizeof(keluhan));
        escapeCSV(diagnosis, daftarRekamMedis[i].diagnosis, sizeof(diagnosis));
        escapeCSV(catatan, daftarRekamMedis[i].catatanDokter, sizeof(catatan));
        escapeCSV(tindakan, daftarRekamMedis[i].tindakan, sizeof(tindakan));
        
        fprintf(file, "%d,%d,%s,%s,%s,%s,%s,%s\n",
                daftarRekamMedis[i].id, daftarRekamMedis[i].idPasien,
                daftarRekamMedis[i].tanggalKunjungan, keluhan, diagnosis,
                catatan, tindakan, daftarRekamMedis[i].tanggalKontrol);
    }
    
    fclose(file);
    free(daftarRekamMedis);
    return found ? 0 : -1;
}

// Update resep di CSV
int updateResepInCSV(Resep *resep, int id) {
    Resep *daftarResep;
    int count = loadResepFromCSV(&daftarResep);
    
    FILE *file = fopen(CSV_RESEP, "w");
    if (file == NULL) {
        free(daftarResep);
        return -1;
    }
    
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (daftarResep[i].id == id) {
            daftarResep[i] = *resep;
            found = 1;
        }
        
        char namaObat[MAX_NAMA_OBAT * 2], catatan[MAX_CATATAN_RESEP * 2];
        escapeCSV(namaObat, daftarResep[i].namaObat, sizeof(namaObat));
        escapeCSV(catatan, daftarResep[i].catatan, sizeof(catatan));
        
        fprintf(file, "%d,%d,%d,%s,%s,%s,%s,%s,%s,%s\n",
                daftarResep[i].id, daftarResep[i].idPasien, daftarResep[i].idRekamMedis,
                daftarResep[i].tanggalResep, namaObat, daftarResep[i].dosis,
                daftarResep[i].frekuensi, daftarResep[i].durasi,
                daftarResep[i].aturanPakai, catatan);
    }
    
    fclose(file);
    free(daftarResep);
    return found ? 0 : -1;
}

// Delete resep dari CSV
int deleteResepFromCSV(int id) {
    Resep *daftarResep;
    int count = loadResepFromCSV(&daftarResep);
    
    FILE *file = fopen(CSV_RESEP, "w");
    if (file == NULL) {
        free(daftarResep);
        return -1;
    }
    
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (daftarResep[i].id == id) {
            found = 1;
            continue;
        }
        
        char namaObat[MAX_NAMA_OBAT * 2], catatan[MAX_CATATAN_RESEP * 2];
        escapeCSV(namaObat, daftarResep[i].namaObat, sizeof(namaObat));
        escapeCSV(catatan, daftarResep[i].catatan, sizeof(catatan));
        
        fprintf(file, "%d,%d,%d,%s,%s,%s,%s,%s,%s,%s\n",
                daftarResep[i].id, daftarResep[i].idPasien, daftarResep[i].idRekamMedis,
                daftarResep[i].tanggalResep, namaObat, daftarResep[i].dosis,
                daftarResep[i].frekuensi, daftarResep[i].durasi,
                daftarResep[i].aturanPakai, catatan);
    }
    
    fclose(file);
    free(daftarResep);
    return found ? 0 : -1;
}
