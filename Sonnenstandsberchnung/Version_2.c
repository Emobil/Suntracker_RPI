/* HTW-LABOR ELEKTROTECHNIK
   PROJEKTARBEIT: NACHFÜHRUNG EINER SOLARANLAGE
   AUTHOR: RENÉ KOST
*/



//----------------------- ---------------Präprozessor--------------------------------
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sleep.h>
#define PII 3.141592653589793l 

/**** directions ****/
#define BACKWARD 			0x01
#define FORWARD				0x02
#define	HORIZONTAL 			0x11
#define	VERTICAL 			0x12

/**** motor modes ****/
#define	NORMAL_MODE 			0x20
#define FAST_MODE				0x21
#define GOTO_REFERENCE			0x22

/**** I2C commands ****/
#define	SET_ELEVATION 			0x02
#define	SET_AZIMUTH 			0x03
#define	GOTO_VERTICAL_REFERENCE 	0x04
#define	GOTO_HORIZONTAL_REFERENCE	0x05
#define CMD_TERMINATED			0x33


//I2C 
#define SLAVE_ADDRESS 0x13
#define I2C_DEV "/dev/i2c-1"  // Versionsabhaengig
#define I2C_BUFFER_SIZE 4 // Number of Bytes 


char buffer[I2C_BUFFER_SIZE];

int i2c_read (int slaveaddr, int length, unsigned char *buffer){
	int deviceHandle = open(I2C_DEV,O_RDWR);
	ioctl(deviceHandle, I2C_SLAVE, slaveaddr);
	write(deviceHandle, 0x00, 0);
	read(deviceHandle,buffer,length);	
	close(deviceHandle);
	return 0; 
}

int i2c_write (int slaveaddr, int length, unsigned char *buffer){
	int deviceHandle = open(I2C_DEV,O_WRONLY);
	ioctl(deviceHandle, I2C_SLAVE, slaveaddr);
	write(deviceHandle, buffer, length);
	close(deviceHandle);
	return 0; 
}






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


//Systemzeit
today();
//geographiche Lage
Longitude = 13.7376630;
Latidue = 51.0345464;


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

int Azimut_int, Hoehe_int;

Hoehe_int = (int)Hoehe;
Azimut_int = (int)Azimut;

// ----------------------sichtbarere Programablauf -----------------------

printf(" \n Zum Starten Bitte Taste drücken ! \n ");
scanf ("%d", &check);

//Sende I2C- Daten 

buffer[3]=Azimut_int;
buffer[2]=SET_AZIMUTH;

i2c_write(SLAVE_ADDRESS , I2C_BUFFER_SIZE , buffer);

check = 0 ; 
// Warte bis Motor an Position gefahren ist ! 
while(buffer[1]!=CMD_TERMINATED)
{i2c_read(SLAVE_ADDRESS , I2C_BUFFER_SIZE , buffer);
sleep(1);
}

buffer[3]=Hoehe_int;
buffer[2]=SET_ELEVATION;

i2c_write(SLAVE_ADDRESS , I2C_BUFFER_SIZE , buffer);

// Ausgabe zur Kontrolle 
printf("\n\n Hoehe  = %lf \n\n Azimut = %lf \n\n", Hoehe, Azimut); 


  system("PAUSE");	
  return 0;
}
