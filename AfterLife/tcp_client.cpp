

#include "tcp_client.h"
using namespace std;
// Sets default values
Atcp_client::Atcp_client()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void Atcp_client::BeginPlay()
{
	Super::BeginPlay();
	
}

bool Atcp_client::ValidFTPConnect()
{
	return (ftp_session != NULL || internet != NULL);	
}




// Called every frame
void Atcp_client::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

}

bool Atcp_client::Connect(const FString & ipAddress, FString port)
{
	const char* IP = TCHAR_TO_ANSI(*ipAddress);
	const char* Port = TCHAR_TO_ANSI(*port);
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		DisconnectMSG = "WSAStartup failed with error: " + iResult;
		//	printf("WSAStartup failed with error: %d\n", iResult);
		OnDisconnected(DisconnectMSG);
		return false;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;


	// Resolve the server address and port
	iResult = getaddrinfo(IP, Port, &hints, &result);
	if (iResult != 0) {
		DisconnectMSG = "getaddrinfo failed with error: " + iResult;
		OnDisconnected(DisconnectMSG);
		WSACleanup();
		return false;
	}

// Attempt to connect to an address until one succeeds
for (ptr = result; ptr != NULL;ptr = ptr->ai_next) {

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
		ptr->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET) {
		//	printf("socket failed with error: %ld\n", WSAGetLastError());
		DisconnectMSG = "socket failed with error: %ld\n" + WSAGetLastError();
		OnDisconnected(DisconnectMSG);
		WSACleanup();
		return false;
	}

	// Connect to server.
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
		continue;
	}
	break;
}
	freeaddrinfo(result);
//	recv(ConnectSocket, (char*)& recvbuf, recvbuflen, 0);
	OnConnected();
	return true;
}





