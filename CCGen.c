//Coded by sRBill1996
//Sites:netixzen.blogspot.com.ar
//      github.com/srbill1996 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define FILE_DB_BINS "db_bins.txt"
#define LONG_CARD_NUM 16

int   checkLuhn(char binNum[]);       	    //Luhn comprobation
void  randomizeBin(char binNum[]);				//Fill Bin X
void  genCC(char binNum[], int cantidad);	//Gen random BIN
int   genCCV();								//Gen CCV
char *genMonth();							//Gen Month
char *locationOfBin(char binNum[], int buffersize); //Search in db bins
int   getInputStr(char binNum[], char *nameInput, int limit);
void  printLogo();
void  locatePersonalBin();
void  lowerArray(char array[]);
void  randomBinFromDb();

char lastBin[100] = "000000";
int main(int argc, char *argv[])
{	
	srand(time(NULL));
	char binNum[20];
	long int amountGen = 10;
	while(1){
		system("cls");
		printLogo();
		printf("Insert opcion, input 'b' for back to the menu.\n");
		printf("1)Generate from BIN base.\n");
		printf("2)Check BIN validation.\n");
		printf("3)Set amount of Bins to generate.\n");
		printf("4)Search BIN.\n");
		printf("5)Search a BIN by keywords.\n");
		printf("6)Drop Random BIN\n");
		printf("7)Generate BIN from last in memory.\n");
		printf("8)Exit");
		printf("\nOption>:");
		int opcion;
		switch((opcion = getchar())){
			case '1':
				while(getInputStr(binNum, "BIN:", 1) != -1)
					genCC(binNum, amountGen);
				break;
			case '2':
				while(getInputStr(binNum, "BIN:", 16) != -1){
					if(checkLuhn(binNum) == 1)
						printf("Is a Valid BIN.\n");
					else
						printf("Is a Invalid BIN.\n");
				}
				break;
			case '3'://BIN SET AMOUNT
				printf("Amount:");
				scanf("%ld", &amountGen);
				printf("Done.\n");
				break;
			case '4':
				while(getInputStr(binNum, "BIN:", 6) != -1){
					printf("Result:%s", locationOfBin(binNum, 100));
					puts("");
				}
				break;
			case '5'://SEARCH BIN BY KEYWORDS
				while(getInputStr(binNum, "Keywords:",2) != -1)
					locatePersonalBin(binNum);
			case '6'://CHOICE RANDOM BIN FROM DB
				printf("'c' to cancel.\n");
				while(getchar()!='c'){
					randomBinFromDb();
				}
				break;
			case '7'://GENERATE BIN FROM LAST IN MEMORY
				printf("'c' to cancel.\n");
				while(getchar()!='c'){
					genCC(lastBin, amountGen);
				}
				break;
			case '8'://EXIT
				exit(1);
			default:
				printf("Invalid option(?\n ");
				break;
		}
	}
	return 0;
}

void randomBinFromDb(){
	FILE *bins;
	char buffer[100];
	int choicer = rand() % 604899;
	int choice = 0;
	if(!(bins = fopen(FILE_DB_BINS, "r"))){
		printf("%s was not found :(.\n", FILE_DB_BINS);
	}else{
		while(choice++ != choicer){
			fgets(buffer, 100, bins);
		}
		printf("%s",buffer);
		strncpy(lastBin, buffer, 6);
		return;
	}
}

void locatePersonalBin(char keywords[]){
	FILE *bins;
	char buffer[100];
	char tmpkeywords[100];
	
	char *word;
	if(!(bins = fopen(FILE_DB_BINS, "r"))){
		printf("%s was not found :(.\n", FILE_DB_BINS);
	}else{
		while(!feof(bins)){
			fgets(buffer, 100, bins);
			lowerArray(buffer);
			strcpy(tmpkeywords, keywords);
			lowerArray(tmpkeywords);
			word = strtok(tmpkeywords,",");
			do{
				if(!strstr(buffer, word))
					break;
			}while((word = strtok(NULL, ",")));
			if(word==NULL)
				printf("%s\n", buffer);
		}
		strncpy(lastBin, buffer, 6);
	}
}

int getInputStr(char binNum[], char *nameInput, int limit){
	binNum[0] = '\0';
	while(strlen(binNum) < limit){
		printf("%s", nameInput);
		scanf("%s", binNum);
		if(strlen(binNum) < limit)
			printf("Error in long limit of digits.\n");
		if(binNum[0] == 'b'){//back to the menu
			return -1;
		}
	}
	return 1;
}

