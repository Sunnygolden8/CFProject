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
int calculatePhysicalAdd(string input3);
int calculateClusterAdd(string input2);
int calculateBytesAdd(string input);
int extractLogical(string input, int bytesOffset, char type, int t, int r, int k, int f);
int extractCluster(string input2, int bytesOffset, char type);
int extractPhysical(string input2, int bytesOffset, char type, int t, int r, int k, int f);

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

		//delete the "address4forensics" part of the string already found until the space part
		input.erase(0, input.find(" "));

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
	if ((pos = inputCommand.find(" ")) != string::npos)
	{
		/*Checks for given bytes or byte address*/
		//find the substring from the beginning position to the one that is before the space (Note: second param is count from the first param)
		//two conditions here: if a dash is found, or if the next position is also a dash Ex.[-b, -s, or --byte]
		if (inputCommand.substr(0, inputCommand.find(" ")) == "-L" || inputCommand.substr(0, inputCommand.find(" ")) == "--logical")
		{
			//delete the "-L or --logical" part of the string already found until the space part
			inputCommand.erase(0, input.find(" "));

			calculateLogicalAdd(inputCommand);
		}
		else if (inputCommand.substr(0, inputCommand.find(" ")) == "-P" || inputCommand.substr(0, inputCommand.find(" ")) == "--physical")
		{
			//delete the "-P or --physical" part of the string already found until the space part
			inputCommand.erase(0, input.find(" "));

			calculatePhysicalAdd(inputCommand);
		}
		else if (inputCommand.substr(0, inputCommand.find(" ")) == "-C" || inputCommand.substr(0, inputCommand.find(" ")) == "--cluster")
		{
			//delete the "-C or --cluster" part of the string already found until the space part
			inputCommand.erase(0, input.find(" "));

			calculateClusterAdd(inputCommand);
		}
		else
		{
			std::cout << "Please enter the right command. Ex. address4forensics -L|-P|-C [–b offset] [-B [-s bytes]] [-l address] [-p address] [-c address -k sectors -r sectors -t tables -f sectors]" << endl;
		}
		/*//if L, P, or C is not found but -b is found then checks if there is an accompanying sector number
		else if(inputCommand.find("-b") != string::npos)
		{
		string trimString = inputCommand.substr(inputCommand.at(pos));
		calculateBytesAdd(inputCommand);
		}*/
	}
}
//find the next few positions that contain certain commands
int calculateLogicalAdd(string input2)
{
	//differentiate alternative commands by boolean
	bool shortCommand, longCommand = false;
	std::size_t currentPos = 0; //get the current position of the string 
	int bytesOffset = 0; //byteoffsets data

	//values for Logical Address 
	int pos = 0;  //counter position of the char in the line

	string extractedByteOffset;

	//if we are converting to logical, then we must know the bits offset
	//Checks for given bytes or byte address
	//two conditions here: if a dash is found, or if the next position is also a dash Ex.[-b, -s, or --byte]		
	if (input2.substr(0, input2.find(" ")) == "-b" || (input2.substr(0, input2.find(" ")) == "--partition-start="))
	{
		if (input2.substr(0, input2.find(" ")) == "-b")
		{
			pos = input2.find(" ");

			//find the string next to the -b
			extractedByteOffset = input2.substr(pos, input2.find(" "));
		}
		else if (input2.substr(0, input2.find("=") + 1) == "--partition-start=")
		{
			pos = input2.find("=") + 1;

			//find the string next to the -b
			extractedByteOffset = input2.substr(pos, input2.find(" "));
		}
		try
		{
			//convert the string to integer
			bytesOffset = stoi(extractedByteOffset);
		}
		catch (int e) //integer exception
		{
			std::cout << "Please enter a number for the bytes offsets. Ex. [–b offset]" << endl;
			return 0;
		}

		//removes the bytes offset part from the input command to make extraction of data from string easy.
		input2.erase(pos, input2.find(" "));

		if (input2.substr(0, input2.find("=") + 1) == "--physical-known=" || input2.substr(0, input2.find(" ")) == "-p")
		{
			//thus, we can find out if there is further information for the conversion
			//if we found physical-known address, then compute the logical address
			return extractPhysical(input2, bytesOffset, 'l', 0, 0, 0, 0);
		}
		else if (input2.substr(0, input2.find("=") + 1) == "--cluster-known=" || input2.substr(0, input2.find(" ")) == "-c")
		{
			/*--Finding Cluster Address--*/
			return extractCluster(input2, bytesOffset, 'l');
		}

	}//end of getting data from input command

	return 0;
}//end of parsing data from input string

