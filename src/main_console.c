#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "models/pasien.h"
#include "models/rekam_medis.h"
#include "models/resep.h"
#include "utils/csv_handler.h"
#include "utils/date_utils.h"

// Warna untuk console
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

// Mode tema (0 = light, 1 = dark)
int modeTema = 0;

// Fungsi untuk clear screen
void clearScreen() {
    system("cls");
}

// Fungsi untuk set warna console
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Fungsi untuk pause
void pause() {
    printf("\nTekan Enter untuk melanjutkan...");
    getchar();
    getchar();
}

// Fungsi untuk print header
void printHeader() {
    clearScreen();
    setColor(11); // Cyan terang
    printf("================================================================================\n");
    printf("                   SISTEM MANAJEMEN RUMAH SAKIT JIWA SULTHIM\n");
    printf("================================================================================\n");
    setColor(7); // White
}

// Fungsi untuk print box
void printBox(const char *title, const char *content) {
    setColor(10); // Green terang
    printf("\n+------------------------------------------------------------------------------+\n");
    printf("| %-76s |\n", title);
    printf("+------------------------------------------------------------------------------+\n");
    setColor(7);
    printf("| %-76s |\n", content);
    setColor(10);
    printf("+------------------------------------------------------------------------------+\n");
    setColor(7);
}

// Fungsi untuk tampilkan dashboard
void tampilkanDashboard() {
    printHeader();
    
    setColor(14); // Yellow terang
    printf("\n                              DASHBOARD STATISTIK\n\n");
    setColor(7);
    
    // Ambil statistik
    int totalPasien = hitungTotalPasien();
    int pasienAktif = hitungPasienAktif();
    int kunjunganBulan = hitungKunjunganBulanIni();
    int resepBulan = hitungResepBulanIni();
    
    // Tampilkan dalam box
    printf("  +------------------+  +------------------+  +------------------+  +------------------+\n");
    printf("  |  Total Pasien    |  |  Pasien Aktif    |  | Kunjungan Bulan  |  |  Resep Bulan     |\n");
    printf("  |                  |  |                  |  |                  |  |                  |\n");
    
    setColor(11);
    printf("  |       %3d        |  |       %3d        |  |       %3d        |  |       %3d        |\n", 
           totalPasien, pasienAktif, kunjunganBulan, resepBulan);
    setColor(7);
    
    printf("  +------------------+  +------------------+  +------------------+  +------------------+\n");
    
    // Info tambahan
    printf("\n");
    setColor(10);
    printf("  Status Sistem: ");
    setColor(7);
    printf("Online\n");
    
    setColor(10);
    printf("  Database: ");
    setColor(7);
    printf("CSV Files (data/)\n");
    
    setColor(10);
    printf("  Tema: ");
    setColor(7);
    printf("%s\n", modeTema ? "Dark Mode" : "Light Mode");
    
    pause();
}

// Fungsi untuk tampilkan daftar pasien
void tampilkanDaftarPasien() {
    printHeader();
    
    setColor(14);
    printf("\n                              DAFTAR PASIEN\n\n");
    setColor(7);
    
    Pasien *daftarPasien;
    int jumlah = getAllPasien(&daftarPasien);
    
    if (jumlah == 0) {
        setColor(12); // Red
        printf("  Belum ada data pasien.\n");
        setColor(7);
        pause();
        return;
    }
    
    // Header tabel
    printf("  +----+----------------------+---------------+------+---------------+----------+\n");
    printf("  | ID | Nama                 | Jenis Kelamin | Usia | Telepon       | Status   |\n");
    printf("  +----+----------------------+---------------+------+---------------+----------+\n");
    
    // Data
    for (int i = 0; i < jumlah; i++) {
        printf("  | %-2d | %-20s | %-13s | %-4d | %-13s | %-8s |\n",
               daftarPasien[i].id,
               daftarPasien[i].nama,
               daftarPasien[i].jenisKelamin,
               daftarPasien[i].usia,
               daftarPasien[i].telepon,
               daftarPasien[i].status);
    }
    
    printf("  +----+----------------------+---------------+------+---------------+----------+\n");
    printf("\n  Total: %d pasien\n", jumlah);
    
    free(daftarPasien);
    pause();
}

