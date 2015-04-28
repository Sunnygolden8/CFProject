#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>
#include <iomanip>
#include <ctype.h>
#include <fcntl.h>

using namespace std;

//forward declarations
std::string getFS(char* c);
int getHexadecimal(std::string hex);

//global variables
int partition1, partition2, partition3, partition4 = 0; //partition iterators
int part1, part2, part3, part4 = 0; //second iterators
int FS1, FS2, FS3, FS4 = 0; //holds whether partition is FAT32/16
int place1, place2, place3, place4; //entrypoint for each partition
std::string FSname1, FSname2, FSname3, FSname4; //holds fs names for each partition
std::string startingsector; //holds starting sector hex
std::string sectorsize; //holds starting sector size hex
std::string sectorspercluster;
std::string numOfFAT;
std::string sizeOfFAT, sizeOfFAT2, sizeOfFAT3, sizeOfFAT4;
std::string reservedsize, reservedsize2, reservedsize3, reservedsize4; //holds sector size hex of reserved area
int startofFAT;
int partionNo = 0;

typedef struct OPTIONS_STR
{
	int max;
	int printName;
} OPTIONS;

void defaultOptions(OPTIONS *options)
{
	options->max = -1;
	options->printName = 0;
}

void Options(OPTIONS *options, char *arg)
{
	if (strlen(arg) < 2)
	{
		return;
	}
	
	if (arg[0] != '-')
	{
		return;
	}
	
	if (arg[1] == 'n')
	{
		options->printName = 1;
	}
}