void lowerArray(char array[]){
	int i;for(i=0;i < strlen(array);i++){
		array[i] = tolower(array[i]);
	}
}

char *locationOfBin(char bin[], int buffersize){
	FILE *bins;
	if(buffersize > 100)
		printf("Searching bin..\n");
	char buffer[buffersize];
	if(!(bins = fopen(FILE_DB_BINS, "r")) ){
		printf("%s was not found :(.\n", FILE_DB_BINS);
		system("pause");
		exit(-1);
	}else{
		while(!feof(bins)){
			fgets(buffer, buffersize, bins);
			if(!strncmp(buffer, bin, 6)){
				return strdup(buffer);
			}
		}
	}
	return "not found";
}

void printLogo(){

puts(" .d8888b.  .d8888b.  .d8888b.          ");
puts("d88P  Y88bd88P  Y88bd88P  Y88b         ");
puts("888    888888    888888    888         ");
puts("888       888       888                ");
puts("888       888       888  88888         ");
puts("888    888888    888888    888         ");
puts("Y88b  d88PY88b  d88PY88b  d88P 		");
printf(" \"Y8888P\"  \"%s\"  \"3/12/17\" \n",lastBin);
puts("    ---- By:sRBill1996 ----\n");
}

void genCC(char binNum[], int amountToGen){
	printf("Generating..\n");
	if(strlen(binNum) > LONG_CARD_NUM){
		printf("Max of 16 digits exceded.\n");
		return;
	}
	char tmpBIN[19];
	int count = 0, failRounds = 0;
	strcpy(tmpBIN, binNum);
	time_t time_ = time(&time_);
	struct tm *date = localtime(&time_);
	int actualYear = 1900 + (date->tm_year);
	while(amountToGen > count){
		strcpy(tmpBIN, binNum);
		randomizeBin(tmpBIN);
		if(checkLuhn(tmpBIN) == 1){
			printf("%s | %d | %s/%d\n", 
									tmpBIN, 
									genCCV(), 
									genMonth(),
									(actualYear-2000)+rand()%6
			);
			count++;
			strncpy(lastBin, tmpBIN, 6);
			failRounds = 0;
		}else{
			failRounds++;
			if(failRounds > 10000){
				printf("Max rounds limit exceded, please insert a valid BIN..\n");
				break;
			}
		}
	}
	printf("\n>BIN Location:%s\n\n", locationOfBin(binNum, 100));
}

char *genMonth(){
	int month_ = rand() & 12;
	switch(month_){
		case 1:
			return "01";
		case 2:
			return "02";
		case 3:
			return "03";
		case 4:
			return "04";
		case 5:
			return "05";
		case 6:
			return "06";
		case 7:
			return "07";
		case 8:
			return "08";
		case 9:
			return "09";
		case 10:
			return "10";
		case 11:
			return "11";
		case 12:
			return "12";
		default:
			return "01";
		}
		return "01";
}

int genCCV(){
	int ccv = 0;
	while(ccv < 101){
		ccv  = rand() % 998;
	}
	return ccv;
}

void randomizeBin(char binNum[]){
	int cnt;
	//generate 
	if(strlen(binNum) < LONG_CARD_NUM){
		for(cnt=strlen(binNum); cnt < LONG_CARD_NUM; cnt++){
			binNum[cnt] = 'x';
		}
		binNum[LONG_CARD_NUM] = '\0';
		for(cnt=0; cnt<LONG_CARD_NUM; cnt++){
			if(!isdigit(binNum[cnt])){
				binNum[cnt] = (rand() % 9) + '0';
			}
		}
	}else{
		for(cnt=0; cnt<strlen(binNum); cnt++){
			if(tolower(binNum[cnt]) == 'x'){
				binNum[cnt] = (rand() % 9) + '0';
			}
		}
	}
}

int sumNumDigits(int num){
	if(num < 10){
		return num;
	}else{
		return num % 10 + sumNumDigits(num/10);
	}
}

int checkLuhn(char binNum[]){
	int i,num,suma = 0;
	for(i=strlen(binNum)-1; i>=0; i--)
	{
		if(i%2!=0){
			suma+= (int)binNum[i]-'0';
		}else{
			num = ((int)binNum[i] - '0') * 2;
			if(num > 9){
				suma+= sumNumDigits(num);
			}else{
				suma+=num;
			}
		}
	}
	return (suma%10 == 0)? 1:-1;
}
