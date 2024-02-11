<div align="center">
    <h1>Penyelesaian Cyberpunk 2077 Breach Protocol dengan Algoritma Brute Force</h1>
    <h3>Tugas Kecil 1 IF2211 Strategi Algoritma</h3>

</br>
</br>
</div>

 Cyberpunk 2077 Breach Protocol adalah minigame meretas pada permainan video Cyberpunk 2077.
 Minigame ini merupakan simulasi peretasan jaringan local dari ICE (Intrusion Countermeasures
 Electronics) pada permainan Cyberpunk 2077. Komponen pada permainan ini antara lain adalah:
 1. Token–terdiri dari dua karakter alfanumerik seperti E9, BD, dan 55.
 2. Matriks– terdiri atas token-token yang akan dipilih untuk menyusun urutan kode.
 3. Sekuens–sebuah rangkaian token (dua atau lebih) yang harus dicocokkan.
 4. Buffer– jumlah maksimal token yang dapat disusun secara sekuensial.

 Aturan permainan Breach Protocol antara lain:
 1. Pemain bergerak dengan pola horizontal, vertikal, horizontal, vertikal (bergantian) hingga
 semua sekuens berhasil dicocokkan atau buffer penuh.
 2. Pemain memulai dengan memilih satu token pada posisi baris paling atas dari matriks.
 3. Sekuens dicocokkan pada token-token yang berada di buffer.
 4. Satutoken pada buffer dapat digunakan pada lebih dari satu sekuens.
 5. Setiap sekuens memiliki bobot hadiah atau reward yang variatif.
 6. Sekuens memiliki panjang minimal berupa dua token.

## Syarat menjalankan program
Install c++ compiler

## Cara mengkompilasi program
Buka folder projek ini lalu ketik:
```shell
g++ -o bin/main src/main.cpp
```

## Cara menjalankan program

Buka folder projek ini pada terminal lalu buka folder bin dengan:
```shell
cd bin
```

Lalu jalankan program:
```shell
./main
```

## Dibuat oleh
| NIM | Nama | Linkedin |
| :---: | :---: | :---: |
| 13522161 | Mohamad Akmal Ramadan | [LinkedIn](https://www.linkedin.com/in/akmalrmn/) |
