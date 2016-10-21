#include <stdio.h>
//#include <conio.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
//globalne konstante:
#define NAZIV 11 // 10+1 definicija dužina naziva u cijelom programu
#define pi 3.141592 //broj pi
#define R 6371      // Polumjer Zemlje

/*
Stuktura za tocke
*/

typedef struct v_t{
	char v_naziv[NAZIV];
	unsigned int v_visina;
	float v_lat;
	float v_lon;
	unsigned int v_kod;
	}v_tocka; //def. podataka za tocku


typedef struct v_celija {
v_tocka v_OkTocke;
struct v_celija *v_sljedeca;
} v_TOCKE; //def. liste za cuvanje tocaka


/*
Stukture za racun
*/

typedef struct v_xcc{
	double a;
	double b;
	double c;
	float v_proDizanje;
	float v_brzinaVjetra;
	unsigned int  v_smjerVjetra;
	}v_XCcalc; //def podatka o perfomansi jedrilice i atmosfere

typedef struct v_xcc2p{
	v_XCcalc v_xcPerf;
	v_tocka *v_Txc1;
	v_tocka *v_Txc2;
	}v_XCcalc2pt; //definicja parametara za izracun dionice


// ------------------ FUNKCIJE ------------------------

double v_Stup_Rad (float v_ks){
double v_kr;
v_kr=(pi/180)* v_ks;
return v_kr;
}

double v_Rad_Stup (float v_kr){
double v_ks;
v_ks = v_kr/(pi/180);
return v_ks;
}


void v_UPIS_NAZIVA(char *v_naziv) //upis znakova u polje naziva
{

	int v_duz_polja=0;
	//unos znakova u polje
	char c; while ((c=getchar()) != '\n' && c != EOF);
	do {
			v_naziv[v_duz_polja]= getchar();
			v_duz_polja ++;
		}
	while (v_duz_polja<=NAZIV-1 && v_naziv[v_duz_polja-1]!=10);
	//v_naziv[v_duz_polja]= "\0";
}

void v_ISPIS_NAZIVA(char *v_naziv)//ispis znakova iz polja naziva
	{
	//char v_naziv[NAZIV];
	int v_duz_polja=0;
	//printf("\nUpisano je:");
	while (v_naziv[v_duz_polja] != '\0')
		{
		printf("%c",v_naziv[v_duz_polja]);
		v_duz_polja++;
		}
	}
//poziv v_USPOREDBA(adresa_tekst1, adresa_tekst2) - vraca: 1 za isto , 0 za razlicito
int v_USPOREDBA_tocka(char *v_tekst1, char *v_tekst2)
{
while (*v_tekst1 != '\0' && *v_tekst1 != '\n' && *v_tekst2 != '\0' && *v_tekst2 != '\n')
{
if (*v_tekst1 != *v_tekst2) return 0; //v_tekst je razlicit
v_tekst1++;
v_tekst2++;
}
if ((*v_tekst1 == '\0' || *v_tekst1 == '\n' ) && (*v_tekst2 == '\0'  || *v_tekst2 == '\n')) return 1; //v_tekst je isti
return 0; //v_tekst je razlicit
}

// funkcija za izracun udaljenosti dvije geografske koordinate
double v_GCdist(double v_lat1,double v_lon1, double v_lat2,double v_lon2)
{
    double v_dlat, v_dlon;
    double v_a,v_c,v_d;
    v_lat1 =v_Stup_Rad (v_lat1);
    v_lon1 =v_Stup_Rad (v_lon1);
    v_lat2 =v_Stup_Rad (v_lat2);
    v_lon2 =v_Stup_Rad (v_lon2);
    v_dlat = (v_lat2-v_lat1);
    v_dlon = (v_lon2-v_lon1);
    v_a = sin(v_dlat/2)*sin(v_dlat/2) + ( sin(v_dlon/2)*sin(v_dlon/2) )*cos(v_lat1)*cos(v_lat2);

    v_c = 2 * atan2(sqrt(v_a),sqrt(1-v_a));
    v_d= R * v_c;
    return v_d;
}





// ------------------ FUNKCIJE tocaka------------------------


//FUNKCIJA ZA PRONALAZAK ZADNJEG CLANA:

 v_TOCKE *v_ZADNJI_tocka(v_TOCKE *v_pokLista)
 {
  v_TOCKE *v_celija;
  v_celija = v_pokLista;
  if (v_celija->v_sljedeca== NULL) return v_celija;
  do
   {
     v_celija = v_celija->v_sljedeca;
     if (v_celija->v_sljedeca== NULL) return v_celija;
    }
  while (v_celija->v_sljedeca !=NULL);
 }