//calculate the physical address from the current information
int calculatePhysicalAdd(string input3)
{
	int bytesOffset = 0; //byteoffsets data

	//values for Logical Address 
	int pos = 0;  //counter position of the char in the line

	string extractedByteOffset;

	//if we are converting to logical, then we must know the bits offset
	//Checks for given bytes or byte address
	//two conditions here: if a dash is found, or if the next position is also a dash Ex.[-b, -s, or --byte]		
	if (input3.substr(0, input3.find(" ")) == "-b")
	{
		if (input3.substr(0, input3.find(" ")) == "-b")
		{
			pos = input3.find(" ");

			//find the string next to the -b
			extractedByteOffset = input3.substr(pos, input3.find(" "));
		}
		else if (input3.substr(0, input3.find("=") + 1) == "--partition-start=")
		{
			pos = input3.find("=") + 1;

			//find the string next to the -b
			extractedByteOffset = input3.substr(pos, input3.find(" "));
		}
		try
		{
			//convert the string to integer
			bytesOffset = stoi(extractedByteOffset);
		}
		catch (int e) //integer exception
		{
			std::cout << "Please enter a number for the bytes offsets. Ex. [–b offset]" << endl;
			return 0;
		}

		//removes the bytes offset part from the input command to make extraction of data from string easy.
		input3.erase(pos, input3.find(" "));

		/*--Finding logical Address--*/
		if (input3.substr(0, input3.find("=") + 1) == "--logical-known=" || input3.substr(0, input3.find(" ")) == "-l")
		{
			//thus, we can find out if there is further information for the conversion
			//if we found physical-known address, then compute the logical address
			return extractPhysical(input3, bytesOffset, 'p', 0, 0, 0, 0);
		}
		/*--Finding cluster Address--*/
		else if (input3.substr(0, input3.find("=") + 1) == "--cluster-known=" || input3.substr(0, input3.find(" ")) == "-c")
		{
			/*--Finding Cluster Address--*/
			return extractCluster(input3, bytesOffset, 'p');
		}

	}//end of getting data from input command

	return 0;
}//end of parsing data from input string


