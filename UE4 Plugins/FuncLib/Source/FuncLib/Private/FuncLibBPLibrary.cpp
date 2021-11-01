// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FuncLibBPLibrary.h"
#include "FuncLib.h"

UFuncLibBPLibrary::UFuncLibBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}


unsigned long sizeof_ifstream(std::ifstream &is)
{
	long p = is.tellg();
	is.seekg(0, std::ios::end);
	long res = is.tellg();
	is.seekg(p);
	return res;
}
bool send_parts(SOCKET s, const int part_size,const int buffer_size, const char *buffer)
{
	if (send(s, (char *)&buffer_size, sizeof(buffer_size), 0) == SOCKET_ERROR)
		return false;

	int parts_count = buffer_size / part_size;
	int last_part_size = buffer_size % part_size;

	for (int i = 0; i < parts_count; i++)
	{
		if (send(s, buffer, part_size, 0) == SOCKET_ERROR) return false;
		buffer += part_size;
	}

	if (last_part_size && send(s, buffer, last_part_size, 0) == SOCKET_ERROR)
		return false;

	return true;
}
/*вилидность файла*/
bool FileIsExist(std::string filePath);
/*высота ширина png
bool GetImageSize(const char *fn, int *x, int *y);*/


bool UFuncLibBPLibrary::FileSaveString(FString SaveTextB, FString FileNameB)
{
	return FFileHelper::SaveStringToFile(SaveTextB, *(FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() + FileNameB)));
}

bool UFuncLibBPLibrary::FileLoadString(FString FileNameA, FString & SaveTextA)
{
	return FFileHelper::LoadFileToString(SaveTextA, *(FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() + FileNameA)));
}

bool UFuncLibBPLibrary::FileIOSaveStringArrayToFile(FString SaveDirectory, FString JoyfulFileName, TArray<FString> SaveText, bool AllowOverWriting)
{
	//get complete file path
	SaveDirectory += "\\";
	SaveDirectory += JoyfulFileName;

	//No over-writing?
	if (!AllowOverWriting)
	{
		//Check if file exists already
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory))
		{
			//no overwriting
			return false;
		}
	}

	FString FinalStr = "";
	for (FString& Each : SaveText)
	{
		FinalStr += Each;
		FinalStr += LINE_TERMINATOR;
	}



	return FFileHelper::SaveStringToFile(FinalStr, *SaveDirectory);
}

/*------------------------ФАЙЛ МЕНЕДЖЕР НАЧАЛО------------------------*/
void UFuncLibBPLibrary::DisplayDirectoryTreeImp(const fs::path & pathToShow, int level, TArray<FString>& FileList)
{
	if (fs::exists(pathToShow) && fs::is_directory(pathToShow))
	{
		auto lead = std::string(level * 0, ' ');
		for (const auto& entry : fs::directory_iterator(pathToShow))
		{
			auto filename = entry.path().filename();			
			FileList.Add(filename.c_str());
		}
	
	}
}
void UFuncLibBPLibrary::DisplayDirectoryTree(const fs::path & pathToShow, TArray<FString>& FileList)
{
	DisplayDirectoryTreeImp(pathToShow, 0, FileList);
}
void UFuncLibBPLibrary::StartFileManger(FString DriveLabel, TArray<FString>& FileList)
{
	char* str = TCHAR_TO_UTF8(*DriveLabel);
	const fs::path pathToShow{ fs::u8path(str) };
	DisplayDirectoryTree(pathToShow, FileList);
}

bool is_dir(const TCHAR* dir);	/*------------------------проверка директории на существование------------------------*/

bool is_dir(const TCHAR* dir) {
	DWORD flag = GetFileAttributes(dir);
	if (flag == 0xFFFFFFFFUL) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			return false;
	}
	if (!(flag & FILE_ATTRIBUTE_DIRECTORY))
		return false;
	return true;
}

void UFuncLibBPLibrary::GetDiskLabel(TArray<FString>& DriveList)
{
	int n;
	DWORD dr = GetLogicalDrives(); // функция возвращает битовую маску
	for (int x = 0;x < 26;x++) // проходимся циклом по битам
	{
		n = ((dr >> x) & 1); // узнаём значение текущего бита
		if (n) // если единица - диск с номером x есть
		{
			stringstream dir, out;
			dir << ((char)(65 + x)) + ":\\";
			out << ((char)(65 + x));

			DriveList.Add(out.str().c_str());
		}
	}
}

