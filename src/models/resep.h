#ifndef RESEP_H
#define RESEP_H

#define MAX_NAMA_OBAT 100
#define MAX_DOSIS 50
#define MAX_FREKUENSI 50
#define MAX_DURASI 50
#define MAX_ATURAN 100
#define MAX_CATATAN_RESEP 500

// Struct untuk resep obat
typedef struct {
    int id;
    int idPasien;
    int idRekamMedis;
    char tanggalResep[15];  // Format: DD/MM/YYYY
    char namaObat[MAX_NAMA_OBAT];
    char dosis[MAX_DOSIS];
    char frekuensi[MAX_FREKUENSI];
    char durasi[MAX_DURASI];
    char aturanPakai[MAX_ATURAN];
    char catatan[MAX_CATATAN_RESEP];
} Resep;

// Function declarations
int tambahResep(Resep *resep);
int updateResep(Resep *resep, int id);
int hapusResep(int id);
Resep* cariResepById(int id);
int getResepByPasien(int idPasien, Resep **resepList);
int getResepByRekamMedis(int idRekamMedis, Resep **resepList);
int getAllResep(Resep **resepList);
int hitungTotalResep();
int hitungResepBulanIni();

#endif
