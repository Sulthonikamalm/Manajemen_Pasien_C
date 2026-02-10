#ifndef GUI_LAYOUT_H
#define GUI_LAYOUT_H

#include <raylib.h>

// Warna - DEEPUI - MODERN PALETTE
// -----------------------------------------------------
// Primary Colors
#define COL_PRIMARY      (Color){ 30, 144, 255, 255 }  // Dodger Blue
#define COL_PRIMARY_DARK (Color){ 25, 118, 210, 255 } 
#define COL_SECONDARY    (Color){ 0, 200, 83, 255 }   // Green Accent
#define COL_ACCENT       (Color){ 255, 171, 64, 255 } // Orange Accent

// Surface Colors (Light Mode)
#define COL_BG_LIGHT     (Color){ 245, 247, 250, 255 } // Soft Gray Background
#define COL_CARD_LIGHT   (Color){ 255, 255, 255, 255 } // White Card
#define COL_TEXT_LIGHT   (Color){ 33, 33, 33, 255 }    // Dark Text
#define COL_TEXT_SEC_LIGHT (Color){ 117, 117, 117, 255 } // Gray Text
#define COL_BORDER_LIGHT (Color){ 224, 224, 224, 255 }

// Surface Colors (Dark Mode)
#define COL_BG_DARK      (Color){ 18, 18, 18, 255 }    // Deep Dark
#define COL_CARD_DARK    (Color){ 30, 30, 30, 255 }    // Elevation 1
#define COL_TEXT_DARK    (Color){ 255, 255, 255, 255 } // White Text
#define COL_TEXT_SEC_DARK (Color){ 179, 179, 179, 255 }
#define COL_BORDER_DARK  (Color){ 66, 66, 66, 255 }

// Layout Constants - MODERN & SPACIOUS
#define SIDEBAR_WIDTH    280
#define NAVBAR_HEIGHT    80
#define PADDING          35
#define CARD_RADIUS      16

// Font Sizes - LARGE & READABLE
#define FONT_HEADER      36
#define FONT_TITLE       28
#define FONT_SUBTITLE    22
#define FONT_BODY        20
#define FONT_BUTTON      20
#define FONT_TABLE       18
#define FONT_SMALL       16
#define FONT_TINY        14

// Spacing - GENEROUS
#define SPACING_SMALL    15
#define SPACING_MEDIUM   30
#define SPACING_LARGE    45
#define SPACING_XLARGE   60

// State
typedef enum {
    PAGE_DASHBOARD,
    PAGE_PASIEN,
    PAGE_PASIEN_ADD,
    PAGE_PASIEN_EDIT,
    PAGE_REKAM_MEDIS,
    PAGE_REKAM_MEDIS_ADD,
    PAGE_RESEP,
    PAGE_RESEP_ADD,
    PAGE_EXPORT,
    PAGE_SETTINGS
} PageState;

// Dialog State
typedef enum {
    DIALOG_NONE,
    DIALOG_DELETE_CONFIRM,
    DIALOG_SUCCESS,
    DIALOG_ERROR
} DialogState;

// Form State untuk Add/Edit
typedef struct {
    int editingId;
    char nama[100];
    char nik[20];
    char jenisKelamin[15];
    char tanggalLahir[15];
    char alamat[200];
    char telepon[20];
    char riwayatPenyakit[500];
    char status[15];
} FormPasien;

typedef struct {
    char idPasien[10];
    char tanggalKunjungan[15];
    char keluhan[500];
    char diagnosis[200];
} FormRekamMedis;

typedef struct {
    char idPasien[10];
    char tanggalResep[15];
    char namaObat[100];
    char dosis[50];
    char frekuensi[50];
    char durasi[50];
} FormResep;

typedef struct {
    int isDarkMode;
    PageState currentPage;
    int scrollOffset;
    int selectedPasienId;
    char searchQuery[100];
    
    // Dialog
    DialogState dialogState;
    int dialogTargetId;
    char dialogMessage[200];
    
    // Form states
    FormPasien formPasien;
    FormRekamMedis formRekamMedis;
    FormResep formResep;
    
    // Input focus
    int focusedInput;
    
    // Font Assets
    Font fontRegular;
    Font fontBold;

    // Animation States
    float sidebarAnim;
    float buttonHoverAnim;
} AppState;

// Helper Global untuk Custom Font
void UI_DrawText(const char *text, float x, float y, float fontSize, Color color);
void UI_DrawTextBg(const char *text, float x, float y, float fontSize, Color color, Color bgColor);
float UI_MeasureText(const char *text, float fontSize);

#endif
