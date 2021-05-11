#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Bu program Algoritma ve programlama-2 dersinin donem projesi olup sinema rezervasyon islemlerinin basit bir uygulamasidir.
//Proje baslangic tarihi 20 Nisan 2021
//Proje son bakým tarihi 23 Nisan 2021
//Proje gelistiricisi Mustafa Arslan 202803075
//Proje olabildigince moduler yapilmaya calisilmistir.


//salon struct yapisi
typedef struct
{
    char Matine[5];
    char Koltuk[3][5][50];
    char KoltukKontrol[3][5][50];
    int Ucret;

}Sinema;

//kullanilacak fonksiyonlar
void HesapKontrol();
void KullaniciOlustur();
void KullaniciDenetim();
int SalonSecici();
int MatineSec();
void KoltukDoldur(Sinema* sin);
void KoltukKontrol(Sinema* sin);
void KoltukAtama(Sinema* sin, int salon, int matine);
void UcretAl(Sinema* sin);
void GelirOgren();
void GelirYaz(Sinema* sin);
int KoltukVerileri(int i, int j, int k);
void SonCiktiVer(int salon, int matine);


//Client
int main()
{
    HesapKontrol();
    Menu();

    return 0;
}

//Kullanici var mi yok mu kontrol fonksiyonlari
void HesapKontrol()
{
    FILE* pdosya;
    pdosya = fopen("Sifre.txt", "r+"); //Sifre adli dosyayi okuma yazma modunda actim dosya varsa acilacak
    if (pdosya == NULL)//eger dosya acilamazsa daha once olusturulmadi demektir
    {
        KullaniciOlustur();//kullanicinin kayit olmasi lazim
        system("CLS");
        KullaniciDenetim();//kullanici kayit olduktan sonra giris yapabilir
    }
    else //Eger dosya acildiysa daha onceden kayit olunmus demektir
    {
        KullaniciDenetim();//giris islemlerini baslat
    }

    fclose(pdosya);//dosyayi kapat
}
void KullaniciOlustur()
{
    char kulad[50];
    printf("Kullanici Adi : ");
    scanf("%s", kulad);

    int sifre;
    printf("Sifre : ");
    scanf("%d", &sifre);

    FILE* pdosya;
    pdosya = fopen("Sifre.txt", "a+");//Sifre dosyasini a+ modunda ac eger dosya yoksa olusturulacak
    if (pdosya == NULL)
    {
        printf("Dosya acilamadi");
    }

    fprintf(pdosya, "%s %d\n", kulad, sifre);//dosyaya kulanici adi ve sifreyi kaydet
    printf("\nKullanici Adi ve Sifreniz Kaydedildi.\n");
    printf("Lutfen Giris Yapiniz..\n\n");
    sleep(1);

    fclose(pdosya);//dosyayi kapat
}
void KullaniciDenetim()
{
    FILE* pdosya;
    pdosya = fopen("Sifre.txt", "r");//Sifre dosyasini sadece okuma modunda ac

    if (pdosya == NULL)
        printf("Dosya Okunamadi");

    char ad[50];
    int s;
    int kontrol = 1;

    fscanf(pdosya, "%s %d", ad, &s);//dosyadan kulanici adi ve sifreyi cek degiskenlere aktar

    do//bu bolum yanlýs giris olursa kullanici tekrar tekrar deneyebilsin diye do-while ile yapildi
    {
        char kulad[50];
        printf("Kullanici Adi : ");
        scanf("%s", kulad);

        int sifre;
        printf("Sifre : ");
        scanf("%d", &sifre);

        int length = strlen(ad);//dosyadan cekilen kullanici adi uzunlugunu degiskene aktar

        for (int i = 0; i < length; i++)//kullanici adinin uzunlugu kadar dongu calistir
        {
            if ((kulad[i] != ad[i]) || (sifre != s))//eger kullanici adinin herhangi bir harfi yanlissa veya sifre yanlissa giris basarisiz demektir
                kontrol = 0;//kontrolu 0 yap
            else
                kontrol = 1;//sartlar uygunsa kontrolu 1 yap
        }

        if (kontrol == 1)//eger kontrol 1 ise kullanici dogru bilgileri girmis demektir. Dongu bitebilir
        {
            printf("Kullanici Kayitli\n");
            printf("Menuye Yonlendiriliyorsunuz..\n");
            sleep(1);
            break;
        }
        else
        {
            printf("\nKullanici Girisi Hatali. Lutfen Tekrar Deneyiniz..");
            sleep(1);
        }
    } while (kontrol < 1);

    fclose(pdosya);//dosyayi kapat
}

