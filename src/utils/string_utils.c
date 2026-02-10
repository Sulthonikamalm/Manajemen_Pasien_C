#include "string_utils.h"
#include <string.h>
#include <ctype.h>

// Hapus spasi dari awal dan akhir string
void rapikanString(char *str) {
    if (str == NULL) return;
    
    // Hapus spasi di awal
    int awal = 0;
    while (str[awal] && isspace((unsigned char)str[awal])) {
        awal++;
    }
    
    // Hapus spasi di akhir
    int akhir = strlen(str) - 1;
    while (akhir >= awal && isspace((unsigned char)str[akhir])) {
        akhir--;
    }
    
    // Pindahkan string yang sudah dirapikan ke awal
    int i;
    for (i = 0; i <= akhir - awal; i++) {
        str[i] = str[awal + i];
    }
    str[i] = '\0';
}

// Ubah string menjadi huruf kecil
void keHurufKecil(char *str) {
    if (str == NULL) return;
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Ubah string menjadi huruf besar
void keHurufBesar(char *str) {
    if (str == NULL) return;
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

// Kapitalisasi huruf pertama setiap kata
void kapitalisasiKata(char *str) {
    if (str == NULL) return;
    
    bool kataBaru = true;
    for (int i = 0; str[i]; i++) {
        if (isspace((unsigned char)str[i])) {
            kataBaru = true;
        } else if (kataBaru) {
            str[i] = toupper((unsigned char)str[i]);
            kataBaru = false;
        } else {
            str[i] = tolower((unsigned char)str[i]);
        }
    }
}

// Cek apakah string mengandung substring
bool mengandungString(const char *teksUtama, const char *teksYangDicari) {
    if (teksUtama == NULL || teksYangDicari == NULL) return false;
    return strstr(teksUtama, teksYangDicari) != NULL;
}

// Cek apakah string dimulai dengan prefix
bool dimulaiDengan(const char *str, const char *awalan) {
    if (str == NULL || awalan == NULL) return false;
    return strncmp(str, awalan, strlen(awalan)) == 0;
}

// Cek apakah string diakhiri dengan suffix
bool diakhiriDengan(const char *str, const char *akhiran) {
    if (str == NULL || akhiran == NULL) return false;
    
    int panjangStr = strlen(str);
    int panjangAkhiran = strlen(akhiran);
    
    if (panjangAkhiran > panjangStr) return false;
    
    return strcmp(str + panjangStr - panjangAkhiran, akhiran) == 0;
}

// Pisahkan string berdasarkan delimiter
int pisahkanString(char *str, char pemisah, char **token, int maksToken) {
    if (str == NULL || token == NULL) return 0;
    
    int jumlah = 0;
    char *bagian = strtok(str, &pemisah);
    
    while (bagian != NULL && jumlah < maksToken) {
        token[jumlah++] = bagian;
        bagian = strtok(NULL, &pemisah);
    }
    
    return jumlah;
}

// Ganti karakter dalam string
void gantiKarakter(char *str, char cari, char ganti) {
    if (str == NULL) return;
    
    for (int i = 0; str[i]; i++) {
        if (str[i] == cari) {
            str[i] = ganti;
        }
    }
}

// Salin string dengan aman
void salinString(char *tujuan, const char *sumber, int maksPanjang) {
    if (tujuan == NULL || sumber == NULL) return;
    
    strncpy(tujuan, sumber, maksPanjang - 1);
    tujuan[maksPanjang - 1] = '\0';
}

// Bandingkan string tanpa memperhatikan huruf besar/kecil
int bandingkanStringIgnoreCase(const char *str1, const char *str2) {
    if (str1 == NULL || str2 == NULL) return -1;
    
    while (*str1 && *str2) {
        char c1 = tolower((unsigned char)*str1);
        char c2 = tolower((unsigned char)*str2);
        
        if (c1 != c2) {
            return c1 - c2;
        }
        
        str1++;
        str2++;
    }
    
    return tolower((unsigned char)*str1) - tolower((unsigned char)*str2);
}
