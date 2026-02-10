#include "gui_pages.h"
#include "ui_components.h"
#include "../models/pasien.h"
#include "../models/rekam_medis.h"
#include "../models/resep.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Referensi state eksternal (diambil dari main_gui.c)
extern AppState appState;
extern Pasien *cachedPasien;
extern int cachedPasienCount;
extern int statsTotal;
extern int statsAktif;

// Menampilkan halaman dashboard dengan statistik real-time
void GambarHalamanDashboard(int x, int y, int lebar, int tinggi) {
    (void)tinggi;
    
    Color warnaTeks = appState.isDarkMode ? COL_TEXT_DARK : COL_TEXT_LIGHT;
    Color warnaTeksSekunder = appState.isDarkMode ? COL_TEXT_SEC_DARK : COL_TEXT_SEC_LIGHT;
    
    // Header dengan font lebih besar (BOLD)
    if (appState.fontBold.texture.id != 0) {
        DrawTextEx(appState.fontBold, "Dashboard Overview", (Vector2){x + 40, y + 30}, FONT_HEADER, 1.0f, warnaTeks);
    } else {
        DrawText("Dashboard Overview", x + 40, y + 30, FONT_HEADER, warnaTeks);
    }
    
    UI_DrawText("Statistik Real-time Sistem RS Jiwa SultHim", x + 40, y + 65, FONT_SUBTITLE, warnaTeksSekunder);
    
    int mulaiY = y + 120;
    int lebarKartu = 280;
    int tinggiKartu = 140;
    int jarak = 30;

    // Kartu 1: Total Pasien
    GambarKartuStat((Rectangle){ (float)(x + 40), (float)mulaiY, (float)lebarKartu, (float)tinggiKartu }, 
                 "Total Pasien", 
                 TextFormat("%d", statsTotal), 
                 "Data Terdaftar di Sistem", 
                 COL_PRIMARY);

    // Kartu 2: Pasien Aktif
    GambarKartuStat((Rectangle){ (float)(x + 40 + lebarKartu + jarak), (float)mulaiY, (float)lebarKartu, (float)tinggiKartu }, 
                 "Pasien Aktif", 
                 TextFormat("%d", statsAktif), 
                 "Status Perawatan Aktif", 
                 COL_SECONDARY);

    // Kartu 3: Kunjungan
    GambarKartuStat((Rectangle){ (float)(x + 40 + (lebarKartu + jarak) * 2), (float)mulaiY, (float)lebarKartu, (float)tinggiKartu }, 
                 "Kunjungan Hari Ini", 
                 "0", 
                 "Belum ada data kunjungan", 
                 COL_ACCENT);

    // Bagian Info dengan font lebih besar
    int infoY = mulaiY + tinggiKartu + 50;
    Color warnaKartu = appState.isDarkMode ? COL_CARD_DARK : COL_CARD_LIGHT;
    Rectangle areaInfo = { (float)(x + 40), (float)infoY, (float)(lebar - 80), 250 };
    
    DrawRectangleRounded(areaInfo, 0.1f, 0, warnaKartu);
    
    if (appState.fontBold.texture.id != 0) {
        DrawTextEx(appState.fontBold, "Selamat Datang Administrator", (Vector2){areaInfo.x + 30, areaInfo.y + 30}, FONT_TITLE, 1.0f, warnaTeks);
    } else {
        UI_DrawText("Selamat Datang Administrator", areaInfo.x + 30, areaInfo.y + 30, FONT_TITLE, warnaTeks);
    }
    
    UI_DrawText("Sistem Manajemen Pasien Rumah Sakit Jiwa SultHim", 
             areaInfo.x + 30, areaInfo.y + 70, FONT_BODY, warnaTeksSekunder);
    
    UI_DrawText("Aplikasi ini dirancang untuk kemudahan administrasi dan manajemen data pasien.", 
             areaInfo.x + 30, areaInfo.y + 100, FONT_BODY, warnaTeksSekunder);
             
    UI_DrawText("Versi: 1.0.0 (Beta) - DeepUI Edition", areaInfo.x + 30, areaInfo.y + 210, FONT_SMALL, Fade(warnaTeksSekunder, 0.6f));
}