//Menu islemleri
int Menu()
{
    int secim;
    printf("Ana Menuye Hosgeldiniz..\n\n");
    printf("1-Film Sec\n");
    printf("2-Geliri Ogren\n");
    printf("3-Cikis\n");
    printf("Seciminiz.. (1-2-3)");
    scanf("%d", &secim);

    if (secim == 1)
    {
        int salonsecim;
        salonsecim = SalonSecici();//secilen salon bilgisini degiskene aktar

        int matinesecim;
        matinesecim = MatineSec();//secilen matine bilgisini degiskene aktar

        Sinema sinema;
        KoltukDoldur(&sinema);//koltuk numaralarini doldur
        KoltukKontrol(&sinema);//koltuklarin rezerve edilip edilmedini kontrol et ve diziye yaz
        KoltukAtama(&sinema, salonsecim, matinesecim);//secilen salondan, secilen matineden otomatik olarak en bastaki kltugu rezerve et

        UcretAl(&sinema);//bilet ucretlerini al
        GelirYaz(&sinema);//bilet ucretlerini dosyaya yaz
        SonCiktiVer(salonsecim, matinesecim);//Cikti Dosyasina secilen salon ve matineyi yaz
    }
    else if (secim == 2)
    {
        GelirOgren();//geliri ekrana yaz
        sleep(1);
        //kullanici baska islem yapmak isterse diye kontrol
        int secenek;
        printf("\nAnamenuye Geri Donmek Icin 1 Tusuna Basiniz..");
        printf("\nCikis Yapmak Icin Herhangi Bir Tusa Basiniz..\n");
        scanf("%d", &secenek);
        if (secenek == 1)
        {
            sleep(1);
            printf("Anamenuye Donuluyor..");
            sleep(1);
            Menu();
        }
        else
        {
            printf("\nSistemden Cikiliyor..\n");
            sleep(1);
            exit(0);
        }

    }
    else
    {
        exit(0);
    }
}
int SalonSecici()
{
    int sayi;
    printf("Suanki Filmlerimiz:\n");
    printf("1- Tenet --> Red Salonunda\n");
    printf("2- Inception  --> Green Salonunda\n");
    printf("3- Interstaller --> Blue Salonunda\n");
    printf("Hangi Filmi Izlemek Istiyorsunuz? (1-2-3)\t");
    scanf("%d", &sayi);
    return sayi;
}
int MatineSec()
{
    int secim;
    printf("Lutfen Matine Seciniz. (1-2-3-4-5) \t");
    scanf("%d", &secim);
    return secim;
}
void KoltukDoldur(Sinema* sin)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            for (int k = 0; k < 50; k++)
            {
                (*sin).Koltuk[i][j][k] = k + 1;
            }
        }
    }
}
void KoltukKontrol(Sinema* sin)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            for (int k = 0; k < 50; k++)
            {
                if (KoltukVerileri(i, j, k) == 1)//koltuk verileri fonskiyonundan gelen veri 1 ise daha once rezerve edilmemis demektir
                    (*sin).KoltukKontrol[i][j][k] = 'b';//bos olarak isaretle
                else
                    (*sin).KoltukKontrol[i][j][k] = 'd';//gelen veri 0 ise daha once rezerve edilmis demektir dolu olarak isaretle
            }
        }
    }
}
void KoltukAtama(Sinema* sin, int salon, int matine)
{
    for (int i = salon; i <= salon; i++)
    {
        for (int j = matine; j <= matine; j++)
        {
            for (int k = 0; k < 50; k++)
            {
                if ((*sin).KoltukKontrol[i][j][k] == 'b')//verilen salon bilgisi,matine bilgisi ne gore koltuklar sira ile atanir bos olanlar dolu olarak isaretlenir
                {
                    (*sin).KoltukKontrol[i][j][k] = 'd';
                    printf("%d.Koltuk Atandi\n", k + 1);//ekrana hangi koltugun rezerve edildigini yaz

                    FILE* fp;

                    fp = fopen("Salon.txt", "a+");//dosyaya hangi salondan rezerve alindigini yaz
                    fprintf(fp, "%d\n", salon);
                    fclose(fp);//dosyayi kapat

                    fp = fopen("Matine.txt", "a+");//dosyaya hangi matineden rezerve alindigini yaz
                    fprintf(fp, "%d\n", matine);
                    fclose(fp);//dosyayi kapat

                    fp = fopen("Koltuk.txt", "a+");//dosyaya hangi koltugun rezerve edlidigini yaz
                    fprintf(fp, "%d\n", k);
                    fclose(fp);//dosyayi kapat
                    break;//donguyu kir
                }
            }
        }
    }
    sleep(1);
}
void UcretAl(Sinema* sin)
{
    int secim;
    printf("Bilet Seciniz\n");
    printf("1- Tam Bilet     --->    30TL\n");
    printf("2- Ogrenci Bilet --->    20TL\n");
    printf("Seciminiz  (1-2)");
    scanf("%d", &secim);
    if (secim == 1)
    {
        (*sin).Ucret = 30;
        printf("\nTam Ucret Secildi.\n");
        sleep(1);
        printf("Tam Ucret Tahsil Edildi.\n");
    }
    else if (secim == 2)
    {
        (*sin).Ucret = 20;
        printf("\nOgrenci Ucret Secildi\n");
        sleep(1);
        printf("Ogrenci Ucret Tahsil Edildi.\n");
    }
    else
    {
        printf("\nSeciminiz Gecersiz.Cikis Yapiliyor..");
        exit(0);
    }

    sleep(1);
    printf("Biletiniz Olusturuluyor..\n");
    sleep(1);
    printf("Biletiniz Olusturuldu..\n");
    sleep(1);
}
void GelirYaz(Sinema* sin)
{
    int para;
    para = (*sin).Ucret;
    int icerdekipara;
    FILE* fp;
    fp = fopen("Gelir.txt", "a+");//dosyayi a+ modunda ac icinde veri varsa degiskene aktar yoksa icine veriyi yaz
    fscanf(fp, "%d", &icerdekipara);//
    fclose(fp);

    para = para + icerdekipara;//eger icinde veri varsa bilet ucreti ile topla

    fp = fopen("Gelir.txt", "w+");//dosyayý w+ modunda ac toplanan parayi dosyaya aktar
    fprintf(fp, "%d", para);
    fclose(fp);//dosyayi kapat
}

