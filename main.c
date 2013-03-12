#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdio.h>
#include <fcntl.h>

#define SLAVE_ADDRESS 0x10

// I2C commands
#define ALL_RELAYS_OFF 	0x17
#define ALL_RELAYS_ON 	0x16
#define RELAY_1_OFF 	0x14
#define RELAY_2_OFF 	0x15
#define RELAY_1_ON 	0x12
#define RELAY_2_ON 	0x13
#define CLEAR_S0_1	0x18
#define CLEAR_S0_2	0x19

#define NO_STATE	0xFF

#define I2C_DEV "/dev/i2c-0"
#define I2C_BUFFER_SIZE 11 // Number of Bytes 

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

int main (void)
{  
    
    
// ------------------------------------------------------------------------------
//Teste ab eine Spannung an einen Pinangeschlossen ist

char JaNein
 printf("Prüfe ab Hall 1 = 1  ist");
 
 
   while (1) { 
   
   i2c_read(SLAVE_ADDRESS, I2C_BUFFER_SIZE , buffer) // Lesen der buffer adressen
   
   If (buffer[1]==0x01){  // Wird durch Eingangssignal initialisiert
                      buffer[2]=0x02 // EN - Laampe an 
                      printf("Signal liegt an ! \n ");
                       }
                       else 
                       {
                      buffer[2]=0x00 //EN - Lampe aus
                      printf("Kein Signal liegt an ! \n ");
                       }
                       printf("Erneute Prüfung des Anschlusses? Y/N \n");
                       JaNein = getchar();
 i2c_write(SLAVE_ADDRESS , I2C_BUFFER_SIZE , buffer);

}

//-----------------------------------------------------------------------------------------------------------------------------
/*  int i,j = 0;
  
  unsigned long int S0_1, S0_2;

  buffer[2] = ALL_RELAYS_ON;
  i2c_write(SLAVE_ADDRESS , I2C_BUFFER_SIZE , buffer); 

  while(1){

	i2c_read(SLAVE_ADDRESS, I2C_BUFFER_SIZE , buffer);	

/*	for ( i=0; i<I2C_BUFFER_SIZE ; i++){ 
		printf("0x%02X\n", buffer[i]); 
	}
*/

/*	S0_1 = ((buffer[2] << 8 ) | buffer[3]);
	printf("S01: %i\n",S0_1);

	S0_2 = ((buffer[4] << 8 ) | buffer[5]);
	printf("S02: %i\n\n",S0_2);

	if(buffer[1] == ALL_RELAYS_OFF){ 
		printf("Nichts angeschlossen !\n"); 
	}
	if(buffer[1] == RELAY_1_OFF){ 
		printf("Stecker 1 entfernt !\n"); 
		buffer[2] = CLEAR_S0_1;	
	}
	if(buffer[1] == RELAY_2_OFF){ 
		printf("Stecker 2 entfernt !\n"); 
		buffer[2] = CLEAR_S0_2;
	}

	i2c_write(SLAVE_ADDRESS , I2C_BUFFER_SIZE , buffer);
	sleep(1); */
  }

  return 0;
}
