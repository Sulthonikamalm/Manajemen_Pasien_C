#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H

#include "gui_layout.h"

// Komponen UI yang Dapat Digunakan Kembali
// --------------------------------------------------------------------------------

/**
 * Interpolasi linear untuk transisi warna yang halus.
 * @param warna1 Warna awal
 * @param warna2 Warna akhir
 * @param jumlah Nilai interpolasi (0.0 - 1.0)
 * @return Warna hasil interpolasi
 */
Color UI_InterpolasiWarna(Color warna1, Color warna2, float jumlah);

/**
 * Menggambar tombol navigasi sidebar dengan efek hover yang halus.
 * @param teks Label tombol
 * @param y Posisi Y
 * @param terpilih Status aktif halaman
 * @return True jika diklik
 */
bool GambarTombolNav(const char *teks, int y, bool terpilih);

/**
 * Menggambar kartu dashboard dengan bayangan dan border halus.
 * @param area Area kartu
 * @param judul Judul kartu
 * @param nilai Nilai utama (angka besar)
 * @param subjudul Subtitle/keterangan tambahan
 * @param warnaAksen Warna aksen untuk nilai
 */
void GambarKartuStat(Rectangle area, const char *judul, const char *nilai, const char *subjudul, Color warnaAksen);

/**
 * Menggambar header tabel yang konsisten.
 * @param mulaiX Posisi X awal
 * @param mulaiY Posisi Y awal
 * @param lebar Lebar total tabel
 * @param kolom Array string judul kolom
 * @param offsetKolomX Array offset X untuk setiap kolom
 * @param jumlahKolom Jumlah kolom
 */
void GambarHeaderTabel(float mulaiX, float mulaiY, float lebar, const char **kolom, const float *offsetKolomX, int jumlahKolom);

/**
 * Menggambar latar belakang baris tabel dengan efek hover dan zebra striping.
 * @param area Area baris
 * @param barisGenap Apakah baris genap (untuk zebra striping)
 * @param sedangHover Apakah mouse berada di atas baris ini
 */
void GambarLatarBarisTabel(Rectangle area, bool barisGenap, bool sedangHover);

/**
 * Menggambar dialog konfirmasi.
 * @param judul Judul dialog
 * @param pesan Pesan dialog
 * @param onYes Callback ketika tombol Ya diklik
 * @param onNo Callback ketika tombol Tidak diklik
 * @return True jika Ya diklik, False jika Tidak diklik, -1 jika belum ada aksi
 */
int GambarDialogKonfirmasi(const char *judul, const char *pesan);

/**
 * Menggambar input text field.
 */
void GambarInputText(Rectangle area, const char *placeholder, char *buffer, int maxLen, int fieldId);

#endif