void do_file(FILE *in, FILE *out, OPTIONS *options)
{
	char ch;
    int loop = 0;
    char buf[81];
    buf[0] = '\0';

    while (options->max == -1 || loop < options->max)
	{
		if (feof(in))
		{
			break;
		}

		if (loop % 16 == 0)
		{
			if (strlen(buf) > 0)
			{
                buf[0] = '\0';
			}
        
			//if(loop == 9000){

				//exit(0);
			//}
		}
		else if (loop % 8 == 0)
		{
			strcat(buf, " ");
		}
        fread(&ch, 1, 1, in);

		/////////////////////MBR
		if(loop >= 448 && loop < 464){ //first partition (16bytes)

			if(partition1 == 0){
					printf("===================================================\n");
			}
			
			int decform = (int)(ch & 0x00FF);
			char hex[33];
 
			//itoa (decform,hex,16);
			sprintf(hex,"%x", decform);

			//File system
			if(partition1 == 2){
				fprintf(out, "(%02x)", (int)(ch & 0x00FF));
				std::string filesystem = getFS(hex);

				if (filesystem.find("32") != std::string::npos || filesystem.find("16") != std::string::npos){
					FS1 = 1;
					FSname1 = filesystem;
				}

				std::cout << " " << filesystem << ", ";
			}

			///////////////////////Starting sector
			if(partition1 == 6){
				if(std::string(hex).size() > 1){
					startingsector = std::string(hex);
				}
				else
					startingsector = "0" + std::string(hex);

			}
			if(partition1 >= 7 && partition1 <=9){	
				if(std::string(hex).size() > 1){
					startingsector = std::string(hex) + startingsector;
				}
				else
					startingsector = "0" + std::string(hex) + startingsector;
			}

			if(partition1 == 10){

				int finaldec = getHexadecimal(startingsector); //get decimal of starting sector hex	

				place1 = finaldec;

				std::cout << std::setfill('0') << std::setw(10) << finaldec << ", "; //print with padding 0s

				if(std::string(hex).size() > 1){
					sectorsize = std::string(hex);
				}
				else
					sectorsize = "0" + std::string(hex);

			}
			///////////////////////end starting sector

			///////////////////////Sector size
			if(partition1 >= 11 && partition1 <=13){	
				if(std::string(hex).size() > 1){
					sectorsize = std::string(hex) + sectorsize;
				}
				else
					sectorsize = "0" + std::string(hex) + sectorsize;
			}

			if(partition1 == 14){
				
				int finaldec = getHexadecimal(sectorsize); //convert hex to decimal
				std::cout << std::setfill('0') << std::setw(10) << finaldec << "\n"; //print with padding 0s

			}
			///////////////////////end sector size

			partition1++;
		}
		if(loop >= 464 && loop < 480){ //second partition (16bytes)
			
			int decform = (int)(ch & 0x00FF);
			char hex [33];
 
			//itoa (decform,hex,16);
			sprintf(hex,"%x", decform);
		
			//File system
			if(partition2 == 2){
				fprintf(out, "(%02x)", (int)(ch & 0x00FF));
				std::string filesystem = getFS(hex);

				if (filesystem.find("32") != std::string::npos || filesystem.find("16") != std::string::npos){
					FS2 = 1;
					FSname2 = filesystem;
				}
					
				std::cout << " " << filesystem << ", ";
			}

			//Starting sector
			if(partition2 == 6){
				if(std::string(hex).size() > 1){
					startingsector = std::string(hex);
				}
				else
					startingsector = "0" + std::string(hex);

			}
			if(partition2 >= 7 && partition2 <=9){	
				if(std::string(hex).size() > 1){
					startingsector = std::string(hex) + startingsector;
				}
				else
					startingsector = "0" + std::string(hex) + startingsector;
			}

			if(partition2 == 10){

				int finaldec = getHexadecimal(startingsector); //get decimal of starting sector hex	

				place2 = finaldec;

				std::cout << std::setfill('0') << std::setw(10) << finaldec << ", "; //print with padding 0s

				if(std::string(hex).size() > 1){
					sectorsize = std::string(hex);
				}
				else
					sectorsize = "0" + std::string(hex);
		
			}
			//end starting sector

			///////////////////////Sector size
			if(partition2 >= 11 && partition2 <=13){	
				if(std::string(hex).size() > 1){
					sectorsize = std::string(hex) + sectorsize;
				}
				else
					sectorsize = "0" + std::string(hex) + sectorsize;
			}

			if(partition2 == 14){
				
				int finaldec = getHexadecimal(sectorsize); //convert hex to decimal
				std::cout << std::setfill('0') << std::setw(10) << finaldec << "\n"; //print with padding 0s

			}
			///////////////////////end sector size

			partition2++;
		}
		if(loop >= 480 && loop < 496){ //third partition (16bytes)
			//fprintf(out, "%02x", (int)(ch & 0x00FF));

			int decform = (int)(ch & 0x00FF);
			char hex [33];
 
			//itoa (decform,hex,16);
			sprintf(hex,"%x", decform);

			//File system
			if(partition3 == 2){
				fprintf(out, "(%02x)", (int)(ch & 0x00FF));
				std::string filesystem = getFS(hex);

				if (filesystem.find("32") != std::string::npos || filesystem.find("16") != std::string::npos){
					FS3 = 1;
					FSname3 = filesystem;
				}

				std::cout << " " << filesystem << ", ";
			}

			//Starting sector
			if(partition3 == 6){
				if(std::string(hex).size() > 1){
					startingsector = std::string(hex);
				}
				else
					startingsector = "0" + std::string(hex);

			}
			if(partition3 >= 7 && partition3 <=9){	
				if(std::string(hex).size() > 1){
					startingsector = std::string(hex) + startingsector;
				}
				else
					startingsector = "0" + std::string(hex) + startingsector;
			}

			if(partition3 == 10){
				
				int finaldec = getHexadecimal(startingsector); //get decimal of starting sector hex	

				place3 = finaldec;

				std::cout << std::setfill('0') << std::setw(10) << finaldec << ", "; //print with padding 0s

				if(std::string(hex).size() > 1){
					sectorsize = std::string(hex);
				}
				else
					sectorsize = "0" + std::string(hex);
			}
			//end starting sector

				///////////////////////Sector size
			if(partition3 >= 11 && partition3 <=13){	
				if(std::string(hex).size() > 1){
					sectorsize = std::string(hex) + sectorsize;
				}
				else
					sectorsize = "0" + std::string(hex) + sectorsize;
			}

			if(partition3 == 14){
				
				int finaldec = getHexadecimal(sectorsize); //convert hex to decimal
				std::cout << std::setfill('0') << std::setw(10) << finaldec << "\n"; //print with padding 0s

			}
			///////////////////////end sector size

			partition3++;
		}
		if(loop >= 496 && loop < 512){ //fourth partition (16bytes)
			//fprintf(out, "%02x", (int)(ch & 0x00FF));

			int decform = (int)(ch & 0x00FF);
			char hex [33];
 
			//itoa (decform,hex,16);
			sprintf(hex,"%x", decform);

			//File system
			if(partition4 == 2){
				fprintf(out, "(%02x)", (int)(ch & 0x00FF));
				std::string filesystem = getFS(hex);

				if (filesystem.find("32") != std::string::npos || filesystem.find("16") != std::string::npos){
					FS4 = 1;
					FSname4 = filesystem;
				}

				std::cout << " " << filesystem << ", ";
			}

			//Starting sector
			if(partition4 == 6){
				if(std::string(hex).size() > 1){
					startingsector = std::string(hex);
				}
				else
					startingsector = "0" + std::string(hex);

			}
			if(partition4 >= 7 && partition4 <=9){	
				if(std::string(hex).size() > 1){
					startingsector = std::string(hex) + startingsector;
				}
				else
					startingsector = "0" + std::string(hex) + startingsector;
			}

			if(partition4 == 10){
				
				int finaldec = getHexadecimal(startingsector); //get decimal of starting sector hex	

				place4 = finaldec;

				std::cout << std::setfill('0') << std::setw(10) << finaldec << ", "; //print with padding 0s

				if(std::string(hex).size() > 1){
					sectorsize = std::string(hex);
				}
				else
					sectorsize = "0" + std::string(hex);
			
			}
			//end starting sector

			///////////////////////Sector size
			if(partition4 >= 11 && partition4 <=13){	
				if(std::string(hex).size() > 1){
					sectorsize = std::string(hex) + sectorsize;
				}
				else
					sectorsize = "0" + std::string(hex) + sectorsize;
			}

			if(partition4 == 14){
				
				int finaldec = getHexadecimal(sectorsize); //convert hex to decimal
				std::cout << std::setfill('0') << std::setw(10) << finaldec << "\n"; //print with padding 0s

			}
			///////////////////////end sector size

			partition4++;
		}
		/////////////////////End MBR


		//////////////////////////////////VBR
		if(FS1 == 1){ //partition1
			if(loop >= place1*512 && loop <= (place1*512)+48){ //find entry point
				
				//fprintf(out, "%02x", (int)(ch & 0x00FF));

				int decform = (int)(ch & 0x00FF);
				char hex [33];
				//itoa (decform,hex,16);
				sprintf(hex,"%x", decform);

				if(part1 == 0){
					printf("===================================================\n");
					std::cout << "Partition " << partionNo << "(" << FSname1 << "):\n";
					std::cout << "Reserved area: Start sector: 0 Ending sector: ";
					partionNo++;
				}

				if(part1 == 13){ //sector per cluster
					sectorspercluster = std::string(hex);
				}
				if(part1 >= 14 && part1 <=15){	//reserved area sector size
					if(std::string(hex).size() > 1){
						reservedsize = std::string(hex) + reservedsize ;
					}
					else
						reservedsize  = "0" + std::string(hex) + reservedsize ;
				}
				if(part1 == 16){ //number of FATs
					numOfFAT = std::string(hex);
					int finaldec = getHexadecimal(reservedsize); //convert hex to decimal
					startofFAT = finaldec;
					std::cout << finaldec-1 << " Size: " << finaldec << " sectors\n";
					std::cout << "Sectors per cluster: " <<  sectorspercluster << " sectors\n";
					std::cout << "Fat area: Start sector: " << startofFAT;
				}

				if (FSname1.find("16") != std::string::npos){ //size of each FAT
					if(part1 >= 22 && part1 <=23){	
						if(std::string(hex).size() > 1){
							sizeOfFAT = std::string(hex) + sizeOfFAT;
						}
						else
							 sizeOfFAT  = "0" + std::string(hex) +  sizeOfFAT ;
					}
				}
				else if (FSname1.find("32") != std::string::npos){
					if(part1 >= 36 && part2 <=39){	//reserved area sector size
						if(std::string(hex).size() > 1){
							sizeOfFAT = std::string(hex) + sizeOfFAT;
						}
						else
							 sizeOfFAT  = "0" + std::string(hex) +  sizeOfFAT ;
					}
				}

				if (FSname1.find("16") != std::string::npos){ 
					if(part1 == 24){	
						int finaldec = getHexadecimal(sizeOfFAT); //convert hex to decimal
						int numoffat = atoi(numOfFAT.c_str());
						int endsect = (finaldec*numoffat + startofFAT) - 1;
						std::cout << " End sector: " << endsect << "\n";
						std::cout << "# of FATS: " << numOfFAT << "\n";
						std:: cout << "The size of each FAT: " << finaldec << "\n";
						
					}
				}
				else if (FSname1.find("32") != std::string::npos){
					if(part1 == 40){
						int finaldec = getHexadecimal(sizeOfFAT); //convert hex to decimal
						int numoffat = atoi(numOfFAT.c_str());
						int endsect = (finaldec*numoffat + startofFAT) - 1;
						std::cout << " End sector: " << endsect << "\n";
						std::cout << "# of FATS: " << numOfFAT << "\n";
						std:: cout << "The size of each FAT: " << finaldec << "\n";
					}
				}
				part1++;
			}	
		}

		//------------------------------------//
		if(FS2 == 1){ //partition2
			
			if(loop >= place2*512 && loop <= (place2*512)+48){ //find entry point
			
				int decform = (int)(ch & 0x00FF);
				char hex [33];
				//itoa (decform,hex,16);
				sprintf(hex,"%x", decform);

				if(part2 == 0){
					printf("===================================================\n");
					std::cout << "Partition " << partionNo << "(" << FSname2 << "):\n";
					std::cout << "Reserved area: Start sector: 0 Ending sector: ";
					partionNo++;
				}

				if(part2 == 13){ //sector per cluster
					sectorspercluster = std::string(hex);
				}
				if(part2 >= 14 && part2 <=15){	//reserved area sector size
					if(std::string(hex).size() > 1){
						reservedsize2 = std::string(hex) + reservedsize2 ;
					}
					else
						reservedsize2  = "0" + std::string(hex) + reservedsize2 ;
				}
				if(part2 == 16){ //number of FATs
					numOfFAT = std::string(hex);
					int finaldec = getHexadecimal(reservedsize2); //convert hex to decimal
					startofFAT = finaldec;
					std::cout << finaldec-1 << " Size: " << finaldec << " sectors\n";
					std::cout << "Sectors per cluster: " <<  sectorspercluster << " sectors\n";
					std::cout << "Fat area: Start sector: " << startofFAT;
				}

				if (FSname2.find("16") != std::string::npos){ //size of each FAT
					if(part2 >= 22 && part2 <=23){	
						if(std::string(hex).size() > 1){
							sizeOfFAT2 = std::string(hex) + sizeOfFAT2;
						}
						else
							 sizeOfFAT2  = "0" + std::string(hex) +  sizeOfFAT2 ;
					}
				}
				else if (FSname2.find("32") != std::string::npos){
					if(part2 >= 36 && part2 <=39){	//reserved area sector size
						if(std::string(hex).size() > 1){
							sizeOfFAT2 = std::string(hex) + sizeOfFAT2;
						}
						else
							 sizeOfFAT2  = "0" + std::string(hex) +  sizeOfFAT2;
					}
				}

				if (FSname2.find("16") != std::string::npos){ 
					if(part2 == 24){	
						int finaldec = getHexadecimal(sizeOfFAT2); //convert hex to decimal
						int numoffat = atoi(numOfFAT.c_str());
						int endsect = (finaldec*numoffat + startofFAT) - 1;
						std::cout << " End sector: " << endsect << "\n";
						std::cout << "# of FATS: " << numOfFAT << "\n";
						std:: cout << "The size of each FAT: " << finaldec << "\n";
						
					}
				}
				else if (FSname2.find("32") != std::string::npos){
					if(part2 == 40){
						int finaldec = getHexadecimal(sizeOfFAT2); //convert hex to decimal
						int numoffat = atoi(numOfFAT.c_str());
						int endsect = (finaldec*numoffat + startofFAT) - 1;
						std::cout << " End sector: " << endsect << "\n";
						std::cout << "# of FATS: " << numOfFAT << "\n";
						std:: cout << "The size of each FAT: " << finaldec << "\n";
					}
				}
				part2++;
			}	
		}

		//------------------------------------//
		if(FS3 == 1){ //partition3
			
			if(loop >= place3*512 && loop <= (place3*512)+48){ //find entry point
			
				int decform = (int)(ch & 0x00FF);
				char hex [33];
				//itoa (decform,hex,16);
				sprintf(hex,"%x", decform);

				if(part3 == 0){
					printf("===================================================\n");
					std::cout << "Partition " << partionNo << "(" << FSname3 << "):\n";
					std::cout << "Reserved area: Start sector: 0 Ending sector: ";
					partionNo++;
				}

				if(part3 == 13){ //sector per cluster
					sectorspercluster = std::string(hex);
				}
				if(part3 >= 14 && part3 <=15){	//reserved area sector size
					if(std::string(hex).size() > 1){
						reservedsize3 = std::string(hex) + reservedsize3 ;
					}
					else
						reservedsize3  = "0" + std::string(hex) + reservedsize3;
				}
				if(part3 == 16){ //number of FATs
					numOfFAT = std::string(hex);
					int finaldec = getHexadecimal(reservedsize3); //convert hex to decimal
					startofFAT = finaldec;
					std::cout << finaldec-1 << " Size: " << finaldec << " sectors\n";
					std::cout << "Sectors per cluster: " <<  sectorspercluster << " sectors\n";
					std::cout << "Fat area: Start sector: " << startofFAT;
				}

				if (FSname3.find("16") != std::string::npos){ //size of each FAT
					if(part3 >= 22 && part3 <=23){	
						if(std::string(hex).size() > 1){
							sizeOfFAT3 = std::string(hex) + sizeOfFAT3;
						}
						else
							 sizeOfFAT3  = "0" + std::string(hex) +  sizeOfFAT3;
					}
				}
				else if (FSname3.find("32") != std::string::npos){
					if(part3 >= 36 && part3 <=39){	//reserved area sector size
						if(std::string(hex).size() > 1){
							sizeOfFAT3 = std::string(hex) + sizeOfFAT3;
						}
						else
							 sizeOfFAT3  = "0" + std::string(hex) +  sizeOfFAT3;
					}
				}

				if (FSname3.find("16") != std::string::npos){ 
					if(part3 == 24){	
						int finaldec = getHexadecimal(sizeOfFAT3); //convert hex to decimal
						int numoffat = atoi(numOfFAT.c_str());
						int endsect = (finaldec*numoffat + startofFAT) - 1;
						std::cout << " End sector: " << endsect << "\n";
						std::cout << "# of FATS: " << numOfFAT << "\n";
						std:: cout << "The size of each FAT: " << finaldec << "\n";
						
					}
				}
				else if (FSname3.find("32") != std::string::npos){
					if(part3 == 40){
						int finaldec = getHexadecimal(sizeOfFAT3); //convert hex to decimal
						int numoffat = atoi(numOfFAT.c_str());
						int endsect = (finaldec*numoffat + startofFAT) - 1;
						std::cout << " End sector: " << endsect << "\n";
						std::cout << "# of FATS: " << numOfFAT << "\n";
						std:: cout << "The size of each FAT: " << finaldec << "\n";
					}
				}
				part3++;
			}	
		}

		//------------------------------------//
		if(FS4 == 1){ //partition4
			
			if(loop >= place4*512 && loop <= (place4*512)+48){ //find entry point
			
				int decform = (int)(ch & 0x00FF);
				char hex [33];
				//itoa (decform,hex,16);

				sprintf(hex,"%x", decform);

				if(part4 == 0){
					printf("===================================================\n");
					std::cout << "Partition " << partionNo << "(" << FSname4 << "):\n";
					std::cout << "Reserved area: Start sector: 0 Ending sector: ";
					partionNo++;
				}

				if(part4 == 13){ //sector per cluster
					sectorspercluster = std::string(hex);
				}
				if(part4 >= 14 && part4 <=15){	//reserved area sector size
					if(std::string(hex).size() > 1){
						reservedsize4 = std::string(hex) + reservedsize4 ;
					}
					else
						reservedsize4  = "0" + std::string(hex) + reservedsize4;
				}
				if(part4 == 16){ //number of FATs
					numOfFAT = std::string(hex);
					int finaldec = getHexadecimal(reservedsize4); //convert hex to decimal
					startofFAT = finaldec;
					std::cout << finaldec-1 << " Size: " << finaldec << " sectors\n";
					std::cout << "Sectors per cluster: " <<  sectorspercluster << " sectors\n";
					std::cout << "Fat area: Start sector: " << startofFAT;
				}

				if (FSname4.find("16") != std::string::npos){ //size of each FAT
					if(part4 >= 22 && part4 <=23){	
						if(std::string(hex).size() > 1){
							sizeOfFAT4 = std::string(hex) + sizeOfFAT4;
						}
						else
							 sizeOfFAT4  = "0" + std::string(hex) +  sizeOfFAT4;
					}
				}
				else if (FSname4.find("32") != std::string::npos){
					if(part4 >= 36 && part4 <=39){	//reserved area sector size
						if(std::string(hex).size() > 1){
							sizeOfFAT4 = std::string(hex) + sizeOfFAT4;
						}
						else
							 sizeOfFAT4  = "0" + std::string(hex) +  sizeOfFAT4;
					}
				}

				if (FSname4.find("16") != std::string::npos){ 
					if(part4 == 24){	
						int finaldec = getHexadecimal(sizeOfFAT4); //convert hex to decimal
						int numoffat = atoi(numOfFAT.c_str());
						int endsect = (finaldec*numoffat + startofFAT) - 1;
						std::cout << " End sector: " << endsect << "\n";
						std::cout << "# of FATS: " << numOfFAT << "\n";
						std:: cout << "The size of each FAT: " << finaldec << "\n";
						
					}
				}
				else if (FSname4.find("32") != std::string::npos){
					if(part4 == 40){
						int finaldec = getHexadecimal(sizeOfFAT4); //convert hex to decimal
						int numoffat = atoi(numOfFAT.c_str());
						int endsect = (finaldec*numoffat + startofFAT) - 1;
						std::cout << " End sector: " << endsect << "\n";
						std::cout << "# of FATS: " << numOfFAT << "\n";
						std:: cout << "The size of each FAT: " << finaldec << "\n";
					}
				}
				part4++;
			}	
		}

		///////////////////////////////////End VBR

		if (ch <= 0)
		{
			strcat(buf, ".");
		}
		else if (isalnum(ch))
		{
			char tmp[2];
			tmp[0] = ch;
			tmp[1] = '\0';
			strcat(buf, tmp);
		}
		else
		{
			strcat(buf, ".");
		}
		loop++;
	}
	int len = strlen(buf);
	if (len > 0)
	{
		int spaces = 3 + 3 * (17 - len);
		if (len < 8)
		{
			spaces--;
		}

		int loop;

		for (loop = 0; loop < spaces; loop++)
		{	
			fputs(" ", out);
		}
		fputs(buf, out);
	}
}