bool SendTextures(const char* IPAdress, const char* PORT, const char* FullSavePath)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	char recvbuf[16384];
	int iResult;
	int recvbuflen = 16384;
	const char *sendbuf = "-PNG";

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {


		UE_LOG(LogTemp, Warning, TEXT("Initialize Winsock error"));
		return false;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;


	// Resolve the server address and port
	iResult = getaddrinfo(IPAdress, PORT, &hints, &result);
	if (iResult != 0) {
		WSACleanup();
		UE_LOG(LogTemp, Warning, TEXT("Resolve the server address and port error"));
		return false;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL;ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			WSACleanup();
			UE_LOG(LogTemp, Warning, TEXT("Create a SOCKET for connecting to server error"));
			return false;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			UE_LOG(LogTemp, Warning, TEXT("connecting to server error"));
			continue;
		}
		break;
	}

	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	freeaddrinfo(result);

//	Sleep(1000);
	
	*(recvbuf + recvbuflen) = '\0';

		string str;
		str = recvbuf;
		ifstream f(FullSavePath, std::ios::in | std::ios::binary);
		if (f.is_open())
		{
			int buffer_size = sizeof_ifstream(f);
			char *buffer = new char[buffer_size];
			f.read(buffer, buffer_size);
			if (!send_parts(ConnectSocket, 1024, buffer_size, buffer)) return false;
			else
				return true;
		}



	//	if (str == "PNG_READY")
	//	{
			//FILE *fd;
			//if (fopen_s(&fd, FullSavePath, "rb") == 0)
			//{
			//	char buffer[16384]; //выделяем блок 16Кб
			//	size_t readed;
			//	*(&buffer[0] + iResult) = '\0';
			//	while ((readed = fread(buffer, 1, sizeof(buffer), fd)) != 0)
			//	{

			//		printf((char*)buffer);
			//		send(ConnectSocket, buffer, readed, 0);
			//	}

			//	fclose(fd);
			//	closesocket(ConnectSocket);
			//	WSACleanup();
			//	return true;
			//}
			//UE_LOG(LogTemp, Warning, TEXT("CREATE NEW FILE error"));
			//fclose(fd);
//			closesocket(ConnectSocket);
//			WSACleanup();
			return false;
	//	}
	
	UE_LOG(LogTemp, Warning, TEXT("???? error"));

	closesocket(ConnectSocket);
	WSACleanup();
	return false;
}
bool UFuncLibBPLibrary::SendPNG(FString IPAdress, FString PORT, FString FullPath)
{
	const char* IP = TCHAR_TO_ANSI(*IPAdress);
	const char* Port = TCHAR_TO_ANSI(*PORT);
	const char* FilePath = TCHAR_TO_ANSI(*FullPath);
	bool res = SendTextures(IP, Port, FilePath);
	return res;
	
}

bool UFuncLibBPLibrary::TestSendPNG(FString IPAdress, FString PORT, FString FullPath)
{
	const char* IP = TCHAR_TO_ANSI(*IPAdress);
	const char* Port = TCHAR_TO_ANSI(*PORT);
	const char* FilePath = TCHAR_TO_ANSI(*FullPath);
	WSADATA wsa_data;
	if (WSAStartup(0x101, &wsa_data) || wsa_data.wVersion != 0x101) return -1;

	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET) return false;
	
	SOCKADDR_IN sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(atoi(Port));
	sa.sin_addr.S_un.S_addr = inet_addr(IP);

	printf("wait for server...\n");

	while (true)
	{
		int connect_res = connect(s, (SOCKADDR *)&sa, sizeof(sa));
		if (!connect_res) break;

		Sleep(250);
	}

	UE_LOG(LogTemp, Warning, TEXT("connected\n"));

	std::ifstream is;
	is.open(FilePath, std::ios::in | std::ios::binary);
	int buffer_size = sizeof_ifstream(is);
	char *buffer = new char[buffer_size];
	is.read(buffer, buffer_size);
	is.close();
	if (is.fail()) return false;

	UE_LOG(LogTemp, Warning, TEXT("load ok\n"));
	if (!send_parts(s, 1024, buffer_size, buffer)) return false;

	UE_LOG(LogTemp, Warning, TEXT("send ok\n"));
	delete buffer;

//	closesocket(s);

//	WSACleanup();

	return true;
}

