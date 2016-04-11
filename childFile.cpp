#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>

#ifdef __linux__
#define MASSIVESFILEPATH "/home/erick_vb/spo/lab1/massives.txt"
#endif

#ifdef _WIN32
#define MASSIVESFILEPATH "C:\spovmProjects\lab1\massives.txt"
#endif

#ifdef _WIN64
#define MASSIVESFILEPATH "C:\spovmProjects\lab1\massives.txt"
#endif

using namespace std;

const int  MASSIVELENGTH = 10;

int main(int argc, char* argv[])
{
	int massive[MASSIVELENGTH];
	fstream file;
	file.open(MASSIVESFILEPATH, ios::in);
	for (int j = 0; j < MASSIVELENGTH; j++)
	{
		file >> massive[j];
	}
	file.close();
	sort(massive, massive + MASSIVELENGTH);
	file.open(MASSIVESFILEPATH, ios::out | ios::trunc);
	for (int j = 0; j < MASSIVELENGTH; j++)
	{
		file << massive[j] << ' ';
	}
	file.close();
	return 0;
}



















/*#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

const int  MASSIVELENGTH = 10;

int main(int argc, char* argv[])
{
	int temp;
	int massive[MASSIVELENGTH];
	fstream file;	
	file.open("/home/erick_vb/spo/lab1/massives.txt", ios::in);
    for (int j = 0; j < MASSIVELENGTH; j++)
	{
		file >> massive[j];
	}
	file.close();
    sort(massive, massive + MASSIVELENGTH);
    file.open("/home/erick_vb/spo/lab1/massives.txt", ios::out | ios::trunc);
    for (int j = 0; j < MASSIVELENGTH; j++)
	{
		file << massive[j] << ' ';
	}
	file.close();
	return 0;
}*/
