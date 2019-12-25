# VIST-RocketScience

Bu repo Vist Roketinin aviyonik yazılımlarını içermekte ve ilgili dökümanları bulundurmaktadır.

Yazılım ve donanım için yürütülecek test süreci aşağıdaki gibidir
1. Standart donanım ile range testi,
GPS anteni oryantasyon testi,
GPS doğruluk testi,
Haberleşme anteni oryantasyon testi
2. Aluminyum govde etkisi testi,
Cam fiber gövde etkisi testi
3. Azami range sağlayan modül ile network yazılımı
4. Network yazılım testi,
Network yazılımı ile range artışının tespiti
5. Yönlendirilmiş anten ile range testi

GPS Kontrolü yapılması gerekenler :
1. Her yeni GPS'in çalışıp çalışmadığı u-center yazılımı ile kontrol edilmelidir.
2. GPS bir UART-USB çeviricisine Vcc->Vcc, Gnd->Gnd, Rx->Tx, Tx->Rx olacak şekilde bağlanmalıdır.
3. Sonrasında U-center'ın sol üst köşesinde bulunan yeşil buton vasıtasıyla Com portu ve yanındaki sekmeden baudrate'i ayarlanmalıdır. GPS sinyali alınana kadar beklenmelidir. 
GPS sinyalinin alındığı iki şekilde anlaşılabilir. GPS çipinin ışığı aralıklı olarak yanıp sönüyor olmalıdır veya U-center'da sağ üst köşedeki pencerede konum bilgileri güncelleniyor olmalıdır.

