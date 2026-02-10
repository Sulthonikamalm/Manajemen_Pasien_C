#ifndef PASIEN_H
#define PASIEN_H

#define MAX_NAMA 100
#define MAX_NIK 20
#define MAX_ALAMAT 200
#define MAX_TELEPON 20
#define MAX_RIWAYAT 500

// Struct untuk data pasien
typedef struct {
    int id;
    char nama[MAX_NAMA];
    char nik[MAX_NIK];
    char jenisKelamin[15];  // "Laki-laki" atau "Perempuan"
    char tanggalLahir[15];  // Format: DD/MM/YYYY
    int usia;
    char alamat[MAX_ALAMAT];
    char telepon[MAX_TELEPON];
    char riwayatPenyakit[MAX_RIWAYAT];
    char tanggalRegistrasi[15];
    char status[15];  // "Aktif" atau "Tidak Aktif"
} Pasien;

// Function declarations
int tambahPasien(Pasien *pasien);
int updatePasien(Pasien *pasien, int id);
int hapusPasien(int id);
Pasien* cariPasienById(int id);
Pasien* cariPasienByNama(const char *nama);
Pasien* cariPasienByNIK(const char *nik);
int getAllPasien(Pasien **pasienList);
int hitungTotalPasien();
int hitungPasienAktif();
void urutkanPasienByNama(Pasien *pasienList, int jumlah);
void urutkanPasienByUsia(Pasien *pasienList, int jumlah);
void urutkanPasienByTanggal(Pasien *pasienList, int jumlah);

#endif