// Menampilkan halaman daftar pasien dengan tabel interaktif
void GambarHalamanPasien(int x, int y, int lebar, int tinggi) {
    Color warnaTeks = appState.isDarkMode ? COL_TEXT_DARK : COL_TEXT_LIGHT;
    Color warnaTeksSekunder = appState.isDarkMode ? COL_TEXT_SEC_DARK : COL_TEXT_SEC_LIGHT;
    
    // Header dengan font lebih besar
    DrawText("Data Pasien", x + 40, y + 30, FONT_HEADER, warnaTeks);
    
    // Badge jumlah dengan font lebih besar
    char teksJumlah[32];
    snprintf(teksJumlah, 32, "%d Terdaftar", cachedPasienCount);
    float lebarBadge = UI_MeasureText(teksJumlah, FONT_BODY) + 30;
    DrawRectangleRounded((Rectangle){ (float)(x + 250), (float)(y + 35), (float)lebarBadge, 28 }, 0.5f, 0, Fade(COL_PRIMARY, 0.2f));
    UI_DrawText(teksJumlah, x + 265, y + 40, FONT_BODY, COL_PRIMARY);

    UI_DrawText("Daftar lengkap pasien yang terdaftar di rumah sakit.", x + 40, y + 70, FONT_SUBTITLE, warnaTeksSekunder);

    // Tombol Tambah Pasien (Modern & Besar)
    Rectangle tombolTambah = { (float)(lebar - 220), (float)(y + 30), 180, 45 };
    bool hoverTambah = CheckCollisionPointRec(GetMousePosition(), tombolTambah);
    
    Color warnaTombol = hoverTambah ? Fade(COL_SECONDARY, 0.9f) : COL_SECONDARY;
    DrawRectangleRounded(tombolTambah, 0.3f, 0, warnaTombol);
    
    if (appState.fontBold.texture.id != 0) {
        DrawTextEx(appState.fontBold, "+ Tambah Pasien", (Vector2){tombolTambah.x + 20, tombolTambah.y + 12}, FONT_BUTTON, 1.0f, WHITE);
    } else {
        UI_DrawText("+ Tambah Pasien", tombolTambah.x + 20, tombolTambah.y + 12, FONT_BUTTON, WHITE);
    }
    
    if (hoverTambah && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        appState.currentPage = PAGE_PASIEN_ADD;
        return;
    }

    // Kontainer Tabel
    int tabelY = y + 130;
    int tinggiTabel = tinggi - tabelY - 40;
    Color warnaKartu = appState.isDarkMode ? COL_CARD_DARK : COL_CARD_LIGHT;
    
    DrawRectangleRounded((Rectangle){ (float)(x + 40), (float)tabelY, (float)(lebar - 80), (float)tinggiTabel }, 0.1f, 0, warnaKartu);

    // Konfigurasi Kolom dengan spacing lebih lebar
    const char *kolom[] = { "ID", "NAMA LENGKAP", "GENDER", "USIA", "TELEPON", "STATUS", "AKSI" };
    const float offsetKolom[] = { 25, 90, 350, 470, 560, 720, 850 }; 
    
    GambarHeaderTabel((float)(x + 40), (float)(tabelY + 20), (float)(lebar - 80), kolom, offsetKolom, 7);

    // Baris Data dengan font lebih besar
    int barisY = tabelY + 65;
    int tinggiBaris = 50;  // Lebih tinggi untuk readability
    int maksBaris = (tinggiTabel - 80) / tinggiBaris;

    if (cachedPasienCount == 0) {
         DrawText("Tidak ada data pasien.", x + (lebar/2) - 100, barisY + 80, FONT_SUBTITLE, warnaTeksSekunder);
         DrawText("Klik tombol 'Tambah Pasien' untuk menambah data baru.", x + (lebar/2) - 200, barisY + 120, FONT_BODY, Fade(warnaTeksSekunder, 0.7f));
         return;
    }

    // Logika Scroll
    int indeksMulai = appState.scrollOffset;
    if (indeksMulai > cachedPasienCount - maksBaris) indeksMulai = cachedPasienCount - maksBaris;
    if (indeksMulai < 0) indeksMulai = 0;
    
    appState.scrollOffset = indeksMulai;
    
    for (int i = 0; i < maksBaris && (indeksMulai + i) < cachedPasienCount; i++) {
        Pasien p = cachedPasien[indeksMulai + i];
        
        Rectangle areaBaris = { (float)(x + 40), (float)(barisY + (i * tinggiBaris)), (float)(lebar - 80), (float)tinggiBaris };
        bool sedangHover = CheckCollisionPointRec(GetMousePosition(), areaBaris);
        
        GambarLatarBarisTabel(areaBaris, i % 2 == 0, sedangHover);

        // Render Sel dengan font lebih besar
        UI_DrawText(TextFormat("%d", p.id), x + 40 + offsetKolom[0], areaBaris.y + 15, FONT_TABLE, warnaTeksSekunder);
        
        // Nama (Potong jika terlalu panjang)
        char namaTampil[35];
        strncpy(namaTampil, p.nama, 28);
        namaTampil[28] = '\0';
        if (strlen(p.nama) > 28) strcat(namaTampil, "...");
        
        // Nama Bold
        if (appState.fontBold.texture.id != 0) {
            DrawTextEx(appState.fontBold, namaTampil, (Vector2){x + 40 + offsetKolom[1], areaBaris.y + 15}, FONT_TABLE, 1.0f, warnaTeks);
        } else {
            UI_DrawText(namaTampil, x + 40 + offsetKolom[1], areaBaris.y + 15, FONT_TABLE, warnaTeks);
        }

        // Gender (Singkat)
        const char *genderSingkat = (strcmp(p.jenisKelamin, "Laki-laki") == 0) ? "L" : "P"; 
        UI_DrawText(genderSingkat, x + 40 + offsetKolom[2], areaBaris.y + 15, FONT_TABLE, warnaTeks);
        
        // Usia
        UI_DrawText(TextFormat("%d Thn", p.usia), x + 40 + offsetKolom[3], areaBaris.y + 15, FONT_TABLE, warnaTeks);
        
        // Telepon
        UI_DrawText(p.telepon, x + 40 + offsetKolom[4], areaBaris.y + 15, FONT_TABLE, warnaTeks);
        
        // Status Badge
        Color warnaBadge = (strcmp(p.status, "Aktif") == 0) ? COL_SECONDARY : COL_TEXT_SEC_LIGHT;
        UI_DrawText(p.status, x + 40 + offsetKolom[5], areaBaris.y + 15, FONT_TABLE, warnaBadge);
        
        // Tombol Aksi (Edit & Delete) - Perlebar ukuran untuk teks
        Rectangle tombolEdit = { (float)(x + 40 + offsetKolom[6]), (float)(areaBaris.y + 10), 45, 30 };
        Rectangle tombolHapus = { (float)(x + 40 + offsetKolom[6] + 55), (float)(areaBaris.y + 10), 45, 30 };
        
        bool hoverEdit = CheckCollisionPointRec(GetMousePosition(), tombolEdit);
        bool hoverHapus = CheckCollisionPointRec(GetMousePosition(), tombolHapus);
        
        // Tombol Edit
        DrawRectangleRounded(tombolEdit, 0.3f, 0, hoverEdit ? Fade(COL_PRIMARY, 0.3f) : Fade(COL_PRIMARY, 0.1f));
        UI_DrawText("EDIT", tombolEdit.x + 8, tombolEdit.y + 8, 12, COL_PRIMARY); // Ganti Icon dgn Teks
        
        // Tombol Hapus
        DrawRectangleRounded(tombolHapus, 0.3f, 0, hoverHapus ? Fade(RED, 0.3f) : Fade(RED, 0.1f));
        UI_DrawText("HPS", tombolHapus.x + 10, tombolHapus.y + 8, 12, RED); // Ganti Icon dgn Teks
        
        if (hoverEdit && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // Load data pasien untuk edit
            Pasien *pasienEdit = cariPasienById(p.id);
            if (pasienEdit) {
                appState.formPasien.editingId = pasienEdit->id;
                strcpy(appState.formPasien.nama, pasienEdit->nama);
                strcpy(appState.formPasien.nik, pasienEdit->nik);
                strcpy(appState.formPasien.jenisKelamin, pasienEdit->jenisKelamin);
                strcpy(appState.formPasien.tanggalLahir, pasienEdit->tanggalLahir);
                strcpy(appState.formPasien.alamat, pasienEdit->alamat);
                strcpy(appState.formPasien.telepon, pasienEdit->telepon);
                strcpy(appState.formPasien.riwayatPenyakit, pasienEdit->riwayatPenyakit);
                strcpy(appState.formPasien.status, pasienEdit->status);
                free(pasienEdit);
                appState.currentPage = PAGE_PASIEN_EDIT;
            }
        }
        
        if (hoverHapus && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // Tampilkan dialog konfirmasi
            appState.dialogState = DIALOG_DELETE_CONFIRM;
            appState.dialogTargetId = p.id;
            snprintf(appState.dialogMessage, sizeof(appState.dialogMessage), 
                     "Apakah Anda yakin ingin menghapus data pasien '%s'?", p.nama);
        }
    }
    
    // Pagination info
    DrawText(TextFormat("Menampilkan %d - %d dari %d pasien", 
             indeksMulai + 1, 
             (indeksMulai + maksBaris > cachedPasienCount) ? cachedPasienCount : indeksMulai + maksBaris,
             cachedPasienCount), 
             x + 60, barisY + (maksBaris * tinggiBaris) + 15, FONT_SMALL, warnaTeksSekunder);
}

// Menampilkan halaman placeholder untuk fitur yang sedang dikembangkan
void GambarHalamanSegeraHadir(int x, int y, int lebar, int tinggi, const char *judul) {
    (void)lebar;   // Menandai parameter tidak digunakan
    (void)tinggi;  // Menandai parameter tidak digunakan
    
    Color warnaTeks = appState.isDarkMode ? COL_TEXT_DARK : COL_TEXT_LIGHT;
    DrawText(judul, x + 40, y + 30, 24, warnaTeks);
    
    DrawText("Fitur ini sedang dalam pengembangan.", x + 40, y + 100, 16, Fade(warnaTeks, 0.6f));
}


// Forward declaration
extern void MuatDataPasien();

