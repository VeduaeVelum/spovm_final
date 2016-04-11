#include "childFile.h"

int main(int argc, char* argv[])
{
	int *massive = NULL;
	massive = new int[MASSIVELENGTH];
	std::fstream file;
	massive = get_massive(massive, file);

	CHILD_TERM_ACTION

	if (massive == NULL)
	{
		std::cout << "File couldn't be opened for reading, \
					 program will terminate" << std::endl;
		return 1;
	}
	std::sort(massive, massive + MASSIVELENGTH);
	show_massive(massive);
	if (save_sort_massive(massive, file) == false)
	{
		std::cout << "File couldn't be opened for writing, \
					 program will terminate" << std::endl;
		return 1;
	}
	while (outFlag == false)
	{
		SLEEP(1);
	}
	return 0;
}

int* get_massive(int* massive, std::fstream& file)
{
	file.open(MASSIVESFILEPATH, std::ios::in);
	if (file.is_open() == false)
	{
		delete massive;
		return NULL;
	}
	for (int j = 0; j < MASSIVELENGTH; j++)
	{
		file >> massive[j];
	}
	file.close();
	return massive;
}
bool save_sort_massive(int* massive, std::fstream& file)
{
	file.open(MASSIVESFILEPATH, std::ios::out | std::ios::trunc);
	if (file.is_open() == false)
	{
		delete massive;
		return false;
	}
	for (int j = 0; j < MASSIVELENGTH; j++)
	{
		file << massive[j] << ' ';
	}
	file.close();
	delete massive;
	return true;
}
void show_massive(int* massive)
{
	for (int j = 0; j < MASSIVELENGTH; j++)
	{
		std::cout.width(NUMBERWIGHT);
		std::cout << massive[j];
	}
	std::cout << std::endl;
}