#include "ui_components.h"
#include <stdio.h>
#include <string.h>

// Helper: Interpolasi Linear untuk transisi warna yang halus
Color UI_InterpolasiWarna(Color warna1, Color warna2, float jumlah) {
    if (jumlah < 0) jumlah = 0;
    if (jumlah > 1) jumlah = 1;

    return (Color){
        (unsigned char)(warna1.r + (warna2.r - warna1.r) * jumlah),
        (unsigned char)(warna1.g + (warna2.g - warna1.g) * jumlah),
        (unsigned char)(warna1.b + (warna2.b - warna1.b) * jumlah),
        (unsigned char)(warna1.a + (warna2.a - warna1.a) * jumlah)
    };
}

extern AppState appState; // Akses global state untuk tema

// Helper Wrapper untuk DrawTextEx dengan Font Custom
void UI_DrawText(const char *text, float x, float y, float fontSize, Color color) {
    // Gunakan font regular by default, spacing 1px
    if (appState.fontRegular.texture.id != 0) {
        DrawTextEx(appState.fontRegular, text, (Vector2){x, y}, fontSize, 1.0f, color);
    } else {
        DrawText(text, (int)x, (int)y, (int)fontSize, color); // Fallback
    }
}

float UI_MeasureText(const char *text, float fontSize) {
    if (appState.fontRegular.texture.id != 0) {
        Vector2 size = MeasureTextEx(appState.fontRegular, text, fontSize, 1.0f);
        return size.x;
    } else {
        return MeasureText(text, (int)fontSize);
    }
}

// Menggambar tombol navigasi sidebar dengan efek hover
bool GambarTombolNav(const char *teks, int y, bool terpilih) {
    Color warnaHover = appState.isDarkMode ? Fade(COL_PRIMARY, 0.15f) : Fade(COL_PRIMARY, 0.1f);
    Color warnaPilih = appState.isDarkMode ? Fade(COL_PRIMARY, 0.3f) : Fade(COL_PRIMARY, 0.2f);
    Color warnaTeks = appState.isDarkMode ? COL_TEXT_DARK : COL_TEXT_LIGHT;
    
    Rectangle areaTombol = { 15, (float)y, SIDEBAR_WIDTH - 30, 50 };
    bool sedangHover = CheckCollisionPointRec(GetMousePosition(), areaTombol);
    bool diklik = false;

    // Logika visual
    if (terpilih) {
        DrawRectangleRounded(areaTombol, 0.2f, 0, warnaPilih);
        
        // Indikator aktif di kiri (lebih tebal)
        DrawRectangle(15, (int)y + 12, 5, 26, COL_PRIMARY);
        
        warnaTeks = COL_PRIMARY;
    } else if (sedangHover) {
        DrawRectangleRounded(areaTombol, 0.2f, 0, warnaHover);
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            diklik = true;
        }
    }

    // Gunakan UI_DrawText untuk font bagus
    UI_DrawText(teks, 60, (float)y + 15, FONT_BUTTON, warnaTeks);
    
    return diklik;
}

// Menggambar kartu statistik dengan bayangan dan aksen warna
void GambarKartuStat(Rectangle area, const char *judul, const char *nilai, const char *subjudul, Color warnaAksen) {
    Color warnaKartu = appState.isDarkMode ? COL_CARD_DARK : COL_CARD_LIGHT;
    Color warnaTeksSekunder = appState.isDarkMode ? COL_TEXT_SEC_DARK : COL_TEXT_SEC_LIGHT;

    // Efek bayangan (offset sederhana)
    DrawRectangleRounded((Rectangle){area.x + 3, area.y + 3, area.width, area.height}, 0.15f, 0, Fade(BLACK, 0.15f));
    
    // Kartu utama
    DrawRectangleRounded(area, 0.15f, 0, warnaKartu);
    
    // Border aksen
    DrawRectangleRoundedLines(area, 0.15f, 0, Fade(warnaAksen, 0.3f));

    // Judul dengan font lebih besar (BOLD)
    if (appState.fontBold.texture.id != 0) {
        DrawTextEx(appState.fontBold, judul, (Vector2){area.x + 25, area.y + 25}, FONT_SUBTITLE, 1.0f, warnaTeksSekunder);
    } else {
        DrawText(judul, (int)area.x + 25, (int)area.y + 25, FONT_SUBTITLE, warnaTeksSekunder);
    }
    
    // Nilai (font sangat besar untuk impact visual) - Bold juga
    if (appState.fontBold.texture.id != 0) {
        DrawTextEx(appState.fontBold, nilai, (Vector2){area.x + 25, area.y + 55}, 48, 1.0f, warnaAksen);
    } else {
        DrawText(nilai, (int)area.x + 25, (int)area.y + 55, 48, warnaAksen);
    }
    
    // Subjudul - Regular
    UI_DrawText(subjudul, area.x + 25, area.y + 110, FONT_BODY, Fade(warnaTeksSekunder, 0.7f));
}