// Menampilkan form tambah pasien baru
void GambarHalamanTambahPasien(int x, int y, int lebar, int tinggi) {
    (void)tinggi;
    
    Color warnaTeks = appState.isDarkMode ? COL_TEXT_DARK : COL_TEXT_LIGHT;
    Color warnaTeksSekunder = appState.isDarkMode ? COL_TEXT_SEC_DARK : COL_TEXT_SEC_LIGHT;
    Color warnaKartu = appState.isDarkMode ? COL_CARD_DARK : COL_CARD_LIGHT;
    
    // Header
    DrawText("Tambah Pasien Baru", x + 40, y + 30, FONT_HEADER, warnaTeks);
    DrawText("Lengkapi form di bawah untuk menambahkan data pasien baru", x + 40, y + 70, FONT_SUBTITLE, warnaTeksSekunder);
    
    // Form Container
    int formY = y + 130;
    Rectangle formArea = { (float)(x + 40), (float)formY, (float)(lebar - 80), 800 };
    DrawRectangleRounded(formArea, 0.1f, 0, warnaKartu);
    
    int inputX = (int)formArea.x + 40;
    int inputY = (int)formArea.y + 40;
    int inputWidth = 500;
    int inputHeight = 45;
    int spacing = 70;
    
    // Tombol Aksi (definisi dulu untuk cek klik)
    int tombolY = (int)formArea.y + 720;
    Rectangle tombolBatal = { (float)(inputX), (float)tombolY, 150, 50 };
    Rectangle tombolSimpan = { (float)(inputX + 170), (float)tombolY, 150, 50 };
    
    bool hoverBatal = CheckCollisionPointRec(GetMousePosition(), tombolBatal);
    bool hoverSimpan = CheckCollisionPointRec(GetMousePosition(), tombolSimpan);
    
    // Unfocus input jika klik tombol atau klik di luar form
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (hoverBatal || hoverSimpan || !CheckCollisionPointRec(GetMousePosition(), formArea)) {
            appState.focusedInput = -1;
        }
    }
    
    // Label dan Input Fields
    if (appState.fontBold.texture.id != 0) {
        DrawTextEx(appState.fontBold, "Nama Lengkap *", (Vector2){inputX, inputY}, FONT_BODY, 1.0f, warnaTeks);
    } else {
        UI_DrawText("Nama Lengkap *", inputX, inputY, FONT_BODY, warnaTeks);
    }
    Rectangle inputNama = { (float)inputX, (float)(inputY + 25), (float)inputWidth, (float)inputHeight };
    GambarInputText(inputNama, "Masukkan nama lengkap", appState.formPasien.nama, 100, 0); // ID 0
    
    inputY += spacing;
    DrawText("NIK (16 digit) *", inputX, inputY, FONT_BODY, warnaTeks);
    Rectangle inputNIK = { (float)inputX, (float)(inputY + 25), (float)inputWidth, (float)inputHeight };
    GambarInputText(inputNIK, "Contoh: 7371012345678901", appState.formPasien.nik, 20, 1); // ID 1
    
    inputY += spacing;
    DrawText("Jenis Kelamin *", inputX, inputY, FONT_BODY, warnaTeks);
    // Radio buttons
    Rectangle radioLaki = { (float)inputX, (float)(inputY + 25), 25, 25 };
    Rectangle radioPer = { (float)(inputX + 200), (float)(inputY + 25), 25, 25 };
    
    bool isLaki = strcmp(appState.formPasien.jenisKelamin, "Laki-laki") == 0;
    bool isPer = strcmp(appState.formPasien.jenisKelamin, "Perempuan") == 0;
    
    DrawCircle((int)radioLaki.x + 12, (int)radioLaki.y + 12, 12, appState.isDarkMode ? COL_BORDER_DARK : COL_BORDER_LIGHT);
    if (isLaki) DrawCircle((int)radioLaki.x + 12, (int)radioLaki.y + 12, 7, COL_PRIMARY);
    DrawText("Laki-laki", (int)radioLaki.x + 35, (int)radioLaki.y + 3, FONT_BODY, warnaTeks);
    
    DrawCircle((int)radioPer.x + 12, (int)radioPer.y + 12, 12, appState.isDarkMode ? COL_BORDER_DARK : COL_BORDER_LIGHT);
    if (isPer) DrawCircle((int)radioPer.x + 12, (int)radioPer.y + 12, 7, COL_PRIMARY);
    DrawText("Perempuan", (int)radioPer.x + 35, (int)radioPer.y + 3, FONT_BODY, warnaTeks);
    
    if (CheckCollisionPointCircle(GetMousePosition(), (Vector2){radioLaki.x + 12, radioLaki.y + 12}, 12) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        strcpy(appState.formPasien.jenisKelamin, "Laki-laki");
    }
    if (CheckCollisionPointCircle(GetMousePosition(), (Vector2){radioPer.x + 12, radioPer.y + 12}, 12) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        strcpy(appState.formPasien.jenisKelamin, "Perempuan");
    }
    
    inputY += spacing;
    DrawText("Tanggal Lahir (DD/MM/YYYY) *", inputX, inputY, FONT_BODY, warnaTeks);
    Rectangle inputTglLahir = { (float)inputX, (float)(inputY + 25), (float)inputWidth, (float)inputHeight };
    GambarInputText(inputTglLahir, "Contoh: 15/08/1990", appState.formPasien.tanggalLahir, 15, 2); // ID 2
    
    inputY += spacing;
    DrawText("Nomor Telepon *", inputX, inputY, FONT_BODY, warnaTeks);
    Rectangle inputTelp = { (float)inputX, (float)(inputY + 25), (float)inputWidth, (float)inputHeight };
    GambarInputText(inputTelp, "Contoh: 081234567890", appState.formPasien.telepon, 20, 3); // ID 3
    
    inputY += spacing;
    DrawText("Alamat", inputX, inputY, FONT_BODY, warnaTeks);
    Rectangle inputAlamat = { (float)inputX, (float)(inputY + 25), (float)inputWidth, (float)inputHeight };
    GambarInputText(inputAlamat, "Masukkan alamat lengkap", appState.formPasien.alamat, 200, 4); // ID 4
    
    inputY += spacing;
    DrawText("Riwayat Penyakit", inputX, inputY, FONT_BODY, warnaTeks);
    Rectangle inputRiwayat = { (float)inputX, (float)(inputY + 25), (float)inputWidth, (float)inputHeight };
    GambarInputText(inputRiwayat, "Riwayat penyakit (opsional)", appState.formPasien.riwayatPenyakit, 500, 5); // ID 5
    
    inputY += spacing;
    DrawText("Status *", inputX, inputY, FONT_BODY, warnaTeks);
    // Radio buttons untuk status
    Rectangle radioAktif = { (float)inputX, (float)(inputY + 25), 25, 25 };
    Rectangle radioNonAktif = { (float)(inputX + 200), (float)(inputY + 25), 25, 25 };
    
    bool isAktif = strcmp(appState.formPasien.status, "Aktif") == 0;
    bool isNonAktif = strcmp(appState.formPasien.status, "Non-Aktif") == 0;
    
    DrawCircle((int)radioAktif.x + 12, (int)radioAktif.y + 12, 12, appState.isDarkMode ? COL_BORDER_DARK : COL_BORDER_LIGHT);
    if (isAktif) DrawCircle((int)radioAktif.x + 12, (int)radioAktif.y + 12, 7, COL_SECONDARY);
    DrawText("Aktif", (int)radioAktif.x + 35, (int)radioAktif.y + 3, FONT_BODY, warnaTeks);
    
    DrawCircle((int)radioNonAktif.x + 12, (int)radioNonAktif.y + 12, 12, appState.isDarkMode ? COL_BORDER_DARK : COL_BORDER_LIGHT);
    if (isNonAktif) DrawCircle((int)radioNonAktif.x + 12, (int)radioNonAktif.y + 12, 7, RED);
    DrawText("Non-Aktif", (int)radioNonAktif.x + 35, (int)radioNonAktif.y + 3, FONT_BODY, warnaTeks);
    
    if (CheckCollisionPointCircle(GetMousePosition(), (Vector2){radioAktif.x + 12, radioAktif.y + 12}, 12) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        strcpy(appState.formPasien.status, "Aktif");
    }
    if (CheckCollisionPointCircle(GetMousePosition(), (Vector2){radioNonAktif.x + 12, radioNonAktif.y + 12}, 12) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        strcpy(appState.formPasien.status, "Non-Aktif");
    }
    
    // Render Tombol Aksi
    // Tombol Batal
    DrawRectangleRounded(tombolBatal, 0.3f, 0, hoverBatal ? Fade(RED, 0.2f) : Fade(RED, 0.1f));
    DrawRectangleRoundedLines(tombolBatal, 0.3f, 0, RED);
    UI_DrawText("Batal", tombolBatal.x + 50, tombolBatal.y + 15, FONT_BUTTON, RED);
    
    // Tombol Simpan
    DrawRectangleRounded(tombolSimpan, 0.3f, 0, hoverSimpan ? Fade(COL_SECONDARY, 0.9f) : COL_SECONDARY);
    if (appState.fontBold.texture.id != 0) {
        DrawTextEx(appState.fontBold, "Simpan", (Vector2){tombolSimpan.x + 40, tombolSimpan.y + 15}, FONT_BUTTON, 1.0f, WHITE);
    } else {
        UI_DrawText("Simpan", tombolSimpan.x + 40, tombolSimpan.y + 15, FONT_BUTTON, WHITE);
    }
    
    if (hoverBatal && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Reset form
        memset(&appState.formPasien, 0, sizeof(FormPasien));
        appState.focusedInput = -1;
        appState.currentPage = PAGE_PASIEN;
    }
    
    if (hoverSimpan && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Set default status jika belum dipilih
        if (!appState.formPasien.status[0]) {
            strcpy(appState.formPasien.status, "Aktif");
        }
        
        // Debug: Print form data
        printf("=== DEBUG FORM PASIEN ===\n");
        printf("Nama: '%s'\n", appState.formPasien.nama);
        printf("NIK: '%s'\n", appState.formPasien.nik);
        printf("Jenis Kelamin: '%s'\n", appState.formPasien.jenisKelamin);
        printf("Tanggal Lahir: '%s'\n", appState.formPasien.tanggalLahir);
        printf("Telepon: '%s'\n", appState.formPasien.telepon);
        printf("Status: '%s'\n", appState.formPasien.status);
        
        // Validasi dan simpan
        if (appState.formPasien.nama[0] && appState.formPasien.nik[0] && 
            appState.formPasien.jenisKelamin[0] && appState.formPasien.tanggalLahir[0] && 
            appState.formPasien.telepon[0]) {
            
            printf("Validasi OK, mencoba simpan...\n");
            
            Pasien pasienBaru;
            strcpy(pasienBaru.nama, appState.formPasien.nama);
            strcpy(pasienBaru.nik, appState.formPasien.nik);
            strcpy(pasienBaru.jenisKelamin, appState.formPasien.jenisKelamin);
            strcpy(pasienBaru.tanggalLahir, appState.formPasien.tanggalLahir);
            strcpy(pasienBaru.telepon, appState.formPasien.telepon);
            strcpy(pasienBaru.alamat, appState.formPasien.alamat[0] ? appState.formPasien.alamat : "-");
            strcpy(pasienBaru.riwayatPenyakit, appState.formPasien.riwayatPenyakit[0] ? appState.formPasien.riwayatPenyakit : "-");
            strcpy(pasienBaru.status, appState.formPasien.status);
            
            int result = tambahPasien(&pasienBaru);
            printf("Hasil tambahPasien: %d\n", result);
            
            if (result == 0) {
                printf("Berhasil! Kembali ke halaman pasien.\n");
                memset(&appState.formPasien, 0, sizeof(FormPasien));
                appState.focusedInput = -1;
                MuatDataPasien();
                appState.currentPage = PAGE_PASIEN;
            } else {
                printf("GAGAL! Error code: %d\n", result);
            }
        } else {
            printf("Validasi GAGAL!\n");
        }
    }
    
    // Info required fields dan status validasi
    const char *statusMsg = "* Field wajib diisi";
    Color statusColor = Fade(warnaTeksSekunder, 0.7f);
    
    // Cek validasi untuk feedback
    if (hoverSimpan) {
        if (!appState.formPasien.nama[0]) {
            statusMsg = "⚠ Nama wajib diisi!";
            statusColor = RED;
        } else if (!appState.formPasien.nik[0]) {
            statusMsg = "⚠ NIK wajib diisi!";
            statusColor = RED;
        } else if (!appState.formPasien.jenisKelamin[0]) {
            statusMsg = "⚠ Jenis Kelamin wajib dipilih!";
            statusColor = RED;
        } else if (!appState.formPasien.tanggalLahir[0]) {
            statusMsg = "⚠ Tanggal Lahir wajib diisi!";
            statusColor = RED;
        } else if (!appState.formPasien.telepon[0]) {
            statusMsg = "⚠ Nomor Telepon wajib diisi!";
            statusColor = RED;
        } else {
            statusMsg = "✓ Siap disimpan!";
            statusColor = COL_SECONDARY;
        }
    }
    
    DrawText(statusMsg, inputX, tombolY + 70, FONT_SMALL, statusColor);
}