//vraće adresu celije sa podacima (PRVOG PODATKA) koja je na "v_pozicija" rednom broju
//poziv CELIJA_NA_POZICIJI(redni broj u listi, pokazivacListe)
v_TOCKE *CELIJA_NA_POZICIJI(int v_pozicija, v_TOCKE *v_pokLista)
 {
  v_TOCKE *v_tockaPoz;
  v_tockaPoz = v_pokLista;
  int v_brojac=0;
  do
     {
     v_brojac = v_brojac +1;
     if (v_brojac == v_pozicija) return v_tockaPoz;
     v_tockaPoz = v_tockaPoz->v_sljedeca;
    }
  while (v_tockaPoz->v_sljedeca !=NULL);
  if (v_pozicija == v_brojac + 1) return v_tockaPoz;
  return NULL;
  }


/*upisuje u listu na poslanu ardresu
poziv funkcije:
v_UBACI (&a_b,v_ZADNJI(listaBrojeva));// ubacivanje prvog elementa - pojedinacno -funkcija v_ZADNJI() pronalazi kraj liste

*/
void v_UBACI_tocka (v_tocka *v_x, v_TOCKE *v_pozicija_ubacivanja)
{
  v_TOCKE *v_privremeno;
  v_privremeno = v_pozicija_ubacivanja->v_sljedeca;
  v_pozicija_ubacivanja->v_sljedeca = (v_TOCKE*) malloc(sizeof(v_TOCKE));
  v_pozicija_ubacivanja->v_sljedeca->v_OkTocke = *v_x;
  v_pozicija_ubacivanja->v_sljedeca->v_sljedeca=v_privremeno;
}

// ispis cijele liste nazivi
void v_ISPIS_tocka (v_TOCKE *v_pokLista)
 {
  v_TOCKE *v_ceIspis;
  v_ceIspis = v_pokLista;
  printf("\n\nISPIS LISTE: \n");
  int v_rb=1;
  do
   {
     if(v_ceIspis->v_sljedeca== NULL) break ;
     v_ceIspis = v_ceIspis->v_sljedeca;
       	printf("\nRb:%d", v_rb); v_rb++;
       	printf("\tNaziv Tocke: %s",v_ceIspis->v_OkTocke.v_naziv);
		//printf("\tindex: %d",v_ceIspis->v_OkTocke.v_kod);

	 }
  while (v_ceIspis->v_sljedeca !=NULL);

}

// ispis cijele liste nazivi i vrijednosti
void v_ISPIS_PODACI_tocka (v_TOCKE *v_pokLista)
 {
  v_TOCKE *v_ceIspis;
  v_ceIspis = v_pokLista;
  int v_rb=1;
  printf("\n\nISPIS podataka liste tocaka: \n");
  do
   {
		if(v_ceIspis->v_sljedeca== NULL) break ;
		v_ceIspis = v_ceIspis->v_sljedeca;
		printf("\nRb:%d", v_rb); v_rb++;
       	//printf(" Naziv Tocke:"); v_ISPIS_NAZIVA(v_ceIspis->v_OkTocke.v_naziv);
		printf("\tNaziv Tocke: %s",v_ceIspis->v_OkTocke.v_naziv);
		printf("\t\t MSL: %d",v_ceIspis->v_OkTocke.v_visina);
		printf("\t index: %d",v_ceIspis->v_OkTocke.v_kod);
		printf("\n lat:%.4f \tlon:%.4f",v_ceIspis->v_OkTocke.v_lat, v_ceIspis->v_OkTocke.v_lon);
	 }
  while (v_ceIspis->v_sljedeca !=NULL);
}


