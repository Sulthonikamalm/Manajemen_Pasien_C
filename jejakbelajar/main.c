#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

struct Pasien {
    char nama[50];
    char jenisKelamin[10];
    int usia;
    char riwayatPenyakit[100];
};

// Fungsi untuk membersihkan layar konsol
void bersihkanLayar() {
    system(CLEAR_SCREEN);
    printf("================================================================================================\n");
    printf("************************************** Rumah Sakit Jiwa SultHim ********************************\n");
    printf("\t\t\tSelamat Datang!Dirumah sakit kami, ada yang bisa dibantu?\n\n");
}


// Fungsi untuk menambahkan data pasien baru
void tambahPasien(struct Pasien *pasien, int *jumlahPasien) {
    bersihkanLayar();
    printf("=== Tambah Data Pasien ===\n");
    printf("Nama: ");
    scanf(" %[^\n]s", pasien[*jumlahPasien].nama);
    printf("Jenis Kelamin: ");
    scanf("%s", pasien[*jumlahPasien].jenisKelamin);
    printf("Usia: ");
    scanf("%d", &pasien[*jumlahPasien].usia);
    printf("Riwayat Penyakit: ");
    scanf(" %[^\n]s", pasien[*jumlahPasien].riwayatPenyakit);

    (*jumlahPasien)++;
}

// Fungsi untuk menampilkan data pasien
void tampilkanDataPasien(struct Pasien *pasien, int jumlahPasien) {
    bersihkanLayar();
    printf("=== Data Pasien ===\n");
    for (int i = 0; i < jumlahPasien; i++) {
        printf("Nama: %s\n", pasien[i].nama);
        printf("Jenis Kelamin: %s\n", pasien[i].jenisKelamin);
        printf("Usia: %d\n", pasien[i].usia);
        printf("Riwayat Penyakit: %s\n", pasien[i].riwayatPenyakit);
        printf("-------------------\n");
    }
}

// Fungsi pencarian pasien berdasarkan nama
void cariPasien(struct Pasien *pasien, int jumlahPasien, char *namaCari) {
    bersihkanLayar();
    printf("=== Cari Pasien ===\n");
    int ditemukan = 0;
    for (int i = 0; i < jumlahPasien; i++) {
        if (strcmp(pasien[i].nama, namaCari) == 0) {
            printf("Nama: %s\n", pasien[i].nama);
            printf("Jenis Kelamin: %s\n", pasien[i].jenisKelamin);
            printf("Usia: %d\n", pasien[i].usia);
            printf("Riwayat Penyakit: %s\n", pasien[i].riwayatPenyakit);
            ditemukan = 1;
            break;
        }
    }
    if (!ditemukan) {
        printf("Pasien dengan nama %s tidak ditemukan.\n", namaCari);
    }
}

// Fungsi sorting berdasarkan usia (Bubble Sort)
void urutkanBerdasarkanUsia(struct Pasien *pasien, int jumlahPasien) {
    bersihkanLayar();
    printf("=== Data Pasien Setelah Diurutkan ===\n");
    struct Pasien temp;
    for (int i = 0; i < jumlahPasien - 1; i++) {
        for (int j = 0; j < jumlahPasien - i - 1; j++) {
            if (pasien[j].usia > pasien[j + 1].usia) {
                temp = pasien[j];
                pasien[j] = pasien[j + 1];
                pasien[j + 1] = temp;
            }
        }
    }

    // Menampilkan data setelah diurutkan
    tampilkanDataPasien(pasien, jumlahPasien);
}

// Fungsi untuk menyimpan data ke file
void simpanKeFile(struct Pasien *pasien, int jumlahPasien) {
    FILE *file = fopen("data_pasien.txt", "w");
    if (file == NULL) {
        printf("Error: Gagal membuka file untuk penyimpanan.\n");
        return;
    }

    for (int i = 0; i < jumlahPasien; i++) {
        fprintf(file, "%s %s %d %s\n", pasien[i].nama, pasien[i].jenisKelamin, pasien[i].usia, pasien[i].riwayatPenyakit);
    }

    fclose(file);
}

// Fungsi untuk membaca data dari file
int bacaDariFile(struct Pasien *pasien) {
    FILE *file = fopen("data_pasien.txt", "r");
    if (file == NULL) {
        printf("File data_pasien.txt tidak ditemukan.\n");
        return 0;
    }

    int jumlahPasien = 0;
    while (fscanf(file, "%s %s %d %[^\n]s", pasien[jumlahPasien].nama, pasien[jumlahPasien].jenisKelamin, &pasien[jumlahPasien].usia, pasien[jumlahPasien].riwayatPenyakit) == 4) {
        jumlahPasien++;
    }

    fclose(file);
    return jumlahPasien;
}

int main() {
    struct Pasien dataPasien[100]; // Array untuk menyimpan data pasien
    int jumlahPasien = 0; // Jumlah pasien yang sudah dimasukkan

    // Membaca data dari file saat program dimulai
    jumlahPasien = bacaDariFile(dataPasien);

    int pilihan;
    char namaCari[50];

    do {
        bersihkanLayar();
        printf("============================================= Menu =============================================\n");
        printf("1. Tambah Data Pasien\n");
        printf("2. Tampilkan Data Pasien\n");
        printf("3. Cari Pasien\n");
        printf("4. Urutkan Data Pasien berdasarkan Usia\n");
        printf("5. Keluar\n");
        printf("================================================================================================\n");
        printf("Pilih: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                tambahPasien(dataPasien, &jumlahPasien);
                simpanKeFile(dataPasien, jumlahPasien);
                break;
            case 2:
                tampilkanDataPasien(dataPasien, jumlahPasien);
                break;
            case 3:
                printf("\nMasukkan nama pasien yang dicari: ");
                scanf(" %[^\n]s", namaCari);
                cariPasien(dataPasien, jumlahPasien, namaCari);
                break;
            case 4:
                urutkanBerdasarkanUsia(dataPasien, jumlahPasien);
                break;
            case 5:
                printf("Program selesai.\n");
                break;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
        }

        if (pilihan != 5) {
            printf("\nTekan Enter untuk kembali ke menu...");
            printf("\n______________________________________\n");
            getchar(); // Menunggu tombol Enter ditekan
            getchar(); // Menunggu tombol Enter ditekan lagi
        }

    } while (pilihan != 5);

    return 0;
}
