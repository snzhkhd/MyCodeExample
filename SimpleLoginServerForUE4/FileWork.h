#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <direct.h> 
#include <wininet.h>
#include <vector>
#include <direct.h>
#include "md5.h"

#pragma comment(lib, "Wininet")

using namespace std;

struct CharStr
{
	string Name;
	string Fraction;
};




const char* PlayerLogins = "/LoginServer/Player/Login/";

bool FileIsExist(string filePath)
{
	ifstream fin(filePath.c_str());

	if (fin.is_open())
	{
		return true;
		fin.close();
	}
	fin.close();
	return false;
}
//#define PlayerList = string(_getcwd(NULL, 0)+ PlayerLogins + "PlayerList.ini")

bool last_line(const string filename, string &Line)
{
	ifstream inClientFile(filename, ios::in);

	if (!inClientFile)
	{	
		return false;
	}

	string buf;

	while (getline(inClientFile, buf))
	{
		Line = buf;
	}
	return true;
}

string GetPlayerListPath()
{
	stringstream PlayerList;
	PlayerList << _getcwd(NULL, 0) << PlayerLogins << "PlayerList.ini";
	return PlayerList.str();
}
string GetAccountListPath()
{
	stringstream AccountList;
	AccountList << _getcwd(NULL, 0) << PlayerLogins << "AccountList.ini";
	return AccountList.str();
}