size_t GetFileSize(string filename)
{
	struct stat stat_buf;
	int rc = stat(filename.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

void Atcp_client::SendPNG(FString FullPath)
{

	if (iResult == SOCKET_ERROR)
	{
		DisconnectMSG = "SOCKET_ERROR";
		Disconnect();
	}
	*(recvbuf + recvbuflen) = '\0';
	const char* FilePath = TCHAR_TO_ANSI(*FullPath);
	FILE *fd;
	if (fopen_s(&fd, FilePath, "rb") == 0)
	{
		size_t readed;
		char buffer[DEFAULT_BUFLEN]; //выделяем блок 16Кб
	//	size_t len = GetFileSize(FilePath);
	//	send(ConnectSocket, &dd, sizeof(&dd), 0);
		*(&buffer[0] + iResult) = '\0';
		while ((readed = fread(buffer, 1, sizeof(buffer), fd)) !=0 )//sizeof(buffer), fd)) != 0)
		{
			send(ConnectSocket, buffer, readed, 0);
		}

		
		DisconnectMSG = "PNG Sended";
		//	Disconnect();


	}
	else
	{
		DisconnectMSG = "Open file error";
		Disconnect();
	}
	DisconnectMSG = "PNG Sended";
	fclose(fd);
	*(recvbuf + recvbuflen) = '\0';
	send(ConnectSocket, "-close", (int)strlen("-close"), 0);
}

void Atcp_client::Disconnect()
{
//	ioctlsocket(ConnectSocket, FIONBIO, 0);
//	shutdown(ConnectSocket,2);			
//	closesocket(ConnectSocket);		
//	WSACleanup();
}

bool Atcp_client::SendCommand(FString Command)
{
	const char* C = TCHAR_TO_ANSI(*Command);
	return send(ConnectSocket, C, (int)strlen(C), 0);
}


void Atcp_client::OnConnected_Implementation()
{
	if (iResult = recv(ConnectSocket, (char*)& recvbuf, recvbuflen, 0) && iResult != INVALID_SOCKET)
	{
		RecvMSG = recvbuf;
	}
}


void Atcp_client::OnDisconnected_Implementation(FString & Message)
{
	Message = DisconnectMSG;
}

void Atcp_client::OnMessageReceived_Implementation(FString & Message)
{
	Message = recvbuf;
}

FString Atcp_client::Recv()
{
	if (iResult = recv(ConnectSocket, (char*)& recvbuf, recvbuflen, 0) && iResult != INVALID_SOCKET)
	{
		return recvbuf;
	}
	return FString();
}

void Atcp_client::UploadfileAsyncTask(FString file_path, FString file_name, FString Dir, FString IP_Adress, bool & Result)
{
	(new FAutoDeleteAsyncTask<FTP_UploadfileAsyncTask>(file_path,file_name, Dir, IP_Adress, Result))->StartBackgroundTask();
}

void Atcp_client::DownloadfileAsyncTask(FString file_path, FString new_name_full_path, FString Dir, FString IP_Adress, bool & Result)
{
	(new FAutoDeleteAsyncTask<FTP_DownloadfileAsyncTask>(file_path, new_name_full_path, Dir, IP_Adress, Result))->StartBackgroundTask();
	
}

//bool GetFTPFile(HINTERNET session, FString file_name, FString Newfile_name);

bool GetFTPFile(HINTERNET session, FString file_name, FString Newfile_name)
{
	if (session != NULL)
	{
		const char* C_file_name = TCHAR_TO_ANSI(*file_name);
		const char* C_SavePath = TCHAR_TO_ANSI(*Newfile_name);
		return FtpPutFileA(session, C_SavePath, C_file_name, FTP_TRANSFER_TYPE_BINARY, 0);
	}
	return false;
}

bool Atcp_client::DownloadSavefile(FString ftp_host, FString file_name, FString Newfile_name)
{
	HINTERNET internet;
	HINTERNET ftp_session;
	char host[256];
	const char* C_ftp_host = TCHAR_TO_ANSI(*ftp_host);
//	const char* C_uid = "ftp";
//	const char* C_pwd = "ftp";
	strcpy(host, C_ftp_host);
	internet = InternetOpenA(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	ftp_session = InternetConnectA(internet, host, INTERNET_DEFAULT_FTP_PORT, "ftp", "ftp", INTERNET_SERVICE_FTP, 0, 0);
	if (ftp_session != NULL)
	{
	//	const char* C_Dir = "//saved";//TCHAR_TO_ANSI(*Dir);
		if (FtpSetCurrentDirectoryA(ftp_session, "//saved"))
		{
		//	const char* C_file_name = TCHAR_TO_ANSI(*file_name);
		//	const char* C_new_name_full_path = TCHAR_TO_ANSI(*Newfile_name);
		//	if (FtpGetFileA(ftp_session, C_new_name_full_path, C_file_name, TRUE, 0, FTP_TRANSFER_TYPE_BINARY, 0))
			if(GetFTPFile(ftp_session, file_name, Newfile_name))
			{
				GLog->Log("Download end");
				InternetCloseHandle(ftp_session);
				InternetCloseHandle(internet);			
				return true;
			}
			GLog->Log("FtpGetFileA error");
			InternetCloseHandle(ftp_session);
			InternetCloseHandle(internet);			
			return false;
		}
		GLog->Log("SetCurrentDirectory error");
		InternetCloseHandle(ftp_session);
		InternetCloseHandle(internet);
		return false;
	}
	GLog->Log("ftp_session == NULL error");
	return false;
}


bool Atcp_client::ConnectToHost(FString ftp_host)
{
	const char* C_ftp_host = TCHAR_TO_ANSI(*ftp_host);
	strcpy(host, C_ftp_host);
	internet = InternetOpenA(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	ftp_session = InternetConnectA(internet, host, INTERNET_DEFAULT_FTP_PORT, "ftp", "ftp", INTERNET_SERVICE_FTP, 0, 0);
	return (ftp_session != NULL);
}

void Atcp_client::close()
{
	if (ValidFTPConnect())
	{
		InternetCloseHandle(ftp_session);
		InternetCloseHandle(internet);
	}
	
}

bool Atcp_client::SetCurrentDir(FString dir)
{
	if (ValidFTPConnect())
	{
		const char* C_dir = TCHAR_TO_ANSI(*dir);
		return FtpSetCurrentDirectoryA(ftp_session, C_dir);
	}
	return false;
}

bool Atcp_client::put(FString file_path, FString file_name)
{
	if (ValidFTPConnect())
	{
		const char* C_file_path = TCHAR_TO_ANSI(*file_path);
		const char* C_file_name = TCHAR_TO_ANSI(*file_name);
		return FtpPutFileA(ftp_session, C_file_path, C_file_name, FTP_TRANSFER_TYPE_BINARY, 0);
	}
	return false;
}

bool Atcp_client::FtpGet(FString file_name, FString SavePath)
{
	if (ValidFTPConnect())
	{
		const char* C_file_name = TCHAR_TO_ANSI(*file_name);
		const char* C_SavePath = TCHAR_TO_ANSI(*SavePath);
		return FtpPutFileA(ftp_session, C_SavePath, C_file_name, FTP_TRANSFER_TYPE_BINARY, 0);
	}
	return false;
}

bool Atcp_client::deleteFile(FString file_name)
{
	if (ValidFTPConnect())
	{
		const char* C_file_name = TCHAR_TO_ANSI(*file_name);
		return FtpDeleteFileA(ftp_session, C_file_name);
	}
	return false;
}

bool Atcp_client::renameFile(FString f_old, FString f_new)
{
	if (ValidFTPConnect())
	{
		const char* C_f_old = TCHAR_TO_ANSI(*f_old);
		const char* C_f_new = TCHAR_TO_ANSI(*f_new);
		return FtpRenameFileA(ftp_session, C_f_old, C_f_new);
	}
	return false;
}



void Atcp_client::OnFTPSendDone_Implementation()
{
}

void Atcp_client::OnFTPConnectError_Implementation()
{
}

void Atcp_client::OnFTPConnectSeccuses_Implementation()
{
}


