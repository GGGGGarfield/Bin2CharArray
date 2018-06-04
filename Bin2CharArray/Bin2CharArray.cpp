// Bin2CharArray.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

char path[1024];
char sizeStr[1024];
int sizeInt[100];
int sizeSum;
int arrayCount;
int fileSize;
ifstream inFile;
ofstream outFile;


void stop()
{
	inFile.close();
	outFile.close();
}

void getSizeInt()
{
	char *token;
	char *context;
	stringstream ss;

	token = strtok_s(sizeStr, " ,，", &context);
	arrayCount = 0;
	while (token != NULL)
	{
		ss << token;
		ss >> sizeInt[arrayCount++];
		token = strtok_s(NULL, " ,，", &context);
	}
	//delete token;
	//delete context;
}

bool isSizeIntError()
{
	int i = 0;
	sizeSum = 0;
	while (i < arrayCount)
	{
		if (sizeInt[i] <= 0)
			return 1;
		sizeSum += sizeInt[i++];
	}
	return 0;
}

void getFileSize()
{
	char c;
	fileSize = 0;
	while (true)
	{
		inFile.get(c);
		if (inFile.eof())
		{
			break;
		}
		fileSize++;
	}
}


bool cmpSize()
{
	if (sizeSum - fileSize > 0)
	{
		return 1;
	}
	return 0;
}

void storeArrayData()
{
	int arrayIndex = 0;
	int blockCount = 0;
	char chardata;
	unsigned char uchardata;
	for (arrayIndex = 0; arrayIndex < arrayCount; arrayIndex++)
	{
		outFile << "unsigned char dataArray" << arrayIndex << "[" << sizeInt[arrayIndex] << "] =" << endl;
		outFile << "{\n\t";
		blockCount = 0;
		while (blockCount < sizeInt[arrayIndex])
		{
			inFile.get(chardata);
			uchardata = (unsigned char)chardata;
			if (inFile.eof())
			{
				break;
			}
			outFile << "0x" << hex << uppercase << setfill('0') << setw(2) << uchardata << ",";
			blockCount++;
			if (blockCount % 16 == 0)
			{
				outFile << "\n\t";
			}
		}
		outFile << "\n}" << endl;
	}
}




int main()
{
	//Input file path
	cout << "Input file path:" << endl;
	cin.getline(path, 1024);
	//Input each array size
	cout << "Input each array size:(eg:720,100,500,400)" << endl;
	cin.getline(sizeStr, 1024);
	//get each array size in Int
	getSizeInt();
	if (isSizeIntError())
	{
		cout << "Error: Input each array size" << endl;
		stop();
		return 0;
	}
	//open file
	inFile.open(path, ifstream::in|ifstream::binary);
	if (inFile.fail())
	{
		cout << "Error: Fail to open file:" << path << endl;
		stop();
		return 0;
	}
	//get file size
	getFileSize();
	//compare with input size 
	if (cmpSize())
	{
		cout << "Error: SUM of each array size is larger than fileSize" << endl;
		stop();
		return 0;
	}
	
	//create new file to store array data
	strcat_s(path, ".txt");
	outFile.open(path, ofstream::out);
	if (outFile.fail())
	{
		cout << "Error: Fail to open file:" << path << endl;
		stop();
		return 0;
	}
	//flush the file
	outFile << "" << flush << endl;
	//store array data
	storeArrayData();
	//close file
	stop();
	system("pause");
    return 0;
}