// Placeholder untuk halaman lainnya
void GambarHalamanEditPasien(int x, int y, int lebar, int tinggi) {
    (void)tinggi;
    
    Color warnaTeks = appState.isDarkMode ? COL_TEXT_DARK : COL_TEXT_LIGHT;
    Color warnaTeksSekunder = appState.isDarkMode ? COL_TEXT_SEC_DARK : COL_TEXT_SEC_LIGHT;
    Color warnaKartu = appState.isDarkMode ? COL_CARD_DARK : COL_CARD_LIGHT;
    
    // Header
    DrawText("Edit Data Pasien", x + 40, y + 30, FONT_HEADER, warnaTeks);
    DrawText(TextFormat("Mengubah data pasien: %s", appState.formPasien.nama), x + 40, y + 75, FONT_SUBTITLE, warnaTeksSekunder);
    
    // Form Container (sama seperti tambah, tapi dengan data terisi)
    int formY = y + 140;
    Rectangle formArea = { (float)(x + 40), (float)formY, (float)(lebar - 80), 650 };
    DrawRectangleRounded(formArea, 0.1f, 0, warnaKartu);
    
    int inputX = (int)formArea.x + 40;
    int inputY = (int)formArea.y + 40;
    int inputWidth = 600;
    int inputHeight = 50;
    int spacing = 80;
    
    // Nama
    DrawText("Nama Lengkap *", inputX, inputY, FONT_BODY, warnaTeks);
    GambarInputText((Rectangle){ (float)inputX, (float)(inputY + 30), (float)inputWidth, (float)inputHeight },
                    "Masukkan nama lengkap", appState.formPasien.nama, 100, 0);
    
    inputY += spacing;
    DrawText("NIK (16 digit) *", inputX, inputY, FONT_BODY, warnaTeks);
    GambarInputText((Rectangle){ (float)inputX, (float)(inputY + 30), (float)inputWidth, (float)inputHeight },
                    "Contoh: 7371012345678901", appState.formPasien.nik, 20, 1);
    
    inputY += spacing;
    DrawText("Jenis Kelamin *", inputX, inputY, FONT_BODY, warnaTeks);
    Rectangle radioLaki = { (float)inputX, (float)(inputY + 30), 30, 30 };
    Rectangle radioPer = { (float)(inputX + 250), (float)(inputY + 30), 30, 30 };
    
    bool isLaki = strcmp(appState.formPasien.jenisKelamin, "Laki-laki") == 0;
    bool isPer = strcmp(appState.formPasien.jenisKelamin, "Perempuan") == 0;
    
    DrawCircle((int)radioLaki.x + 15, (int)radioLaki.y + 15, 15, appState.isDarkMode ? COL_BORDER_DARK : COL_BORDER_LIGHT);
    if (isLaki) DrawCircle((int)radioLaki.x + 15, (int)radioLaki.y + 15, 9, COL_PRIMARY);
    DrawText("Laki-laki", (int)radioLaki.x + 45, (int)radioLaki.y + 5, FONT_BODY, warnaTeks);
    
    DrawCircle((int)radioPer.x + 15, (int)radioPer.y + 15, 15, appState.isDarkMode ? COL_BORDER_DARK : COL_BORDER_LIGHT);
    if (isPer) DrawCircle((int)radioPer.x + 15, (int)radioPer.y + 15, 9, COL_PRIMARY);
    DrawText("Perempuan", (int)radioPer.x + 45, (int)radioPer.y + 5, FONT_BODY, warnaTeks);
    
    if (CheckCollisionPointCircle(GetMousePosition(), (Vector2){radioLaki.x + 15, radioLaki.y + 15}, 15) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        strcpy(appState.formPasien.jenisKelamin, "Laki-laki");
    }
    if (CheckCollisionPointCircle(GetMousePosition(), (Vector2){radioPer.x + 15, radioPer.y + 15}, 15) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        strcpy(appState.formPasien.jenisKelamin, "Perempuan");
    }
    
    inputY += spacing;
    DrawText("Tanggal Lahir (DD/MM/YYYY) *", inputX, inputY, FONT_BODY, warnaTeks);
    GambarInputText((Rectangle){ (float)inputX, (float)(inputY + 30), (float)inputWidth, (float)inputHeight },
                    "Contoh: 15/08/1990", appState.formPasien.tanggalLahir, 15, 2);
    
    inputY += spacing;
    DrawText("Nomor Telepon *", inputX, inputY, FONT_BODY, warnaTeks);
    GambarInputText((Rectangle){ (float)inputX, (float)(inputY + 30), (float)inputWidth, (float)inputHeight },
                    "Contoh: 081234567890", appState.formPasien.telepon, 20, 3);
    
    inputY += spacing;
    DrawText("Status *", inputX, inputY, FONT_BODY, warnaTeks);
    Rectangle radioAktif = { (float)inputX, (float)(inputY + 30), 30, 30 };
    Rectangle radioNonaktif = { (float)(inputX + 200), (float)(inputY + 30), 30, 30 };
    
    bool isAktif = strcmp(appState.formPasien.status, "Aktif") == 0;
    bool isNonaktif = strcmp(appState.formPasien.status, "Tidak Aktif") == 0;
    
    DrawCircle((int)radioAktif.x + 15, (int)radioAktif.y + 15, 15, appState.isDarkMode ? COL_BORDER_DARK : COL_BORDER_LIGHT);
    if (isAktif) DrawCircle((int)radioAktif.x + 15, (int)radioAktif.y + 15, 9, COL_SECONDARY);
    DrawText("Aktif", (int)radioAktif.x + 45, (int)radioAktif.y + 5, FONT_BODY, warnaTeks);
    
    DrawCircle((int)radioNonaktif.x + 15, (int)radioNonaktif.y + 15, 15, appState.isDarkMode ? COL_BORDER_DARK : COL_BORDER_LIGHT);
    if (isNonaktif) DrawCircle((int)radioNonaktif.x + 15, (int)radioNonaktif.y + 15, 9, RED);
    DrawText("Tidak Aktif", (int)radioNonaktif.x + 45, (int)radioNonaktif.y + 5, FONT_BODY, warnaTeks);
    
    if (CheckCollisionPointCircle(GetMousePosition(), (Vector2){radioAktif.x + 15, radioAktif.y + 15}, 15) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        strcpy(appState.formPasien.status, "Aktif");
    }
    if (CheckCollisionPointCircle(GetMousePosition(), (Vector2){radioNonaktif.x + 15, radioNonaktif.y + 15}, 15) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        strcpy(appState.formPasien.status, "Tidak Aktif");
    }
    
    // Tombol Aksi
    int tombolY = (int)formArea.y + 570;
    Rectangle tombolBatal = { (float)(inputX), (float)tombolY, 180, 55 };
    Rectangle tombolSimpan = { (float)(inputX + 200), (float)tombolY, 180, 55 };
    
    bool hoverBatal = CheckCollisionPointRec(GetMousePosition(), tombolBatal);
    bool hoverSimpan = CheckCollisionPointRec(GetMousePosition(), tombolSimpan);
    
    // Tombol Batal
    DrawRectangleRounded(tombolBatal, 0.3f, 0, hoverBatal ? Fade(RED, 0.2f) : Fade(RED, 0.1f));
    DrawRectangleRoundedLines(tombolBatal, 0.3f, 0, RED);
    int batalWidth = MeasureText("Batal", FONT_BUTTON);
    DrawText("Batal", (int)tombolBatal.x + (180 - batalWidth)/2, (int)tombolBatal.y + 17, FONT_BUTTON, RED);
    
    // Tombol Simpan
    DrawRectangleRounded(tombolSimpan, 0.3f, 0, hoverSimpan ? Fade(COL_SECONDARY, 0.9f) : COL_SECONDARY);
    int simpanWidth = MeasureText("Simpan Perubahan", FONT_BUTTON);
    DrawText("Simpan Perubahan", (int)tombolSimpan.x + (180 - simpanWidth)/2, (int)tombolSimpan.y + 17, FONT_BUTTON, WHITE);
    
    if (hoverBatal && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        memset(&appState.formPasien, 0, sizeof(FormPasien));
        appState.currentPage = PAGE_PASIEN;
    }
    
    if (hoverSimpan && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (appState.formPasien.nama[0] && appState.formPasien.nik[0] && 
            appState.formPasien.jenisKelamin[0] && appState.formPasien.tanggalLahir[0] && 
            appState.formPasien.telepon[0]) {
            
            Pasien pasienUpdate;
            strcpy(pasienUpdate.nama, appState.formPasien.nama);
            strcpy(pasienUpdate.nik, appState.formPasien.nik);
            strcpy(pasienUpdate.jenisKelamin, appState.formPasien.jenisKelamin);
            strcpy(pasienUpdate.tanggalLahir, appState.formPasien.tanggalLahir);
            strcpy(pasienUpdate.telepon, appState.formPasien.telepon);
            strcpy(pasienUpdate.alamat, appState.formPasien.alamat[0] ? appState.formPasien.alamat : "-");
            strcpy(pasienUpdate.riwayatPenyakit, appState.formPasien.riwayatPenyakit[0] ? appState.formPasien.riwayatPenyakit : "-");
            strcpy(pasienUpdate.status, appState.formPasien.status);
            
            if (updatePasien(&pasienUpdate, appState.formPasien.editingId) == 0) {
                memset(&appState.formPasien, 0, sizeof(FormPasien));
                MuatDataPasien();
                appState.currentPage = PAGE_PASIEN;
            }
        }
    }
    
    DrawText("* Field wajib diisi", inputX, tombolY + 75, FONT_SMALL, Fade(warnaTeksSekunder, 0.7f));
}

