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



//---------------------------------------------------------------------------------------
// Hauptprogramm
int main()
{
//Deklaration
   int  i, check;
   double Longitude, Latidue ,  Dek, TZ, ZGL, MOZ, WOZ, StW, Hoehe, Azimut, ZD, AUF, UNT, temp;  

  static const long double  Bogen = 0.01745329251994329576923690768489 ;   // Für die Umrechnung in Bogenmass
  static const long double  Grad =  57.295779513082320876798154814105 ;     // Für die Umrechnung in Gradmass



// ----------------------sichtbarere Programablauf -----------------------

printf(" \n Zum Starten Bitte Taste drücken ! \n ");
scanf ("%d", &check);

//Sende I2C- Daten 

buffer[3]=40;
buffer[2]=SET_AZIMUTH;

i2c_write(SLAVE_ADDRESS , I2C_BUFFER_SIZE , buffer);

  return 0;
}
