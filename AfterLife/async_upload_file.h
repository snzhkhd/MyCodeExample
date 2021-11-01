

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wininet.h>
#include "async_upload_file.generated.h"

#pragma comment(lib, "Wininet")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(Fupload_file,FString, Result);




UCLASS()
class AFTERLIFE_API Uasync_upload_file : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
//		DECLARE_DYNAMIC_MULTICAST_DELEGATE(Async);
public:
	

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
		static Uasync_upload_file* UploadImage(FString FullPath, FString ftp_host, FString file_name);

	UPROPERTY(BlueprintAssignable)
		Fupload_file OnSuccess;

	UPROPERTY(BlueprintAssignable)
		Fupload_file OnFail;

public:

	void Start(FString FullPath, FString ftp_host, FString file_name);
	void Handle(FString FullPath, FString ftp_host, FString file_name);

private:
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(Fdownload_file, FString, Result);

UCLASS()
class AFTERLIFE_API Uasync_download_file : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
		//		DECLARE_DYNAMIC_MULTICAST_DELEGATE(Async);
public:


	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
		static Uasync_download_file* DownloadImage(FString ftp_host, FString FTP_Path, FString SavePath);

	UPROPERTY(BlueprintAssignable)
		Fdownload_file OnSuccess;

	UPROPERTY(BlueprintAssignable)
		Fdownload_file OnFail;

public:

	void Start(FString ftp_host, FString FTP_Path, FString SavePath);
	void Handle(FString ftp_host, FString FTP_Path, FString SavePath);

private:
};