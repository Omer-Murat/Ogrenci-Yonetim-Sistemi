// --- EN TEPEDE BU OLMALI (Hatalari Engeller) ---
#define NOMINMAX 

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <cstdlib> 
#include <windows.h> 

using namespace std;

// --- RENK KODLARI (Red, Green, White) ---
#define RESET   "\033[0m"
#define RED     "\033[31m"  // Kirmizi
#define GREEN   "\033[32m"  // Yesil
#define WHITE   "\033[97m"  // Parlak Beyaz
#define BOLD    "\033[1m"   // Kalin

// Dosya adı
const string DOSYA_ADI = "ogrenciler.txt";

// Ogrenci Yapisi
struct Ogrenci {
    string ogrenci_no = "";
    string ad = "";
    string soyad = "";
    double not_ortalamasi = 0.0;
};

// --- DOSYA ISLEMLERI ---
void DosyaYaz(const vector<Ogrenci>& ogrenciler) {
    ofstream dosya(DOSYA_ADI);
    if (!dosya.is_open()) return;
    for (const auto& o : ogrenciler) {
        dosya << o.ogrenci_no << " " << o.ad << " " << o.soyad << " " << o.not_ortalamasi << endl;
    }
    dosya.close();
}

void DosyaOku(vector<Ogrenci>& ogrenciler) {
    ifstream dosya(DOSYA_ADI);
    if (!dosya.is_open()) return;
    ogrenciler.clear();
    string no, ad, soyad;
    double ort;
    while (dosya >> no >> ad >> soyad >> ort) {
        ogrenciler.push_back({ no, ad, soyad, ort });
    }
    dosya.close();
}

// --- YARDIMCI FONKSIYONLAR ---
void EkranTemizle() {
    system("cls");
}

