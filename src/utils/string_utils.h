#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdbool.h>

// Fungsi utilitas string
void rapikanString(char *str);
void keHurufKecil(char *str);
void keHurufBesar(char *str);
void kapitalisasiKata(char *str);
bool mengandungString(const char *teksUtama, const char *teksYangDicari);
bool dimulaiDengan(const char *str, const char *awalan);
bool diakhiriDengan(const char *str, const char *akhiran);
int pisahkanString(char *str, char pemisah, char **token, int maksToken);
void gantiKarakter(char *str, char cari, char ganti);
void salinString(char *tujuan, const char *sumber, int maksPanjang);
int bandingkanStringIgnoreCase(const char *str1, const char *str2);

#endif
