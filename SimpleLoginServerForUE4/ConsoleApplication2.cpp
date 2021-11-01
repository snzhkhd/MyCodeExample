// Пример простого TCP-эхо-сервера

#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h> // Wincosk2.h должен быть раньше windows!
#include <windows.h>
#include <locale.h>
#include <string>
#include "FileWork.h"
#pragma comment(lib, "ws2_32.lib")

#define MY_PORT 666 // Порт, который слушает сервер 666
using namespace std;
// макрос для печати количества активных пользователей
#define PRINTNUSERS if (nclients) printf("%d user on-line\n", nclients); \
        else printf("No User on line\n");

// прототип функции, обслуживающий подключившихся пользователей
DWORD WINAPI SexToClient(LPVOID client_socket);

// глобальная переменная - количество активных пользователей
int nclients = 0;

int main()
{
	//-------------------------------------------------------------------------------------------------------------------
//	CreateNewAccount("qzwxec", "123444");
	/*CharStr info;
	info.Name = "MyName";
	info.Fraction = "Criminal";
	CreateNewCharacter("snzhkhd", info);*/
//	cout << md5("md5")<< endl;

	ofstream CheckFile;
	if (!FileIsExist(GetAccountListPath()))
	{
		CheckFile.open(GetAccountListPath(), ios::out | ios::app);
		CheckFile << "[ACCOUNT_NAME],[MD5]" << endl;
		CheckFile.close();
	}
	if (!FileIsExist(GetPlayerListPath()))
	{
		CheckFile.open(GetPlayerListPath(), ios::out | ios::app);
		CheckFile << "[PLAYER_NAME],[ACCOUNT_NAME]" << endl;
		CheckFile.close();
	}
	//string Line;
	//last_line("D:\\test\\server\\LoginServer\\Player\\Login\\snzhkhd\\char1\\WEAPON.ini", Line);
	//cout <<"Last line =  " << Line << endl;
	setlocale(0, "");
	char buff[1024]; // Буфер для различных нужд

	printf("TCP SERVER \n");
	// Шаг 1 - Инициализация Библиотеки Сокетов
	// т.к. возвращенная функцией информация не используется
	// ей передается указатель на рабочий буфер, преобразуемый к указателю
	// на структуру WSADATA.
	// Такой прием позволяет сэкономить одну переменную, однако, буфер
	// должен быть не менее полкилобайта размером (структура WSADATA
	// занимает 400 байт)
	if (WSAStartup(0x0202, (WSADATA *)&buff[0]))
	{
		// Ошибка!
		printf("Error WSAStartup %d\n", WSAGetLastError());
		return -1;
	}

	// Шаг 2 - создание сокета
	SOCKET mysocket;
	// AF_INET - сокет Интернета
	// SOCK_STREAM - потоковый сокет (с установкой соединения)
	// 0 - по умолчанию выбирается TCP протокол
	if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		// Ошибка!
		printf("Error socket %d\n", WSAGetLastError());
		WSACleanup(); // Деиницилизация библиотеки Winsock
		return -1;
	}

	// Шаг 3 - связывание сокета с локальным адресом
	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(MY_PORT); // не забываем о сетевом порядке!!!
	local_addr.sin_addr.s_addr = 0; // сервер принимает подключения
									// на все свои IP-адреса

	// вызываем bind для связывания
	if (bind(mysocket, (sockaddr *)&local_addr, sizeof(local_addr)))
	{
		// Ошибка
		printf("Error bind %d\n", WSAGetLastError());
		closesocket(mysocket); // закрываем сокет!
		WSACleanup();
		return -1;
	}
	cout << mysocket;
	// Шаг 4 - ожидание подключений
	// размер очереди - 0x100
	if (listen(mysocket, 0x100))
	{
		// Ошибка
		printf("Error listen %d\n", WSAGetLastError());
		closesocket(mysocket);
		WSACleanup();
		return -1;
	}

	printf("Ожидание подключений...\n");

	// Шаг 5 - извлекаем сообщение из очереди
	SOCKET client_socket; // сокет для клиента
	sockaddr_in client_addr; // адрес клиента (заполняется системой)

	// функции accept необходимо передать размер структуры
	int client_addr_size = sizeof(client_addr);

	// цикл извлечения запросов на подключение из очереди
	while ((client_socket = accept(mysocket, (sockaddr *)&client_addr, \
		&client_addr_size)))
	{
		nclients++; // увеличиваем счетчик подключившихся клиентов

		// пытаемся получить имя хоста
		HOSTENT *hst;
		hst = gethostbyaddr((char *)&client_addr.sin_addr.s_addr, 4, AF_INET);
		
		// вывод сведений о клиенте
		printf("+%s [%s] new connect!\n",
			(hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr));
		PRINTNUSERS
			// Вызов нового потока для обслужвания клиента
			// Да, для этого рекомендуется использовать _beginthreadex
			// но, поскольку никаких вызовов функций стандартной Си библиотеки
			// поток не делает, можно обойтись и CreateThread
			DWORD thID;
		CreateThread(NULL, NULL, SexToClient, &client_socket, NULL, &thID);
	}
	return 0;
}

