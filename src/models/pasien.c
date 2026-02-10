#include "pasien.h"
#include "../utils/csv_handler.h"
#include "../utils/validator.h"
#include "../utils/date_utils.h"
#include "../utils/string_utils.h"
#include <stdlib.h>
#include <string.h>

// Tambah data pasien baru ke sistem
int tambahPasien(Pasien *pasien) {
    if (pasien == NULL) return -1;
    
    // Validasi input sebelum disimpan
    if (!validateNama(pasien->nama)) return -2;
    if (!validateNIK(pasien->nik)) return -3;
    if (!validateJenisKelamin(pasien->jenisKelamin)) return -4;
    if (!validateTanggal(pasien->tanggalLahir)) return -5;
    if (!validateTelepon(pasien->telepon)) return -6;
    
    // Generate ID otomatis
    pasien->id = getNextId(CSV_PASIEN);
    
    // Set tanggal registrasi hari ini
    dapatkanTanggalSekarang(pasien->tanggalRegistrasi, sizeof(pasien->tanggalRegistrasi));
    
    // Hitung usia dari tanggal lahir
    pasien->usia = hitungUsia(pasien->tanggalLahir);
    
    // Simpan ke file CSV
    return savePasienToCSV(pasien);
}

// Update data pasien yang sudah ada
int updatePasien(Pasien *pasien, int id) {
    if (pasien == NULL) return -1;
    
    // Validasi input
    if (!validateNama(pasien->nama)) return -2;
    if (!validateNIK(pasien->nik)) return -3;
    if (!validateJenisKelamin(pasien->jenisKelamin)) return -4;
    if (!validateTanggal(pasien->tanggalLahir)) return -5;
    if (!validateTelepon(pasien->telepon)) return -6;
    
    // Update usia
    pasien->usia = hitungUsia(pasien->tanggalLahir);
    pasien->id = id;
    
    return updatePasienInCSV(pasien, id);
}

// Hapus data pasien dari sistem
int hapusPasien(int id) {
    return deletePasienFromCSV(id);
}

// Cari pasien berdasarkan ID
Pasien* cariPasienById(int id) {
    Pasien *daftarPasien;
    int jumlah = loadPasienFromCSV(&daftarPasien);
    
    // Linear search untuk cari ID
    for (int i = 0; i < jumlah; i++) {
        if (daftarPasien[i].id == id) {
            Pasien *hasil = (Pasien*)malloc(sizeof(Pasien));
            *hasil = daftarPasien[i];
            free(daftarPasien);
            return hasil;
        }
    }
    
    free(daftarPasien);
    return NULL;
}

// Cari pasien berdasarkan nama (case-insensitive)
Pasien* cariPasienByNama(const char *nama) {
    if (nama == NULL) return NULL;
    
    Pasien *daftarPasien;
    int jumlah = loadPasienFromCSV(&daftarPasien);
    
    // Linear search O(n) - cocok untuk data tidak terlalu besar
    for (int i = 0; i < jumlah; i++) {
        if (bandingkanStringIgnoreCase(daftarPasien[i].nama, nama) == 0) {
            Pasien *hasil = (Pasien*)malloc(sizeof(Pasien));
            *hasil = daftarPasien[i];
            free(daftarPasien);
            return hasil;
        }
    }
    
    free(daftarPasien);
    return NULL;
}

// Cari pasien berdasarkan NIK
Pasien* cariPasienByNIK(const char *nik) {
    if (nik == NULL) return NULL;
    
    Pasien *daftarPasien;
    int jumlah = loadPasienFromCSV(&daftarPasien);
    
    for (int i = 0; i < jumlah; i++) {
        if (strcmp(daftarPasien[i].nik, nik) == 0) {
            Pasien *hasil = (Pasien*)malloc(sizeof(Pasien));
            *hasil = daftarPasien[i];
            free(daftarPasien);
            return hasil;
        }
    }
    
    free(daftarPasien);
    return NULL;
}

// Ambil semua data pasien
int getAllPasien(Pasien **daftarPasien) {
    return loadPasienFromCSV(daftarPasien);
}

// Hitung total pasien terdaftar
int hitungTotalPasien() {
    Pasien *daftarPasien;
    int jumlah = loadPasienFromCSV(&daftarPasien);
    free(daftarPasien);
    return jumlah;
}

// Hitung pasien dengan status aktif
int hitungPasienAktif() {
    Pasien *daftarPasien;
    int jumlah = loadPasienFromCSV(&daftarPasien);
    int aktif = 0;
    
    for (int i = 0; i < jumlah; i++) {
        if (strcmp(daftarPasien[i].status, "Aktif") == 0) {
            aktif++;
        }
    }
    
    free(daftarPasien);
    return aktif;
}

// Urutkan pasien berdasarkan nama (Bubble Sort O(n²))
// Cocok untuk data kecil-menengah, mudah dipahami
void urutkanPasienByNama(Pasien *daftarPasien, int jumlah) {
    if (daftarPasien == NULL || jumlah <= 0) return;
    
    for (int i = 0; i < jumlah - 1; i++) {
        for (int j = 0; j < jumlah - i - 1; j++) {
            if (strcmp(daftarPasien[j].nama, daftarPasien[j + 1].nama) > 0) {
                // Tukar posisi
                Pasien temp = daftarPasien[j];
                daftarPasien[j] = daftarPasien[j + 1];
                daftarPasien[j + 1] = temp;
            }
        }
    }
}

// Helper function untuk Quick Sort
static int partisiUsia(Pasien *arr, int kiri, int kanan) {
    int pivot = arr[kanan].usia;
    int i = kiri - 1;
    
    for (int j = kiri; j < kanan; j++) {
        if (arr[j].usia <= pivot) {
            i++;
            Pasien temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    
    Pasien temp = arr[i + 1];
    arr[i + 1] = arr[kanan];
    arr[kanan] = temp;
    
    return i + 1;
}

static void quickSortUsia(Pasien *arr, int kiri, int kanan) {
    if (kiri < kanan) {
        int pi = partisiUsia(arr, kiri, kanan);
        quickSortUsia(arr, kiri, pi - 1);
        quickSortUsia(arr, pi + 1, kanan);
    }
}

// Urutkan pasien berdasarkan usia (Quick Sort O(n log n))
// Lebih efisien untuk data besar
void urutkanPasienByUsia(Pasien *daftarPasien, int jumlah) {
    if (daftarPasien == NULL || jumlah <= 0) return;
    quickSortUsia(daftarPasien, 0, jumlah - 1);
}

// Urutkan pasien berdasarkan tanggal registrasi
void urutkanPasienByTanggal(Pasien *daftarPasien, int jumlah) {
    if (daftarPasien == NULL || jumlah <= 0) return;
    
    // Bubble sort - sederhana dan cukup untuk kebutuhan ini
    for (int i = 0; i < jumlah - 1; i++) {
        for (int j = 0; j < jumlah - i - 1; j++) {
            if (bandingkanTanggal(daftarPasien[j].tanggalRegistrasi, 
                           daftarPasien[j + 1].tanggalRegistrasi) > 0) {
                Pasien temp = daftarPasien[j];
                daftarPasien[j] = daftarPasien[j + 1];
                daftarPasien[j + 1] = temp;
            }
        }
    }
}
