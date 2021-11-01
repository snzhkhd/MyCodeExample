

#pragma once
#include "cpp_character.h"
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Actor.h"
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <fstream>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <direct.h> 
#include <wininet.h>
#include "Async/AsyncWork.h"

#include "tcp_client.generated.h"

#pragma comment(lib, "Wininet")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 16384



UCLASS()
class AFTERLIFE_API Atcp_client : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Atcp_client();

	HINTERNET internet;
	HINTERNET ftp_session;
	UPROPERTY(BlueprintReadOnly, Category = "FTP IPAdress")
		FString IPAdress;
	UPROPERTY(BlueprintReadOnly, Category = "FTP Login")
		FString uid;
	UPROPERTY(BlueprintReadOnly, Category = "FTP Pass")
		FString	pwd;
	char host[256];
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	const char *sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	UPROPERTY(BlueprintReadOnly, Category = "ftp client")
	FString DisconnectMSG;
	UPROPERTY(BlueprintReadOnly, Category = "ftp client")
	FString RecvMSG;
	UPROPERTY(BlueprintReadOnly, Category = "ftp client")
		bool ftp_Result;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	
	bool ValidFTPConnect();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "tcp client")
		bool Connect(const FString& ipAddress, FString port);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Send PNG ---", Keywords = "Send PNG---",Latent), Category = "tcp client")
		void SendPNG(FString FullPath);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Disconnect", Keywords = "Disconnect"), Category = "tcp client")
		void Disconnect();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Send Command", Keywords = "Send Command"), Category = "tcp client")
		bool SendCommand(FString Command);

	
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "OnConnected", Keywords = "OnConnected"), Category = "tcp client")
		void  OnConnected();
		virtual void  OnConnected_Implementation();
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "OnDisconnected", Keywords = "OnDisconnected"), Category = "tcp client")
		void OnDisconnected(FString& Message);
		virtual void OnDisconnected_Implementation(FString& Message);
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "OnMessageReceived", Keywords = "OnMessageReceived"), Category = "tcp client")
		void OnMessageReceived(FString& Message);
		virtual void OnMessageReceived_Implementation(FString& Message);
		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Recv Message", Keywords = "Recv Message"), Category = "tcp client")
		FString Recv();

//	tcp_ConnectDelegate ConnectedDelegate;
//	tcp_MessageDelegate MessageReceivedDelegate;
//	tcp_DisconnectDelegate DisconnectDelegate;


		/*----------------------ftp-------------------------*/

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Ftp Upload file Async Task", Keywords = "Ftp Upload file Async Task",Letent), Category = "Ftp")
			void UploadfileAsyncTask(FString file_path, FString file_name, FString Dir, FString IP_Adress, bool & Result);
		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Ftp Download file Async Task", Keywords = "Ftp Download file Async Task"), Category = "Ftp")
			void DownloadfileAsyncTask(FString file_path, FString new_name_full_path, FString Dir, FString IP_Adress, bool & Result);
		

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "DownloadSavefile", Keywords = "DownloadSavefile"), Category = "Ftp")
			bool DownloadSavefile(FString ftp_host, FString file_name, FString Newfile_name);


		UFUNCTION(BlueprintCallable, meta = (DisplayName = "ConnectToHost", Keywords = "ConnectToHost"), Category = "Ftp")
			bool ConnectToHost(FString ftp_host);
		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Ftp close", Keywords = "Ftp close"), Category = "Ftp")
			void close();
		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Ftp Set Current Dir", Keywords = "Ftp Set Current Dir"), Category = "Ftp")
			bool SetCurrentDir(FString dir);
		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Ftp put", Keywords = "Ftp put"), Category = "Ftp")
			bool put(FString file_path, FString file_name);
		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Ftp Get", Keywords = "Ftp Get"), Category = "Ftp")
			bool FtpGet(FString file_name, FString SavePath);
		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Ftp deleteFile", Keywords = "Ftp deleteFile"), Category = "Ftp")
			bool deleteFile(FString file_name);
		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Ftp renameFile", Keywords = "Ftp renameFile"), Category = "Ftp")
			bool renameFile(FString f_old, FString f_new);
		

		UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "On FTP Result", Keywords = "On FTP Result"), Category = "Ftp")
			void  OnFTPSendDone();
			virtual void  OnFTPSendDone_Implementation();

		UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "OnFTPConnectError", Keywords = "OnFTPConnectError"), Category = "Ftp")
			void  OnFTPConnectError();
			virtual void  OnFTPConnectError_Implementation();
		UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "OnFTPConnectSeccuses", Keywords = "OnFTPConnectSeccuses"), Category = "Ftp")
				void  OnFTPConnectSeccuses();
			virtual void  OnFTPConnectSeccuses_Implementation();
		/*{
			FtpGetFileW(ftp_session, file_path, new_name_full_path, TRUE, 0, FTP_TRANSFER_TYPE_BINARY, 0);
		}*/

private:
	
	

};