// funkcija za unos tocke
void v_UNOS_tocka(v_TOCKE *v_tocke)
{
	//inicijacija strukture tocke
	v_tocka v_jednaTocka;
	int v_dobro;
	printf("\n\nKoliko tocaka? ");int v_unosa; scanf("%d",&v_unosa);
	//printf("\n %d:",v_unosa);
	for(int v_i=1; v_i<=v_unosa; v_i++)
	{
		do //petlja za "popravni" unos
		{
			printf("\n\nUnesi naziv tocke:");
			fflush(stdin);

			//gets(v_jednaTocka.v_naziv);
			scanf("%s", &v_jednaTocka.v_naziv);
			//v_UPIS_NAZIVA(v_jednaTocka.v_naziv);
			printf("\t MSL(m):");scanf("%d",&v_jednaTocka.v_visina);char c; while ((c=getchar()) != '\n' && c != EOF);
			printf("Lat:");	scanf("%f",&v_jednaTocka.v_lat); while ((c=getchar()) != '\n' && c != EOF);
			printf("Lon:");	scanf("%f",&v_jednaTocka.v_lon); while ((c=getchar()) != '\n' && c != EOF);
			printf("Rb:");	scanf("%d",&v_jednaTocka.v_kod); while ((c=getchar()) != '\n' && c != EOF);

			printf("\n--------------- Provjera --------------------");
			printf("\nNaziv tocke:"); v_ISPIS_NAZIVA(v_jednaTocka.v_naziv);
			printf("\t MSL: %d m",v_jednaTocka.v_visina);
			printf("\t Lat:%.6f \tLon:%.6f \tRb:%d",v_jednaTocka.v_lat,v_jednaTocka.v_lon,v_jednaTocka.v_kod);

			printf("\n\nPodaci ispravni ?(D/N):\t");
			scanf("%d",&v_dobro);
			 while ((c=getchar()) != '\n' && c != EOF);
			printf("%c",v_dobro);
		}
		while(v_dobro == 68 || v_dobro == 100);
		v_UBACI_tocka (&v_jednaTocka, v_ZADNJI_tocka(v_tocke));
		printf("\n- tocka je ubacena u listu");
	}
	printf("\n LISTA-tocke ubacene u listu:");
	v_ISPIS_tocka(v_tocke);
	printf("\n gotov ispis tocke ubacene u listu:");
}

void v_UPIS_DATOTEKA_tocka(char *v_nazivDatoteke, v_TOCKE *v_pokLista)

{
   FILE *v_datoteka;
   v_TOCKE *v_privremena;

   v_datoteka = fopen(v_nazivDatoteke, "wb");
   if( v_datoteka == NULL )
   {
     printf("Problem u otvaranju datoteke!");

   }
   else
   { v_privremena = v_pokLista->v_sljedeca;

     while (v_privremena != NULL)
     {
         fwrite(&v_privremena->v_OkTocke, sizeof(v_privremena->v_OkTocke),1, v_datoteka);
         v_privremena = v_privremena->v_sljedeca;
     }
   }
   fclose( v_datoteka );
}


void v_CITANJE_DATOTEKE_tocka(char *v_nazivDatoteke, v_TOCKE *v_pokLista)
{
	printf("\n Citanje Datoteke ->");
	FILE *v_datoteka;
	v_tocka v_x;
	v_datoteka = fopen(v_nazivDatoteke, "rb");
	if( v_datoteka == NULL )
		{
		printf("\n -> Problem u otvaranju datoteke!!!");
		}
	else
		{
		printf("\nKofol cita-");
		int v_pozicija=0;
		while( fread(&v_x,sizeof(v_x),1,v_datoteka))
		{
		v_UBACI_tocka (&v_x, v_ZADNJI_tocka(v_pokLista));
		v_pozicija++;
		}
		fclose(v_datoteka );
		}
		printf("-> gootovo citanje");
}

//brisanje celije u listi
void v_OBRISI_tocka(v_TOCKE *v_pozicija_brisanja){
  v_TOCKE *v_privremeno;
  v_privremeno = v_pozicija_brisanja->v_sljedeca;
  v_pozicija_brisanja->v_sljedeca = v_pozicija_brisanja->v_sljedeca->v_sljedeca;
  free(v_privremeno);
}

v_TOCKE *v_TRAZI_BRISI_tocka(char *v_x, v_TOCKE *v_pokLista)
{
  v_TOCKE *v_tockaT;
  v_TOCKE *v_tockaPrijeBrisane;
  v_tockaT = v_pokLista;
  do
   {
     v_tockaPrijeBrisane=v_tockaT;
     v_tockaT = v_tockaT->v_sljedeca;
     //(USPOREDBA(pokLista->elementi[pozicija].naziv, x))
     //if (v_tockaT->v_tocka.v_naziv == v_x) return v_tockaT;
     if ((v_USPOREDBA_tocka(v_tockaT->v_OkTocke.v_naziv, v_x)) == 1) return v_tockaPrijeBrisane;
     }
  while (v_tockaT->v_sljedeca !=NULL);
  return NULL;
  }




//pretraga po nazivu - vraca adresu celije gdje je prvo pronađen

v_TOCKE *v_TRAZI_tocka(char *v_x, v_TOCKE *v_pokLista)
{
  v_TOCKE *v_tockaT;
  v_tockaT = v_pokLista;
  do
   {
     v_tockaT = v_tockaT->v_sljedeca;
     //(USPOREDBA(pokLista->elementi[pozicija].naziv, x))
     //if (v_tockaT->v_tocka.v_naziv == v_x) return v_tockaT;
     if ((v_USPOREDBA_tocka(v_tockaT->v_OkTocke.v_naziv, v_x)) == 1) return v_tockaT;
     }
  while (v_tockaT->v_sljedeca !=NULL);
  return NULL;
  }



