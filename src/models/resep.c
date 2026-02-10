#include "resep.h"
#include "../utils/csv_handler.h"
#include "../utils/validator.h"
#include "../utils/date_utils.h"
#include <stdlib.h>
#include <string.h>

// Tambah resep obat baru
int tambahResep(Resep *resep) {
    if (resep == NULL) return -1;
    
    // Validasi input
    if (!validateTanggal(resep->tanggalResep)) return -2;
    if (!validateNotEmpty(resep->namaObat)) return -3;
    if (!validateNotEmpty(resep->dosis)) return -4;
    if (!validateNotEmpty(resep->frekuensi)) return -5;
    
    // Generate ID otomatis
    resep->id = getNextId(CSV_RESEP);
    
    // Simpan ke CSV
    return saveResepToCSV(resep);
}

// Update resep obat
int updateResep(Resep *resep, int id) {
    if (resep == NULL) return -1;
    
    // Validasi
    if (!validateTanggal(resep->tanggalResep)) return -2;
    if (!validateNotEmpty(resep->namaObat)) return -3;
    if (!validateNotEmpty(resep->dosis)) return -4;
    if (!validateNotEmpty(resep->frekuensi)) return -5;
    
    resep->id = id;
    return updateResepInCSV(resep, id);
}

// Hapus resep obat
int hapusResep(int id) {
    return deleteResepFromCSV(id);
}

// Cari resep berdasarkan ID
Resep* cariResepById(int id) {
    Resep *daftarResep;
    int jumlah = loadResepFromCSV(&daftarResep);
    
    for (int i = 0; i < jumlah; i++) {
        if (daftarResep[i].id == id) {
            Resep *hasil = (Resep*)malloc(sizeof(Resep));
            *hasil = daftarResep[i];
            free(daftarResep);
            return hasil;
        }
    }
    
    free(daftarResep);
    return NULL;
}

// Ambil semua resep untuk pasien tertentu
int getResepByPasien(int idPasien, Resep **hasilList) {
    Resep *semuaResep;
    int totalJumlah = loadResepFromCSV(&semuaResep);
    
    // Hitung berapa resep untuk pasien ini
    int jumlahHasil = 0;
    for (int i = 0; i < totalJumlah; i++) {
        if (semuaResep[i].idPasien == idPasien) {
            jumlahHasil++;
        }
    }
    
    if (jumlahHasil == 0) {
        free(semuaResep);
        *hasilList = NULL;
        return 0;
    }
    
    // Alokasi memory untuk hasil
    *hasilList = (Resep*)malloc(jumlahHasil * sizeof(Resep));
    
    // Copy data yang sesuai
    int idx = 0;
    for (int i = 0; i < totalJumlah; i++) {
        if (semuaResep[i].idPasien == idPasien) {
            (*hasilList)[idx++] = semuaResep[i];
        }
    }
    
    free(semuaResep);
    return jumlahHasil;
}

// Ambil resep berdasarkan rekam medis
int getResepByRekamMedis(int idRekamMedis, Resep **hasilList) {
    Resep *semuaResep;
    int totalJumlah = loadResepFromCSV(&semuaResep);
    
    int jumlahHasil = 0;
    for (int i = 0; i < totalJumlah; i++) {
        if (semuaResep[i].idRekamMedis == idRekamMedis) {
            jumlahHasil++;
        }
    }
    
    if (jumlahHasil == 0) {
        free(semuaResep);
        *hasilList = NULL;
        return 0;
    }
    
    *hasilList = (Resep*)malloc(jumlahHasil * sizeof(Resep));
    
    int idx = 0;
    for (int i = 0; i < totalJumlah; i++) {
        if (semuaResep[i].idRekamMedis == idRekamMedis) {
            (*hasilList)[idx++] = semuaResep[i];
        }
    }
    
    free(semuaResep);
    return jumlahHasil;
}

// Ambil semua resep
int getAllResep(Resep **daftarResep) {
    return loadResepFromCSV(daftarResep);
}

// Hitung total resep
int hitungTotalResep() {
    Resep *daftarResep;
    int jumlah = loadResepFromCSV(&daftarResep);
    free(daftarResep);
    return jumlah;
}

// Hitung resep bulan ini
int hitungResepBulanIni() {
    Resep *daftarResep;
    int jumlah = loadResepFromCSV(&daftarResep);
    int resepBulanIni = 0;
    
    for (int i = 0; i < jumlah; i++) {
        if (adalahTanggalDiBulanIni(daftarResep[i].tanggalResep)) {
            resepBulanIni++;
        }
    }
    
    free(daftarResep);
    return resepBulanIni;
}
