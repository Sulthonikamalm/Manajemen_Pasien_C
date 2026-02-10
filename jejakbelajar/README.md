# 🏥 Sistem Manajemen Rumah Sakit Jiwa - Versi Console

<div align="center">

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Console Application](https://img.shields.io/badge/Console-Application-green?style=for-the-badge)
![Algorithm](https://img.shields.io/badge/Algorithm-Implementation-orange?style=for-the-badge)

**Tugas Besar Mata Kuliah Algoritma & Pemrograman**

</div>

---

## ⚠️ PERNYATAAN KEASLIAN KODE

> **PENTING**: Folder ini berisi kode **ASLI** dan **ORIGINAL** yang dibuat untuk memenuhi Tugas Besar Mata Kuliah Algoritma & Pemrograman pada Semester 1.
> 
> **Kode dalam folder ini:**
> - ✅ Dikerjakan secara **INDIVIDU** (Personal Project)
> - ✅ **Mahasiswa**: Sulthonika Mahfudz Al Mujahidin (NIM: sesuai)
> - ✅ Dikerjakan pada **Semester 1** sebagai Tugas Besar
> - ✅ **TIDAK ADA PERUBAHAN** satupun dari versi yang dikumpulkan
> - ✅ Disimpan sebagai **BUKTI KEASLIAN** untuk proses pembelajaran
> - ✅ Menjadi **BASELINE** untuk pengembangan lebih lanjut
> 
> **Tujuan Folder Ini:**
> - Membuktikan bahwa kode asli benar-benar hasil karya sendiri
> - Menunjukkan progression dari versi console ke versi desktop GUI
> - Menjaga integritas akademik dan transparansi proses belajar
> - Dokumentasi untuk portofolio dan pengembangan skill C programming
> 
> **Tanggal Pengumpulan Asli**: Semester 1 (Tahun Akademik sesuai periode kuliah)  
> **Status**: ORIGINAL - TIDAK DIMODIFIKASI  
> **Hash Commit Git**: Tersimpan dalam version control untuk verifikasi

---

## 📋 Deskripsi Proyek

Aplikasi console berbasis **C** untuk mengelola data **Pasien Rumah Sakit Jiwa SultHim** menggunakan implementasi **Array of Struct** dan **File I/O**. Proyek ini dibuat sebagai Tugas Besar mata kuliah **Algoritma & Pemrograman** pada semester 1.

Aplikasi ini memungkinkan pengguna untuk:
- Menambah data pasien baru
- Menampilkan semua data pasien
- Mencari pasien berdasarkan nama
- Mengurutkan data pasien berdasarkan usia
- Menyimpan dan membaca data dari file text

---

## 🏗️ Konsep Algoritma yang Diterapkan

| Konsep | Penerapan |
|--------|-----------|
| **Struct** | Penyimpanan data pasien (nama, jenis kelamin, usia, riwayat penyakit) |
| **Array** | Menyimpan multiple data pasien (max 100 pasien) |
| **Bubble Sort** | Algoritma pengurutan data pasien berdasarkan usia O(n²) |
| **Linear Search** | Algoritma pencarian pasien berdasarkan nama O(n) |
| **File I/O** | Persistensi data menggunakan file text |
| **Function** | Modularisasi kode dengan fungsi-fungsi terpisah |

---

## 📁 Struktur File

```
jejakbelajar/
├── main.c            # Source code lengkap aplikasi
├── data_pasien.txt   # File penyimpanan data (auto-generated)
└── README.md         # Dokumentasi proyek (file ini)
```

---

## 🔧 Cara Compile & Run

### Menggunakan GCC (MinGW)
```bash
gcc main.c -o main.exe
./main.exe
```

### Menggunakan Code::Blocks
1. Buka `main.c` di IDE
2. Tekan **F9** atau **Build and Run**

---

## 🎮 Fitur Aplikasi

| Menu | Fungsi |
|------|--------|
| 1 | Tambah Data Pasien |
| 2 | Tampilkan Data Pasien |
| 3 | Cari Pasien (berdasarkan nama) |
| 4 | Urutkan Data Pasien berdasarkan Usia |
| 5 | Keluar dari Sistem |

---

## 📊 Struktur Data

### Struct Pasien
```c
struct Pasien {
    char nama[50];              // Nama lengkap pasien
    char jenisKelamin[10];      // Laki-laki / Perempuan
    int usia;                   // Usia pasien
    char riwayatPenyakit[100];  // Riwayat penyakit mental
};
```

### Array of Struct
```
dataPasien[0] → {nama: "Ahmad", jenisKelamin: "Laki-laki", usia: 35, ...}
dataPasien[1] → {nama: "Siti", jenisKelamin: "Perempuan", usia: 28, ...}
dataPasien[2] → {nama: "Budi", jenisKelamin: "Laki-laki", usia: 42, ...}
...
dataPasien[99] → (max capacity)
```

---

## 🧮 Algoritma yang Diimplementasikan

### 1. Bubble Sort (Pengurutan)
```
Kompleksitas Waktu: O(n²)
Digunakan untuk: Mengurutkan pasien berdasarkan usia

Pseudocode:
for i = 0 to n-1
    for j = 0 to n-i-1
        if pasien[j].usia > pasien[j+1].usia
            swap(pasien[j], pasien[j+1])
```

### 2. Linear Search (Pencarian)
```
Kompleksitas Waktu: O(n)
Digunakan untuk: Mencari pasien berdasarkan nama

Pseudocode:
for i = 0 to n-1
    if strcmp(pasien[i].nama, namaCari) == 0
        return i
return -1 (tidak ditemukan)
```

### 3. File I/O (Persistensi Data)
```
Write: Simpan semua data pasien ke file text
Read: Baca data pasien dari file saat program start
Format: nama jenisKelamin usia riwayatPenyakit
```

---

## 💡 Fitur Utama

### ✅ CRUD Operations
- **Create**: Tambah data pasien baru dengan validasi input
- **Read**: Tampilkan semua data pasien dalam format tabel
- **Update**: (Belum diimplementasikan di versi ini)
- **Delete**: (Belum diimplementasikan di versi ini)

### ✅ Search & Sort
- **Search**: Pencarian linear berdasarkan nama pasien
- **Sort**: Pengurutan bubble sort berdasarkan usia

### ✅ Data Persistence
- **Auto-save**: Data otomatis tersimpan setelah tambah pasien
- **Auto-load**: Data otomatis dimuat saat program start

---

## 🎯 Pembelajaran yang Didapat

Dari proyek ini, saya belajar:
1. **Struktur Data Dasar**: Penggunaan struct dan array dalam C
2. **Algoritma Sorting**: Implementasi Bubble Sort O(n²)
3. **Algoritma Searching**: Implementasi Linear Search O(n)
4. **File Handling**: Membaca dan menulis data ke file
5. **Modular Programming**: Pemisahan fungsi untuk maintainability
6. **Input Validation**: Handling user input dengan scanf
7. **Memory Management**: Penggunaan array statis dengan batas maksimal

---

## 🚀 Pengembangan Selanjutnya

Versi console ini akan dikembangkan menjadi **Desktop Application dengan GTK+** yang mencakup:

### Upgrade Fitur:
- ✨ GUI modern dengan GTK+ 3
- ✨ CRUD lengkap (tambah Update & Delete)
- ✨ Rekam medis per pasien
- ✨ Sistem resep obat
- ✨ Dashboard dengan statistik
- ✨ Dark/Light mode
- ✨ Export & Print functionality
- ✨ CSV storage (upgrade dari txt)
- ✨ Binary Search O(log n) untuk data terurut
- ✨ Quick Sort O(n log n) untuk dataset besar

### Algoritma yang Ditambahkan:
- Binary Search O(log n)
- Quick Sort O(n log n)
- Merge Sort O(n log n)
- Pattern Matching untuk filter
- Multi-criteria sorting

---

## 👨‍🎓 Informasi Akademik

| | |
|---|---|
| **Mahasiswa** | Sulthonika Mahfudz Al Mujahidin |
| **NIM** | (sesuai NIM) |
| **Mata Kuliah** | Algoritma & Pemrograman |
| **Jenis Tugas** | Tugas Besar |
| **Semester** | 1 |
| **Bahasa Pemrograman** | C |

---

## 🙏 Ucapan Terima Kasih

Terima kasih yang sebesar-besarnya kepada:

### **Bapak Bernadus Anggo Seno Aji, S.Kom., M.Kom.**
Selaku Dosen pengampu mata kuliah Algoritma & Pemrograman yang telah memberikan ilmu, bimbingan, dan arahan dalam memahami konsep dasar pemrograman C, algoritma sorting dan searching, serta implementasi file handling.

---

## 📝 Catatan Teknis

### Limitasi Versi Console:
- Maksimal 100 pasien (array statis)
- Bubble Sort O(n²) - lambat untuk data besar
- Linear Search O(n) - tidak optimal
- File text - mudah corrupt
- Tidak ada validasi input yang kuat
- Tidak ada fitur Update & Delete
- UI console sederhana

### Kelebihan:
- Simple & mudah dipahami
- Tidak perlu library eksternal
- Portable (bisa jalan di mana saja)
- Cocok untuk pembelajaran dasar
- File size kecil

---

## 📈 Progression Timeline

```
Semester 1 (Console)          Semester 6 (Desktop GUI)
        │                              │
        ▼                              ▼
┌───────────────┐            ┌──────────────────┐
│ Array of      │            │ Dynamic Memory   │
│ Struct        │───────────▶│ Allocation       │
└───────────────┘            └──────────────────┘
        │                              │
        ▼                              ▼
┌───────────────┐            ┌──────────────────┐
│ Bubble Sort   │            │ Quick Sort       │
│ O(n²)         │───────────▶│ O(n log n)       │
└───────────────┘            └──────────────────┘
        │                              │
        ▼                              ▼
┌───────────────┐            ┌──────────────────┐
│ Linear Search │            │ Binary Search    │
│ O(n)          │───────────▶│ O(log n)         │
└───────────────┘            └──────────────────┘
        │                              │
        ▼                              ▼
┌───────────────┐            ┌──────────────────┐
│ Text File     │            │ CSV File         │
│ Storage       │───────────▶│ + Export/Print   │
└───────────────┘            └──────────────────┘
        │                              │
        ▼                              ▼
┌───────────────┐            ┌──────────────────┐
│ Console UI    │            │ GTK+ GUI         │
│               │───────────▶│ + Dark Mode      │
└───────────────┘            └──────────────────┘
```

---

## 🔗 Repository

Project ini tersimpan di GitHub untuk version control dan dokumentasi:
- **Original Version (Semester 1)**: `/jejakbelajar/` folder
- **Enhanced Version (Semester 6)**: Root project folder

---

<div align="center">

**Made with ❤️ for learning C Programming & Algorithms**

*Sulthonika Mahfudz Al Mujahidin*

**Rumah Sakit Jiwa SultHim - Sistem Manajemen Pasien**

---

*"From Console to Desktop - A Journey of Learning"*

</div>