//Ara fonksiyonlar
void GelirOgren()
{
    int gelir;
    FILE* fp;
    fp = fopen("Gelir.txt", "r");//dosyayi okuma modunda ac
    fscanf(fp, "%d", &gelir);//icindeki veriyi degiskene aktar
    fclose(fp);//dosyayi kapat

    printf("\nToplam Gelir : %d TL\n", gelir);//dosya icindeki veriyi ekrana yaz
}
int KoltukVerileri(int i, int j, int k)
{
    int sonuc = 1;
    int salon[50], matine[50], koltuk[50], l = 0;
    FILE* fp;
    fp = fopen("Salon.txt", "a+");//Salon dosyasini a+ modunda ac
    while (!feof(fp))//dosyanin icindeki veri bitene kadar diziye aktar
    {
        fscanf(fp, "%d", &salon[l]);
        l++;//dizinin icinde ne kadar veri aktarildi kontrolu
    }
    fclose(fp);//dosyayi kapat

    int m = 0;
    fp = fopen("Matine.txt", "a+");//Matine dosyasini a+ modunda ac
    while (!feof(fp))//dosyanin icindeki veri bitene kadar diziye aktar
    {
        fscanf(fp, "%d", &matine[m]);
        m++;//dizinin icinde ne kadar veri aktarildi kontrolu
    }
    fclose(fp);//dosyayi kapat

    int s = 0;
    fp = fopen("Koltuk.txt", "a+");//Koltuk dosyasini a+ modunda ac
    while (!feof(fp))//dosyanin icindeki veri bitene kadar diziye aktar
    {
        fscanf(fp, "%d", &koltuk[s]);
        s++;//icerideki verinin sayisi
    }
    fclose(fp);//dosyayi kapat

    for (int h = 0; h <= l; h++)
    {
        if ((salon[h] == i) && (matine[h] == j) && (koltuk[h] == k))//eger dosyanin icindeki verilerde yeni rezerve edilmek istenen koltuk varsa sonucu 0 yap ve donguyu kýr
        {
            sonuc = 0;
            break;
        }
    }
    return sonuc; //sonucu dondur


}
void SonCiktiVer(int salon, int matine)
{
    FILE* fp;
    fp = fopen("Cikti.txt", "a+");//dosyayi a+ modunda ac
    if (salon == 1)//secilen salon numarasi 1 ise
    {
        fprintf(fp, "%s %d %s", "Red Salonundaki Tenet Filmine ", matine, ". Matineden Randevu Alindi.\n");//dosyaya veriyi yaz
    }
    else if (salon == 2)//secilen salon numarasi 2 ise
    {
        fprintf(fp, "%s %d %s", "Green Salonundaki Inception Filmine ", matine, ". Matineden Randevu Alindi.\n");//dosyaya veriyi yaz
    }
    else//secilen salon 3 ise
    {
        fprintf(fp, "%s %d %s", "Blue Salonundaki Interstaller Filmine ", matine, ". Matineden Randevu Alindi.\n");//dosyaya veriyi yaz
    }
    fclose(fp);//dosyayi kapat

    //kullanici ana menuye donmek isterse diye kontrol
    int secenek;
    printf("\nAnamenuye Geri Donmek Icin 1 Tusuna Basiniz..");
    printf("\nCikis Yapmak Icin Herhangi Bir Tusa Basiniz..\n");
    scanf("%d", &secenek);
    if (secenek == 1)
    {
        sleep(1);
        printf("Anamenuye Donuluyor..");
        sleep(1);
        Menu();
    }
    else
    {
        printf("\nSistemden Cikiliyor..\n");
        sleep(1);
        exit(0);
    }
}