bool UFuncLibBPLibrary::Ftp_connectToHost(FString ftp_host, FString uid, FString pwd)
{
	HINTERNET internet;
	HINTERNET ftp_session;
	char host[256];
	const char* C_ftp_host = TCHAR_TO_ANSI(*ftp_host);
	const char* C_uid = TCHAR_TO_ANSI(*uid);
	const char* C_pwd = TCHAR_TO_ANSI(*pwd);
	strcpy(host, C_ftp_host);
	internet = InternetOpenA(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	ftp_session = InternetConnectA(internet, host, INTERNET_DEFAULT_FTP_PORT, C_uid, C_pwd, INTERNET_SERVICE_FTP, 0, 0);
	return (ftp_session != NULL);
	
}

/*------------------------ФАЙЛ МЕНЕДЖЕР КОНЕЦ------------------------*/



bool UFuncLibBPLibrary::LoadStringArrayFromFile(TArray<FString>& StringArray, int32 & ArraySize, FString FullFilePath, bool ExcludeEmptyLines)
{
	ArraySize = 0;

	if (FullFilePath == "" || FullFilePath == " ") return false;

	//Empty any previous contents!
	StringArray.Empty();

	TArray<FString> FileArray;

	if (!FFileHelper::LoadANSITextFileToStrings(*FullFilePath, NULL, FileArray))
	{
		return false;
	}

	if (ExcludeEmptyLines)
	{
		for (const FString& Each : FileArray)
		{
			if (Each == "") continue;
			//~~~~~~~~~~~~~

			//check for any non whitespace
			bool FoundNonWhiteSpace = false;
			for (int32 v = 0; v < Each.Len(); v++)
			{
				if (Each[v] != ' ' && Each[v] != '\n')
				{
					FoundNonWhiteSpace = true;
					break;
				}
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			}

			if (FoundNonWhiteSpace)
			{
				StringArray.Add(Each);
			}
		}
	}
	else
	{
		StringArray.Append(FileArray);
	}

	ArraySize = StringArray.Num();
	return true;
}

UTexture2D * UFuncLibBPLibrary::ImportFileAsTexture2D(const FString & Filename)
{
	return FImageUtils::ImportFileAsTexture2D(Filename);
}

bool FileIsExist(std::string filePath)
{
	bool isExist = false;
	std::ifstream fin(filePath.c_str());

	if (fin.is_open())
		isExist = true;

	fin.close();
	return isExist;
}


bool UFuncLibBPLibrary::SaveCustomContent(FString FullFilePath, FString Save_Dir)
{	
	char* FilePath = TCHAR_TO_ANSI(*FullFilePath);
	char* Save_DirPath = TCHAR_TO_ANSI(*Save_Dir);

	ifstream infile(FilePath, ios::binary);
	ofstream outfile(Save_DirPath, ios::binary);

	if (!infile || !outfile) return false;

	outfile << infile.rdbuf();
	infile.close();
	outfile.close();
	return FileIsExist(Save_DirPath);
}


bool UFuncLibBPLibrary::IsFileExtFrom(const FString & FullFilePath, const FString & Extension)
{
	string file = TCHAR_TO_ANSI(*FullFilePath);
	string Ext = TCHAR_TO_ANSI(*Extension);
	size_t pos = file.find_last_of('.');
	if (pos <= 0) return false;
	if (file.substr(pos + 1, string::npos) == Ext) return true;

	return false;
}

bool UFuncLibBPLibrary::CheckImageSize(const FString & fn)
{
	//char st 
	int32 x,y;
	const char *file = TCHAR_TO_ANSI(*fn);
	FILE *f;
	errno_t err = fopen_s(&f, file, "rb"); if (f == 0) return false;
	if (err != 0) return false;
	fseek(f, 0, SEEK_END); long len = ftell(f); fseek(f, 0, SEEK_SET);
	if (len < 24) { fclose(f); return false; }
	unsigned char buf[24]; fread(buf, 1, 24, f);
	// PNG: прочитать ширину и высоту из смещений 16 и 20 (по 4 байта, по-умолчанию)
	if (buf[0] == 0x89 && buf[1] == 'P' && buf[2] == 'N' && buf[3] == 'G' && buf[4] == 0x0D && buf[5] == 0x0A && buf[6] == 0x1A && buf[7] == 0x0A
		&& buf[12] == 'I' && buf[13] == 'H' && buf[14] == 'D' && buf[15] == 'R')
	{
		x = (buf[16] << 24) + (buf[17] << 16) + (buf[18] << 8) + (buf[19] << 0);
		y = (buf[20] << 24) + (buf[21] << 16) + (buf[22] << 8) + (buf[23] << 0);
		cout << "x: " << x << " y: " << y << endl;
		if(x <= 1024 && y <= 1024) return true;
		
	}
	return false;
}

bool SaveContent(FString FullFilePath, FString Save_Dir);

bool SaveContent(FString FullFilePath, FString Save_Dir)
{
	char* FilePath = TCHAR_TO_ANSI(*FullFilePath);
	char* Save_DirPath = TCHAR_TO_ANSI(*Save_Dir);

	ifstream infile(FilePath, ios::binary);
	ofstream outfile(Save_DirPath, ios::binary);

	if (!infile || !outfile) return false;

	outfile << infile.rdbuf();
	infile.close();
	outfile.close();
	return FileIsExist(Save_DirPath);
}

FString UFuncLibBPLibrary::SaveTexture(UTexture2D* texture, FString Save_Dir)
{
		if(SaveContent(texture->GetFullName(), Save_Dir))
		{
			return Save_Dir;
			
		}
		return "invalid texture";
}