//------------------------FUNKCIJE tocke kraj---------------------------

double v_XC_ProsjekBrzine(double v_BrzinaVjetra,double v_B,double v_prosjekDizanja,double v_a,double v_b,double v_c )
{
    /*
    Racuna XC brzinu i vrijeme na ruti.
    Parametri: */
    double v_Vxco	;// putna brzina bez vjetra
    double v_Vxc	;// putna brzina + vjetar
    double v_Vg	;// brzina u planiranju
    double v_Txc	;// potrebno vrijeme
    double v_V	    ;// TSI
    double v_Wg	;//propadanje jedilice
    double v_Br	;//radijani
    //printf("\nUlazi su: %.2f: %.2f: %.2f: %.2f: %.2f: %.2f: ", v_BrzinaVjetra, v_B, v_prosjekDizanja, v_a, v_b, v_c );
    v_V = sqrt((v_c - v_prosjekDizanja)/v_a);
    v_Vxco = (v_prosjekDizanja*v_V)/(2*v_prosjekDizanja-2*v_c-v_b*v_V);
    v_Br = v_Stup_Rad(v_B);
    v_Vxc = sqrt((pow(v_Vxco,2))-(pow(v_BrzinaVjetra,2)*((pow(sin(v_Br),2))- v_BrzinaVjetra * cos(v_Br))));
    return v_Vxc;
}


