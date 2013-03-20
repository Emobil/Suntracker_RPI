/* HTW-LABOR ELEKTROTECHNIK
   PROJEKTARBEIT: NACHFÜHRUNG EINER SOLARANLAGE
   AUTHOR: RENÉ KOST
*/

//--------------------------------------Präprozessor--------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define PII 3.141592653589793l 

int tag,monat,jahr, stunde, sekunde;
double minute; // Globale Definition da diese Werte in mehren Gunktionen gebraucht werden
               // Minute als double da bei einer Berchnung über Minute ein double Wert zurück
               //gegen werden sollte. 
               
struct tm *tmnow; // Nutzen der Systemzeit
//------------------------------------------------------------------------------------
// Zeitroutine zum abfragen der Systemzeit
long today(void)
{
   time_t tnow; 
   time(&tnow);
   tmnow = localtime(&tnow);
                       
   tag =tmnow->tm_mday; monat =tmnow-> tm_mon + 1; jahr = tmnow->tm_year + 1900;  // Zuweiseung des Datums 
   printf(" Heute ist der %d.%d.%d \n ", tag, monat , jahr); 
   
   stunde = tmnow->tm_hour; minute = tmnow->tm_min; sekunde = tmnow->tm_sec; //Zuweisung der Uhrzeit
   printf(" um: %d:%lf:%d \n\n ", stunde, minute, sekunde);
                    
   return 1;
}

//--------------Berchnung der Tages Zahl --------------------------------------------------

