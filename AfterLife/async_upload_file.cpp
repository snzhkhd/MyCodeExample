

#include "async_upload_file.h"



Uasync_upload_file* Uasync_upload_file::UploadImage(FString FullPath, FString ftp_host, FString file_name)
{
	Uasync_upload_file* uploadTask = NewObject<Uasync_upload_file>();
	uploadTask->Start(FullPath, ftp_host, file_name);
	
	return uploadTask;
}

void Uasync_upload_file::Start(FString FullPath, FString ftp_host, FString file_name)
{
	Handle(FullPath,ftp_host,file_name);
}

void Uasync_upload_file::Handle(FString FullPath, FString ftp_host, FString file_name)
{
	HINTERNET internet;
	HINTERNET ftp_session;
	char host[256];
	const char* C_ftp_host = TCHAR_TO_ANSI(*ftp_host);
	const char* C_FullPath = TCHAR_TO_ANSI(*FullPath);
	const char* C_file_name = TCHAR_TO_ANSI(*file_name);
	//	const char* C_uid = "ftp";
	//	const char* C_pwd = "ftp";
	strcpy(host, C_ftp_host);
	internet = InternetOpenA(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	ftp_session = InternetConnectA(internet, host, INTERNET_DEFAULT_FTP_PORT, "ftp", "ftp", INTERNET_SERVICE_FTP, 0, 0);
	if (ftp_session != NULL)
	{
		if (FtpSetCurrentDirectoryA(ftp_session, "//png"))
		{
			if (FtpPutFileA(ftp_session, C_FullPath, C_file_name, FTP_TRANSFER_TYPE_BINARY, 0))
			{
				OnSuccess.Broadcast("Upload End");
				InternetCloseHandle(ftp_session);
				InternetCloseHandle(internet);
				return;
			}
			InternetCloseHandle(ftp_session);
			InternetCloseHandle(internet);
			OnFail.Broadcast("Upload error ");
			return;
		}
		InternetCloseHandle(ftp_session);
		InternetCloseHandle(internet);
		OnFail.Broadcast("Error set dir ");
		return;
	}

	OnFail.Broadcast("Upload error ftp_session == NULL");
	return;
}





// -------------------------------------------------------------------------------------------------------------------//

Uasync_download_file* Uasync_download_file::DownloadImage(FString ftp_host, FString FTP_Path, FString SavePath)
{
	Uasync_download_file* DownloadTask = NewObject<Uasync_download_file>();
	DownloadTask->Start(ftp_host, FTP_Path, SavePath);

	return DownloadTask;
}

void Uasync_download_file::Start(FString ftp_host, FString FTP_Path, FString SavePath)
{
	Handle(ftp_host,FTP_Path,SavePath);
}

void Uasync_download_file::Handle(FString ftp_host, FString FTP_Path, FString SavePath)
{
	HINTERNET internet;
	HINTERNET ftp_session;
	char host[256];
	const char* C_ftp_host = TCHAR_TO_ANSI(*ftp_host);
	const char* C_FullPath = TCHAR_TO_ANSI(*SavePath);
	const char* C_file_name = TCHAR_TO_ANSI(*FTP_Path);
	//	const char* C_uid = "ftp";
	//	const char* C_pwd = "ftp";
	strcpy(host, C_ftp_host);
	internet = InternetOpenA(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	ftp_session = InternetConnectA(internet, host, INTERNET_DEFAULT_FTP_PORT, "ftp", "ftp", INTERNET_SERVICE_FTP, 0, 0);
	if (ftp_session != NULL)
	{
		if (FtpSetCurrentDirectoryA(ftp_session, "//png"))
		{
			if (FtpGetFileA(ftp_session, C_file_name, C_FullPath, FALSE, NULL, FTP_TRANSFER_TYPE_BINARY, 0))		///(hFtp, SrvIdFile, idf_name, FALSE, NULL, FTP_TRANSFER_TYPE_BINARY, 0);				
			{
				OnSuccess.Broadcast("Download End");
				InternetCloseHandle(ftp_session);
				InternetCloseHandle(internet);
				return;
			}
			InternetCloseHandle(ftp_session);
			InternetCloseHandle(internet);
			OnFail.Broadcast("Download error ");
			return;
		}
		InternetCloseHandle(ftp_session);
		InternetCloseHandle(internet);
		OnFail.Broadcast("Error set dir ");
		return;
	}

	OnFail.Broadcast("Download error ftp_session == NULL");
	return;
}
