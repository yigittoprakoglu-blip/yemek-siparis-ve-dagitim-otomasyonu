#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h> 


typedef struct yemek{        //yemek verileri için bağlı liste yapısı
    int yemekID;
    char yemekadi[50];       // max 50 karakterlik bir char oluşturmaya yarar.
    int yemekfiyat;
    int sure;
    struct yemek *next;
} yemek;

typedef struct siparis{      //siparişler için kuyruk yapısı 
    int siparisID;
    int yemekID;
    char musteriadi[50];
    char musteritelno[50];
    char musteriadresi[100];
    struct siparis *next;
} siparis;

typedef struct gelir{        //gelir verileri için yığın yapısı
    int siparisID;
    float tutar;
    char tarih[20];
    struct gelir *next;
} gelir;

yemek *menuHead = NULL;             //listelerin başlangıç noktalarını null a eşitliyoruz bunlar global değişken olmalı
siparis *siparisFront = NULL;   
siparis *siparisRear = NULL;
siparis *teslimatFront = NULL;
siparis *teslimatRear = NULL;
gelir *gelirTop = NULL;

void bufferTemizle() {                                //veri girişinden sonra oluşan enterın orada bulunması durumundan oluşan diğer veriyi alamama sorununu çözer.
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void tarihAl(char *buffer) {                            //uygulamanın açıldığı ve verinini alındığı tarihi almak için
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%02d.%02d.%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

float fiyatbul(int yemekID) {                       //ID verip fiyat almak için       
    yemek *temp = menuHead;
    while (temp != NULL) {
        if (temp->yemekID == yemekID) return (float)temp->yemekfiyat;
        temp = temp->next;
    }
    return 0;
}

void gelirEkle(int sID, float tutar) {                //gelir yığınına fiyatları eklemek için 
    gelir *yeni = (gelir *)malloc(sizeof(gelir));
    yeni->siparisID = sID;
    yeni->tutar = tutar;
    tarihAl(yeni->tarih); 
    
    yeni->next = gelirTop;
    gelirTop = yeni;
}

void dosyaKaydet() {       //dosyaları açma ve dosyaya yazı yazma  işlemleri
    FILE *fp;

    fp = fopen("menu.txt", "w");
    if (fp != NULL) {
        yemek *temp = menuHead;
        while (temp != NULL) {
            fprintf(fp, "%d|%s|%d|%d\n", temp->yemekID, temp->yemekadi, temp->yemekfiyat, temp->sure);
            temp = temp->next;
        }
        fclose(fp);
    }

    
    fp = fopen("siparis.txt", "w");
    if (fp != NULL) {
        siparis *temp = siparisFront;
        while (temp != NULL) {
            fprintf(fp, "%d|%d|%s|%s|%s\n", temp->siparisID, temp->yemekID, temp->musteriadi, temp->musteritelno, temp->musteriadresi);
            temp = temp->next;
        }
        fclose(fp);
    }

    
    fp = fopen("gelir.txt", "w");
    if (fp != NULL) {
        gelir *temp = gelirTop;
        while (temp != NULL) {
            fprintf(fp, "%d|%.2f|%s\n", temp->siparisID, temp->tutar, temp->tarih);
            temp = temp->next;
        }
        fclose(fp);
    }
    printf("Tum veriler dosyalara kaydedildi.\n");
}

void yemekekle(int yemekID, char *yemekadi, int yemekfiyat, int sure){
    yemek *yeni = (yemek *)malloc(sizeof(yemek));
    yeni->yemekID = yemekID;                    //bu satırlar yeni bir yemek düğümü oluşturur
    strcpy(yeni->yemekadi, yemekadi);
    yeni->yemekfiyat = yemekfiyat;
    yeni->sure = sure;
    yeni->next = NULL;

    if (menuHead == NULL) {                     //eklenecek yemeğin eklendiği kısmın kontrolünü yapıyoruz
        menuHead = yeni;
        
    } else {
        yemek *temp = menuHead;
        while (temp->next != NULL) temp = temp->next;
        temp->next = yeni;
    }
    printf("Yemek eklendi: %s (ID: %d)\n", yemekadi, yemekID);
}
void siparisver(int sID, int yID, char *ad, char *telno, char *adres){  //kuyruk yapısı olduğundan dolayı en fifo yapsını kullanarak ilk gireni atar 
    if(fiyatbul(yID) == 0){
        
        printf("Gecersiz yemek ID'si (ID: %d)\n", yID);
        return;
    }
    siparis *yeni = (siparis *)malloc(sizeof(siparis));
    yeni->siparisID = sID;
    yeni->yemekID = yID;
    strcpy(yeni->musteriadi, ad);
    strcpy(yeni->musteritelno, telno);       //string ifadelerini kopyalamaya yarar
    strcpy(yeni->musteriadresi, adres);
    yeni->next = NULL;

    if (siparisRear == NULL) {
        siparisFront = siparisRear = yeni;
        printf("Siparis basariyla onaylandi (Siparis ID: %d)\n", sID);
    } else {
        siparisRear->next = yeni;
        siparisRear = yeni;
        printf("Siparis basariyla onaylandi (Siparis ID: %d)\n", sID);
    }
}

void dosyaYukle() {                    //verileri txt dosyalarından çekme işlemi  
    FILE *fp;
    char line[256];

    fp = fopen("menu.txt", "r");
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            int id, fiyat, sure;
            char ad[50];
            sscanf(line, "%d|%[^|]|%d|%d", &id, ad, &fiyat, &sure);
            yemekekle(id, ad, fiyat, sure);
        }
        fclose(fp);
    }

    fp = fopen("siparis.txt", "r");
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            int sID, yID;
            char ad[50], tel[50], adres[100];
            sscanf(line, "%d|%d|%[^|]|%[^|]|%[^\n]", &sID, &yID, ad, tel, adres);
            siparisver(sID, yID, ad, tel, adres);
        }
        fclose(fp);
    }

    fp = fopen("gelir.txt", "r");
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            int sID;
            float tutar;
            char tarih[20];
            sscanf(line, "%d|%f|%[^\n]", &sID, &tutar, tarih);
            
            gelir *yeni = (gelir *)malloc(sizeof(gelir));
            yeni->siparisID = sID;
            yeni->tutar = tutar;
            strcpy(yeni->tarih, tarih);
            yeni->next = gelirTop; 
            gelirTop = yeni;
        }
        fclose(fp);
    }
}