void GambarHalamanRekamMedis(int x, int y, int lebar, int tinggi) {
    Color warnaTeks = appState.isDarkMode ? COL_TEXT_DARK : COL_TEXT_LIGHT;
    Color warnaTeksSekunder = appState.isDarkMode ? COL_TEXT_SEC_DARK : COL_TEXT_SEC_LIGHT;
    
    // Header dengan font besar
    DrawText("Rekam Medis Pasien", x + 40, y + 30, FONT_HEADER, warnaTeks);
    
    // Badge jumlah
    RekamMedis *daftarRekamMedis;
    int jumlahRekamMedis = getAllRekamMedis(&daftarRekamMedis);
    
    char teksJumlah[32];
    snprintf(teksJumlah, 32, "%d Catatan", jumlahRekamMedis);
    int lebarBadge = MeasureText(teksJumlah, FONT_BODY) + 30;
    DrawRectangleRounded((Rectangle){ (float)(x + 350), (float)(y + 35), (float)lebarBadge, 28 }, 0.5f, 0, Fade(COL_PRIMARY, 0.2f));
    DrawText(teksJumlah, x + 365, y + 40, FONT_BODY, COL_PRIMARY);

    DrawText("Riwayat kunjungan dan diagnosis pasien", x + 40, y + 70, FONT_SUBTITLE, warnaTeksSekunder);

    // Tombol Tambah
    Rectangle tombolTambah = { (float)(lebar - 280), (float)(y + 30), 240, 45 };
    bool hoverTambah = CheckCollisionPointRec(GetMousePosition(), tombolTambah);
    
    Color warnaTombol = hoverTambah ? Fade(COL_SECONDARY, 0.9f) : COL_SECONDARY;
    DrawRectangleRounded(tombolTambah, 0.3f, 0, warnaTombol);
    DrawText("+ Tambah Rekam Medis", (int)tombolTambah.x + 20, (int)tombolTambah.y + 12, FONT_BUTTON, WHITE);
    
    if (hoverTambah && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        appState.currentPage = PAGE_REKAM_MEDIS_ADD;
        return;
    }

    // Kontainer Tabel
    int tabelY = y + 130;
    int tinggiTabel = tinggi - tabelY - 40;
    Color warnaKartu = appState.isDarkMode ? COL_CARD_DARK : COL_CARD_LIGHT;
    
    DrawRectangleRounded((Rectangle){ (float)(x + 40), (float)tabelY, (float)(lebar - 80), (float)tinggiTabel }, 0.1f, 0, warnaKartu);

    // Konfigurasi Kolom
    const char *kolom[] = { "ID", "PASIEN", "TGL KUNJUNGAN", "KELUHAN", "DIAGNOSIS", "AKSI" };
    const float offsetKolom[] = { 25, 90, 300, 480, 680, 900 }; 
    
    GambarHeaderTabel((float)(x + 40), (float)(tabelY + 20), (float)(lebar - 80), kolom, offsetKolom, 6);

    // Baris Data
    int barisY = tabelY + 65;
    int tinggiBaris = 50;
    int maksBaris = (tinggiTabel - 80) / tinggiBaris;

    if (jumlahRekamMedis == 0) {
         DrawText("Belum ada rekam medis.", x + (lebar/2) - 120, barisY + 80, FONT_SUBTITLE, warnaTeksSekunder);
         DrawText("Klik tombol 'Tambah Rekam Medis' untuk menambah catatan baru.", x + (lebar/2) - 280, barisY + 120, FONT_BODY, Fade(warnaTeksSekunder, 0.7f));
         free(daftarRekamMedis);
         return;
    }

    // Logika Scroll
    int indeksMulai = appState.scrollOffset;
    if (indeksMulai > jumlahRekamMedis - maksBaris) indeksMulai = jumlahRekamMedis - maksBaris;
    if (indeksMulai < 0) indeksMulai = 0;
    appState.scrollOffset = indeksMulai;
    
    for (int i = 0; i < maksBaris && (indeksMulai + i) < jumlahRekamMedis; i++) {
        RekamMedis rm = daftarRekamMedis[indeksMulai + i];
        
        Rectangle areaBaris = { (float)(x + 40), (float)(barisY + (i * tinggiBaris)), (float)(lebar - 80), (float)tinggiBaris };
        bool sedangHover = CheckCollisionPointRec(GetMousePosition(), areaBaris);
        
        GambarLatarBarisTabel(areaBaris, i % 2 == 0, sedangHover);

        // Cari nama pasien
        Pasien *p = cariPasienById(rm.idPasien);
        char namaPasien[50] = "Unknown";
        if (p) {
            strncpy(namaPasien, p->nama, 20);
            namaPasien[20] = '\0';
            if (strlen(p->nama) > 20) strcat(namaPasien, "...");
            free(p);
        }

        // Render Sel
        DrawText(TextFormat("%d", rm.id), (int)(x + 40 + offsetKolom[0]), (int)(areaBaris.y + 15), FONT_TABLE, warnaTeksSekunder);
        DrawText(namaPasien, (int)(x + 40 + offsetKolom[1]), (int)(areaBaris.y + 15), FONT_TABLE, warnaTeks);
        DrawText(rm.tanggalKunjungan, (int)(x + 40 + offsetKolom[2]), (int)(areaBaris.y + 15), FONT_TABLE, warnaTeks);
        
        // Keluhan (potong)
        char keluhanSingkat[25];
        strncpy(keluhanSingkat, rm.keluhan, 22);
        keluhanSingkat[22] = '\0';
        if (strlen(rm.keluhan) > 22) strcat(keluhanSingkat, "...");
        DrawText(keluhanSingkat, (int)(x + 40 + offsetKolom[3]), (int)(areaBaris.y + 15), FONT_TABLE, warnaTeks);
        
        // Diagnosis (potong)
        char diagnosisSingkat[25];
        strncpy(diagnosisSingkat, rm.diagnosis, 22);
        diagnosisSingkat[22] = '\0';
        if (strlen(rm.diagnosis) > 22) strcat(diagnosisSingkat, "...");
        DrawText(diagnosisSingkat, (int)(x + 40 + offsetKolom[4]), (int)(areaBaris.y + 15), FONT_TABLE, warnaTeks);
        
        // Tombol Aksi
        Rectangle tombolHapus = { (float)(x + 40 + offsetKolom[5]), (float)(areaBaris.y + 10), 45, 30 };
        bool hoverHapus = CheckCollisionPointRec(GetMousePosition(), tombolHapus);
        
        // Tombol Hapus
        DrawRectangleRounded(tombolHapus, 0.3f, 0, hoverHapus ? Fade(RED, 0.3f) : Fade(RED, 0.1f));
        UI_DrawText("HPS", tombolHapus.x + 10, tombolHapus.y + 8, 12, RED);
        
        if (hoverHapus && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            appState.dialogState = DIALOG_DELETE_CONFIRM;
            appState.dialogTargetId = rm.id;
            snprintf(appState.dialogMessage, sizeof(appState.dialogMessage), 
                     "Apakah Anda yakin ingin menghapus rekam medis ini?");
        }
    }
    
    free(daftarRekamMedis);
    
    // Pagination info
    DrawText(TextFormat("Menampilkan %d - %d dari %d rekam medis", 
             indeksMulai + 1, 
             (indeksMulai + maksBaris > jumlahRekamMedis) ? jumlahRekamMedis : indeksMulai + maksBaris,
             jumlahRekamMedis), 
             x + 60, barisY + (maksBaris * tinggiBaris) + 15, FONT_SMALL, warnaTeksSekunder);
}