std::string getFS(char* c)
{
    if(strcmp (c,"1") == 0)
        return "DOS 12-bit FAT";
	if(strcmp (c,"4") == 0)
        return "DOS 16-bit FAT for partitions smaller than 32 MB";
	if(strcmp (c,"5") == 0)
		return "Extended partition";
	if(strcmp (c,"6") == 0)
		return "DOS 16-bit FAT for partitions larger than 32 MB";
	if(strcmp (c,"7") == 0)
		return "NTFS";
	if(strcmp (c,"8") == 0)
		return "AIX bootable partition";
	if(strcmp (c,"9") == 0)
		return "AIX data partition";
	if(strcmp (c,"b") == 0 || strcmp (c,"B") == 0)
		return "DOS 32-bit FAT";
	if(strcmp (c,"c") == 0 || strcmp (c,"C") == 0)
		return "DOS 32-bit FAT for interrupt 13 support";
	if(strcmp (c,"17") == 0)
		return "Hidden NTFS partition (XP and earlier)";
	if(strcmp (c,"1B") == 0 || strcmp (c,"1b") == 0)
		return "Hidden FAT32 partition";
	if(strcmp (c,"1E") == 0 || strcmp (c,"1e") == 0)
		return "Hidden VFAT partition";
	if(strcmp (c,"3C") == 0 || strcmp (c,"3c") == 0)
		return "Partition Magic recovery partition";
	if(strcmp (c,"66") == 0)
		return "Novell partitions";
	if(strcmp (c,"67") == 0)
		return "Novell partitions";
	if(strcmp (c,"68") == 0)
		return "Novell partitions";
	if(strcmp (c,"69") == 0)
		return "Novell partitions";
	if(strcmp (c,"81") == 0)
		return "Linux";
	if(strcmp (c,"82") == 0)
		return "Linus swap partition (can also be associated with Solaris partitions)";
	if(strcmp (c,"83") == 0)
		return "Linux native file systems (Ext2, Ext3, Reiser, xiafs)";
	if(strcmp (c,"86") == 0)
		return "FAT 16 volume/stripe set (Windows NT)";
	if(strcmp (c,"87") == 0)
		return "High Performance File System (HPFS) fault-tolerant mirrored partition or NTFS volume/stripe set";
	if(strcmp (c,"A5") == 0 || strcmp (c,"a5") == 0)
		return "FreeBSD and BSD/386";
	if(strcmp (c,"A6") == 0 || strcmp (c,"a6") == 0)
		return "OpenBSD";
	if(strcmp (c,"A9") == 0 || strcmp (c,"a9") == 0)
		return "NetBSD";
	if(strcmp (c,"C7") == 0 || strcmp (c,"c7") == 0)
		return "Typical of a corrupted NTFS volume/stripe set";
	if(strcmp (c,"EB") == 0 || strcmp (c,"eb") == 0)
		return "BeOS";
	else
		return "unknown FS code";
}

