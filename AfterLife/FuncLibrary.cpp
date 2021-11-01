

#include "FuncLibrary.h"
#include <vector>
#include <fstream>

FString UFuncLibrary::MD5(FString To_MD5)
{
	string md5String = TCHAR_TO_ANSI(*To_MD5);

	return md5(md5String).c_str();
}

bool UFuncLibrary::DeleteFile(FString FullPathToFile)
{
	const char* C_FullPathToFile = TCHAR_TO_ANSI(*FullPathToFile);
	if (remove(C_FullPathToFile) == -1) return false;
	else		
	return true;
}


vector<string> StringSeperation(string Sourse, string divider)
{
	char  buf2[135], *ptr, *longCommand = &Sourse[0];
	vector<string> com;
	for (ptr = strtok(longCommand, divider.c_str()); ptr != NULL; ptr = strtok(NULL, divider.c_str()))
	{
		strcpy(buf2, ptr);
		com.push_back(ptr);
	}
	return com;
}

bool UFuncLibrary::LoadChar(FString Srt, FString & Name, FString & fraction, FString & money, FString & first_wpn, FString & Second_wpn, FString & grenade, FString & current_car, TArray<FString>& CharMods, TArray<FString>& CarMods, TArray<FString>& FirsWpnMod, TArray<FString>& Inventory)
{
	Srt.RemoveAt(0, 5, true);
	const char* C_Srt = TCHAR_TO_ANSI(*Srt);
	vector<string> Arr = StringSeperation(C_Srt, "/");
	string name_s = Arr[0].substr(0, Arr[0].find_last_of('.')), fraction_s = Arr[0].substr(Arr[0].find_last_of('.') + 1, Arr[0].size());
	string money_s = Arr[1].substr(Arr[1].find_last_of('=') + 1, Arr[1].size());

	vector<string> Sep = StringSeperation(Arr[2], "=");
	Sep = StringSeperation(Sep[1], ".");		// -------------   get all weapon
	if (Sep[1] == "\0") return false;
	string first, second, grenade_s, current_car_s;
	first = Sep[0];
	second = Sep[1];
	grenade_s = Sep[2];
	TArray<FString> Inventory_s;
	for (int i = 3; i < Sep.size(); i++)
	{
		if (Sep[0] != "\0")
			Inventory_s.Add(Sep[i].c_str());			// -------------   add to inv
		else
			return false;
	}
	Sep.clear();
	Sep = StringSeperation(Arr[3], "=");		// -------------   get all car
	Sep = StringSeperation(Sep[1], ".");
	if (Sep[1] == "\0") return false;
	current_car_s = Sep[0];
	for (int i = 1; i < Sep.size(); i++)
	{
		if (Sep[0] != "\0")
			Inventory_s.Add(Sep[i].c_str());			// -------------   add to inv
		else
			return false;
	}
	Sep.clear();
	Sep = StringSeperation(Arr[4], "=");		// -------------   get all clothes
	Sep = StringSeperation(Sep[1], ".");
	if (Sep[0] == "\0") return false;
	for (int i = 0; i < Sep.size(); i++)
	{
		if (Sep[0] != "\0")
			Inventory_s.Add(Sep[i].c_str());			// -------------   add to inv
		else
			return false;
	}
	TArray<FString> CharMods_s;
	TArray<FString> CarMods_s;
	TArray<FString> FirsWpnMod_s;
	Sep.clear();
	Sep = StringSeperation(Arr[5], "=");		// -------------   get mods
	Sep = StringSeperation(Sep[1], ".");
	if (Sep[0] == "\0") return false;
	CharMods_s.Add(Sep[0].c_str());
	CharMods_s.Add(Sep[1].c_str());
	CharMods_s.Add(Sep[2].c_str());
	CharMods_s.Add(Sep[3].c_str());
	CarMods_s.Add(Sep[4].c_str());
	FirsWpnMod_s.Add(Sep[5].c_str());

	for (int i = 6; i < Sep.size(); i++)
	{
		if (Sep[0] != "\0")
			Inventory_s.Add(Sep[i].c_str());			// -------------   add to inv
		else
			return false;
	}
	Name = name_s.c_str();
	fraction = fraction_s.c_str();
	money = money_s.c_str();
	first_wpn = first.c_str();
	Second_wpn = second.c_str();
	grenade = grenade_s.c_str();
	current_car = current_car_s.c_str();
	CharMods.Append(CharMods_s);
	CarMods.Append(CarMods_s);
	FirsWpnMod.Append(FirsWpnMod_s);
	Inventory.Append(Inventory_s);
	return true;
}

TArray<FString> UFuncLibrary::FStringSeperation(FString Sourse, FString divider)
{
	char* sSourse = TCHAR_TO_UTF8(*Sourse);
	char* sdivider = TCHAR_TO_UTF8(*divider);
	char  buf2[135], *ptr, *longCommand = &sSourse[0];
	TArray<FString> com;
	for (ptr = strtok(longCommand, sSourse); ptr != NULL; ptr = strtok(NULL, sdivider))
	{
		strcpy(buf2, ptr);
		com.Add(ptr);
	}
	return com;
}

FString UFuncLibrary::PNGtoByte(FString FullPathToFile)
{
	char* Path = TCHAR_TO_UTF8(*FullPathToFile);
	//string str;
	//stringstream s_tr;
	//FILE *fd;
	//size_t readed;
	//if (fopen_s(&fd, Path, "rb") == 0)
	//{		
	//	fseek(fd, 0, SEEK_END);				// устанавливаем позицию в конец файла
	//	long lSize = ftell(fd);				// получаем размер в байтах
	//	rewind(fd);						// Установить указатель в начало файла
	//	char * buffer = (char*)malloc(sizeof(char) * lSize);
	//	if (buffer == NULL) return "error! buffer == NULL";
	//	readed = fread(buffer, 1, lSize, fd);

	//	if (readed != lSize) return "error! readed != lSize";
	//	
	//	return buffer;
	//}
	//return "error read file";
	ifstream f(Path, ios::out | ios::binary);
	if (f.is_open())
	{
		string file, read;
		while (!f.eof())
		{
			std::getline(f,read);
			file.append(read);
		}
		f.close();
		return file.c_str();
	}

	return "ERROR";
}