void yemeklerilistele(){                    //yemek menüsü listeler 
    printf("\n-----Yemek Menusu-----\n");
    yemek *temp = menuHead;                     //menunun en üstündeki elemanı geçici değer olarak atadık
    if(temp == NULL){
        printf("Menu bos.\n");
        return;
    }
    while(temp != NULL){
        printf("ID=%d Yemek adi=%-15s Fiyat=%d Sure=%d dk\n", temp->yemekID, temp->yemekadi, temp->yemekfiyat, temp->sure);
        temp = temp->next;
    }
}

void yemeksil(int yemekID) {
    
    yemek *temp = menuHead;
    yemek *prev = NULL;
    if (temp != NULL && temp->yemekID == yemekID) {     //eğer listede tek eleman varsa veya ilk elemansa

        menuHead = temp->next;
        free(temp);
        printf("Yemek silindi.\n");
        return;
    }

    while (temp != NULL && temp->yemekID != yemekID) {  //listeyi dönüyoruz
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {                                      
        printf("Yemek bulunamadi.\n");
        return;
    }

    prev->next = temp->next;
    free(temp);                                        //silme işlemini free komudunu kullnarak yapıyoruz  
    printf("Yemek silindi.\n");
}
int sadeceintal() {                           //kullanıcının sadece int değerler girmesini sağlayan fonk
    char input[50];
    int deger;
    while (1) {
        fgets(input, 50, stdin);
        if (sadecerakam(input)) {
            sscanf(input, "%d", &deger);
            return deger;
        } 
        else{
            printf("Hatali giris! Lutfen sadece sayi giriniz: ");
        }
    }
}
int rakamkontrolu(char *str) {       //girilen str'nin içinde rakam olup olmadığının kontrolü 
    int i = 0;
    str[strcspn(str, "\n")] = 0;
    if (strlen(str) == 0) return 1;

    while (str[i]) {
        if (isdigit(str[i])) return 1;
        i++;
    }
    return 0;
}

void yemekguncelle(int id) {        //id ile belirlediğimiz yemeğin ismini fiyatını hazırlan süresi vs değiştirme işlemi
    yemek *temp = menuHead;
    while (temp != NULL) {
        if (temp->yemekID == id) {
            printf("\nYemek Bulundu: %s\n", temp->yemekadi);
            printf("----- Yeni bilgileri giriniz -----\n");
            
            
            printf("Yeni ad: "); 
            while(1) {
                bufferTemizle(); 
                fgets(temp->yemekadi, 50, stdin); 
                temp->yemekadi[strcspn(temp->yemekadi, "\n")] = 0;
                if(rakamkontrolu(temp->yemekadi) == 0 && strlen(temp->yemekadi) > 0) break;
                printf("Hata! Yemek adi rakam iceremez. Tekrar giriniz: ");
            }

            printf("Yeni fiyat: ");
            temp->yemekfiyat = sadeceintal();

            printf("Yeni hazirlama suresi: "); 
            temp->sure = sadeceintal();

            printf("Yemek basariyla guncellendi.\n");
            return;
        }
        temp = temp->next;
    }
    printf("HATA: %d ID'li yemek bulunamadi.\n", id);
}



void siparislerilistele(){
    printf("\n-----Bekleyen Siparisler-----\n");
    siparis *temp = siparisFront;                   //kuyruğun başından çekiyoruz     
    if(temp == NULL){
        printf("Bekleyen siparis bulunmamaktadir.\n");
        return;
    }
    while(temp != NULL){
        printf("Siparis ID=%d Musteri Adi=%s Yemek ID=%d\n", temp->siparisID, temp->musteriadi, temp->yemekID);
        temp = temp->next;
    }
}

void siparishazirla(){
    if(siparisFront == NULL){
        printf("Hazirlanacak siparis bulunmamaktadir.\n");
        return;
    }
    siparis *temp = siparisFront;                           
    siparisFront = siparisFront->next;
    if (siparisFront == NULL) siparisRear = NULL;
    temp->next = NULL; 

    if (teslimatRear == NULL) {                          //teslimat kuyruğuna ekle
        teslimatFront = teslimatRear = temp;
    } 
    else {
        teslimatRear->next = temp;
        teslimatRear = temp;
    }
    printf("Siparis=%d hazirlandi ve kuryeye (teslimat kuyruguna) verildi.\n", temp->siparisID);
}

void teslimatYap() {
    if (teslimatFront == NULL) {
        printf("Teslim edilecek hazir siparis yok.\n");
        return;
    }

    siparis *temp = teslimatFront;                      //teslimat kuyruk yapısı ilk alınan siparişi otomatik teslim eder ve siparişin fiyatını gelir yığınına ekler
    teslimatFront = teslimatFront->next;
    if (teslimatFront == NULL) teslimatRear = NULL;

    
    float tutar = fiyatbul(temp->yemekID);
    gelirEkle(temp->siparisID, tutar);

    printf("Siparis ID: %d teslim edildi. %.2f TL Gelir yiginina eklendi.\n", temp->siparisID, tutar);
    
    free(temp);
}


void gelirRaporu() {      //yığının içindeki gelirleri tarihe göre ayırma ve yazdırma 
    char tarih[20];
    float toplam = 0;
    
    printf("Raporlanacak Tarih (Bugun icin 'bugun' yazin veya orn: 22.12.2025): ");
    
    scanf("%s", tarih);

    if(strcmp(tarih, "bugun") == 0) {
        tarihAl(tarih); 
    }

    printf("\n--- %s Tarihli Gelir Raporu ---\n", tarih);
    
    gelir *temp = gelirTop;
    int sayac = 0;
    while(temp != NULL) {
        if(strcmp(temp->tarih, tarih) == 0) {     //str compare yani stringleri birbirleriyle karşılaştırır
            printf("Siparis ID: %d | Tutar: %.2f TL\n", temp->siparisID, temp->tutar);
            toplam += temp->tutar;
            sayac++;
        }
        temp = temp->next;
    }

    if(sayac == 0) {
        printf("Bu tarihe ait gelir bulunamadi.\n");
    } 
    else {
        printf("-------------------------------\n");
        printf("TOPLAM GELIR: %.2f TL\n", toplam);
    }
}
int IDKontrol(int id) {                               //aynı id olmasını engelleyen fonk 
    yemek *temp = menuHead;
    while (temp != NULL) {
        if (temp->yemekID == id) return 1;
        temp = temp->next;
    }
    return 0;
}
int sadecerakam(char *str) {                      //girilen str'nin rakamlardan oluşup oluşmadığının kontrolü
    int i = 0;
    str[strcspn(str, "\n")] = 0;
    if (strlen(str) == 0) return 0; 

    while (str[i]) {
        if (!isdigit(str[i])) return 0; 
        i++;
    }
    return 1;
}
int main(){
    int secim;
    int sID=1; 

    
    dosyaYukle();
    system("cls");
    
    siparis *tempS = siparisFront;
    while(tempS != NULL) {
        if(tempS->siparisID >= sID) sID = tempS->siparisID + 1;
        tempS = tempS->next;
    }
    
    while(1){  
        printf("\n\nYemek Siparisi ve Dagitim Sistemi\n");
        printf("-------------------------------\n");
        printf("1. Yemek ekle\n");
        printf("2. Yemek sil\n");
        printf("3. Yemek guncelle\n");
        printf("4. Menu'yu listele\n");
        printf("5. Siparis ver\n");
        printf("6. Siparis hazirla\n");
        printf("7. Siparisleri Listele\n");
        printf("8. Teslimat Yap\n");
        printf("9. Gunluk gelir raporu\n");
        printf("10. Kaydet ve cik\n");

        printf("Seciminiz: ");
        
        if (scanf("%d", &secim) != 1) { 
            printf("Gecersiz giris, lutfen sayi girin.\n");
            bufferTemizle();
            continue;
        }

        if(secim == 10){
            dosyaKaydet(); 
            printf("Cikis yapiliyor.\n");
            exit(0);
        }
        bufferTemizle();
        switch(secim){
            case 1:{
                int yemekID, sure;
                char yemekadi[50];
                int yemekfiyat;
                
                
                while(1) {
                    printf("Yemek ID (Sadece Sayi): ");
                    yemekID = sadeceintal(); 
                    if(IDKontrol(yemekID)) {
                        printf("Bu ID (%d) zaten kullaniliyor. Baska bir ID giriniz.\n", yemekID);
                    } else {
                        break;
                    }
                }

                while(1) {
                    printf("Yemek Adi: "); 
                    fgets(yemekadi, 50, stdin); 
                    yemekadi[strcspn(yemekadi, "\n")] = 0;
                    if (rakamkontrolu(yemekadi)) {
                        printf("Hata! Yemek adi rakam iceremez.\n");
                    } else {
                        break;
                    }
                }

                
                printf("Fiyat: "); 
                yemekfiyat = sadeceintal();

                
                printf("Hazirlama Suresi: "); 
                sure = sadeceintal();

                yemekekle(yemekID, yemekadi, yemekfiyat, sure);
                break;
            }
            case 2:{
                int yemekID;
                yemeklerilistele(); //yemekleri silmeden önce kullanıcı yemekleri görsün die 
                printf("Silinecek Yemek ID: "); 
                yemekID = sadeceintal(); 
                yemeksil(yemekID);
                break;
            }
            case 3: {
                int yemekID;
                yemeklerilistele();  
                printf("Guncellenecek Yemek ID: "); 
                yemekID = sadeceintal(); 
                yemekguncelle(yemekID);
                break;
            }
            case 4:{
                yemeklerilistele();
                break; 
            }
            case 5: {
                int yId;
                char ad[50], tel[50], adres[100]; 
                yemeklerilistele();
                
                printf("Siparis edilecek yemek ID: "); 
                yId = sadeceintal();

                
                
                while(1) {
                    printf("Musteri adi: "); 
                    fgets(ad, 50, stdin);
                    ad[strcspn(ad, "\n")] = 0;
                    if(rakamkontrolu(ad)) {
                        printf("Hata! Isim rakam iceremez.\n");
                    } else {
                        break;
                    }
                }

                
                while(1) {
                    printf("Telefon: "); 
                    scanf("%s", tel);
                    if(sadecerakam(tel)) {
                        break;
                    } else {
                        printf("Hata! Telefon numarasi sadece rakamlardan olusabilir.\n");
                        bufferTemizle(); 
                    }
                }

                printf("Adres: "); 
                bufferTemizle(); 
                fgets(adres, 100, stdin); 
                adres[strcspn(adres, "\n")] = 0;
                
                siparisver(sID++, yId, ad, tel, adres);
                break;
            }
            case 6:{
                siparishazirla();
                break;
            }
            case 7:{
                siparislerilistele();
                break;
            }
            case 8:{
                teslimatYap();
                break; 
            }
            case 9:{
                gelirRaporu();
                break;
            }
            default:
                printf("gecersiz secim\n");
        }
    } 
    return 0;
}