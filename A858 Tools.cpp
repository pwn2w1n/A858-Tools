#include "stdafx.h"
#include <cstdio>
#include <windows.h>

char y;
int sum = 0;
bool mode1 = false;
bool interesting = false;
bool success;
unsigned long long int iv = 0;
char stringiv[17];
char command[100];
char pass[100];
char variety[100];
char character;
const char *middleCommand;
FILE *data = fopen("data.bin", "r");
FILE *output = fopen("output.bin", "r");

void mode(int i) { //loads decryption method into middleCommand
	if (i == 0) { middleCommand = "aes-128-cbc"; }
	else if (i == 1) { middleCommand = "aes-128-ecb"; }
	else if (i == 2) { middleCommand = "aes-192-cbc"; }
	else if (i == 3) { middleCommand = "aes-192-ecb"; }
	else if (i == 4) { middleCommand = "aes-256-cbc"; }
	else if (i == 5) { middleCommand = "aes-256-ecb"; }
	else if (i == 6) { middleCommand = "base64"; }
	else if (i == 7) { middleCommand = "bf"; }
	else if (i == 8) { middleCommand = "bf-cbc"; }
	else if (i == 9) { middleCommand = "bf-cfb"; }
	else if (i == 10) { middleCommand = "bf-ecb"; }
	else if (i == 11) { middleCommand = "bf-ofb"; }
	else if (i == 12) { middleCommand = "cast"; }
	else if (i == 13) { middleCommand = "cast-cbc"; }
	else if (i == 14) { middleCommand = "cast5-cbc"; }
	else if (i == 15) { middleCommand = "cast5-cfb"; }
	else if (i == 16) { middleCommand = "cast5-ecb"; }
	else if (i == 17) { middleCommand = "cast5-ofb"; }
	else if (i == 18) { middleCommand = "des"; }
	else if (i == 19) { middleCommand = "des-cbc"; }
	else if (i == 20) { middleCommand = "des-cfb"; }
	else if (i == 21) { middleCommand = "des-ecb"; }
	else if (i == 22) { middleCommand = "des-ede"; }
	else if (i == 23) { middleCommand = "des-ede-cbc"; }
	else if (i == 24) { middleCommand = "des-ede-cfb"; }
	else if (i == 25) { middleCommand = "des-ede-ofb"; }
	else if (i == 26) { middleCommand = "des-ede3"; }
	else if (i == 27) { middleCommand = "des-ede3-cbc"; }
	else if (i == 28) { middleCommand = "des-ede3-cfb"; }
	else if (i == 29) { middleCommand = "des-ede3-ofb"; }
	else if (i == 30) { middleCommand = "des-ofb"; }
	else if (i == 31) { middleCommand = "des3"; }
	else if (i == 32) { middleCommand = "desx"; }
	else if (i == 33) { middleCommand = "idea"; }
	else if (i == 34) { middleCommand = "idea-cbc"; }
	else if (i == 35) { middleCommand = "idea-cfb"; }
	else if (i == 36) { middleCommand = "idea-ecb"; }
	else if (i == 37) { middleCommand = "idea-ofb"; }
	else if (i == 38) { middleCommand = "rc2"; }
	else if (i == 39) { middleCommand = "rc2-40-cbc"; }
	else if (i == 40) { middleCommand = "rc2-64-cbc"; }
	else if (i == 41) { middleCommand = "rc2-cbc"; }
	else if (i == 42) { middleCommand = "rc2-cfb"; }
	else if (i == 43) { middleCommand = "rc2-ecb"; }
	else if (i == 44) { middleCommand = "rc2-ofb"; }
	else if (i == 45) { middleCommand = "rc4"; }
	else if (i == 46) { middleCommand = "rc4-40"; }
}

int sumcalculation() { //scoring function
	fseek(data, 0L, SEEK_END); //go to end of file
	int sz = (int)(0.75*ftell(data)); //sz is 75% of the filesize
	return sz;
}

void decrypt0() { //mode 0
	interesting = false;
	int j = 0;
	while (interesting == false) {
		freopen("output.bin", "r", output); //reloads output file
		mode(j); //loads decryption method
		sum = 0; //resets scoring function
		strcpy(command, "openssl "); //fill the command in several steps
		strcat(command, middleCommand);
		strcat(command, " -d -in data.bin -out output.bin -nosalt -pass pass:");
		strcat(command, pass);
		system(command);//send command to console
		while ((y = fgetc(output)) != EOF) { //execution of scoring functions
			if ((int)y >= 32 && (int)y <= 41)  //numbers
			{
				sum++;
			}
			else if ((int)y >= 65 && (int)y <= 90) //lowercase letters
			{
				sum++;
			}
			else if ((int)y >= 97 && (int)y <= 122) { sum++; } //uppercase letters
		}
		j++;
		if (sum > sumcalculation()) { success = true; interesting = true; } //if the number of interesting characters>75% of file, the decryption is succesful
		if (j > 46) {success = false; interesting = true; } //otherwise, it has failed after all modes have been tried
	}
}

void decrypt1() { //mode 1
	interesting = false;
	while (interesting == false) {
		printf("%s", stringiv); //print the IV in this try
		freopen("output.bin", "r", output); //reloads output file
		sprintf(stringiv, "%016X", iv); //load the hex iv in stringiv zero padded
		sum = 0; //resetea el contador de characteres interestings
		strcpy(command, "openssl ");//complete the command
		strcat(command, variety);
		strcat(command, " -d -in data.bin -out output.bin -K ");
		strcat(command, pass);
		strcat(command, " -iv ");
		strcat(command, stringiv);
		system(command);//send command to console
		while ((y = fgetc(output)) != EOF) {//scoring function execution
			if ((int)y >= 32 && (int)y <= 41)  //numbers
			{
				sum++;
			}
			else if ((int)y >= 65 && (int)y <= 90) //lowercase
			{
				sum++;
			}
			else if ((int)y >= 97 && (int)y <= 122) { sum++; } //uppercase
		}
		if (sum > sumcalculation()) { mode1 = true; success = true; interesting = true; }//if the file is more than 75% letters and numbers, the iv was right
		iv++; //increase iv by 1
		if (iv > 0xFFFFFFFFFFFFFFFF) { success = false; interesting = true; } //if iv exceeds the limit, the process failed
	}
}

int main()
{
	printf("A858 Tools 0.0.1 by gregalerna, 2015. Mode 0 to decrypt given key.\nMode 1 to find out IV given method and key. Select mode (0/1)\n");//intro
	scanf("%s", &character);//select mode
	if (character == '1') { printf("Try to brute force IV? (y/n)\n");scanf("%s", &character);if (character != 'y') { return 0; }	printf("Key: ");scanf("%s", &pass); printf("Method: "); scanf("%s", &variety);decrypt1(); }
	else {//mode 0
		printf("Try to decypt data.bin? (y/n)\n");
		scanf("%s", &character);
		if (character != 'y') { return 0; }
		printf("Key: ");
		scanf("%s", &pass);
		decrypt0();
	}
	fclose(data);//close data.bin
	fclose(output);//close output.bin
	if (success == true && mode1==false) { printf("Decryption successful with %s\n",middleCommand); }
	if (mode1==true){ printf("Possible IV was %s\n", stringiv); }
	if(mode1==false &&success==false) { printf("Decryption failed with OpenSSL\n"); }
	return 0;
}

