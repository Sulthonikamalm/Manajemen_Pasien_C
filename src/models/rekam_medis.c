#include "rekam_medis.h"
#include "../utils/csv_handler.h"
#include "../utils/validator.h"
#include "../utils/date_utils.h"
#include <stdlib.h>
#include <string.h>

// Tambah catatan rekam medis baru
int tambahRekamMedis(RekamMedis *rekamMedis) {
    if (rekamMedis == NULL) return -1;
    
    // Validasi input
    if (!validateTanggal(rekamMedis->tanggalKunjungan)) return -2;
    if (!validateNotEmpty(rekamMedis->keluhan)) return -3;
    if (!validateNotEmpty(rekamMedis->diagnosis)) return -4;
    
    // Generate ID otomatis
    rekamMedis->id = getNextId(CSV_REKAM_MEDIS);
    
    // Simpan ke CSV
    return saveRekamMedisToCSV(rekamMedis);
}

// Update catatan rekam medis
int updateRekamMedis(RekamMedis *rekamMedis, int id) {
    if (rekamMedis == NULL) return -1;
    
    // Validasi
    if (!validateTanggal(rekamMedis->tanggalKunjungan)) return -2;
    if (!validateNotEmpty(rekamMedis->keluhan)) return -3;
    if (!validateNotEmpty(rekamMedis->diagnosis)) return -4;
    
    rekamMedis->id = id;
    return updateRekamMedisInCSV(rekamMedis, id);
}

// Hapus catatan rekam medis
int hapusRekamMedis(int id) {
    return deleteRekamMedisFromCSV(id);
}

// Cari rekam medis berdasarkan ID
RekamMedis* cariRekamMedisById(int id) {
    RekamMedis *daftarRekamMedis;
    int jumlah = loadRekamMedisFromCSV(&daftarRekamMedis);
    
    for (int i = 0; i < jumlah; i++) {
        if (daftarRekamMedis[i].id == id) {
            RekamMedis *hasil = (RekamMedis*)malloc(sizeof(RekamMedis));
            *hasil = daftarRekamMedis[i];
            free(daftarRekamMedis);
            return hasil;
        }
    }
    
    free(daftarRekamMedis);
    return NULL;
}

// Ambil semua rekam medis untuk pasien tertentu
int getRekamMedisByPasien(int idPasien, RekamMedis **hasilList) {
    RekamMedis *semuaRekamMedis;
    int totalJumlah = loadRekamMedisFromCSV(&semuaRekamMedis);
    
    // Hitung berapa rekam medis untuk pasien ini
    int jumlahHasil = 0;
    for (int i = 0; i < totalJumlah; i++) {
        if (semuaRekamMedis[i].idPasien == idPasien) {
            jumlahHasil++;
        }
    }
    
    if (jumlahHasil == 0) {
        free(semuaRekamMedis);
        *hasilList = NULL;
        return 0;
    }
    
    // Alokasi memory untuk hasil
    *hasilList = (RekamMedis*)malloc(jumlahHasil * sizeof(RekamMedis));
    
    // Copy data yang sesuai
    int idx = 0;
    for (int i = 0; i < totalJumlah; i++) {
        if (semuaRekamMedis[i].idPasien == idPasien) {
            (*hasilList)[idx++] = semuaRekamMedis[i];
        }
    }
    
    free(semuaRekamMedis);
    return jumlahHasil;
}

// Ambil semua rekam medis
int getAllRekamMedis(RekamMedis **daftarRekamMedis) {
    return loadRekamMedisFromCSV(daftarRekamMedis);
}

// Hitung total kunjungan
int hitungTotalKunjungan() {
    RekamMedis *daftarRekamMedis;
    int jumlah = loadRekamMedisFromCSV(&daftarRekamMedis);
    free(daftarRekamMedis);
    return jumlah;
}

// Hitung kunjungan bulan ini
int hitungKunjunganBulanIni() {
    RekamMedis *daftarRekamMedis;
    int jumlah = loadRekamMedisFromCSV(&daftarRekamMedis);
    int kunjunganBulanIni = 0;
    
    for (int i = 0; i < jumlah; i++) {
        if (adalahTanggalDiBulanIni(daftarRekamMedis[i].tanggalKunjungan)) {
            kunjunganBulanIni++;
        }
    }
    
    free(daftarRekamMedis);
    return kunjunganBulanIni;
}