// Menggambar header tabel dengan garis pemisah
void GambarHeaderTabel(float mulaiX, float mulaiY, float lebar, const char **kolom, const float *offsetKolomX, int jumlahKolom) {
    Color warnaBorder = appState.isDarkMode ? COL_BORDER_DARK : COL_BORDER_LIGHT;
    Color warnaTeksSekunder = appState.isDarkMode ? COL_TEXT_SEC_DARK : COL_TEXT_SEC_LIGHT;

    for (int i = 0; i < jumlahKolom; i++) {
        // Gunakan Bold untuk header tabel
        if (appState.fontBold.texture.id != 0) {
            DrawTextEx(appState.fontBold, kolom[i], (Vector2){mulaiX + offsetKolomX[i], mulaiY}, FONT_TABLE, 1.0f, warnaTeksSekunder);
        } else {
            DrawText(kolom[i], (int)(mulaiX + offsetKolomX[i]), (int)mulaiY, FONT_TABLE, warnaTeksSekunder);
        }
    }
    
    DrawLineEx((Vector2){mulaiX, mulaiY + 30}, (Vector2){mulaiX + lebar, mulaiY + 30}, 2, warnaBorder);
}

// Menggambar latar belakang baris tabel dengan efek zebra dan hover
void GambarLatarBarisTabel(Rectangle area, bool barisGenap, bool sedangHover) {
    Color warnaHover = Fade(COL_PRIMARY, 0.05f);
    Color warnaAlternatif = appState.isDarkMode ? Fade(WHITE, 0.02f) : Fade(BLACK, 0.03f);

    if (sedangHover) {
        DrawRectangleRec(area, warnaHover);
    } else if (barisGenap) {
        DrawRectangleRec(area, warnaAlternatif);
    }
}


// Menggambar dialog konfirmasi dengan desain modern
int GambarDialogKonfirmasi(const char *judul, const char *pesan) {
    int lebarLayar = GetScreenWidth();
    int tinggiLayar = GetScreenHeight();
    
    // Overlay gelap
    DrawRectangle(0, 0, lebarLayar, tinggiLayar, Fade(BLACK, 0.7f));
    
    // Dialog box
    int lebarDialog = 600;
    int tinggiDialog = 300;
    int dialogX = (lebarLayar - lebarDialog) / 2;
    int dialogY = (tinggiLayar - tinggiDialog) / 2;
    
    Rectangle dialogArea = { (float)dialogX, (float)dialogY, (float)lebarDialog, (float)tinggiDialog };
    
    Color warnaDialog = appState.isDarkMode ? COL_CARD_DARK : COL_CARD_LIGHT;
    Color warnaTeks = appState.isDarkMode ? COL_TEXT_DARK : COL_TEXT_LIGHT;
    
    // Shadow
    DrawRectangleRounded((Rectangle){dialogArea.x + 5, dialogArea.y + 5, dialogArea.width, dialogArea.height}, 0.15f, 0, Fade(BLACK, 0.3f));
    
    // Dialog background
    DrawRectangleRounded(dialogArea, 0.15f, 0, warnaDialog);
    DrawRectangleRoundedLines(dialogArea, 0.15f, 0, Fade(RED, 0.5f));
    
    // Icon warning (Ganti Unicode dgn ASCII Art sederhana atau Teks)
    DrawCircle(dialogX + lebarDialog/2, dialogY + 70, 35, Fade(RED, 0.2f));
    DrawText("!", dialogX + lebarDialog/2 - 5, dialogY + 45, 50, RED);
    
    // Judul
    float judulWidth = UI_MeasureText(judul, FONT_TITLE);
    if (appState.fontBold.texture.id != 0) {
        DrawTextEx(appState.fontBold, judul, (Vector2){dialogX + (lebarDialog - judulWidth)/2, dialogY + 130}, FONT_TITLE, 1.0f, warnaTeks);
    } else {
        DrawText(judul, dialogX + (lebarDialog - (int)judulWidth)/2, dialogY + 130, FONT_TITLE, warnaTeks);
    }
    
    // Pesan
    float pesanWidth = UI_MeasureText(pesan, FONT_BODY);
    UI_DrawText(pesan, dialogX + (lebarDialog - pesanWidth)/2, dialogY + 170, FONT_BODY, Fade(warnaTeks, 0.8f));
    
    // Tombol
    int tombolY = dialogY + 220;
    int tombolWidth = 150;
    int tombolHeight = 50;
    int tombolSpacing = 20;
    
    Rectangle tombolTidak = { (float)(dialogX + lebarDialog/2 - tombolWidth - tombolSpacing/2), (float)tombolY, (float)tombolWidth, (float)tombolHeight };
    Rectangle tombolYa = { (float)(dialogX + lebarDialog/2 + tombolSpacing/2), (float)tombolY, (float)tombolWidth, (float)tombolHeight };
    
    bool hoverTidak = CheckCollisionPointRec(GetMousePosition(), tombolTidak);
    bool hoverYa = CheckCollisionPointRec(GetMousePosition(), tombolYa);
    
    // Tombol Tidak
    DrawRectangleRounded(tombolTidak, 0.3f, 0, hoverTidak ? Fade(COL_TEXT_SEC_LIGHT, 0.3f) : Fade(COL_TEXT_SEC_LIGHT, 0.1f));
    DrawRectangleRoundedLines(tombolTidak, 0.3f, 0, COL_TEXT_SEC_LIGHT);
    float tidakWidth = UI_MeasureText("Tidak", FONT_BUTTON);
    UI_DrawText("Tidak", tombolTidak.x + (tombolWidth - tidakWidth)/2, tombolTidak.y + 13, FONT_BUTTON, COL_TEXT_SEC_LIGHT);
    
    // Tombol Ya
    DrawRectangleRounded(tombolYa, 0.3f, 0, hoverYa ? Fade(RED, 0.9f) : RED);
    float yaWidth = UI_MeasureText("Ya, Hapus", FONT_BUTTON);
    UI_DrawText("Ya, Hapus", tombolYa.x + (tombolWidth - yaWidth)/2, tombolYa.y + 13, FONT_BUTTON, WHITE);
    
    if (hoverTidak && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return 0; // Tidak
    }
    
    if (hoverYa && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return 1; // Ya
    }
    
    return -1; // Belum ada aksi
}