void GambarHalamanTambahRekamMedis(int x, int y, int lebar, int tinggi) {
    (void)tinggi;
    
    Color warnaTeks = appState.isDarkMode ? COL_TEXT_DARK : COL_TEXT_LIGHT;
    Color warnaTeksSekunder = appState.isDarkMode ? COL_TEXT_SEC_DARK : COL_TEXT_SEC_LIGHT;
    Color warnaKartu = appState.isDarkMode ? COL_CARD_DARK : COL_CARD_LIGHT;
    
    // Header
    DrawText("Tambah Rekam Medis", x + 40, y + 30, FONT_HEADER, warnaTeks);
    DrawText("Catat kunjungan dan diagnosis pasien", x + 40, y + 70, FONT_SUBTITLE, warnaTeksSekunder);
    
    // Form Container
    int formY = y + 130;
    Rectangle formArea = { (float)(x + 40), (float)formY, (float)(lebar - 80), 700 };
    DrawRectangleRounded(formArea, 0.1f, 0, warnaKartu);
    
    int inputX = (int)formArea.x + 40;
    int inputY = (int)formArea.y + 40;
    int inputWidth = 600;
    int inputHeight = 50;
    int spacing = 90;
    
    // Tombol Aksi (definisi dulu)
    int tombolY = (int)formArea.y + 620;
    Rectangle tombolBatal = { (float)(inputX), (float)tombolY, 180, 55 };
    Rectangle tombolSimpan = { (float)(inputX + 200), (float)tombolY, 180, 55 };
    
    bool hoverBatal = CheckCollisionPointRec(GetMousePosition(), tombolBatal);
    bool hoverSimpan = CheckCollisionPointRec(GetMousePosition(), tombolSimpan);
    
    // Unfocus input jika klik tombol
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (hoverBatal || hoverSimpan || !CheckCollisionPointRec(GetMousePosition(), formArea)) {
            appState.focusedInput = -1;
        }
    }
    
    // ID Pasien
    if (appState.fontBold.texture.id != 0) {
        DrawTextEx(appState.fontBold, "ID Pasien *", (Vector2){inputX, inputY}, FONT_BODY, 1.0f, warnaTeks);
    } else {
        UI_DrawText("ID Pasien *", inputX, inputY, FONT_BODY, warnaTeks);
    }
    GambarInputText((Rectangle){ (float)inputX, (float)(inputY + 30), (float)inputWidth, (float)inputHeight },
                    "Masukkan ID Pasien", appState.formRekamMedis.idPasien, 10, 10);
    
    inputY += spacing;
    if (appState.fontBold.texture.id != 0) {
        DrawTextEx(appState.fontBold, "Tanggal Kunjungan (DD/MM/YYYY) *", (Vector2){inputX, inputY}, FONT_BODY, 1.0f, warnaTeks);
    } else {
        UI_DrawText("Tanggal Kunjungan (DD/MM/YYYY) *", inputX, inputY, FONT_BODY, warnaTeks);
    }
    GambarInputText((Rectangle){ (float)inputX, (float)(inputY + 30), (float)inputWidth, (float)inputHeight },
                    "Contoh: 11/02/2026", appState.formRekamMedis.tanggalKunjungan, 15, 11);
    
    inputY += spacing;
    if (appState.fontBold.texture.id != 0) {
        DrawTextEx(appState.fontBold, "Keluhan Utama *", (Vector2){inputX, inputY}, FONT_BODY, 1.0f, warnaTeks);
    } else {
        UI_DrawText("Keluhan Utama *", inputX, inputY, FONT_BODY, warnaTeks);
    }
    GambarInputText((Rectangle){ (float)inputX, (float)(inputY + 30), (float)inputWidth, 80 }, 
                    "Keluhan pasien...", appState.formRekamMedis.keluhan, 499, 12);
    
    inputY += 130;
    if (appState.fontBold.texture.id != 0) {
        DrawTextEx(appState.fontBold, "Diagnosis *", (Vector2){inputX, inputY}, FONT_BODY, 1.0f, warnaTeks);
    } else {
        UI_DrawText("Diagnosis *", inputX, inputY, FONT_BODY, warnaTeks);
    }
    GambarInputText((Rectangle){ (float)inputX, (float)(inputY + 30), (float)inputWidth, 80 }, 
                    "Hasil diagnosis...", appState.formRekamMedis.diagnosis, 199, 13);
    
    // Render Tombol Aksi
    // Tombol Batal
    DrawRectangleRounded(tombolBatal, 0.3f, 0, hoverBatal ? Fade(RED, 0.2f) : Fade(RED, 0.1f));
    DrawRectangleRoundedLines(tombolBatal, 0.3f, 0, RED);
    UI_DrawText("Batal", tombolBatal.x + 50, tombolBatal.y + 15, FONT_BUTTON, RED);
    
    // Tombol Simpan
    DrawRectangleRounded(tombolSimpan, 0.3f, 0, hoverSimpan ? Fade(COL_SECONDARY, 0.9f) : COL_SECONDARY);
    if (appState.fontBold.texture.id != 0) {
        DrawTextEx(appState.fontBold, "Simpan", (Vector2){tombolSimpan.x + 40, tombolSimpan.y + 15}, FONT_BUTTON, 1.0f, WHITE);
    } else {
        UI_DrawText("Simpan", tombolSimpan.x + 40, tombolSimpan.y + 15, FONT_BUTTON, WHITE);
    }
    
    if (hoverBatal && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        memset(&appState.formRekamMedis, 0, sizeof(FormRekamMedis));
        appState.currentPage = PAGE_REKAM_MEDIS;
    }
    
    if (hoverSimpan && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (appState.formRekamMedis.idPasien[0] && appState.formRekamMedis.tanggalKunjungan[0] && 
            appState.formRekamMedis.keluhan[0] && appState.formRekamMedis.diagnosis[0]) {
            RekamMedis rmBaru;
            rmBaru.idPasien = atoi(appState.formRekamMedis.idPasien);
            strcpy(rmBaru.tanggalKunjungan, appState.formRekamMedis.tanggalKunjungan);
            strcpy(rmBaru.keluhan, appState.formRekamMedis.keluhan);
            strcpy(rmBaru.diagnosis, appState.formRekamMedis.diagnosis);
            strcpy(rmBaru.catatanDokter, "-");
            strcpy(rmBaru.tindakan, "-");
            strcpy(rmBaru.tanggalKontrol, "-");
            
            if (tambahRekamMedis(&rmBaru) == 0) {
                memset(&appState.formRekamMedis, 0, sizeof(FormRekamMedis));
                appState.currentPage = PAGE_REKAM_MEDIS;
            }
        }
    }
    
    DrawText("* Field wajib diisi", inputX, tombolY + 75, FONT_SMALL, Fade(warnaTeksSekunder, 0.7f));
}

