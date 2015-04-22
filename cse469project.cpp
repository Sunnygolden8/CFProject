#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

//forward declarations

//address4Forensics functions
void addressParser(string input1);
int calculateLogicalAdd(string input2);
void calculatePhysicalAdd(string input3);
void calculateClusterAdd(string input4);
void calculateBytesAdd(string input);

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

	getline(cin, input); //get user input, store as string called input

	//if user input contains "mac_conversion" 
	if (input.find("mac_conversion") != std::string::npos) {
		mac_conversion();
	}
	//if user input contains "address4forensics" from position 0 to 20
	else if (input.find("address4forensics") != std::string::npos){

		//call address command parser here to get the right commands
		addressParser(input);
	}
	else
		std::cout << "Unknown Command" << std::endl;

	return 0;
}

/* ------------------------------------------------------------------------------ */
/* -------------------- ADDRESS4FORENSICS CONVERSION SECTION --------------------- */
/* ------------------------------------------------------------------------------ */

//finds valid commands from the input in the utility
void addressParser(string inputCommand)
{
	//values for Logical Address 
	int pos = 0;  //position of the char in the line

	//pos is not at end of the input line 
	while (pos < inputCommand.length())
	{
		/*Checks for given bytes or byte address*/
		//two conditions here: if a dash is found, or if the next position is also a dash Ex.[-b, -s, or --byte]
		if ((inputCommand.at(pos) == '-'  && inputCommand.at(pos + 1) != '-') || (inputCommand.at(pos) == '-' && inputCommand.at(pos + 1) == '-'))
		{
		}
		//must find input in the first two positions after the addressparser; that is, 21 to 22
		if (inputCommand.find("-L") != string::npos) {
			calculateLogicalAdd(inputCommand);
		}
		else if (inputCommand.find("-P") != string::npos) {
			calculatePhysicalAdd(inputCommand);
		}
		else if (inputCommand.find("-C") != string::npos) {
			calculateClusterAdd(inputCommand);
		}
		//if L, P, or C is not found but -b is found then checks if there is an accompanying sector number
		else if (inputCommand.find("-b") != string::npos)
		{
			string trimString = inputCommand.substr(inputCommand.at(pos));
			calculateBytesAdd(inputCommand);
		}
		else
		{
			std::cout << "Please enter the right command. Ex. address4forensics -L|-P|-C [–b offset] [-B [-s bytes]] [-l address] [-p address] [-c address -k sectors -r sectors -t tables -f sectors]" << endl;
		}
	}
}
//find the next few positions that contain certain commands
int calculateLogicalAdd(string input2)
{
	//differentiate alternative commands by boolean
	bool shortCommand, longCommand = false;
	std::size_t currentPos = 0; //get the current position of the string 
	int index = 0, holdNumber = 0;


	//values for Logical Address 
	/*int pos = 0;  //counter position of the char in the line

	//pos is not at end of the input line
	while (pos < inputComm.length())
	{
	//Checks for given bytes or byte address
	//two conditions here: if a dash is found, or if the next position is also a dash Ex.[-b, -s, or --byte]
	if( (inputComm.at(pos) == '-'  && inputComm.at(pos+1) == 'b'))
	{
	//get where "-b" is by finding the pos of b
	currentPos = input2.find("b");
	}
	else if ((inputComm.at(pos) == '-' && inputComm.at(pos+1) == '-') )
	{
	int tempIndex = pos + 1; //temporary counter variable

	//while the variable has not reached the end of the string
	while( tempIndex < input2.length)
	{
	//if a character is an alphabet, = sign, or dash
	if ((inputComm.at(tempIndex) >= 65 && inputComm.at(tempIndex) <= 90) || (inputComm.at(tempIndex) >= 97 && inputComm.at(tempIndex) <= 122) || equals a dash or = sign)
	{

	}
	else
	{
	break;
	}
	}
	//by the reference of the '-', get the substring of the input command that is equal to "partition-start="
	currentPos = input2.substr(pos+1, );
	}

	pos += 1; //increments the position of the line
	}
	*/
	//if we are converting to logical, then we must know the bits offset
	if (input2.find("-b"))
	{
		//get where "-b" is by finding the pos of b 
		currentPos = input2.find("b");
	}
	else if (input2.find("--partition-start="))
	{
		//get where "--partition-start=" is by finding the pos of = in the string 
		currentPos = input2.find("=");
	}
	else
	{
		std::cout << "Please include the byte offsets." << endl;
		return 0;
	}

	index = currentPos + 1;

	//find the number next to the commands by iterating through the string
	while (index < input2.length())
	{
		//assuming the next positions in the string is not a letter
		if (isalpha(index) == false)
		{
			holdNumber++; //keeping track of the subsequent positions that are numbers

		}
		index += 1;

	}
	//if holdNumber has been keeping track of integers
	if (holdNumber > 0)
	{
		input2.substr(currentPos, index);

		//thus, we can find out if there is further information for the conversion
		//if we found physical-known address, then compute the logical address
		if (input2.find("--physical-known="))
		{
			//find the number after the string
			/**/

			//then compute the logical address from the physical
			return /*number from physical-known minus offset*/;
		}
		//otherwise, we would need to know the cluster information to convert from cluster to logical 
		if (input.find("-c") || input.find("--cluster-known=")) //this would need to be more robust
		{
			//find the number next to "-c"

			//then use a temp variable to hold that number

			//find the next piece of information
			if (input.find("-k") || input.find("--cluster-size="))
			{
				//find the number next to "-c"

				//then use a temp variable to hold that number

				//find the next piece of information
				if (input.find("-r") || input.find("--reserved="))
				{
					//find the number next to "-c"

					//then use a temp variable to hold that number

					//find the next piece of information
					if (input.find("-t") || input.find("--fat-tables="))
					{
						//find the number next to "-c"

						//then use a temp variable to hold that number

						if (input.find("-f") || input.find("--fat-length="))
						{
							//find the number next to "-c"

							//then use a temp variable to hold that number

							//compute the logical address using the available information
						}
					}
				}
			}
			else
			{
				std::cout << "Please enter the cluster information." << endl;
				return 0;
			}
		}
	}
}
void calculatePhysicalAdd(string input)
{
}
void calculateClusterAdd(string input)
{
}
void calculateBytesAdd(string input)
{

	if (input.find("-B") != string::npos || input.find("--byte-address") != string::npos)
	{
		//find the next positions after the space
		if (input.find("-s") != string::npos || input.find("--sector-size=") != string::npos)
		{
			//find the next few positions for numbers after the space

			//LBA = (( C x HPC ) + H ) x SPT + S - 1
		}

	}
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
	}
	else {
		cout << "ERROR: The file does not exist.";

		exit(0);
	}

	//get hex from file
	ifstream myfile(filename.c_str());
	string line;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
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
	if (hexvalue.length() != 6){
		cout << "Hex value should be of format 0x1234";
		exit(0);
	}

	for (int i = 0; i<hexvalue.length(); i++)
	{
		if (i == 0){
			if (!(hexvalue[i] == '0')){
				cout << "Hex value should be of format 0x1234";
				exit(0);
			}
		}
		if (i == 1){
			if (!(hexvalue[1] == 'x')){
				cout << "Hex value should be of format 0x1234";
				exit(0);
			}
		}
	}
	//end format check

	//put hex value into char array
	for (int a = 0; a <= hexvalue.size(); a++)
	{
		temp[a] = hexvalue[a];
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
	for (int i = 0; i<sizeof(finalhex); ++i)
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
	hour = s.substr(0, 5); //5bits
	minute = s.substr(5, 6); //6bits
	second = s.substr(11); //5bits

	//get hour, second, minute
	finalhour = BinarytoDecimal(hour);
	finalsecond = BinarytoDecimal(second);
	finalminute = BinarytoDecimal(minute);

	if (finalhour > 11){
		if (finalhour != 12)
			finalhour = finalhour - 12;
		if (finalhour == 24)
			stamp = "AM";
		else
			stamp = "PM";
	}
	else
		stamp = "AM";

	cout << "Time: " << finalhour << ":" << finalminute << ":" << finalsecond * 2 << " " << stamp << endl;

}

void getDate(string s){
	string year;
	string month, finalmonth, finalYear;
	string day;
	int decimalMonth, finalDay, decimalYear;
	std::stringstream decyear;

	//get bits
	year = s.substr(0, 7); //7bits
	month = s.substr(7, 4); //4bits
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
	switch (toupper(c))
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
	switch (c)
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
	switch (c)
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