// Fungsi untuk tambah pasien
void tambahPasienBaru() {
    printHeader();
    
    setColor(14);
    printf("\n                              TAMBAH PASIEN BARU\n\n");
    setColor(7);
    
    Pasien pasienBaru;
    
    // Input data
    printf("  Nama Lengkap: ");
    scanf(" %[^\n]s", pasienBaru.nama);
    
    printf("  NIK (16 digit): ");
    scanf("%s", pasienBaru.nik);
    
    printf("  Jenis Kelamin (Laki-laki/Perempuan): ");
    scanf("%s", pasienBaru.jenisKelamin);
    
    printf("  Tanggal Lahir (DD/MM/YYYY): ");
    scanf("%s", pasienBaru.tanggalLahir);
    
    printf("  Alamat: ");
    scanf(" %[^\n]s", pasienBaru.alamat);
    
    printf("  Telepon: ");
    scanf("%s", pasienBaru.telepon);
    
    printf("  Riwayat Penyakit: ");
    scanf(" %[^\n]s", pasienBaru.riwayatPenyakit);
    
    strcpy(pasienBaru.status, "Aktif");
    
    // Simpan
    int hasil = tambahPasien(&pasienBaru);
    
    printf("\n");
    if (hasil == 0) {
        setColor(10); // Green
        printf("  ✓ Data pasien berhasil disimpan!\n");
        printf("  ID Pasien: %d\n", pasienBaru.id);
    } else {
        setColor(12); // Red
        printf("  ✗ Gagal menyimpan data! Kode error: %d\n", hasil);
        printf("  Periksa kembali input Anda.\n");
    }
    setColor(7);
    
    pause();
}

// Fungsi untuk cari pasien
void cariPasienMenu() {
    printHeader();
    
    setColor(14);
    printf("\n                              CARI PASIEN\n\n");
    setColor(7);
    
    printf("  Cari berdasarkan:\n");
    printf("  1. Nama\n");
    printf("  2. NIK\n");
    printf("  3. ID\n");
    printf("\n  Pilihan: ");
    
    int pilihan;
    scanf("%d", &pilihan);
    
    Pasien *hasil = NULL;
    
    switch(pilihan) {
        case 1: {
            char nama[MAX_NAMA];
            printf("\n  Masukkan nama: ");
            scanf(" %[^\n]s", nama);
            hasil = cariPasienByNama(nama);
            break;
        }
        case 2: {
            char nik[MAX_NIK];
            printf("\n  Masukkan NIK: ");
            scanf("%s", nik);
            hasil = cariPasienByNIK(nik);
            break;
        }
        case 3: {
            int id;
            printf("\n  Masukkan ID: ");
            scanf("%d", &id);
            hasil = cariPasienById(id);
            break;
        }
        default:
            setColor(12);
            printf("\n  Pilihan tidak valid!\n");
            setColor(7);
            pause();
            return;
    }
    
    printf("\n");
    if (hasil != NULL) {
        setColor(10);
        printf("  ✓ Pasien ditemukan!\n\n");
        setColor(7);
        
        printf("  +------------------------------------------------------------------------------+\n");
        printf("  | DETAIL PASIEN                                                                |\n");
        printf("  +------------------------------------------------------------------------------+\n");
        printf("  | ID               : %-59d |\n", hasil->id);
        printf("  | Nama             : %-59s |\n", hasil->nama);
        printf("  | NIK              : %-59s |\n", hasil->nik);
        printf("  | Jenis Kelamin    : %-59s |\n", hasil->jenisKelamin);
        printf("  | Tanggal Lahir    : %-59s |\n", hasil->tanggalLahir);
        printf("  | Usia             : %-59d |\n", hasil->usia);
        printf("  | Alamat           : %-59s |\n", hasil->alamat);
        printf("  | Telepon          : %-59s |\n", hasil->telepon);
        printf("  | Riwayat Penyakit : %-59s |\n", hasil->riwayatPenyakit);
        printf("  | Tanggal Daftar   : %-59s |\n", hasil->tanggalRegistrasi);
        printf("  | Status           : %-59s |\n", hasil->status);
        printf("  +------------------------------------------------------------------------------+\n");
        
        free(hasil);
    } else {
        setColor(12);
        printf("  ✗ Pasien tidak ditemukan!\n");
        setColor(7);
    }
    
    pause();
}

