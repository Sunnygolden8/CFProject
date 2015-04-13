#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

//forward declarations

//mac_conversion methods
void mac_conversion();
void time_conversion();
void date_conversion();
void file_conversion();
string hex_conversion();
string hexTobinary(char c);
void getDate(string binarystring);
void getTime(string binarystring);
int BinarytoDecimal(string s);
string decToMonth(int c);
string decToYear(char c);
//end mac_conversion methods

//global variables
std::string input; //holds user input

int main(int argc, char *argv[]){
	
	getline(cin,input); //get user input, store as string called input
	
	//if user input contains "mac_conversion" 
	if (input.find("mac_conversion") != std::string::npos) {
		mac_conversion();
	}
	//if user input contains "address4forensics"
	else if(input.find("address4forensics") != std::string::npos){

		//Christine, you can call your method here if you want to or just delete this

	}
	else
		std::cout << "Unknown Command" << std::endl;

	return 0;
}

/* ----------------------------------------------------------------- */
/* -------------------- MAC CONVERSION SECTION --------------------- */
/* ----------------------------------------------------------------- */

void mac_conversion(){
	if (input.find("-D") != std::string::npos) {
		date_conversion();
	}
	else if (input.find("-T") != std::string::npos) {
		time_conversion();
	}
	else
		std::cout << "ERROR: -D or -T expected" << std::endl;
}

void time_conversion(){
	string binarystring;

	if (input.find("-h") != std::string::npos) {
		binarystring = hex_conversion();
		getTime(binarystring);
	}
	else if (input.find("-f") != std::string::npos) {
		file_conversion();
		binarystring = hex_conversion();
		getTime(binarystring);
	}
	else
		std::cout << "ERROR: -h or -f expected" << std::endl;
}

void date_conversion(){
	string binarystring;

	if (input.find("-h") != std::string::npos) {
		binarystring = hex_conversion();
		getDate(binarystring);
	}
	else if (input.find("-f") != std::string::npos) {
		file_conversion();
		binarystring = hex_conversion();
		getDate(binarystring);
	}
	else
		std::cout << "ERROR: -h or -f expected" << std::endl;
}

void file_conversion(){
	size_t pos = 0;
	std::string filename = input;
	std::string delimiter = " ";

	//Extracting filename from input
	std::string token;
	while ((pos = filename.find(delimiter)) != std::string::npos) {
		token = filename.substr(0, pos);
		filename.erase(0, pos + delimiter.length());
	}

	//check if file exists
	FILE* fp = fopen(filename.c_str(), "r");
	
	if (fp) {
		fclose(fp);
	} else {
		cout << "ERROR: The file does not exist.";

		exit(0);
	}

	//get hex from file
	ifstream myfile (filename.c_str());
	string line;
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			input = line;
		}
			myfile.close();
	}

	else cout << "ERROR: Unable to open file."; 
}

string hex_conversion(){

	std::string hexvalue = input;
	std::string delimiter = " ";
	std::string binaryvals;
	char temp[1024];
	char finalhex[4];
	size_t pos = 0;

	//Extracting hex value from input
	std::string token;
	while ((pos = hexvalue.find(delimiter)) != std::string::npos) {
		token = hexvalue.substr(0, pos);
		hexvalue.erase(0, pos + delimiter.length());
	}

	//check if input is correct format
	if(hexvalue.length()!=6){
		cout << "Hex value should be of format 0x1234";
		exit(0);
	}

	for(int i=0;i<hexvalue.length();i++)
	{
		if(i==0){
			if(!(hexvalue[i]=='0')){
				cout << "Hex value should be of format 0x1234";
				exit(0);
			}
		}
		if(i==1){
			if(!(hexvalue[1]=='x')){
				cout << "Hex value should be of format 0x1234";
				exit(0);
			}
		}		
	}
	//end format check

	//put hex value into char array
	for (int a=0;a<=hexvalue.size();a++)
    {
		temp[a]=hexvalue[a];
    }
	
	//std::strncpy(temp, hexvalue.c_str(), sizeof(temp));
	//temp[sizeof(temp) - 1] = 0;

	//convert to little endian
	finalhex[2] = temp[2];
	finalhex[3] = temp[3];
	finalhex[0] = temp[4];
	finalhex[1] = temp[5];

	//debugging
	/*cout << "TEMP HEX IS: \n";
	for(int i=0 ; i<sizeof(temp) ; ++i)
	{
	  cout << temp[i];
	}

	cout << "\n\nFINAL HEX IS: \n";
	for(int i=0 ; i<sizeof(finalhex) ; ++i)
	{
	  cout << finalhex[i];
	}*/
	
	//convert hex to binary
	for(int i=0 ; i<sizeof(finalhex) ; ++i)
	{
	  string holder = hexTobinary(finalhex[i]);
	  binaryvals.append(holder);
	}

	return binaryvals;
}