// Menggambar input text field modern
void GambarInputText(Rectangle area, const char *placeholder, char *buffer, int maxLen, int fieldId) {
    bool isFocused = (appState.focusedInput == fieldId);
    
    // Cek klik untuk set focus
    if (CheckCollisionPointRec(GetMousePosition(), area) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        appState.focusedInput = fieldId;
        isFocused = true;
    }

    Color warnaBg = appState.isDarkMode ? Fade(WHITE, 0.1f) : Fade(BLACK, 0.05f);
    Color warnaBorder = isFocused ? COL_PRIMARY : (appState.isDarkMode ? COL_BORDER_DARK : COL_BORDER_LIGHT);
    Color warnaTeks = appState.isDarkMode ? COL_TEXT_DARK : COL_TEXT_LIGHT;
    Color warnaPlaceholder = Fade(appState.isDarkMode ? COL_TEXT_SEC_DARK : COL_TEXT_SEC_LIGHT, 0.5f);
    
    // Background
    DrawRectangleRounded(area, 0.2f, 0, warnaBg);
    
    // Border (lebih tebal jika focused)
    if (isFocused) {
        DrawRectangleRoundedLines(area, 0.2f, 0, warnaBorder);
        DrawRectangleRoundedLines((Rectangle){area.x - 1, area.y - 1, area.width + 2, area.height + 2}, 0.2f, 0, Fade(warnaBorder, 0.5f));
    } else {
        DrawRectangleRoundedLines(area, 0.2f, 0, warnaBorder);
    }
    
    // Text atau placeholder
    const char *displayText = buffer[0] ? buffer : placeholder;
    Color textColor = buffer[0] ? warnaTeks : warnaPlaceholder;
    
    UI_DrawText(displayText, area.x + 20, area.y + 12, FONT_BODY, textColor);
    
    // Logic Input & Cursor
    if (isFocused) {
        // Logika Input Teks
        int key = GetCharPressed();
        while (key > 0) {
            // Hanya terima karakter yang bisa dicetak (ASCII 32-126)
            if ((key >= 32) && (key <= 125) && (strlen(buffer) < (size_t)maxLen)) {
                int len = strlen(buffer);
                buffer[len] = (char)key;
                buffer[len + 1] = '\0';
            }
            key = GetCharPressed();  // Cek next char
        }

        // Backspace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            int len = strlen(buffer);
            if (len > 0) {
                buffer[len - 1] = '\0';
            }
        }
        
        // Tab atau Enter untuk pindah focus
        if (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_ENTER)) {
            appState.focusedInput = -1;
        }
        
        // Blink cursor
        if (((int)(GetTime() * 2) % 2 == 0)) {
            float textWidth = UI_MeasureText(buffer, FONT_BODY);
            DrawRectangle((int)(area.x + 20 + textWidth), (int)area.y + 10, 2, 30, COL_PRIMARY);
        }
    }
}
