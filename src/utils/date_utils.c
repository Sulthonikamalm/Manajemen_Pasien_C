#include "date_utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Dapatkan tanggal saat ini dalam format DD/MM/YYYY
void dapatkanTanggalSekarang(char *buffer, int ukuran) {
    time_t sekarang = time(NULL);
    struct tm *t = localtime(&sekarang);
    snprintf(buffer, ukuran, "%02d/%02d/%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
}

// Format tanggal ke DD/MM/YYYY
void formatTanggal(char *buffer, int ukuran, int hari, int bulan, int tahun) {
    snprintf(buffer, ukuran, "%02d/%02d/%04d", hari, bulan, tahun);
}

// Parse string tanggal DD/MM/YYYY
bool parseTanggal(const char *stringTanggal, int *hari, int *bulan, int *tahun) {
    if (stringTanggal == NULL) return false;
    return sscanf(stringTanggal, "%d/%d/%d", hari, bulan, tahun) == 3;
}

// Cek apakah tahun kabisat
bool adalahTahunKabisat(int tahun) {
    return (tahun % 4 == 0 && tahun % 100 != 0) || (tahun % 400 == 0);
}

// Dapatkan jumlah hari dalam bulan
int dapatkanJumlahHariDalamBulan(int bulan, int tahun) {
    int hari[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (bulan < 1 || bulan > 12) return 0;
    
    if (bulan == 2 && adalahTahunKabisat(tahun)) {
        return 29;
    }
    
    return hari[bulan - 1];
}

// Validasi tanggal
bool adalahTanggalValid(int hari, int bulan, int tahun) {
    if (tahun < 1900 || tahun > 2100) return false;
    if (bulan < 1 || bulan > 12) return false;
    if (hari < 1 || hari > dapatkanJumlahHariDalamBulan(bulan, tahun)) return false;
    return true;
}

// Hitung usia dari tanggal lahir
int hitungUsia(const char *tanggalLahir) {
    int hari, bulan, tahun;
    if (!parseTanggal(tanggalLahir, &hari, &bulan, &tahun)) {
        return -1;
    }
    
    time_t sekarang = time(NULL);
    struct tm *t = localtime(&sekarang);
    
    int tahunSekarang = t->tm_year + 1900;
    int bulanSekarang = t->tm_mon + 1;
    int hariSekarang = t->tm_mday;
    
    int usia = tahunSekarang - tahun;
    
    if (bulanSekarang < bulan || (bulanSekarang == bulan && hariSekarang < hari)) {
        usia--;
    }
    
    return usia;
}

// Bandingkan dua tanggal (return -1 jika tanggal1 < tanggal2, 0 jika sama, 1 jika tanggal1 > tanggal2)
int bandingkanTanggal(const char *tanggal1, const char *tanggal2) {
    int h1, b1, t1, h2, b2, t2;
    
    if (!parseTanggal(tanggal1, &h1, &b1, &t1) || !parseTanggal(tanggal2, &h2, &b2, &t2)) {
        return 0;
    }
    
    if (t1 != t2) return (t1 > t2) ? 1 : -1;
    if (b1 != b2) return (b1 > b2) ? 1 : -1;
    if (h1 != h2) return (h1 > h2) ? 1 : -1;
    
    return 0;
}

// Cek apakah tanggal ada di bulan ini
bool adalahTanggalDiBulanIni(const char *tanggal) {
    int hari, bulan, tahun;
    if (!parseTanggal(tanggal, &hari, &bulan, &tahun)) {
        return false;
    }
    
    time_t sekarang = time(NULL);
    struct tm *t = localtime(&sekarang);
    
    int tahunSekarang = t->tm_year + 1900;
    int bulanSekarang = t->tm_mon + 1;
    
    return (tahun == tahunSekarang && bulan == bulanSekarang);
}

// Tambahkan hari ke tanggal
void tambahkanHari(char *buffer, int ukuran, const char *tanggal, int jumlahHari) {
    int h, b, t;
    if (!parseTanggal(tanggal, &h, &b, &t)) {
        strncpy(buffer, tanggal, ukuran);
        return;
    }
    
    h += jumlahHari;
    
    while (h > dapatkanJumlahHariDalamBulan(b, t)) {
        h -= dapatkanJumlahHariDalamBulan(b, t);
        b++;
        if (b > 12) {
            b = 1;
            t++;
        }
    }
    
    formatTanggal(buffer, ukuran, h, b, t);
}