void getTime(string s){
	string hour, second, minute, stamp;
	int finalhour, finalsecond, finalminute;

	//get bits
	hour = s.substr(0,5); //5bits
	minute = s.substr(5,6); //6bits
	second = s.substr(11); //5bits

	//get hour, second, minute
	finalhour = BinarytoDecimal(hour);
	finalsecond = BinarytoDecimal(second);
	finalminute = BinarytoDecimal(minute);

	if(finalhour > 11){
		if(finalhour != 12)
			finalhour = finalhour - 12;
		if(finalhour == 24)
			stamp = "AM";
		else
			stamp = "PM";
	}
	else
		stamp = "AM";
	
	cout << "Time: " << finalhour << ":" << finalminute << ":" << finalsecond*2 << " " << stamp << endl;

}

void getDate(string s){
	string year;
	string month, finalmonth, finalYear;
	string day;
	int decimalMonth, finalDay, decimalYear;
	std::stringstream decyear;
	
	//get bits
	year = s.substr(0,7); //7bits
	month = s.substr(7,4); //4bits
	day = s.substr(11); //5bits

	//get year
	decimalYear = BinarytoDecimal(year);
	
	decyear << decimalYear;
	string yeartemp = decyear.str();
	char first = yeartemp[0];
	finalYear = decToYear(first);
	finalYear.push_back(yeartemp[1]);
	
	//get month
	decimalMonth = BinarytoDecimal(month);
	finalmonth = decToMonth(decimalMonth);

	//get day
	finalDay = BinarytoDecimal(day);

	//print
	std::cout << "Date: " << finalmonth << " " << finalDay << ", " << finalYear << endl;
}

string hexTobinary(char c)
{
    switch(toupper(c))
    {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
    }
}

string decToMonth(int c)
{
    switch(c)
    {
        case 1: return "Jan";
        case 2: return "Feb";
		case 3: return "Mar";
		case 4: return "Apr";
		case 5: return "May";
		case 6: return "Jun";
		case 7: return "Jul";
		case 8: return "Aug";
		case 9: return "Sept";
		case 10: return "Oct";
		case 11: return "Nov";
		case 12: return "Dec";
    }
}

string decToYear(char c)
{
    switch(c)
    {
        case '1': return "199";
        case '2': return "200";
		case '3': return "201";
		case '4': return "202";
		case '5': return "203";
		case '6': return "204";
		case '7': return "205";
		case '8': return "206";
		case '9': return "207";
    }
}

int BinarytoDecimal(string s){ 

	long binary, decimal = 0, remainder, base = 1, num;
	int finaldecimal;
	 
	num = atol(s.c_str());
    binary = num;

    while (num > 0)
    {
        remainder = num % 10;
        decimal = decimal + remainder * base;
        base = base * 2;
        num = num / 10;
    }

	finaldecimal = (int)decimal;

    //cout << "The decimal equivalent of " << binary << " : " << finaldecimal << endl;

	return finaldecimal;
}