#ifndef REKAM_MEDIS_H
#define REKAM_MEDIS_H

#define MAX_KELUHAN 500
#define MAX_DIAGNOSIS 200
#define MAX_CATATAN 1000
#define MAX_TINDAKAN 500

// Struct untuk rekam medis
typedef struct {
    int id;
    int idPasien;
    char tanggalKunjungan[15];  // Format: DD/MM/YYYY
    char keluhan[MAX_KELUHAN];
    char diagnosis[MAX_DIAGNOSIS];
    char catatanDokter[MAX_CATATAN];
    char tindakan[MAX_TINDAKAN];
    char tanggalKontrol[15];
} RekamMedis;

// Function declarations
int tambahRekamMedis(RekamMedis *rekamMedis);
int updateRekamMedis(RekamMedis *rekamMedis, int id);
int hapusRekamMedis(int id);
RekamMedis* cariRekamMedisById(int id);
int getRekamMedisByPasien(int idPasien, RekamMedis **rekamMedisList);
int getAllRekamMedis(RekamMedis **rekamMedisList);
int hitungTotalKunjungan();
int hitungKunjunganBulanIni();

#endif