bool recv_parts(SOCKET s, const int part_size,int * const buffer_size, char ** const buffer)
{
	if (recv(s, (char *)buffer_size, sizeof(*buffer_size), 0) == SOCKET_ERROR)
	{
		*buffer_size = 0;
		*buffer = 0;
		return false;
	}

	*buffer = new char[*buffer_size];

	char *p = *buffer;

	int parts_count = *buffer_size / part_size;
	int last_part_size = *buffer_size % part_size;

	for (int i = 0; i < parts_count; i++)
	{
		if (recv(s, p, part_size, 0) == SOCKET_ERROR)
		{
			*buffer_size = 0;
			delete *buffer;
			*buffer = 0;
			return false;
		}
		p += part_size;
	}

	if (last_part_size && recv(s, p, last_part_size, 0) == SOCKET_ERROR)
	{
		*buffer_size = 0;
		delete *buffer;
		*buffer = 0;
		return false;
	}

	return true;
}

bool FoundLogin(char *login, bool &isBanned)
{
	stringstream ss;
	ss << _getcwd(NULL, 0) << PlayerLogins << "/" << login;
	string bann;
	//	printf(ss.str().c_str());
	ifstream file(ss.str().c_str());
	if (file)
	{
		file >> bann;
		if (bann == "true")
		{
			isBanned = true;
			file.close();
			return false;
		}
		isBanned = false;
		file.close();
		return true;
	}
	isBanned = false;
	printf("	LOGIN IS NOT VALID	");
	file.close();
	return false;
}
bool CreateAccount(string login, string password, string &Message)
{
	stringstream ss;
	ss << _getcwd(NULL, 0) << PlayerLogins <<"//" << login;	//_getcwd(NULL, 0) <<
	ifstream file(ss.str().c_str());
	if (file)
	{
		file.close();
		Message = "existing username";	
		return false;
	}
	return CreateNewAccount(login, password);
}


#define PNG_PATH "D:\\test\\ServerSave\\"
string rnd_name()
{
	string FileName;
	const char* sym[36]{ "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","1","2","3","4","5","6","7","8","9","0"};
	FileName.insert(FileName.length(), PNG_PATH);
	for (int i = 0; i < 8;i++)
	{		
		int rnd = (rand() % 36);
		string temp = sym[rnd];
		FileName.insert(FileName.length(), temp);
	}
	FileName.insert(FileName.length(), ".png");
	cout << FileName ;
	return FileName;
}



bool foundPNG(string FilePath)
{
	stringstream ss;
	ss << "D:\\test\\ServerSave\\png\\skin\\"<< FilePath;
	ifstream file(ss.str());
	printf(ss.str().c_str());
	if (file)
	{
		file.close();
		return true;
	}
	return false;
}

int getFileSize(string fullcommand)
{
	int posC = fullcommand.find_last_of('.');
//	cout << fullcommand.substr(posC + 1, fullcommand.length()).c_str();
	return atoi(fullcommand.substr(posC + 1, fullcommand.length()).c_str());
}

