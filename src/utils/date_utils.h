#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include <time.h>
#include <stdbool.h>

// Fungsi utilitas tanggal
void dapatkanTanggalSekarang(char *buffer, int ukuran);
void formatTanggal(char *buffer, int ukuran, int hari, int bulan, int tahun);
bool parseTanggal(const char *stringTanggal, int *hari, int *bulan, int *tahun);
int hitungUsia(const char *tanggalLahir);
bool adalahTanggalValid(int hari, int bulan, int tahun);
bool adalahTahunKabisat(int tahun);
int dapatkanJumlahHariDalamBulan(int bulan, int tahun);
int bandingkanTanggal(const char *tanggal1, const char *tanggal2);
bool adalahTanggalDiBulanIni(const char *tanggal);
void tambahkanHari(char *buffer, int ukuran, const char *tanggal, int jumlahHari);

#endif
