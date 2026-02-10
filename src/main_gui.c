#include <raylib.h>
#include "gui/gui_layout.h"
#include "gui/gui_pages.h"
#include "gui/ui_components.h"
#include "models/pasien.h"
#include "models/rekam_medis.h"
#include "models/resep.h"
#include "utils/csv_handler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// State Global
AppState appState = {
    .isDarkMode = 0,
    .currentPage = PAGE_DASHBOARD,
    .scrollOffset = 0,
    .sidebarAnim = 1.0f,
    .selectedPasienId = 0,
    .searchQuery = "",
    .dialogState = DIALOG_NONE,
    .dialogTargetId = 0,
    .dialogMessage = "",
    .formPasien = {0},
    .formRekamMedis = {0},
    .formResep = {0},
    .focusedInput = -1
};

// Cache Data
Pasien *cachedPasien = NULL;
int cachedPasienCount = 0;
int statsTotal = 0;
int statsAktif = 0;

// Helper Internal
void GambarSidebar(int tinggiLayar);
void MuatDataPasien();
void MuatStatistik();

// Titik Masuk Utama
int main(void)
{
    // Inisialisasi
    const int lebarLayar = 1280;
    const int tinggiLayar = 720;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(lebarLayar, tinggiLayar, "RS Jiwa SultHim - DeepUI System");

    SetTargetFPS(60);
    
    // Konfigurasi Font DeepUI (Segoe UI)
    // Muat font dengan flag anti-aliasing (bilinear filtering)
    appState.fontRegular = LoadFontEx("C:/Windows/Fonts/segoeui.ttf", 64, 0, 0); // Load high-res 64px
    appState.fontBold = LoadFontEx("C:/Windows/Fonts/segoeuib.ttf", 64, 0, 0);   // Bold version
    
    // Set filter texture agar smooth saat discale down
    SetTextureFilter(appState.fontRegular.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(appState.fontBold.texture, TEXTURE_FILTER_BILINEAR);
    
    // Muat data awal
    MuatStatistik();
    MuatDataPasien();

    // Loop utama
    while (!WindowShouldClose())
    {
        // Logika Update
        int lebar = GetScreenWidth();
        int tinggi = GetScreenHeight();
        
        // Logika Scroll
        float roda = GetMouseWheelMove();
        if (roda != 0) {
            appState.scrollOffset -= (int)(roda * 3.0f);
            if (appState.scrollOffset < 0) appState.scrollOffset = 0;
        }
        
        // Render
        BeginDrawing();
            Color warnaBg = appState.isDarkMode ? COL_BG_DARK : COL_BG_LIGHT;
            ClearBackground(warnaBg);

            // Layout
            GambarSidebar(tinggi);
            
            // Kalkulasi area konten
            int kontenX = SIDEBAR_WIDTH;
            int lebarKonten = lebar - SIDEBAR_WIDTH;
            
            // Routing Halaman
            switch (appState.currentPage) {
                case PAGE_DASHBOARD:
                    GambarHalamanDashboard(kontenX, 0, lebarKonten, tinggi);
                    break;
                case PAGE_PASIEN:
                    GambarHalamanPasien(kontenX, 0, lebarKonten, tinggi);
                    break;
                case PAGE_PASIEN_ADD:
                    GambarHalamanTambahPasien(kontenX, 0, lebarKonten, tinggi);
                    break;
                case PAGE_PASIEN_EDIT:
                    GambarHalamanEditPasien(kontenX, 0, lebarKonten, tinggi);
                    break;
                case PAGE_REKAM_MEDIS:
                    GambarHalamanRekamMedis(kontenX, 0, lebarKonten, tinggi);
                    break;
                case PAGE_REKAM_MEDIS_ADD:
                    GambarHalamanTambahRekamMedis(kontenX, 0, lebarKonten, tinggi);
                    break;
                case PAGE_RESEP:
                    GambarHalamanResep(kontenX, 0, lebarKonten, tinggi);
                    break;
                case PAGE_RESEP_ADD:
                    GambarHalamanTambahResep(kontenX, 0, lebarKonten, tinggi);
                    break;
                case PAGE_EXPORT:
                    GambarHalamanExport(kontenX, 0, lebarKonten, tinggi);
                    break;
                default:
                    GambarHalamanSegeraHadir(kontenX, 0, lebarKonten, tinggi, "Halaman Tidak Ditemukan");
                    break;
            }

            // Gambar dialog jika ada
            if (appState.dialogState == DIALOG_DELETE_CONFIRM) {
                int result = GambarDialogKonfirmasi("Konfirmasi Hapus", appState.dialogMessage);
                if (result == 1) {
                    // Ya, hapus - panggil fungsi sesuai halaman aktif
                    if (appState.currentPage == PAGE_PASIEN || 
                        appState.currentPage == PAGE_PASIEN_ADD || 
                        appState.currentPage == PAGE_PASIEN_EDIT) {
                        hapusPasien(appState.dialogTargetId);
                        MuatDataPasien();
                    } else if (appState.currentPage == PAGE_REKAM_MEDIS || 
                               appState.currentPage == PAGE_REKAM_MEDIS_ADD) {
                        hapusRekamMedis(appState.dialogTargetId);
                    } else if (appState.currentPage == PAGE_RESEP || 
                               appState.currentPage == PAGE_RESEP_ADD) {
                        hapusResep(appState.dialogTargetId);
                    }
                    appState.dialogState = DIALOG_NONE;
                } else if (result == 0) {
                    // Tidak
                    appState.dialogState = DIALOG_NONE;
                }
            }

        EndDrawing();
    }

    // Pembersihan
    if (cachedPasien) free(cachedPasien);
    UnloadFont(appState.fontRegular);
    UnloadFont(appState.fontBold);
    CloseWindow();
    return 0;
}

void MuatStatistik() {
    statsTotal = hitungTotalPasien();
    statsAktif = hitungPasienAktif();
}

void MuatDataPasien() {
    if (cachedPasien) {
        free(cachedPasien);
        cachedPasien = NULL;
    }
    cachedPasienCount = getAllPasien(&cachedPasien);
}

void GambarSidebar(int tinggiLayar) {
    Color warnaSidebar = appState.isDarkMode ? COL_CARD_DARK : COL_CARD_LIGHT;
    Color warnaTeks = appState.isDarkMode ? COL_TEXT_DARK : COL_TEXT_LIGHT;
    
    DrawRectangle(0, 0, SIDEBAR_WIDTH, tinggiLayar, warnaSidebar);
    
    // Kontainer Logo / Judul dengan tinggi lebih besar
    DrawRectangle(0, 0, SIDEBAR_WIDTH, NAVBAR_HEIGHT, COL_PRIMARY);
    DrawText("RSJ SULTHIM", 35, 22, FONT_TITLE, WHITE);
    
    // Menu Navigasi dengan spacing lebih besar
    int mulaiY = 120;
    int tinggiTombol = 60;

    if (GambarTombolNav("Dashboard", mulaiY, appState.currentPage == PAGE_DASHBOARD)) {
        appState.currentPage = PAGE_DASHBOARD;
        appState.scrollOffset = 0;
    }

    if (GambarTombolNav("Data Pasien", mulaiY + tinggiTombol, 
                        appState.currentPage == PAGE_PASIEN || 
                        appState.currentPage == PAGE_PASIEN_ADD || 
                        appState.currentPage == PAGE_PASIEN_EDIT)) {
        if (appState.currentPage != PAGE_PASIEN) { // Reset hanya jika pindah dari luar
             appState.scrollOffset = 0;
             MuatDataPasien();
        }
        appState.currentPage = PAGE_PASIEN;
    }

    if (GambarTombolNav("Rekam Medis", mulaiY + (tinggiTombol * 2), 
                        appState.currentPage == PAGE_REKAM_MEDIS || 
                        appState.currentPage == PAGE_REKAM_MEDIS_ADD)) {
        appState.currentPage = PAGE_REKAM_MEDIS;
        appState.scrollOffset = 0;
    }

    if (GambarTombolNav("Resep Obat", mulaiY + (tinggiTombol * 3), 
                        appState.currentPage == PAGE_RESEP || 
                        appState.currentPage == PAGE_RESEP_ADD)) {
        appState.currentPage = PAGE_RESEP;
        appState.scrollOffset = 0;
    }
    
    if (GambarTombolNav("Export Data", mulaiY + (tinggiTombol * 4), appState.currentPage == PAGE_EXPORT)) {
        appState.currentPage = PAGE_EXPORT;
        appState.scrollOffset = 0;
    }
    
    // Tombol Toggle Tema (Bawah) dengan ukuran lebih besar
    Rectangle tombolTema = { 15, (float)(tinggiLayar - 70), SIDEBAR_WIDTH - 30, 50 };
    bool hoverTema = CheckCollisionPointRec(GetMousePosition(), tombolTema);
    
    if (hoverTema) {
        DrawRectangleRounded(tombolTema, 0.2f, 0, Fade(COL_PRIMARY, 0.2f));
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            appState.isDarkMode = !appState.isDarkMode;
        }
    }
    
    const char *teksTema = appState.isDarkMode ? "TERANG" : "GELAP";
    DrawText(teksTema, 35, tinggiLayar - 55, FONT_BUTTON, warnaTeks);
}