void GambarHalamanResep(int x, int y, int lebar, int tinggi) {
    Color warnaTeks = appState.isDarkMode ? COL_TEXT_DARK : COL_TEXT_LIGHT;
    Color warnaTeksSekunder = appState.isDarkMode ? COL_TEXT_SEC_DARK : COL_TEXT_SEC_LIGHT;
    
    // Header
    DrawText("Resep Obat", x + 40, y + 30, FONT_HEADER, warnaTeks);
    
    // Badge jumlah
    Resep *daftarResep;
    int jumlahResep = getAllResep(&daftarResep);
    
    char teksJumlah[32];
    snprintf(teksJumlah, 32, "%d Resep", jumlahResep);
    int lebarBadge = MeasureText(teksJumlah, FONT_BODY) + 30;
    DrawRectangleRounded((Rectangle){ (float)(x + 250), (float)(y + 35), (float)lebarBadge, 28 }, 0.5f, 0, Fade(COL_PRIMARY, 0.2f));
    DrawText(teksJumlah, x + 265, y + 40, FONT_BODY, COL_PRIMARY);

    DrawText("Daftar resep obat untuk pasien", x + 40, y + 70, FONT_SUBTITLE, warnaTeksSekunder);

    // Tombol Tambah
    Rectangle tombolTambah = { (float)(lebar - 240), (float)(y + 30), 200, 45 };
    bool hoverTambah = CheckCollisionPointRec(GetMousePosition(), tombolTambah);
    
    Color warnaTombol = hoverTambah ? Fade(COL_SECONDARY, 0.9f) : COL_SECONDARY;
    DrawRectangleRounded(tombolTambah, 0.3f, 0, warnaTombol);
    DrawText("+ Tambah Resep", (int)tombolTambah.x + 20, (int)tombolTambah.y + 12, FONT_BUTTON, WHITE);
    
    if (hoverTambah && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        appState.currentPage = PAGE_RESEP_ADD;
        return;
    }

    // Kontainer Tabel
    int tabelY = y + 130;
    int tinggiTabel = tinggi - tabelY - 40;
    Color warnaKartu = appState.isDarkMode ? COL_CARD_DARK : COL_CARD_LIGHT;
    
    DrawRectangleRounded((Rectangle){ (float)(x + 40), (float)tabelY, (float)(lebar - 80), (float)tinggiTabel }, 0.1f, 0, warnaKartu);

    // Konfigurasi Kolom
    const char *kolom[] = { "ID", "PASIEN", "TGL RESEP", "OBAT", "DOSIS", "FREKUENSI", "AKSI" };
    const float offsetKolom[] = { 25, 90, 280, 420, 580, 720, 900 }; 
    
    GambarHeaderTabel((float)(x + 40), (float)(tabelY + 20), (float)(lebar - 80), kolom, offsetKolom, 7);

    // Baris Data
    int barisY = tabelY + 65;
    int tinggiBaris = 50;
    int maksBaris = (tinggiTabel - 80) / tinggiBaris;

    if (jumlahResep == 0) {
         DrawText("Belum ada resep obat.", x + (lebar/2) - 100, barisY + 80, FONT_SUBTITLE, warnaTeksSekunder);
         DrawText("Klik tombol 'Tambah Resep' untuk menambah resep baru.", x + (lebar/2) - 250, barisY + 120, FONT_BODY, Fade(warnaTeksSekunder, 0.7f));
         free(daftarResep);
         return;
    }

    // Logika Scroll
    int indeksMulai = appState.scrollOffset;
    if (indeksMulai > jumlahResep - maksBaris) indeksMulai = jumlahResep - maksBaris;
    if (indeksMulai < 0) indeksMulai = 0;
    appState.scrollOffset = indeksMulai;
    
    for (int i = 0; i < maksBaris && (indeksMulai + i) < jumlahResep; i++) {
        Resep r = daftarResep[indeksMulai + i];
        
        Rectangle areaBaris = { (float)(x + 40), (float)(barisY + (i * tinggiBaris)), (float)(lebar - 80), (float)tinggiBaris };
        bool sedangHover = CheckCollisionPointRec(GetMousePosition(), areaBaris);
        
        GambarLatarBarisTabel(areaBaris, i % 2 == 0, sedangHover);

        // Cari nama pasien
        Pasien *p = cariPasienById(r.idPasien);
        char namaPasien[30] = "Unknown";
        if (p) {
            strncpy(namaPasien, p->nama, 18);
            namaPasien[18] = '\0';
            if (strlen(p->nama) > 18) strcat(namaPasien, "...");
            free(p);
        }

        // Render Sel
        DrawText(TextFormat("%d", r.id), (int)(x + 40 + offsetKolom[0]), (int)(areaBaris.y + 15), FONT_TABLE, warnaTeksSekunder);
        DrawText(namaPasien, (int)(x + 40 + offsetKolom[1]), (int)(areaBaris.y + 15), FONT_TABLE, warnaTeks);
        DrawText(r.tanggalResep, (int)(x + 40 + offsetKolom[2]), (int)(areaBaris.y + 15), FONT_TABLE, warnaTeks);
        
        // Obat (potong)
        char obatSingkat[20];
        strncpy(obatSingkat, r.namaObat, 17);
        obatSingkat[17] = '\0';
        if (strlen(r.namaObat) > 17) strcat(obatSingkat, "...");
        DrawText(obatSingkat, (int)(x + 40 + offsetKolom[3]), (int)(areaBaris.y + 15), FONT_TABLE, warnaTeks);
        
        DrawText(r.dosis, (int)(x + 40 + offsetKolom[4]), (int)(areaBaris.y + 15), FONT_TABLE, warnaTeks);
        DrawText(r.frekuensi, (int)(x + 40 + offsetKolom[5]), (int)(areaBaris.y + 15), FONT_TABLE, warnaTeks);
        
        // Tombol Aksi
        Rectangle tombolHapus = { (float)(x + 40 + offsetKolom[6]), (float)(areaBaris.y + 10), 45, 30 };
        bool hoverHapus = CheckCollisionPointRec(GetMousePosition(), tombolHapus);
        
        // Tombol Hapus
        DrawRectangleRounded(tombolHapus, 0.3f, 0, hoverHapus ? Fade(RED, 0.3f) : Fade(RED, 0.1f));
        UI_DrawText("HPS", tombolHapus.x + 10, tombolHapus.y + 8, 12, RED);
        
        if (hoverHapus && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            appState.dialogState = DIALOG_DELETE_CONFIRM;
            appState.dialogTargetId = r.id;
            snprintf(appState.dialogMessage, sizeof(appState.dialogMessage), 
                     "Apakah Anda yakin ingin menghapus resep obat '%s'?", r.namaObat);
        }
    }
    
    free(daftarResep);
    
    // Pagination info
    DrawText(TextFormat("Menampilkan %d - %d dari %d resep", 
             indeksMulai + 1, 
             (indeksMulai + maksBaris > jumlahResep) ? jumlahResep : indeksMulai + maksBaris,
             jumlahResep), 
             x + 60, barisY + (maksBaris * tinggiBaris) + 15, FONT_SMALL, warnaTeksSekunder);
}