int calculateClusterAdd(string input2)
{
	std::size_t currentPos = 0; //get the current position of the string 
	int bytesOffset = 0; //byteoffsets data

	//values for Logical Address 
	int pos = 0;  //counter position of the char in the line

	string extractedByteOffset;

	//if we are converting to logical, then we must know the bits offset
	//Checks for given bytes or byte address
	//two conditions here: if a dash is found, or if the next position is also a dash Ex.[-b, -s, or --byte]		
	if (input2.substr(0, input2.find(" ")) == "-b" || (input2.substr(0, input2.find(" ")) == "--partition-start="))
	{
		if (input2.substr(0, input2.find(" ")) == "-b")
		{
			pos = input2.find(" ");

			//find the string next to the -b
			extractedByteOffset = input2.substr(pos, input2.find(" "));
		}
		else if (input2.substr(0, input2.find("=") + 1) == "--partition-start=")
		{
			pos = input2.find("=") + 1;

			//find the string next to the -b
			extractedByteOffset = input2.substr(pos, input2.find(" "));
		}
		try
		{
			//convert the string to integer
			bytesOffset = stoi(extractedByteOffset);
		}
		catch (int e) //integer exception
		{
			std::cout << "Please enter a number for the bytes offsets. Ex. [–b offset]" << endl;
			return 0;
		}

		//removes the bytes offset part from the input command to make extraction of data from string easy.
		input2.erase(pos, input2.find(" "));

		/*--Finding logical Address--*/
		if (input2.substr(0, input2.find("=") + 1) == "--logical-known=" || input2.substr(0, input2.find(" ")) == "-l")
		{
			//thus, we can find out if there is further information for the conversion
			//if we found physical-known address, then compute the logical address
			return extractPhysical(input2, bytesOffset, 'c', 0, 0, 0, 0);
		}
		else if (input2.substr(0, input2.find("=") + 1) == "--physical-known=" || input2.substr(0, input2.find(" ")) == "-p")
		{
			//thus, we can find out if there is further information for the conversion
			//if we found physical-known address, then compute the logical address
			return extractPhysical(input2, bytesOffset, 'c', 0, 0, 0, 0);
		}
		return 0;
	}
	int calculateBytesAdd(string input)
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
		return 0;
	}
	int extractLogical(string input, int bytesOffset, char type, /*for cluster info:*/ int t, int r, int k, int f)
	{
		int logicalAddress = 0, pos = 0;
		string extractedLogAdd;

		/*--Finding logical Address--*/
		if (input.substr(0, input.find("=") + 1) == "--logical-known=")
		{
			//thus, we can find out if there is further information for the conversion
			//if we found physical-known address, then compute the logical address
			pos = input.find("=") + 1;

			//find the string next to the -b
			extractedLogAdd = input.substr(pos, input.find(" "));
		}

		else if (input.substr(0, input.find(" ")) == "-l")
		{
			pos = input.find(" ");

			//find the string next to the -b
			extractedLogAdd = input.substr(pos, input.find(" "));
		}
		try
		{
			//convert the string to integer
			logicalAddress = stoi(extractedLogAdd);

			switch (type)
			{
			case 'c':
				return (((logicalAddress - (r + t * f)) / k) + 2);
				break;

				/*------Compute the physical address using the available logical information------*/
			case 'p':
				return logicalAddress + bytesOffset;
				break;

			default:
				std::cout << "Cannot convert from logical to logical. Enter another information." << endl;
				break;
			}
		}
		catch (int e) //integer exception
		{
			std::cout << "Please enter a number for the physical address offsets. Ex. [-p address] or --physical-known=address" << endl;
			return 0;
		}//end of extracting data from physical address

		return 0;
	}
	int extractPhysical(string input2, int bytesOffset, char type, /*for cluster info:*/ int t, int r, int k, int f)
	{
		int physicalAddress = 0;
		int pos = 0;

		/*--Finding Physical Address--*/
		string extractedPhyAdd;

		/*Find the next piece of information after getting the bytes offset*/
		//converting from physical to logical or cluster
		//extract string from position 0 to include also "=" sign
		if (input2.substr(0, input2.find("=") + 1) == "--physical-known=")
		{
			//thus, we can find out if there is further information for the conversion
			//if we found physical-known address, then compute the logical address
			pos = input2.find("=") + 1;

			//find the string next to the -b
			extractedPhyAdd = input2.substr(pos, input2.find(" "));
		}
		else if (input2.substr(0, input2.find(" ")) == "-p")
		{
			//thus, we can find out if there is further information for the conversion
			//if we found physical-known address, then compute the logical address
			pos = input2.find(" ");

			//find the string next to the -b
			extractedPhyAdd = input2.substr(pos, input2.find(" "));
		}
		try
		{
			//convert the string to integer
			physicalAddress = stoi(extractedPhyAdd);

			/*number from physical-known minus offset*/
			switch (type)
			{
			case 'c':
				return (((physicalAddress - bytesOffset) - (r + t * f)) / k) + 2;
				break;

				/*------Compute the logical address using the available physical information------*/
			case 'l':
				return physicalAddress - bytesOffset;
				break;

			default:
				std::cout << "Cannot convert from physical to physical. Enter another information." << endl;
				break;
			}
		}
		catch (int e) //integer exception
		{
			std::cout << "Please enter a number for the physical address offsets. Ex. [-p address] or --physical-known=address" << endl;
			return 0;
		}//end of extracting data from physical address

		return 0;
	}

	//extract cluster information and calculate it
	int extractCluster(string input2, int bytesOffset, char type)
	{
		int pos = 0;
		int clusterAddress = 0, clusterSize = 0, reservedSector = 0, fatTable = 0, fatLength = 0;

		string extractedClusterAdd;
		//we would need to know the cluster information to convert from cluster to logical or physical 
		//start of condition for extracting cluster address
		if (input2.substr(0, input2.find("=") + 1) == "--cluster-known=" || input2.substr(0, input2.find(" ")) == "-c")
		{
			if (input2.substr(0, input2.find("=") + 1) == "--cluster-known=")
			{
				//find the number next to "-c"
				pos = input2.find("=") + 1;

				//find the string next to the cluster address until the next space is found
				extractedClusterAdd = input2.substr(pos, input2.find(" "));
			}
			else if (input2.substr(0, input2.find(" ")) == "-c")
			{
				//find the number next to "-c"
				pos = input2.find(" ");

				//find the string next to the cluster address until the next space is found
				extractedClusterAdd = input2.substr(pos, input2.find(" "));
			}
			try
			{
				//convert the string to integer
				clusterAddress = stoi(extractedClusterAdd);
			}
			catch (int e) //integer exception
			{
				std::cout << "Please enter a number for the cluster address. Ex. -c number " << endl;
				return 0;
			}

			//removes the bytes offset part from the input command to make extraction of data from string easy.
			input2.erase(pos, input2.find(" "));

			/*--Finding Cluster Size--*/
			string extractedClusterSize;

			//find the next piece of information; that is -k
			if (input2.substr(0, input2.find("=") + 1) == "--cluster-size=")
			{
				//find the number next to "-c"
				pos = input2.find("=") + 1;

				//find the string next to the cluster address until the next space is found
				extractedClusterSize = input2.substr(pos, input2.find(" "));
			}
			else if (input2.substr(0, input2.find(" ")) == "-k")
			{
				//find the number next to "-c"
				pos = input2.find(" ");

				//find the string next to the cluster address until the next space is found
				extractedClusterSize = input2.substr(pos, input2.find(" "));
			}
			try
			{
				//convert the string to integer
				clusterSize = stoi(extractedClusterSize);
			}
			catch (int e) //integer exception
			{
				std::cout << "Please enter a number for the cluster size. Ex. -k number " << endl;
				return 0;
			}

			//removes the bytes offset part from the input command to make extraction of data from string easy.
			input2.erase(pos, input2.find(" "));

			/*--Finding Reserved Sector--*/
			string extractedReservedArea;

			if (input2.substr(0, input2.find("=") + 1) == "--reserved=")
			{
				pos = input2.find("=") + 1;

				//find the string next to the cluster address until the next space is found
				extractedReservedArea = input2.substr(pos, input2.find(" "));
			}
			else if (input2.substr(0, input2.find(" ")) == "-r")
			{
				pos = input2.find(" ");

				//find the string next to the cluster address until the next space is found
				extractedReservedArea = input2.substr(pos, input2.find(" "));
			}
			try
			{
				//convert the string to integer
				reservedSector = stoi(extractedReservedArea);
			}
			catch (int e) //integer exception
			{
				std::cout << "Please enter a number for the reserved area. Ex. -r number " << endl;
				return 0;
			}

			//removes the bytes offset part from the input command to make extraction of data from string easy.
			input2.erase(pos, input2.find(" "));

			/*--Finding the # of FAT Tables--*/
			string extractedFATTables;

			if (input2.substr(0, input2.find("=") + 1) == "--fat-tables=")
			{
				pos = input2.find("=") + 1;

				//find the fat Tables string next to the reserved sector until the next space is found
				extractedFATTables = input2.substr(pos, input2.find(" "));
			}
			else if (input2.substr(0, input2.find(" ")) == "-t")
			{
				pos = input2.find(" ");

				//find the fat Tables string next to the reserved sector until the next space is found
				extractedFATTables = input2.substr(pos, input2.find(" "));
			}
			try
			{
				//convert the string to integer
				fatTable = stoi(extractedFATTables);
			}
			catch (int e) //integer exception
			{
				std::cout << "Please enter a number for the fat Tables. Ex. -t number " << endl;
				return 0;
			}

			//removes the bytes offset part from the input command to make extraction of data from string easy.
			input2.erase(pos, input2.find(" "));

			/*--Finding the length of FAT Tables--*/
			string extractedFATLength;
			if (input2.substr(0, input2.find("=") + 1) == "--fat-length=")
			{
				pos = input2.find("=") + 1;

				//find the fat Tables string next to the reserved sector until the next space is found
				extractedFATLength = input2.substr(pos, input2.find(" "));
			}
			else if (input2.substr(0, input2.find(" ")) == "-f")
			{
				pos = input2.find(" ");

				//find the fat Tables string next to the reserved sector until the next space is found
				extractedFATLength = input2.substr(pos, input2.find(" "));
			}
			try
			{
				//convert the string to integer
				fatLength = stoi(extractedFATLength);
			}
			catch (int e) //integer exception
			{
				std::cout << "Please enter a number for the fat length. Ex. -f number " << endl;
				return 0;
			}


			switch (type)
			{
			case 'p':
				return ((clusterAddress - 2) * clusterSize) + (fatTable * fatLength) + bytesOffset + reservedSector;
				break;

				/*------Compute the logical address using the available cluster information------*/
			case 'l':
				return ((clusterAddress - 2)* clusterSize) + reservedSector + (fatTable * fatLength) - bytesOffset;
				break;

			default:
				std::cout << "Cannot convert from cluster to cluster. Enter another information." << endl;
				break;
			}

		}//end of getting data from cluster address

		//otherwise, ask for cluster information 
		else
		{
			std::cout << "Please enter the cluster information." << endl;
			return 0;
		}

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