wstring s2ws(const string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
LPWSTR CharToLPWSTR(LPCSTR char_string)
{
	LPWSTR res;
	DWORD res_len = MultiByteToWideChar(1251, 0, char_string, -1, NULL, 0);
	res = (LPWSTR)GlobalAlloc(GPTR, (res_len + 1) * sizeof(WCHAR));
	MultiByteToWideChar(1251, 0, char_string, -1, res, res_len);
	return res;
}

bool SaveCharParam(string CharName, string Param, string Value);
string GetCharParam(string CharName, string Param);


bool AccountIsExists(string AccountName);
bool PlayerNameIsExists(string PlayerName);
bool PlayerLogin(string Login, string md5, string &Message);
bool CreateNewAccount(string AccountName, string Password);
bool FoundFreeCharSlot(string AccountName, string &PathToFile);
bool CreateNewCharacter(string AccountName, CharStr CharInfo);
bool FoundAndEditParam(string FilePath, string FoundParam, string Value);
bool DirectoryExists(wstring FileName);

bool SendPlayerInvetory(SOCKET sock, string FilePath);
bool SendPlayerWeapon(SOCKET sock, string FilePath);
bool SendPlayerCar(SOCKET sock, string FilePath);
bool SendPlayerClothes(SOCKET sock, string FilePath);
bool SendPlayerMod(SOCKET sock, string FilePath);
bool SendPlayerCustomization(SOCKET sock, string FilePath);

bool DeleteChar(string AccountName, string CharName);

void AddAccountInfo(string AccountName, string Param, string FileName);
void FoundAndAddParam(string AccountName, string ParamName, string Param);

string CreateEmptyChar(string AccountName, string CharName);
string CreateEmptyInventory(string AccountName, string CharName);
string CreateEmptyFrendList(string AccountName, string CharName);
string CreateEmptyCustomization(string AccountName, string CharName);
string CreateWeaponList(string AccountName, string CharName);
string CreateCarList(string AccountName, string CharName);
string CreateClothesList(string AccountName, string CharName);
string CreateModsList(string AccountName, string CharName);



string GetParamValue(string FilePath, string FoundParam);


bool DirectoryExists(wstring FileName)
{
	DWORD dwFileAttributes = GetFileAttributes(FileName.c_str());
	if (dwFileAttributes == 0xFFFFFFFF)
		return false;
	return dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}



vector<string> StringSeperation(string Sourse);
void SendCharInfo(SOCKET sock, string PlayerName);


bool PlayerLogin(string Login, string md5, string &Message)
{
	ifstream AccFile(GetAccountListPath());
	string ExistAcc;
	if (AccFile.is_open())
	{
		while (!AccFile.eof())
		{
			getline(AccFile, ExistAcc);

			if (ExistAcc.substr(0, ExistAcc.find_last_of(',')) == Login)
			{

				if (ExistAcc.substr(ExistAcc.find_last_of(',') + 1, ExistAcc.size()) == md5)
				{
					Message = "Login OK!";
					AccFile.close();
					return true;
				}
			}
		}
		Message = "incorrect data";
		AccFile.close();
		return false;
	}
	Message = "error 456";
	return false;
}

bool SaveCharParam(string CharName, string Param, string Value)
{
	ifstream NameFile(GetPlayerListPath());
	string ExistAcc;
	string PlayerPath;
	if (NameFile.is_open())
	{
		while (!NameFile.eof())
		{
			getline(NameFile, ExistAcc);

			if (ExistAcc.substr(0, ExistAcc.find_last_of(',')).c_str() == CharName)
			{
				NameFile.close();
				stringstream AccountList;
				string LoginName = ExistAcc.substr(ExistAcc.find_last_of(',') + 1, ExistAcc.size());
				AccountList << _getcwd(NULL, 0) << PlayerLogins << LoginName << "\\" << LoginName << ".ini";
				for (int i = 1; i != 5; i++)
				{
					string Chars = "char";
					Chars.append(to_string(i));
					PlayerPath = GetParamValue(AccountList.str(), Chars);
					if (GetParamValue(PlayerPath, "name") == CharName)
					{
						if (FoundAndEditParam(PlayerPath, Param, Value))
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}
inline string GetCharParam(string CharName, string Param)
{
	ifstream NameFile(GetPlayerListPath());
	string ExistAcc;
	string PlayerPath;
	if (NameFile.is_open())
	{
		while (!NameFile.eof())
		{
			getline(NameFile, ExistAcc);

			if (ExistAcc.substr(0, ExistAcc.find_last_of(',')).c_str() == CharName)
			{
				NameFile.close();
				stringstream AccountList;
				string LoginName = ExistAcc.substr(ExistAcc.find_last_of(',') + 1, ExistAcc.size());
				AccountList << _getcwd(NULL, 0) << PlayerLogins << LoginName << "\\" << LoginName << ".ini";
				for (int i = 1; i != 5; i++)
				{
					string Chars = "char";
					Chars.append(to_string(i));
					PlayerPath = GetParamValue(AccountList.str(), Chars);
					if (GetParamValue(PlayerPath, "name") == CharName)
					{						
							return GetParamValue(PlayerPath, Param);
					}
				}
			}
		}
	}
	return "\0";
}
//-------------------------------------------------------------------Exists function start
bool AccountIsExists(string AccountName)
{
	ifstream AccFile(GetAccountListPath());
	string ExistAcc;
	if (AccFile.is_open())
	{
		while (!AccFile.eof())
		{
			getline(AccFile, ExistAcc);
			
			if (ExistAcc.substr(0, ExistAcc.find_last_of(',')) == AccountName)
			{
				AccFile.close();
				return true;
			}		
		}
		AccFile.close();
		return false;
	}
	return false;
}

bool PlayerNameIsExists(string PlayerName)
{
	ifstream NameFile(GetPlayerListPath());
	string ExistAcc;
	if (NameFile.is_open())
	{
		while (!NameFile.eof())
		{
			getline(NameFile, ExistAcc);

			if (ExistAcc.substr(0, ExistAcc.find_last_of(',')) == PlayerName)
			{
				NameFile.close();
				return true;
			}
		}
		NameFile.close();
		return false;
	}
	return false;
}




//-------------------------------------------------------------------Exists function end

//-------------------------------------------------------------------Create New Account start
bool CreateNewAccount(string AccountName, string Password)
{
	if (AccountIsExists(AccountName))
	{
		return false;
	}
	stringstream filename;
	stringstream FolderName;
	FolderName << _getcwd(NULL, 0) << PlayerLogins << AccountName;

	if (!DirectoryExists(s2ws(FolderName.str().c_str()).c_str()))
	{	
		_mkdir(FolderName.str().c_str());
	}
	else
	{
		cout << FolderName.str();
		return false;
	}
	
	filename << FolderName.str().c_str() <<	"\\" << AccountName << ".ini";
	ofstream out(filename.str());
	ofstream AccList(GetAccountListPath(), ios::out | ios::app);
	if (out.is_open() || AccList.is_open())
	{
		
		stringstream tmp;	
		tmp << "[" << AccountName << "]" ; 
		out << tmp.str();
		tmp.str().clear();	
		out.close();
		AddAccountInfo(AccountName, "--------------------------", AccountName);
		for (int i = 4;i != 0;i--)
		{
			
			string char_name = "char";
			char_name.append(to_string(i));
			string param = char_name;
			param.append("=");			
			string char_path = CreateEmptyChar(AccountName, char_name);
			param.append(char_path);
			AddAccountInfo(AccountName, param, AccountName);
		}
		AddAccountInfo(AccountName, "--------------------------", AccountName);
		AddAccountInfo(AccountName, "Threat=green", AccountName);
		AddAccountInfo(AccountName, "bann=false", AccountName);

		string AccInf = "pass=";		
		AccInf.append(Password);
		string pass = AccInf;
		AddAccountInfo(AccountName, AccInf, AccountName);		
		AccInf.clear();

		AccInf = "Login=";		
		AccInf.append(AccountName);
		AddAccountInfo(AccountName, AccInf, AccountName);
		AccInf.clear();

		AccInf = "md5=";
		AccInf.append(md5(Password));
		AddAccountInfo(AccountName, AccInf, AccountName);
				
		AddAccountInfo(AccountName, "CurrectChar=", AccountName);

		AccList << AccountName << "," << md5(Password) << endl;
		AccList.close();
		
		cout << "new acc created" << endl;
		return true;
	}
	return false;
}

//------------------------------------------------------------------Create New Account end
bool FoundFreeCharSlot(string AccountName, string &PathToFile)
{
	stringstream AccountFile;
	AccountFile << _getcwd(NULL, 0) << PlayerLogins << AccountName << "\\" << AccountName << ".ini";
	string name;
	for (int i = 1;i != 5; i++)
	{
		name = "char";
		name.append(to_string(i));
		if (GetParamValue(GetParamValue(AccountFile.str(), name), "name") == "\0")
		{
			PathToFile = GetParamValue(AccountFile.str(), name);
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------------Create New Char start
bool CreateNewCharacter(string AccountName, CharStr CharInfo)
{
	if (PlayerNameIsExists(CharInfo.Name))
	{
		return false;
	}
	stringstream AccountFile;
//	stringstream PlayerList;
	AccountFile << _getcwd(NULL, 0) << PlayerLogins << AccountName << "\\" << AccountName << ".ini";
//	PlayerList << _getcwd(NULL, 0) << PlayerLogins << "PlayerList.ini";

	ofstream CharList(GetPlayerListPath(), ios::out | ios::app);
	if (!CharList.is_open())
	{
		return false;
	}
	CharList << CharInfo.Name  << "," << AccountName  << endl;
	CharList.close();
	string name;
	string freeSlot;
	if (FoundFreeCharSlot(AccountName, freeSlot))
	{
		if (freeSlot != "\0")
		{
			
			FoundAndEditParam(freeSlot, "name", CharInfo.Name);
			FoundAndEditParam(freeSlot, "fraction", CharInfo.Fraction);

			FoundAndEditParam(AccountFile.str(), "CurrectChar", freeSlot);
			return true;
		}
		return false;
	}
	
	cout << "free slot not found" << endl;
	return false;
}
//------------------------------------------------------------------Create New Char end


//------------------------------------------------------------------FOUND AND SET PARAM FUNCTION START
void FoundAndAddParam(string AccountName, string ParamName, string Param)
{
	stringstream filename;
	stringstream ParamName_str;
	ParamName_str << "[" << ParamName << "]";
	filename << _getcwd(NULL, 0) << PlayerLogins << AccountName << ".ini";

	ifstream ifs(filename.str().c_str());
	if (!ifs)
	{
		cout << "Error. Can`t open file " << filename.str().c_str() << '\n';
	}
	vector<string> Vec;
	int cnt = 0;
	while (!ifs.eof())
	{
		string tmp;
		ifs >> tmp;
		if (tmp == ParamName_str.str())
		{
			Vec.push_back(tmp);
			tmp.clear();
			tmp = Param;
			++cnt;
		}
		Vec.push_back(tmp);
	}
	ofstream ofs(filename.str().c_str());
	for (vector<std::string>::const_iterator it = Vec.begin(); it != Vec.end(); ++it)
	{
		ofs << *it;
		ofs << '\n';
	}

	ofs.close();
	ifs.close();
}
//------------------------------------------------------------------FOUND AND SET PARAM FUNCTION END

void AddAccountInfo(string AccountName,string Param, string FileName)
{
	stringstream filename;
	stringstream AccountName_str;
	AccountName_str << "[" <<AccountName<< "]";
	filename << _getcwd(NULL, 0) << PlayerLogins << AccountName <<"\\"<< FileName << ".ini";
	
	ifstream ifs(filename.str().c_str());
	if (!ifs)
	{
		cout << "Error. Can`t open file " << filename.str().c_str() << '\n';
	}	
	vector<string> Vec;
	int cnt = 0;
	while (!ifs.eof())
	{
		string tmp;
		ifs >> tmp;
		if (tmp == AccountName_str.str())
		{
			Vec.push_back(tmp);
		//	tmp.clear();
			tmp = Param;
			++cnt;
		}
		Vec.push_back(tmp);
	}
	ofstream ofs(filename.str().c_str());
	for (vector<std::string>::const_iterator it = Vec.begin(); it != Vec.end(); ++it)
	{
		ofs << *it;
		ofs << '\n';
	}
	
	ofs.close();
	ifs.close();
}
bool FoundAndEditParam(string FilePath, string FoundParam, string Value)
{
	stringstream s_param;
	s_param << FoundParam << "=";
	ifstream ifs(FilePath);
	bool result = false;
	if (!ifs)
	{
		return false;
	}
	vector<string> Vec;
	int cnt = 0;
	while (!ifs.eof())
	{
		string tmp;
		ifs >> tmp;
		int pos = tmp.find_last_of('=');
		if (tmp.substr(0, pos).c_str() == FoundParam)
		{
			s_param << Value;
			tmp = s_param.str();
			++cnt;
			result = true;
		}
		Vec.push_back(tmp);
	}
	if (!result) return false;
	ofstream ofs(FilePath);
	for (vector<std::string>::const_iterator it = Vec.begin(); it != Vec.end(); ++it)
	{
		ofs << *it;
		ofs << '\n';
	}
	ofs.close();
	ifs.close();
	return result;
}
//-------------------------------------------------------------------get function start
string GetParamValue(string FilePath, string FoundParam)
{
	ifstream ifs(FilePath);
	if (!ifs)
	{
		return "";
	}
	while (!ifs.eof())
	{
		string tmp;
		ifs >> tmp;
		int pos = tmp.find_last_of('=');
		if (tmp.substr(0, pos).c_str() == FoundParam)
		{
			ifs.close();
			return tmp.substr(pos + 1, tmp.length()).c_str();
		}
	}
	ifs.close();
	return 	"";
}
//-------------------------------------------------------------------get function end
string CreateEmptyChar(string AccountName,string CharName)
{	
	stringstream filePath;
	filePath << _getcwd(NULL, 0) << PlayerLogins << AccountName << "\\" << CharName;
	_mkdir(filePath.str().c_str());
	stringstream filename;
	filename << filePath.str() <<"\\"<< CharName << ".ini";
	ofstream out(filename.str());
	out << "name=" << endl;
	out << "fraction=" << endl;
	out << CreateEmptyInventory(AccountName, CharName)<<endl;
	out << CreateEmptyFrendList(AccountName, CharName) << endl;
	out << CreateEmptyCustomization(AccountName,CharName) << endl;
	out << "param4=" << endl;
	out << "param5=" << endl;
	out << "param6=" << endl;
	out.close();	
	return filename.str();
}
string CreateEmptyInventory(string AccountName, string CharName)
{
	stringstream filename;
	filename << _getcwd(NULL, 0) << PlayerLogins << AccountName << "\\" << CharName<< "\\" << "inventory"<< ".ini";
	ofstream out(filename.str());
	out << CreateWeaponList(AccountName,CharName) << endl;
	out << CreateCarList(AccountName, CharName) << endl;
	out << CreateClothesList(AccountName, CharName) << endl;
	out << CreateModsList(AccountName, CharName) << endl;
	out << "param2=" << endl;
	out << "param3=" << endl;
	out << "param4=" << endl;
	out << "param5=" << endl;
	out << "MONEY=" << endl;
	out.close();
	string param;
	param.append("Inventory=");
	param.append(filename.str());
	return param;
}
string CreateEmptyFrendList(string AccountName, string CharName)
{
	stringstream filename;
	filename << _getcwd(NULL, 0) << PlayerLogins << AccountName << "\\" << CharName << "\\" << "FrendList" << ".ini";
	ofstream out(filename.str());
	out << "[Frend]" << endl;
	out << "[Ignore]" << endl;
	out.close();
	string param;
	param.append("FrendList=");
	param.append(filename.str());
	return param;
}
string CreateEmptyCustomization(string AccountName, string CharName)
{
	stringstream filename;
	filename << _getcwd(NULL, 0) << PlayerLogins << AccountName << "\\" << CharName << "\\" << "Customization" << ".ini";
	ofstream out(filename.str());
	out << "Hair=Default" << endl;
	out << "Head=Default" << endl;
	out << "Body=Default" << endl;
	out.close();
	string param;
	param.append("Customization=");
	param.append(filename.str());
	return param;
}

inline string CreateWeaponList(string AccountName, string CharName)
{
	stringstream filename;
	filename << _getcwd(NULL, 0) << PlayerLogins << AccountName << "\\" << CharName << "\\" << "WEAPON" << ".ini";
	ofstream out(filename.str());
	out << "first_wpn=0" << endl;
	out << "second_wpn=1" << endl;
	out << "grenade=2" << endl;
	out << "0=none" << endl;
	out << "1=none" << endl;
	out << "2=none" << endl;
	out << "3=none" << endl;
	out << "4=none" << endl;
	out << "5=none" << endl;
	out.close();
	string param;
	param.append("WEAPON=");
	param.append(filename.str());
	return param;
}

inline string CreateCarList(string AccountName, string CharName)
{
	stringstream filename;
	filename << _getcwd(NULL, 0) << PlayerLogins << AccountName << "\\" << CharName << "\\" << "CAR" << ".ini";
	ofstream out(filename.str());
	out << "current_car=0" << endl;
	out << "0=none" << endl;
	out << "1=none" << endl;
	out << "2=none" << endl;
	out << "3=none" << endl;
	out << "4=none" << endl;
	out << "5=none" << endl;
	out.close();
	string param;
	param.append("CAR=");
	param.append(filename.str());
	return param;
}

inline string CreateClothesList(string AccountName, string CharName)
{
	stringstream filename;
	filename << _getcwd(NULL, 0) << PlayerLogins << AccountName << "\\" << CharName << "\\" << "CLOTHES" << ".ini";
	ofstream out(filename.str());
	out << "head=0" << endl;
	out << "neck=0" << endl;
	out << "body=0" << endl;
	out << "l_foot=0" << endl;
	out << "r_foot=0" << endl;
	out << "l_knee=0" << endl;
	out << "r_knee=0" << endl;
	out << "l_leg=0" << endl;
	out << "r_leg=0" << endl;	
	out << "hair=0" << endl;
	out << "jaw=0" << endl;	
	out << "0=none" << endl;
	out << "1=none" << endl;
	out << "2=none" << endl;
	out << "3=none" << endl;
	out << "4=none" << endl;
	out << "5=none" << endl;
	out.close();
	string param;
	param.append("CLOTHES=");
	param.append(filename.str());
	return param;
}

inline string CreateModsList(string AccountName, string CharName)
{
	stringstream filename;
	filename << _getcwd(NULL, 0) << PlayerLogins << AccountName << "\\" << CharName << "\\" << "MODS" << ".ini";
	ofstream out(filename.str());
	out << "char_mod1=0" << endl;
	out << "char_mod2=0" << endl;
	out << "char_mod3=0" << endl;
	out << "char_mod4=0" << endl;
	out << "current_car=0" << endl;
	out << "first_wpn=0" << endl;
	out << "second_wpn=0" << endl;
	out << "grenade=0" << endl;
	out << "0=none" << endl;
	out << "1=none" << endl;
	out << "2=none" << endl;
	out << "3=none" << endl;
	out << "4=none" << endl;
	out << "5=none" << endl;
	out.close();
	string param;
	param.append("MODS=");
	param.append(filename.str());
	return param;
}
//-------------------------------------------------------------------Help function start
vector<string> StringSeperation(string Sourse)
{
	char  buf2[135], *ptr, * longCommand = &Sourse[0];
	vector<string> com;
	for (ptr = strtok(longCommand, "."); ptr != NULL; ptr = strtok(NULL, "."))
	{
		strcpy(buf2, ptr);
		com.push_back(ptr);
	}
	return com;
}



//-------------------------------------------------------------------Send char info  start
void SendCharInfo(SOCKET sock, string PlayerName)
{
	if (send(sock, "-load", sizeof("-load"), 0) == SOCKET_ERROR) return;
	
	ifstream NameFile(GetPlayerListPath());
	string ExistAcc;
	string PlayerPath;
	if (NameFile.is_open())
	{
		while (!NameFile.eof())
		{
			getline(NameFile, ExistAcc);
			
			if (ExistAcc.substr(0, ExistAcc.find_last_of(',')).c_str() == PlayerName)
			{
				
				NameFile.close();

				stringstream AccountList;
				string LoginName = ExistAcc.substr(ExistAcc.find_last_of(',')+1, ExistAcc.size());
				AccountList << _getcwd(NULL, 0) << PlayerLogins << LoginName << "\\" << LoginName << ".ini";
				
				for (int i = 1; i != 5; i++)
				{
					string Chars = "char";
					Chars.append(to_string(i));
				//	NameFile.open(GetParamValue(AccountList.str(), Chars));
					PlayerPath = GetParamValue(AccountList.str(), Chars);
				
					if (GetParamValue(PlayerPath, "name") == PlayerName)
					{
						string sendStr = GetParamValue(PlayerPath, "name");
						if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
						{
							send(sock, ".", 1, 0);
							string sendStr = GetParamValue(PlayerPath, "fraction");
							if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
							{
								if (SendPlayerInvetory(sock, PlayerPath))
								{
									if (SendPlayerCustomization(sock, PlayerPath))
									{
										cout << " Char Info send success"<< endl;
									}
								}
							}
							
							return;
						}	
						return;
					}
				}
			}
		}
		NameFile.close();
		cout << PlayerName << " NOT FOUND!     " << ExistAcc << endl;
		return;	
	}
	cout << "File is not open!" << endl;
	return;
}


//-------------------------------------------------------------------Send inventory  start
bool SendPlayerInvetory(SOCKET sock, string FilePath)
{
	string sendStr;
	send(sock, "/money=", 7, 0);
	sendStr = (GetParamValue(GetParamValue(FilePath, "Inventory"), "MONEY"));
	if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
	{
		if (SendPlayerWeapon(sock, FilePath))
		{
			if (SendPlayerCar(sock, FilePath))
			{
				if (SendPlayerClothes(sock, FilePath))
				{
					if (SendPlayerMod(sock, FilePath))
					{
						return true;
					}
				}
			}

		}
	}

	return false;
}
bool SendPlayerWeapon(SOCKET sock, string FilePath)
{
	send(sock, "/weapon=", 8, 0);
	string sendStr;
	sendStr = GetParamValue(GetParamValue(GetParamValue(FilePath, "Inventory"), "WEAPON"), "first_wpn");
	if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
	{
		send(sock, ".", 1, 0);
		sendStr = GetParamValue(GetParamValue(GetParamValue(FilePath, "Inventory"), "WEAPON"), "second_wpn");
		if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
		{
			send(sock, ".", 1, 0);
			sendStr = GetParamValue(GetParamValue(GetParamValue(FilePath, "Inventory"), "WEAPON"), "grenade");
			if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
			{
				send(sock, ".", 1, 0);
				if (last_line(GetParamValue(GetParamValue(FilePath, "Inventory"), "WEAPON"),sendStr))
				{
					
					int Wpn_count =  atoi(sendStr.substr(0, sendStr.find_last_of('=')).c_str());
					for (int i = Wpn_count; i != -1; i--)
					{

						sendStr = GetParamValue(GetParamValue(GetParamValue(FilePath, "Inventory"), "WEAPON"), to_string(i));
						if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
						{
							if(i != 0)
								send(sock, ".", 1, 0);
						}
					}
					return true;
				}
				
			}
		}
	}
	return false;
}
bool SendPlayerCar(SOCKET sock, string FilePath)
{
	send(sock, "/car=", 5, 0);
	string sendStr;
	sendStr = GetParamValue(GetParamValue(GetParamValue(FilePath, "Inventory"), "CAR"), "current_car");
	if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
	{
		send(sock, ".", 1, 0);		
		if (last_line(GetParamValue(GetParamValue(FilePath, "Inventory"), "CAR"), sendStr))
		{
			int car_count = atoi(sendStr.substr(0, sendStr.find_last_of('=')).c_str());
			for (int i = car_count; i != -1; i--)
			{
				sendStr = GetParamValue(GetParamValue(GetParamValue(FilePath, "Inventory"), "CAR"), to_string(i));
				if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
				{
					if (i != 0)
						send(sock, ".", 1, 0);
				}
			}
			return true;
		}

			
		
	}
	return false;
}
bool SendPlayerClothes(SOCKET sock, string FilePath)
{
	send(sock, "/clothes=", 9, 0);
	string sendStr;
/*	sendStr = GetParamValue(GetParamValue(GetParamValue(FilePath, "Inventory"), "CLOTHES"), "current_car");
	if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
	{
		send(sock, ".", 1, 0);*/
		if (last_line(GetParamValue(GetParamValue(FilePath, "Inventory"), "CLOTHES"), sendStr))
		{
			int Clot_count = atoi(sendStr.substr(0, sendStr.find_last_of('=')).c_str());
			for (int i = Clot_count; i != -1; i--)
			{
				sendStr = GetParamValue(GetParamValue(GetParamValue(FilePath, "Inventory"), "CLOTHES"), to_string(i));
				if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
				{
					if (i != 0)
						send(sock, ".", 1, 0);
				}
			}
			return true;
		}
//	}
	return false;
}
bool SendPlayerMod(SOCKET sock, string FilePath)
{
	send(sock, "/mods=", 6, 0);
	string sendStr;
	sendStr = GetParamValue(GetParamValue(GetParamValue(FilePath, "Inventory"), "MODS"), "char_mod1");
	if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
	{
		send(sock, ".", 1, 0);
		sendStr = GetParamValue(GetParamValue(GetParamValue(FilePath, "Inventory"), "MODS"), "char_mod2");
		if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
		{
			send(sock, ".", 1, 0);
			sendStr = GetParamValue(GetParamValue(GetParamValue(FilePath, "Inventory"), "MODS"), "char_mod3");
			if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
			{
				send(sock, ".", 1, 0);
				sendStr = GetParamValue(GetParamValue(GetParamValue(FilePath, "Inventory"), "MODS"), "char_mod4");
				if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
				{
					send(sock, ".", 1, 0);
					sendStr = GetParamValue(GetParamValue(GetParamValue(FilePath, "Inventory"), "MODS"), "current_car");
					if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
					{
						send(sock, ".", 1, 0);
						sendStr = GetParamValue(GetParamValue(GetParamValue(FilePath, "Inventory"), "MODS"), "first_wpn");
						if (last_line(GetParamValue(GetParamValue(FilePath, "Inventory"), "MODS"), sendStr))
						{
							int mod_count = atoi(sendStr.substr(0, sendStr.find_last_of('=')).c_str());
							for (int i = mod_count; i != -1; i--)
							{
								sendStr = GetParamValue(GetParamValue(GetParamValue(FilePath, "Inventory"), "MODS"), to_string(i));
								if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
								{
									if (i != 0)
										send(sock, ".", 1, 0);
								}
							}
							return true;
						}
					}
				}

			}

		}
	}
	return false;
}
//-------------------------------------------------------------------Send inventory  end

bool SendPlayerCustomization(SOCKET sock, string FilePath)
{
	send(sock, "/cstmz=", sizeof("/cstmz="), 0);
	string sendStr;
	sendStr = GetParamValue(GetParamValue(FilePath,"Customization"), "Hair");
	cout << sendStr << endl;
	if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
	{
		send(sock, ".", 1, 0);
		sendStr = GetParamValue(GetParamValue(FilePath, "Customization"), "Head");
		if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
		{
			send(sock, ".", 1, 0);
			sendStr = GetParamValue(GetParamValue(FilePath, "Customization"), "Body");
			if (send(sock, sendStr.c_str(), sendStr.length(), 0) != SOCKET_ERROR)
			{
				return true;
			}
		}
	}
	return false;
}

bool DeleteChar(string AccountName, string CharName)
{
	stringstream filename;
	string Path;
	filename << _getcwd(NULL, 0) << PlayerLogins << AccountName << "\\" << AccountName << ".ini";
	for (int i = 1; i != 5; i++)
	{
		string Chars = "char";
		Chars.append(to_string(i));	
		Path = GetParamValue(filename.str(), Chars);
		if (GetParamValue(GetParamValue(filename.str(), Chars), "name") == CharName)
		{
			CreateEmptyChar(AccountName, Chars);
			if (GetParamValue(filename.str(), "CurrectChar") == Path)
			{
				if (FoundAndEditParam(filename.str(), "CurrectChar", ""))
				{
					ifstream ifs(GetPlayerListPath());
					bool result = false;
					if (!ifs)
					{
						return false;
					}
					vector<string> Vec;
					int cnt = 0;
					while (!ifs.eof())
					{
						string tmp;
						ifs >> tmp;
						int pos = tmp.find_last_of(',');
						if (tmp.substr(0, pos).c_str() == CharName)
						{
							tmp = "\0";
							++cnt;
							result = true;
						}
						
						Vec.push_back(tmp);
					}
					if (!result) return false;
					ofstream ofs(GetPlayerListPath());
					for (vector<std::string>::const_iterator it = Vec.begin(); it != Vec.end(); ++it)
					{
						ofs << *it;
						ofs << '\n';
					}
					ofs.close();
					ifs.close();
					return result;
				}
			}	
		}
		
	}
//	CreateEmptyChar(AccountName,CharName);
	return false;
}

//-------------------------------------------------------------------Send char info  end




//Рекурсивное удаление каталога
//BOOL DeleteFolder(LPCSTR szPath)
//{
//	WIN32_FIND_DATA FindFileData;
//	HANDLE hFind = INVALID_HANDLE_VALUE;
//	DWORD dwError, dwAttrs;
//	BOOL bRes;
//	int nLength;
//
//	LPCWSTR dir;
//	dir = s2ws(szPath).c_str();
//	char cPath[MAX_PATH], cCurrentFile[MAX_PATH];
////	LPWSTR l_Path, l_CurrentFile;
////	l_Path = s2ws(cPath).c_str();
////	l_CurrentFile = s2ws(cCurrentFile).c_str();
//
//
//	if (szPath == NULL)
//		return FALSE;
//
//	if (lstrcmpi(dir + 1, TEXT(":")) == 0 || szPath[0] == '\\' || szPath[0] == '\0')
//		return FALSE;
//
//	//Объекта уже нет
//	dwAttrs = GetFileAttributes(dir);
//	if (dwAttrs == INVALID_FILE_ATTRIBUTES)
//		return TRUE;
//
//	//Если объект не директория 
//	if (~dwAttrs & FILE_ATTRIBUTE_DIRECTORY)
//		return FALSE;
//
//	SetLastError(0);
//
//	//Пробуем сразу удалить директорию
//	bRes = RemoveDirectory(dir);
//	if (bRes == TRUE)
//		return TRUE;
//
//	if (bRes == FALSE && GetLastError() != ERROR_DIR_NOT_EMPTY)
//		return FALSE;
//
//	nLength = lstrlen(dir);
//	
//	if (nLength + lstrlen(s2ws("\\*.*").c_str()) + 1 > MAX_PATH)
//		return FALSE;
//	
//	if (szPath[nLength - 1] == '\\')
//		wsprintf(CharToLPWSTR(cPath), s2ws("%s*.*").c_str(), szPath);			// CharToLPWSTR(cPath)
//	else	
//		wsprintf(CharToLPWSTR(cPath), s2ws("%s\\*.*").c_str(), szPath);
//
//	hFind = FindFirstFile(s2ws(cPath).c_str(), &FindFileData);
//	if (hFind == INVALID_HANDLE_VALUE)
//		return FALSE;
//
//	lstrcpy(CharToLPWSTR(cPath), dir);
//
//	if (cPath[nLength - 1] == '\\')
//		cPath[nLength - 1] = TEXT('\0');
//
//	do
//	{
//		//Опускаем начальные . и ..						
//		if (lstrcmpi(FindFileData.cFileName, s2ws(".").c_str()) == 0 || lstrcmpi(FindFileData.cFileName, s2ws("..").c_str()) == 0)
//			continue;
//
//		//Пропускаем слишком длинные имена файлов					CharToLPWSTR(cCurrentFile)	
//		if (lstrlen(s2ws(cPath).c_str()) + lstrlen(s2ws("").c_str()) + lstrlen(FindFileData.cFileName) + 1 > MAX_PATH)
//			continue;
//		
//		wsprintf(CharToLPWSTR(cCurrentFile), s2ws("%s\\%s").c_str(), cPath, FindFileData.cFileName);
//		//Удаляем папку
//		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//		{
//			//Сбрасываем аттрибуты
//			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
//			{
//				FindFileData.dwFileAttributes &= ~FILE_ATTRIBUTE_READONLY;
//				SetFileAttributes(s2ws(cCurrentFile).c_str(), FindFileData.dwFileAttributes);
//			}
//
//			bRes = DeleteFolder(cCurrentFile);
//		}
//		//Иначе удаляем файл
//		else
//		{
//
//			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) ||
//				(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
//				SetFileAttributes(s2ws(cCurrentFile).c_str(), FILE_ATTRIBUTE_NORMAL);
//
//			DeleteFile(s2ws(cCurrentFile).c_str());
//		}
//	} while (FindNextFile(hFind, &FindFileData));
//
//	dwError = GetLastError();
//
//	if (hFind != INVALID_HANDLE_VALUE)
//		FindClose(hFind);
//
//	if (dwError != ERROR_NO_MORE_FILES)
//		return FALSE;
//
//	bRes = RemoveDirectory(dir);
//
//	return bRes;
//}