//----------------------------------------------------------------------
int main()
{

v_XCcalc2pt v_izracunDionice; //inicijacija strukture za racun dionice
char v_ime[NAZIV];
char c;//cistac bafera

//zadane vrijednosti za polaru
v_izracunDionice.v_xcPerf.a =-0.00082;
v_izracunDionice.v_xcPerf.b= 0.13048;
v_izracunDionice.v_xcPerf.c=-7.48360;
printf("Koriste se ove vrijednosti za polaru: ");
printf("%f ",v_izracunDionice.v_xcPerf.a);
printf("%f ",v_izracunDionice.v_xcPerf.b);
printf("%f ",v_izracunDionice.v_xcPerf.c);


//inicijacija prazne liste tocka
v_TOCKE *v_tocke; //incijalizacija liste
v_tocke = (v_TOCKE*) malloc(sizeof(v_TOCKE)); //dinamicka rezervacija memorije
v_tocke->v_sljedeca=NULL; //definicija prazne liste

//ucitavanje tocaka u program iz datoteke
v_CITANJE_DATOTEKE_tocka("tocke.xc", v_tocke);
if(v_tocke->v_sljedeca==NULL)
	{
	printf("\n Ups, nema tocaka, treba unjeti najmanje dvije!");
	v_UNOS_tocka(v_tocke);//obavezan unos u slučaju prazne liste tocaka
	}

printf("\n - snimanje podataka u datoteku -");
v_UPIS_DATOTEKA_tocka("tocke.xc",v_tocke);//UPIS u datoteku


//printf("\nTEST- XC prosjek: %lf",v_XC_ProsjekBrzine(5,15,2,v_izracunDionice.v_xcPerf.a,v_izracunDionice.v_xcPerf.b,v_izracunDionice.v_xcPerf.c ));


//-------------------------I Z B O R N I K ----------------------
int v_izbor;
do{
	printf("\n----- Izbornik-----");
	printf("\n-1 UNOS tocaka");
	printf("\n-2 Pregled i Brisanje tocaka");
	printf("\n-3 XC racun");
	printf("\n-4 KRAJ\n");
	scanf("%d",&v_izbor);
if (v_izbor==1){
	printf("\nOdabran-1 UNOS tocaka");
	v_UNOS_tocka(v_tocke);
	printf("\n - snimanje upisanih podataka u datoteku -");
	v_UPIS_DATOTEKA_tocka("tocke.xc",v_tocke);//UPIS u datoteku


	}
if (v_izbor==2){
					printf("\nOdabran-2 Brisanje tocaka");
					v_ISPIS_PODACI_tocka(v_tocke);//ispis kratki naziv i index
					printf("\n Upisi ime tocke za brisanje: "); while ((c=getchar()) != '\n' && c != EOF);
					gets(v_ime);
					if(v_TRAZI_BRISI_tocka(v_ime,v_tocke)==NULL)
					{
					printf("\nNema tocke takvog naziva!");
					continue;
					}
					v_TOCKE *v_pt=(v_TRAZI_BRISI_tocka(v_ime,v_tocke));//pretraga po upisanom imenu
					v_OBRISI_tocka(v_pt);//brisanje jedne tocke
					printf("\n Obrisano");
					v_ISPIS_tocka(v_tocke);//ispis kratki naziv i index

					}
if (v_izbor==3){
	printf("\nOdabran-3 XC racun");
			do{
				printf("\n----- Izbornik racuna rute----");
					printf("\n-1 Odabir tocaka");
					printf("\n-2 Unos meteo uvjeta");
					printf("\n-3 Izracun");
					printf("\n-4 KRAJ\n");
					scanf("%d",&v_izbor);
				if (v_izbor==1){
					printf("\n----------------------ODABIR_TOCAKA-------------------");//Odabir tocke:
					printf("\n- odaberi okretne tocke dionice od upisnih u listu: -");
					v_ISPIS_tocka(v_tocke);//ispis kratki naziv i index


					do{
						printf("\n Upisi ime 1.tocke: "); gets(v_ime);
					v_TOCKE *v_pt=(v_TRAZI_tocka(v_ime,v_tocke));//pretraga po upisanom imenu
					v_izracunDionice.v_Txc1 = &(v_pt->v_OkTocke);
					printf("\nOdabrana je tocka:  %s", v_izracunDionice.v_Txc1->v_naziv);
					}
					while(v_izracunDionice.v_Txc1->v_naziv == NULL) ;

					do{
						printf("\n Upisi ime 2.tocke: "); gets(v_ime);
					v_TOCKE *v_pt=(v_TRAZI_tocka(v_ime,v_tocke));//pretraga po upisanom imenu
					v_izracunDionice.v_Txc2 = &(v_pt->v_OkTocke);
					printf("\nOdabrana je tocka:  %s", v_izracunDionice.v_Txc2->v_naziv);
					}
					while(v_izracunDionice.v_Txc1->v_naziv == NULL) ;

					printf("\n---------------------------------------------");


					}
				if (v_izbor==2){
					printf("\nOdabir-2 Unos meteo uvjeta");
					printf("\nBrzina vjetra: ");while ((c=getchar()) != '\n' && c != EOF);
					scanf("%f",&v_izracunDionice.v_xcPerf.v_brzinaVjetra);
					printf("\nSmjer vjetra (0-359): ");while ((c=getchar()) != '\n' && c != EOF);
					scanf("%d",&v_izracunDionice.v_xcPerf.v_smjerVjetra);
					printf("\nOcekivano prosjecno dizanje: ");while ((c=getchar()) != '\n' && c != EOF);
					scanf("%f",&v_izracunDionice.v_xcPerf.v_proDizanje);
					}
				if (v_izbor==3){
					printf("\nOdabir-3 Pokretanje i prikaz XC Izracuna");

					printf("\nDonica je od tocke %s do tocke %s.",v_izracunDionice.v_Txc1->v_naziv,v_izracunDionice.v_Txc2->v_naziv);

                    double v_duljinaDionice = v_GCdist(v_izracunDionice.v_Txc1->v_lat,v_izracunDionice.v_Txc1->v_lon,v_izracunDionice.v_Txc2->v_lat,v_izracunDionice.v_Txc2->v_lon);
                    printf("\nDuljina je %.2f km", v_duljinaDionice);

					printf("\nVjetar je smjera %d i brzine  %.2f m/s",v_izracunDionice.v_xcPerf.v_smjerVjetra, v_izracunDionice.v_xcPerf.v_brzinaVjetra );
					printf("\nProjecno ocekivano dizanje je %.2f m/s",v_izracunDionice.v_xcPerf.v_proDizanje);

					double v_Vxco=v_XC_ProsjekBrzine(v_izracunDionice.v_xcPerf.v_brzinaVjetra,v_izracunDionice.v_xcPerf.v_smjerVjetra ,v_izracunDionice.v_xcPerf.v_proDizanje,v_izracunDionice.v_xcPerf.a,v_izracunDionice.v_xcPerf.b,v_izracunDionice.v_xcPerf.c );

					printf("\nOcekivana prosjecna brzina je %.1lf km/h",v_Vxco);
					double v_vrijeme=v_duljinaDionice/v_Vxco;
					int v_minute = v_vrijeme*60;
					int v_minutaPrekoSati = v_minute % 60;
					printf("\nOcekivano potrebno vrijeme preleta dionice je %.0fh %dm",v_vrijeme, v_minutaPrekoSati );

					}
				}
				while (v_izbor!=4);


	}
}
while (v_izbor!=4);




}

