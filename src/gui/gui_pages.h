#ifndef GUI_PAGES_H
#define GUI_PAGES_H

#include "gui_layout.h"

// Fungsi Render Halaman Utama
/**
 * Menampilkan halaman dashboard dengan ringkasan statistik.
 */
void GambarHalamanDashboard(int x, int y, int lebar, int tinggi);

/**
 * Menampilkan halaman daftar pasien dalam bentuk tabel interaktif.
 */
void GambarHalamanPasien(int x, int y, int lebar, int tinggi);

/**
 * Menampilkan form tambah pasien baru.
 */
void GambarHalamanTambahPasien(int x, int y, int lebar, int tinggi);

/**
 * Menampilkan form edit pasien.
 */
void GambarHalamanEditPasien(int x, int y, int lebar, int tinggi);

/**
 * Menampilkan halaman rekam medis.
 */
void GambarHalamanRekamMedis(int x, int y, int lebar, int tinggi);

/**
 * Menampilkan form tambah rekam medis.
 */
void GambarHalamanTambahRekamMedis(int x, int y, int lebar, int tinggi);

/**
 * Menampilkan halaman resep obat.
 */
void GambarHalamanResep(int x, int y, int lebar, int tinggi);

/**
 * Menampilkan form tambah resep.
 */
void GambarHalamanTambahResep(int x, int y, int lebar, int tinggi);

/**
 * Menampilkan halaman export data.
 */
void GambarHalamanExport(int x, int y, int lebar, int tinggi);

/**
 * Menampilkan halaman placeholder untuk fitur 'Segera Hadir'.
 */
void GambarHalamanSegeraHadir(int x, int y, int lebar, int tinggi, const char *judul);

#endif