// Эта функция создается в отдельном потоке
// и обсуживает очередного подключившегося клиента независимо от остальных
DWORD WINAPI SexToClient(LPVOID client_socket)
{
	enum ERECVTTYPE
	{
		CLEAR,
		RECV_LOGIN,
		SEND_REG,
		RECV__REG,
		SEND_PNG,
		RECV__PNG,
		WAIT,
		FOUND_FILE,
		UPLOADFILE,
		DOWNLOADFILE,
		LONGCOMMAND
	} RecvType = WAIT;
	SOCKET my_sock;
	my_sock = ((SOCKET *)client_socket)[0];
	char buff[46384];
#define sHELLO "Welcome\r\n"
#define sSend "connected"
#define sLoginFail "LOGIN IS NOT VALID"	
#define sBANN "???\r\n"	

#define PNG_READY "PNG_READY"
#define REG_READY "REG_READY"
#define REG_DONE "REG_DONE"
#define LOG_READY "LOG_READY"
	
	// отправляем клиенту приветствие
	send(my_sock, sHELLO, sizeof(sHELLO), 0);
//	bool isBanned;
	stringstream command;
	// цикл эхо-сервера: прием строки от клиента и возвращение ее клиенту
	int bytes_recv;
	string longCom;
//	size_t file_size;
	CharStr NewCharInfo;

	vector<string> com;
	//	(bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0)) && 
	while ((bytes_recv = recv(my_sock, &buff[0], sizeof(&buff[0]), 0)) && bytes_recv != SOCKET_ERROR)
	{
	//	int len;
		ofstream file;
		string LoginMessage;
		string str,longcom;
		*(buff + bytes_recv) = '\0';
		str = buff;
	//	cout << str << endl;
//		size_t pos;// = str.find_last_of('.');
		int buffer_size = 0;
		char *buffer = 0;
		if (str == "-png")
		{
			RecvType = RECV__PNG;
			
		}
		switch (RecvType)		//   RecvType = CLEAR;
		{

		case RECV__PNG:
			if (str == "-end")
			{
				file.close();
				RecvType = WAIT;
				longCom.clear();
				str.clear();
				LoginMessage.clear();
				cout << "--------------write end--------------" << endl;
				break;
			}
			file.open("D:\\file.png", ios::out | ios::app | ios::binary);	//	 
			if (file.is_open())
			{	
				if (str != "-png")
				{
					file.write(str.c_str(), str.length());
					cout << "-write-" << endl;
				}				
				file.close();
				longCom.clear();
				str.clear();
				LoginMessage.clear();
				break;
			}
			cout << "--------------file.is_open() false--------------" << endl;
			longCom.clear();
			str.clear();
			LoginMessage.clear();
			break;

		case WAIT:

				if (str != "-end")
				{
					longCom.append(str);				
					send(my_sock, "next", sizeof("next"), 0);
					RecvType = WAIT;
					
					break;
				}	
				if (longCom == "\0" && longCom == ";") break;
				else
				{						/// ------------------- create new char start    
										/// ------------------- commanline exemple  -  -CreateNewChar.snzhkhd.MyChar.criminal
					com = StringSeperation(longCom);	
					if (com[0] == "-CreateNewChar")
					{

						NewCharInfo.Name = com[2];
						NewCharInfo.Fraction = com[3];
						if (CreateNewCharacter(com[1], NewCharInfo))
							send(my_sock, "New Char Created", sizeof("New Char Created"), 0);
						else
							send(my_sock, "Char Not Created", sizeof("Char Not Created"), 0);	

						RecvType = WAIT;
						longCom.clear();
						str.clear();
						LoginMessage.clear();
						break;
					}
					/// ------------------- create new char end  
					/// ------------------- create new account start    
					/// ------------------- commanline exemple  -  -reg.AccountName.Password
					if (com[0] == "-reg")
					{

						if (CreateNewAccount(com[1], com[2]))
						{
							send(my_sock, REG_DONE, sizeof(REG_DONE), 0);
							RecvType = WAIT;
							longCom.clear();
							str.clear();
							LoginMessage.clear();
							break;
						}
						else
						{
							send(my_sock, "RegError", sizeof("RegError"), 0);
							RecvType = WAIT;
							longCom.clear();
							str.clear();
							LoginMessage.clear();
							break;
						}
						RecvType = WAIT;
						longCom.clear();
						str.clear();
						LoginMessage.clear();
						break;
					}
					/// ----------------------------------------------------------- login start
					if (com[0] == "-log")
					{
						if (PlayerLogin(com[1], com[2], LoginMessage))
						{						
							send(my_sock, LoginMessage.c_str(), sizeof(LoginMessage), 0);
							RecvType = WAIT;
							longCom.clear();
							str.clear();
							LoginMessage.clear();
							break;
						}
						else
						{
							send(my_sock, LoginMessage.c_str(), sizeof(LoginMessage), 0);
							RecvType = WAIT;
							longCom.clear();
							str.clear();
							LoginMessage.clear();
							break;
						}
						LoginMessage.clear();
						longCom.clear();
						str.clear();
						RecvType = WAIT;
						break;
					}						/// ----------------------------------------------------------- login end	
					/*if (com[0] == "-load")
					{
						SendCharInfo(my_sock,com[1]);
					}*/
					if (com[0] == "-get")
					{
						if(GetCharParam(com[1], com[2]) != "\0")
							send(my_sock, GetCharParam(com[1], com[2]).c_str(), GetCharParam(com[1], com[2]).length(), 0);
						else
							send(my_sock, "\0", sizeof("\0"), 0);

						RecvType = WAIT;
						longCom.clear();
						str.clear();
						LoginMessage.clear();
						break;
					}
					if (com[0] == "-del")
					{
						if (DeleteChar(com[1], com[2]))
						{
							send(my_sock, "Character delete", sizeof("Character delete"), 0);
							RecvType = WAIT;
							longCom.clear();
							str.clear();
							LoginMessage.clear();
							break;
						}
						else
						{
							send(my_sock, "ERROR! Character not delete", sizeof("ERROR! Character not delete"), 0);
							RecvType = WAIT;
							longCom.clear();
							str.clear();
							LoginMessage.clear();
							break;
						}						
					}							/// ------------------- EDIT/SAVE CHARACTER  PARAMETR
					if (com[0] == "-save")		/// ------------------- commanline exemple  -  -save.MyChar.fraction.criminal
					{
						if (SaveCharParam(com[1], com[2], com[3]))
						{
							send(my_sock, "char save", sizeof("char save"), 0);
						}
						else
							send(my_sock, "char not saved", sizeof("char not saved"), 0);

						RecvType = WAIT;
						longCom.clear();
						str.clear();
						LoginMessage.clear();
					}/// ------------------- commanline exemple  -  -save.MyChar.fraction.criminal
					
					break;
				}
				
					
					
				
					
								
		}
		

	}
	// если мы здесь, то произошел выход из цикла по причине
	// возращения функцией recv ошибки - соединение с клиентом разорвано
	nclients--; // уменьшаем счетчик активных клиентов
	printf("-disconnect\n"); PRINTNUSERS

		// закрываем сокет
		closesocket(my_sock);
	return 0;
}