class FTP_UploadfileAsyncTask : public FNonAbandonableTask
{
public:
	FString file_path;
	FString file_name;
	FString Dir;
	FString IP_Adress;
	FString DelCom = "delete_now_";
	UPROPERTY(BlueprintReadOnly, Category = "ftp client")
	bool * Result;
	/*Default constructor*/
	FTP_UploadfileAsyncTask(FString file_path, FString file_name, FString Dir, FString IP_Adress, bool & Result)
	{
		this->file_path = file_path;
		this->file_name = file_name;
		this->Dir = Dir;
		this->IP_Adress = IP_Adress;
		this->Result = &Result;
	}
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTP_UploadfileAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
	/*This function is executed when we tell our task to execute*/
	void DoWork()
	{
	//	Atcp_client tcp;
		HINTERNET internet;
		HINTERNET ftp_session;
		char host[256];
		const char* C_ftp_host = TCHAR_TO_ANSI(*IP_Adress);
		const char* C_uid = "ftp";
		const char* C_pwd = "ftp";
		strcpy(host, C_ftp_host);
		internet = InternetOpenA(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
		ftp_session = InternetConnectA(internet, host, INTERNET_DEFAULT_FTP_PORT, C_uid, C_pwd, INTERNET_SERVICE_FTP, 0, 0);
		if (ftp_session != NULL)
		{
			const char* C_Dir = TCHAR_TO_ANSI(*Dir);
			if (FtpSetCurrentDirectoryA(ftp_session, C_Dir))
			{
				const char* C_file_path = TCHAR_TO_ANSI(*file_path);
				const char* C_file_name = TCHAR_TO_ANSI(*file_name);	
				
//				GLog->Log(C_file_name);
//				remove(C_file_name);
				if (FtpPutFileA(ftp_session, C_file_path, C_file_name, FTP_TRANSFER_TYPE_BINARY, 0))
				{
						GLog->Log("Put File end");
						InternetCloseHandle(ftp_session);
						InternetCloseHandle(internet);				
						*Result = true;
						return;
				}				
				/*if (FtpDeleteFile(ftp_session, (LPCWSTR)C_file_name))
				{
					GLog->Log("delete File ");
					FtpPutFileA(ftp_session, C_file_path, C_file_name, FTP_TRANSFER_TYPE_BINARY, 0);
					GLog->Log("Put File end");
					InternetCloseHandle(ftp_session);
					InternetCloseHandle(internet);
					Result = true;
					return;
				}
				GLog->Log(" delete File error");*/
				InternetCloseHandle(ftp_session);
				InternetCloseHandle(internet);
				*Result = false;
				return;
			}
			GLog->Log("SetCurrentDirectory error");
			InternetCloseHandle(ftp_session);
			InternetCloseHandle(internet);
			*Result = false;
			return;
		}
		GLog->Log("ftp_session == NULL");
		*Result = false;
		return;
	}

};


class FTP_DownloadfileAsyncTask : public FNonAbandonableTask
{
public:
	
	FString file_path;
	FString Newfile_name;
	FString Dir;
	FString IP_Adress;
	bool Result;
	FTP_DownloadfileAsyncTask(FString file_path, FString Newfile_name, FString Dir, FString IP_Adress, bool &Result)
	{
		this->file_path = file_path;
		this->Newfile_name = Newfile_name;
		this->Dir = Dir;
		this->IP_Adress = IP_Adress;
		this->Result = &Result;
	}
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTP_DownloadfileAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
	/*This function is executed when we tell our task to execute*/
	void DoWork()
	{
		
		HINTERNET internet;
		HINTERNET ftp_session;
		char host[256];
		const char* C_ftp_host = TCHAR_TO_ANSI(*IP_Adress);
		const char* C_uid = "ftp";
		const char* C_pwd = "ftp";
		strcpy(host, C_ftp_host);
		internet = InternetOpenA(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
		ftp_session = InternetConnectA(internet, host, INTERNET_DEFAULT_FTP_PORT, C_uid, C_pwd, INTERNET_SERVICE_FTP, 0, 0);
		if (ftp_session != NULL)
		{
			const char* C_Dir = TCHAR_TO_ANSI(*Dir);
			if (FtpSetCurrentDirectoryA(ftp_session, C_Dir))
			{
				const char* C_file_path = TCHAR_TO_ANSI(*file_path);
				const char* C_new_name_full_path = TCHAR_TO_ANSI(*Newfile_name);
				if (FtpPutFileA(ftp_session, C_file_path, C_new_name_full_path, FTP_TRANSFER_TYPE_BINARY, 0))
				{
					GLog->Log("Download end");
					InternetCloseHandle(ftp_session);
					InternetCloseHandle(internet);
					Result = true;
					
					return;
				}
				GLog->Log("FtpGetFileA error");
				InternetCloseHandle(ftp_session);
				InternetCloseHandle(internet);
				Result = false;
				return;
			}
			GLog->Log("SetCurrentDirectory error");
			InternetCloseHandle(ftp_session);
			InternetCloseHandle(internet);
			Result = false;
			return;
		}
		GLog->Log("ftp_session == NULL error");
		Result = false;
		return;
	}
};