int getHexadecimal(std::string hex){
	return (int)strtol(hex.c_str(), 0 , 16);
}

ostream *fileOutStream;
string filename = "", changedFileName = "", newFileName = "";
fstream fileInput;

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("No file specified\n");
		return 1;
	}
	
	OPTIONS options;
	defaultOptions(&options);
	
	int arg;
	for (arg = 1; arg < argc; arg++)
	{
		char *current = argv[arg];
		FILE *f = fopen(current, "rb");
		if (f == NULL)
		{
			fprintf(stdout, "Could not open file '%s'", current);
		}
		else
		{
			//Create MD5 and SHA1 file names
			arg++;

			filename = argv[arg];

			string changedFileName = "fciv " + filename + " -MD5 > " + filename + "-MD5.txt ";

			system(changedFileName.c_str()); //pass filename to the system

			changedFileName = "fciv " + filename + " -SHA1 > " + filename + "-SHA1.txt ";
			
			system(changedFileName.c_str());

			fileInput.open(filename, ios::in | ios::binary);

			//after opening the file name, open the new files
			newFileName = argv[arg];

			fileInput.open(newFileName, ios::out);
			fileOutStream = &fileInput;

			//catch error if the file is not open
			if (!fileInput.is_open()) 
			{
				cout << "Unable to open the file, " << endl;
				return 0;	
			}

			do_file(f, stdout, &options);
			fclose(f);
		}
	}

	return 0;
}