void DevamEtBekle() {
    cout << "\n" << GREEN << "Devam etmek icin bir tusa basin..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

template<typename T>
T GuvenliGirdiAl(const string& mesaj, T min_val, T max_val) {
    T girdi;
    cout << WHITE << mesaj << RESET;
    while (!(cin >> girdi) || girdi < min_val || girdi > max_val) {
        cout << RED << "Hatali giris! Tekrar deneyin: " << RESET;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return girdi;
}

// --- LOGO GÖSTERİMİ (Riskli Karakter Yok, Sadece # ve Renkler) ---
void BaslangicLogosuGoster() {
    EkranTemizle();

    // KIRMIZI CERCEVE
    cout << BOLD << RED << "==========================================================" << RESET << endl;
    cout << BOLD << RED << "||                                                      ||" << RESET << endl;

    // YESIL LOGO (Standart '#' karakteri ile yapildi, bozulmaz)
    cout << BOLD << GREEN << R"(
||    ##     ##  ##   ##  #######     ###    ########   ||
||    ###   ###  ##   ##  ##    ##   ## ##      ##      ||
||    #### ####  ##   ##  ##    ##  ##   ##     ##      ||
||    ## ### ##  ##   ##  #######  ##     ##    ##      ||
||    ##     ##  ##   ##  ##   ##  #########    ##      ||
||    ##     ##   #####   ##    ## ##     ##    ##      ||
    )" << RESET << endl;

    cout << BOLD << RED << "||                                                      ||" << RESET << endl;
    cout << BOLD << RED << "==========================================================" << RESET << endl;

    // BEYAZ YAZI
    cout << "\n" << BOLD << WHITE << "      >>> OGRENCI YONETIM SISTEMINE HOS GELDINIZ <<<" << RESET << endl;
    cout << "\n" << RED << "==========================================================" << RESET << endl;

    cout << "\n" << GREEN << "Sistem baslatiliyor... Enter'a basin." << RESET;
    cin.get();
}

// --- ISLEM FONKSIYONLARI ---
void OgrenciEkle(vector<Ogrenci>& ogrenciler) {
    EkranTemizle();
    cout << RED << "--- " << WHITE << "OGRENCI EKLEME" << RED << " ---" << RESET << endl;

    Ogrenci yeni;
    cout << WHITE << "Ogrenci No: " << RESET; cin >> yeni.ogrenci_no;

    for (const auto& o : ogrenciler) {
        if (o.ogrenci_no == yeni.ogrenci_no) {
            cout << RED << "Bu numara zaten kayitli!\n" << RESET;
            DevamEtBekle();
            return;
        }
    }

    cout << WHITE << "Ad: " << RESET; cin >> yeni.ad;
    cout << WHITE << "Soyad: " << RESET; cin >> yeni.soyad;
    yeni.not_ortalamasi = GuvenliGirdiAl<double>("Ortalama (0.0 - 4.0): ", 0.0, 4.0);

    ogrenciler.push_back(yeni);
    DosyaYaz(ogrenciler);
    cout << GREEN << "Basariyla eklendi.\n" << RESET;
    DevamEtBekle();
}

void OgrenciListele(const vector<Ogrenci>& ogrenciler) {
    EkranTemizle();
    cout << RED << "--- " << WHITE << "OGRENCI LISTESI" << RED << " ---" << RESET << endl;

    if (ogrenciler.empty()) {
        cout << RED << "Kayit bulunamadi.\n" << RESET;
    }
    else {
        cout << left << setw(10) << "NO" << setw(15) << "AD" << setw(15) << "SOYAD" << "ORT" << endl;
        cout << RED << "--------------------------------------------" << RESET << endl;
        for (const auto& o : ogrenciler) {
            cout << WHITE << left << setw(10) << o.ogrenci_no
                << setw(15) << o.ad
                << setw(15) << o.soyad
                << GREEN << fixed << setprecision(2) << o.not_ortalamasi << RESET << endl;
        }
    }
    DevamEtBekle();
}

void NotGuncelle(vector<Ogrenci>& ogrenciler) {
    EkranTemizle();
    cout << RED << "--- " << WHITE << "NOT GUNCELLE" << RED << " ---" << RESET << endl;
    string no;
    cout << WHITE << "No girin: " << RESET; cin >> no;

    bool bulundu = false;
    for (auto& o : ogrenciler) {
        if (o.ogrenci_no == no) {
            cout << "Bulunan: " << o.ad << " " << o.soyad << " (Mevcut: " << o.not_ortalamasi << ")\n";
            o.not_ortalamasi = GuvenliGirdiAl<double>("Yeni Ortalama: ", 0.0, 4.0);
            DosyaYaz(ogrenciler);
            cout << GREEN << "Guncellendi.\n" << RESET;
            bulundu = true;
            break;
        }
    }
    if (!bulundu) cout << RED << "Ogrenci bulunamadi.\n" << RESET;
    DevamEtBekle();
}

void OgrenciSil(vector<Ogrenci>& ogrenciler) {
    EkranTemizle();
    cout << RED << "--- " << WHITE << "SILME ISLEMI" << RED << " ---" << RESET << endl;
    string no;
    cout << WHITE << "Silinecek No: " << RESET; cin >> no;

    auto it = remove_if(ogrenciler.begin(), ogrenciler.end(),
        [&no](const Ogrenci& o) { return o.ogrenci_no == no; });

    if (it != ogrenciler.end()) {
        ogrenciler.erase(it, ogrenciler.end());
        DosyaYaz(ogrenciler);
        cout << GREEN << "Silindi.\n" << RESET;
    }
    else {
        cout << RED << "Ogrenci bulunamadi.\n" << RESET;
    }
    DevamEtBekle();
}

void MenuGoster() {
    EkranTemizle();
    cout << BOLD << RED << "=== " << WHITE << "ANA MENU" << RED << " ===\n" << RESET;
    cout << WHITE << "1. Ogrenci Ekle\n";
    cout << "2. Listele\n";
    cout << "3. Not Guncelle\n";
    cout << "4. Sil\n";
    cout << RED << "5. Cikis\n" << RESET;
    cout << GREEN << "Secim: " << RESET;
}

// --- ANA FONKSIYON ---
int main() {
    // UTF-8 destegi (Logo bozulmasin diye)
    SetConsoleOutputCP(65001);

    vector<Ogrenci> ogrenciler;
    DosyaOku(ogrenciler);

    BaslangicLogosuGoster();

    int secim;
    while (true) {
        MenuGoster();
        if (!(cin >> secim)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (secim == 1) OgrenciEkle(ogrenciler);
        else if (secim == 2) OgrenciListele(ogrenciler);
        else if (secim == 3) NotGuncelle(ogrenciler);
        else if (secim == 4) OgrenciSil(ogrenciler);
        else if (secim == 5) break;
    }

    cout << RED << "Program kapaniyor...\n" << RESET;
    return 0;
}