void GambarHalamanTambahResep(int x, int y, int lebar, int tinggi) {
    (void)tinggi;
    
    Color warnaTeks = appState.isDarkMode ? COL_TEXT_DARK : COL_TEXT_LIGHT;
    Color warnaTeksSekunder = appState.isDarkMode ? COL_TEXT_SEC_DARK : COL_TEXT_SEC_LIGHT;
    Color warnaKartu = appState.isDarkMode ? COL_CARD_DARK : COL_CARD_LIGHT;
    
    // Header
    DrawText("Tambah Resep Obat", x + 40, y + 30, FONT_HEADER, warnaTeks);
    DrawText("Buat resep obat untuk pasien", x + 40, y + 70, FONT_SUBTITLE, warnaTeksSekunder);
    
    // Form Container
    int formY = y + 130;
    Rectangle formArea = { (float)(x + 40), (float)formY, (float)(lebar - 80), 750 };
    DrawRectangleRounded(formArea, 0.1f, 0, warnaKartu);
    
    int inputX = (int)formArea.x + 40;
    int inputY = (int)formArea.y + 40;
    int inputWidth = 600;
    int inputHeight = 50;
    int spacing = 85;
    
    // Tombol Aksi (definisi dulu)
    int tombolY = (int)formArea.y + 670;
    Rectangle tombolBatal = { (float)(inputX), (float)tombolY, 180, 55 };
    Rectangle tombolSimpan = { (float)(inputX + 200), (float)tombolY, 180, 55 };
    
    bool hoverBatal = CheckCollisionPointRec(GetMousePosition(), tombolBatal);
    bool hoverSimpan = CheckCollisionPointRec(GetMousePosition(), tombolSimpan);
    
    // Unfocus input jika klik tombol
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (hoverBatal || hoverSimpan || !CheckCollisionPointRec(GetMousePosition(), formArea)) {
            appState.focusedInput = -1;
        }
    }
    
    // ID Pasien
    UI_DrawText("ID Pasien *", inputX, inputY, FONT_BODY, warnaTeks);
    GambarInputText((Rectangle){ (float)inputX, (float)(inputY + 30), (float)inputWidth, (float)inputHeight },
                    "Masukkan ID Pasien", appState.formResep.idPasien, 10, 20);
    
    inputY += spacing;
    UI_DrawText("Tanggal Resep (DD/MM/YYYY) *", inputX, inputY, FONT_BODY, warnaTeks);
    GambarInputText((Rectangle){ (float)inputX, (float)(inputY + 30), (float)inputWidth, (float)inputHeight },
                    "Contoh: 11/02/2026", appState.formResep.tanggalResep, 15, 21);
    
    inputY += spacing;
    UI_DrawText("Nama Obat *", inputX, inputY, FONT_BODY, warnaTeks);
    GambarInputText((Rectangle){ (float)inputX, (float)(inputY + 30), (float)inputWidth, (float)inputHeight },
                    "Contoh: Alprazolam 0.5mg", appState.formResep.namaObat, 100, 22);
    
    inputY += spacing;
    UI_DrawText("Dosis *", inputX, inputY, FONT_BODY, warnaTeks);
    GambarInputText((Rectangle){ (float)inputX, (float)(inputY + 30), (float)inputWidth, (float)inputHeight },
                    "Contoh: 1 tablet", appState.formResep.dosis, 50, 23);
    
    inputY += spacing;
    UI_DrawText("Frekuensi *", inputX, inputY, FONT_BODY, warnaTeks);
    GambarInputText((Rectangle){ (float)inputX, (float)(inputY + 30), (float)inputWidth, (float)inputHeight },
                    "Contoh: 2x sehari", appState.formResep.frekuensi, 50, 24);
    
    inputY += spacing;
    UI_DrawText("Durasi *", inputX, inputY, FONT_BODY, warnaTeks);
    GambarInputText((Rectangle){ (float)inputX, (float)(inputY + 30), (float)inputWidth, (float)inputHeight },
                    "Contoh: 14 hari", appState.formResep.durasi, 50, 25);
    
    // Render Tombol Aksi
    // Tombol Batal
    DrawRectangleRounded(tombolBatal, 0.3f, 0, hoverBatal ? Fade(RED, 0.2f) : Fade(RED, 0.1f));
    DrawRectangleRoundedLines(tombolBatal, 0.3f, 0, RED);
    int batalWidth = MeasureText("Batal", FONT_BUTTON);
    DrawText("Batal", (int)tombolBatal.x + (180 - batalWidth)/2, (int)tombolBatal.y + 17, FONT_BUTTON, RED);
    
    // Tombol Simpan
    DrawRectangleRounded(tombolSimpan, 0.3f, 0, hoverSimpan ? Fade(COL_SECONDARY, 0.9f) : COL_SECONDARY);
    int simpanWidth = MeasureText("Simpan", FONT_BUTTON);
    DrawText("Simpan", (int)tombolSimpan.x + (180 - simpanWidth)/2, (int)tombolSimpan.y + 17, FONT_BUTTON, WHITE);
    
    if (hoverBatal && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        memset(&appState.formResep, 0, sizeof(FormResep));
        appState.currentPage = PAGE_RESEP;
    }
    
    if (hoverSimpan && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (appState.formResep.idPasien[0] && appState.formResep.tanggalResep[0] && 
            appState.formResep.namaObat[0] && appState.formResep.dosis[0] && 
            appState.formResep.frekuensi[0] && appState.formResep.durasi[0]) {
            Resep resepBaru;
            resepBaru.idPasien = atoi(appState.formResep.idPasien);
            resepBaru.idRekamMedis = 0; // Optional
            strcpy(resepBaru.tanggalResep, appState.formResep.tanggalResep);
            strcpy(resepBaru.namaObat, appState.formResep.namaObat);
            strcpy(resepBaru.dosis, appState.formResep.dosis);
            strcpy(resepBaru.frekuensi, appState.formResep.frekuensi);
            strcpy(resepBaru.durasi, appState.formResep.durasi);
            strcpy(resepBaru.aturanPakai, "Sesudah makan");
            strcpy(resepBaru.catatan, "-");
            
            if (tambahResep(&resepBaru) == 0) {
                memset(&appState.formResep, 0, sizeof(FormResep));
                appState.currentPage = PAGE_RESEP;
            }
        }
    }
    
    DrawText("* Field wajib diisi", inputX, tombolY + 75, FONT_SMALL, Fade(warnaTeksSekunder, 0.7f));
}

void GambarHalamanExport(int x, int y, int lebar, int tinggi) {
    GambarHalamanSegeraHadir(x, y, lebar, tinggi, "Export Data");
}
