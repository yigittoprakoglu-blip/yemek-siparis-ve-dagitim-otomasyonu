# yemek-siparis-ve-dagitim-otomasyonu
C dili ile geliştirilmiş; menü, sipariş ve gelir yönetimini temel veri yapıları (Bağlı Liste, Kuyruk, Yığın) ve dosya işlemleri kullanarak simüle eden konsol tabanlı restoran otomasyonu.

### Proje Hakkında
Sistem, kullanıcının terminal üzerinden bir restoranı yönetmesini sağlar. Yeni yemekler eklenebilir, müşterilerden sipariş alınabilir ve bu siparişler aşama aşama (hazırlanma ve kuryeye teslim) işlenebilir. Projenin en güçlü yanı, program kapatılıp açıldığında bile verilerin kaybolmamasını sağlayan dosya okuma/yazma (File I/O) entegrasyonudur.

### Öne Çıkan Özellikler ve Kullanılan Veri Yapıları
* **Dinamik Menü Yönetimi (Bağlı Liste - Linked List):** Yemek ekleme, silme, fiyat/süre güncelleme ve listeleme işlemleri bağlı liste yapısı kullanılarak bellek dostu bir şekilde gerçekleştirilir.
* **Sipariş ve Teslimat Süreci (Kuyruk - Queue):** Gelen siparişler ve kurye teslimatları "İlk Giren İlk Çıkar" (FIFO) mantığıyla işlenir. Müşteriden alınan sipariş önce hazırlık kuyruğuna alınır, hazırlandığında ise teslimat kuyruğuna aktarılır.
* **Gelir Raporlama (Yığın - Stack):** Teslimatı tamamlanan siparişlerin gelir kayıtları "Son Giren İlk Çıkar" (LIFO) yapısı ile tutulur. İstenen tarihe (veya bugüne) ait toplam gelir raporu çekilebilir.
* **Veri Kalıcılığı:** Menü (`menu.txt`), aktif siparişler (`siparis.txt`) ve geçmiş gelir bilgileri (`gelir.txt`) text dosyalarına anlık olarak kaydedilir. Program her açıldığında bu veriler otomatik olarak ilgili veri yapılarına geri yüklenir.
* **Gelişmiş Veri Doğrulama:** Kullanıcıların isim, telefon veya fiyat girerken yapabileceği hataları (örneğin isme rakam, fiyata harf girilmesi) tespit edip engelleyen `rakamkontrolu` ve `sadecerakam` gibi string manipülasyon fonksiyonları içerir.

### Kullanılan Teknolojiler
* C Programlama Dili (Standart Kütüphaneler: `stdio.h`, `stdlib.h`, `string.h`, `time.h`, `ctype.h`)

### Kurulum ve Çalıştırma
1. Proje dosyalarını (`.c` uzantılı kaynak kod ve varsa `.txt` dosyalarını) bilgisayarınıza indirin.
2. Terminal veya komut satırını açarak dosyaların bulunduğu dizine gidin.
3. Kodu derlemek için terminale `gcc main.c -o program` komutunu yazın. (Kaynak dosyanızın adı farklıysa `main.c` kısmını değiştirin).
4. Derleme tamamlandıktan sonra programı başlatmak için Windows'ta `.\program.exe`, Linux/macOS'te ise `./program` komutunu çalıştırın.
5. Açılan konsol menüsünü kullanarak işlemleri gerçekleştirebilirsiniz. Sistemden "Kaydet ve çık" (10) seçeneği ile çıkış yapmayı unutmayın.