int tage_seit_jahresanfang(int tag, int monat, int jahr){

    int tage_pro_monat[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int ret = 0;
    int i = 1; 

    // Volle Monate
    for ( i = 1; i < monat; i++){
      ret += tage_pro_monat[i - 1];
    }

    // Tag des aktuellen Monats
    ret += tag;

    // Schaltjahr
 if (ist_schaltjahr(jahr) == 1 && monat > 2){
                             ret = ret + 1; 
                             }
    else {
         ret = ret; 
         }

    return ret;
}

int ist_schaltjahr(int jahr){

    int schaltjahr = 0;

    if (jahr % 4 == 0) schaltjahr = 1;
    if (jahr % 100 == 0) schaltjahr = 0;
    if (jahr % 400 == 0) schaltjahr = 1;

    return schaltjahr;
}
//---------------------------------------------------------------------------------------
// Hauptprogramm
int main()
{
//Deklaration
   int  i, check;
   double Longitude, Latidue ,  Dek, TZ, ZGL, MOZ, WOZ, StW, Hoehe, Azimut, ZD, AUF, UNT, temp;  

  static const long double  Bogen = 0.01745329251994329576923690768489 ;   // Für die Umrechnung in Bogenmass
  static const long double  Grad =  57.295779513082320876798154814105 ;     // Für die Umrechnung in Gradmass


// Frage nach Systemzeit oder selbst gewählten Datum und Zeit

do {
printf("Wollen Sie das Aktuelle Datum Nutzen Eingabe 1 oder \n wollen Sie manuell ein Datum eingeben 2 ");
scanf ("%d", &check); 
    if (check==1) { // Abfrage der Aktuellen Zeit aus funktion "today()" 
              today();
              
               i = 0; // für Abbruch der Schleife 
              }
              else if (check == 2)  
              {
              // Datumsabfrage Maunell
               printf("Bitte gib den zu berechnenden Tag ein \n");
               printf("Tag : "); scanf("%d",&tag);   //Tag
               printf("Monat : "); scanf("%d",&monat);  //Monat
               printf("Jahr (jjjj) : "); scanf("%d",&jahr);  //Jahr
            
            //  Zeitabfrage Manuell
               printf("Bitte gib die zu berechnen Tageszeit ein \n");
               printf("Stunde : ");  scanf("%d",&stunde); // Stunde
               printf("Minute : ");   scanf("%d",&minute);  // Minute 
               printf("Sekunde : ");  scanf("%d",&sekunde);   //Sekunde 
               
               printf(" Heute ist der %d.%d.%d \n ", tag, monat , jahr);
                  printf(" um: %d:%lf:%d \n\n ", stunde, minute , sekunde);  // Rückgabe zu sicherung 
               
              i = 0; // für Abbruch der Schleife 
              }
                      else 
                      {
                       printf("Bitte geben Sie einen richtigen Wert ein \n\n");
                       
                      i=1; // für neuen Schleifendurchlauf 
                      }
}while(i==1);    // i als Prüfbedingung wenn i==1 (wahr) erneuter Schleifendurchlauf 
   
//Frage nach der geographichen Lage
check = 0; i = 1;  // Prüfvariablen auf 0 setzen um gespeichereten Wert zu löschen

do { 
     
printf("Wollen Sie die Gegoraphiche Lage des Labor nutzen? --> 1 \n Oder wollen Sie manuell eine Lage eingeben? --> 2");
scanf ("%d", &check); 
     if ( check == 1) { // Benutzen der Labordaten für den Standort der Photovoltaikanlage
     Longitude = 13.7376630;
     Latidue = 51.0345464;
     printf("Sie haben eingegen Longitude (Länge) = %lf         Latidude (Breite) %lf \n\n",Longitude,  Latidue);
     i=0;
        }
        else if (check == 2) {
        printf("Bitte gib die Geographiche Lage an \n");       
        printf("Longitude : "); scanf("%lf",&Longitude);  // Longitude --> Längengrad 
        printf("Latidue : ");  scanf("%lf",&Latidue);   // Latidude --> Breitengrad
        printf("Sie haben eingegen Longitude (Länge) = %lf         Latidude (Breite) %lf \n\n",Longitude,  Latidue);
        i=0;     
             }
             else {
             printf("Bitte geben Sie einen richtigen Wert ein \n\n");
                       
             i=1; // für neuen Schleifendurchlauf 
                  }
    
    }while (i==1); 

//-------Eigentliche Berchnung ------------------------------------------------------------
// Berechnung von "DIN 5034 Teil 2"
//TZ = Tageszahl / Dek = Deklination / ZGL = Zeitgleichung / MOZ = Mittlere Ortszeit / WOZ = Wahre Ortzszeit (Wahre Sonnenzeit)
TZ = tage_seit_jahresanfang(tag, monat, jahr);
Dek= 0.3948-(23.2559*cos(Bogen*(TZ+9.1)))-(0.3915*cos(Bogen*(2*TZ+5.4)))-(0.1764*cos(Bogen*(3*TZ+26)));
ZGL = 0.0066+7.3525*cos(Bogen*(TZ+85.9))+9.9359*cos(Bogen*(2*TZ+108.9))+0.3387*cos(Bogen*(3*TZ+105.2));  // !!!! Minuten
MOZ = ((stunde*60)+minute)-60+4*Longitude;  // in Minuten
WOZ = MOZ + ZGL; // in  MInuten
MOZ = ((MOZ/60)-(int)(MOZ/60))+(int)(MOZ/60);   // in Stunden
WOZ = ((WOZ/60)-(int)(WOZ/60))+(int)(WOZ/60);   // in Studnen
StW =  (12-WOZ)*15;  // in Grad

Hoehe =  Grad*(asin((cos(Bogen*StW)*cos(Bogen*Latidue)*cos(Bogen*Dek)) + (sin(Bogen*Latidue)*sin(Bogen*Dek)))) ;
Azimut = Grad*(acos(((sin(Bogen*Hoehe)*sin(Bogen*Latidue))-sin(Bogen*Dek))/(cos(Bogen*Hoehe)*cos(Bogen*Latidue))));
if (WOZ>12){
            Azimut = Azimut + 180;
            }
            else {
                 Azimut = Azimut + 180;
                 }


//ZD = Zeitdifferenz / AUF = Sonnenaufgang / UNT = Sonnenuntergang 
ZD = 12*acos((sin(-0.0145)-sin(Bogen*Latidue)*sin(Bogen*Dek))/(cos(Bogen*Latidue)*cos(Bogen*Dek)))/PII;
temp = (ZGL/100);
AUF = (12-ZD-(ZGL/100))-(Longitude/15)+1;        // Dezimale Zeitangabe Normalzeit im Sommer "+2" Für Zeitumstellung 
UNT = (12+ZD-(ZGL/100))-(Longitude/15)+1;        // Dezimale Zeitangabe Normalzeit im Sommer "+2" Für Zeitumstellung 

// Ausgabe zur Kontrolle 
printf("Tageszahl %lf \n\n Deklination Länge = %lf \n\n Zeitgleichung = %lf \n\n MOZ = %lf \
\n\n WOZ = %lf \n\n Stundenwinkel = %lf \n\n Hoehe  = %lf \n\n Azimut = %lf \n\n \
Zeitdifferenz = %lf \n\n Sonnenaufgang = %lf \n\n Sonnenuntergnag = %lf \n\n \
",TZ,  Dek, ZGL , MOZ, WOZ,StW, Hoehe, Azimut, ZD, AUF, UNT ); 


  system("PAUSE");	
  return 0;
}