// Fungsi untuk urutkan pasien
void urutkanPasienMenu() {
    printHeader();
    
    setColor(14);
    printf("\n                              URUTKAN PASIEN\n\n");
    setColor(7);
    
    printf("  Urutkan berdasarkan:\n");
    printf("  1. Nama (A-Z)\n");
    printf("  2. Usia (Termuda-Tertua)\n");
    printf("  3. Tanggal Registrasi (Terbaru-Terlama)\n");
    printf("\n  Pilihan: ");
    
    int pilihan;
    scanf("%d", &pilihan);
    
    Pasien *daftarPasien;
    int jumlah = getAllPasien(&daftarPasien);
    
    if (jumlah == 0) {
        setColor(12);
        printf("\n  Belum ada data pasien.\n");
        setColor(7);
        pause();
        return;
    }
    
    // Urutkan
    switch(pilihan) {
        case 1:
            urutkanPasienByNama(daftarPasien, jumlah);
            printf("\n  Diurutkan berdasarkan Nama (Bubble Sort O(n²))\n");
            break;
        case 2:
            urutkanPasienByUsia(daftarPasien, jumlah);
            printf("\n  Diurutkan berdasarkan Usia (Quick Sort O(n log n))\n");
            break;
        case 3:
            urutkanPasienByTanggal(daftarPasien, jumlah);
            printf("\n  Diurutkan berdasarkan Tanggal (Bubble Sort O(n²))\n");
            break;
        default:
            setColor(12);
            printf("\n  Pilihan tidak valid!\n");
            setColor(7);
            free(daftarPasien);
            pause();
            return;
    }
    
    // Tampilkan hasil
    printf("\n  +----+----------------------+---------------+------+---------------+----------+\n");
    printf("  | ID | Nama                 | Jenis Kelamin | Usia | Telepon       | Status   |\n");
    printf("  +----+----------------------+---------------+------+---------------+----------+\n");
    
    for (int i = 0; i < jumlah; i++) {
        printf("  | %-2d | %-20s | %-13s | %-4d | %-13s | %-8s |\n",
               daftarPasien[i].id,
               daftarPasien[i].nama,
               daftarPasien[i].jenisKelamin,
               daftarPasien[i].usia,
               daftarPasien[i].telepon,
               daftarPasien[i].status);
    }
    
    printf("  +----+----------------------+---------------+------+---------------+----------+\n");
    
    free(daftarPasien);
    pause();
}

// Fungsi untuk toggle tema
void toggleTema() {
    modeTema = !modeTema;
    saveConfig("theme", modeTema ? "dark" : "light");
    
    if (modeTema) {
        system("color 0F"); // Dark mode (black bg, white text)
    } else {
        system("color F0"); // Light mode (white bg, black text)
    }
    
    printHeader();
    setColor(10);
    printf("\n  ✓ Tema berubah ke: %s\n", modeTema ? "Dark Mode" : "Light Mode");
    setColor(7);
    pause();
}

// Menu utama
void menuUtama() {
    int pilihan;
    
    do {
        printHeader();
        
        setColor(14);
        printf("\n                                  MENU UTAMA\n\n");
        setColor(7);
        
        printf("  1. Dashboard\n");
        printf("  2. Lihat Daftar Pasien\n");
        printf("  3. Tambah Pasien Baru\n");
        printf("  4. Cari Pasien\n");
        printf("  5. Urutkan Pasien\n");
        printf("  6. Toggle Tema (Light/Dark)\n");
        printf("  0. Keluar\n");
        
        printf("\n  Pilihan: ");
        scanf("%d", &pilihan);
        
        switch(pilihan) {
            case 1:
                tampilkanDashboard();
                break;
            case 2:
                tampilkanDaftarPasien();
                break;
            case 3:
                tambahPasienBaru();
                break;
            case 4:
                cariPasienMenu();
                break;
            case 5:
                urutkanPasienMenu();
                break;
            case 6:
                toggleTema();
                break;
            case 0:
                clearScreen();
                setColor(10);
                printf("\n  Terima kasih telah menggunakan sistem ini!\n");
                printf("  Rumah Sakit Jiwa SultHim\n\n");
                setColor(7);
                break;
            default:
                setColor(12);
                printf("\n  Pilihan tidak valid!\n");
                setColor(7);
                pause();
        }
        
    } while(pilihan != 0);
}

// Main function
int main() {
    // Set console untuk support ANSI colors
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    
    // Load tema dari config
    char *tema = loadConfig("theme");
    if (tema && strcmp(tema, "dark") == 0) {
        modeTema = 1;
        system("color 0F");
    } else {
        system("color F0");
    }
    
    // Jalankan menu utama
    menuUtama();
    
    return 0;
}
