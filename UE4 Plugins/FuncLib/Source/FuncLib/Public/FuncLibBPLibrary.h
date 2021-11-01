// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once
#define WIN32_LEAN_AND_MEAN

#include "Kismet/BlueprintFunctionLibrary.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "ImageUtils.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include <iostream>
#include <fstream>
#include <string>   
#include <sstream>  
#include <experimental/filesystem> 
#include <Windows/MinWindows.h>
#include <direct.h> 
#include <wininet.h>
#include "FuncLibBPLibrary.generated.h"
#define WIN32_NO_STATUS
#pragma comment(lib, "Wininet")
using namespace std;
namespace fs = experimental::filesystem;
/* 
*	Function library class.
*/
UCLASS()
class UFuncLibBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Save file", Keywords = "Save file"), Category = "snzhkhd")
		static bool FileSaveString(FString SaveTextB, FString FileNameB);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Load file", Keywords = "Load file"), Category = "snzhkhd")
		static bool FileLoadString(FString FileNameA, FString& SaveTextA);


	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Save array file", Keywords = "Save array file"), Category = "snzhkhd")
		static bool FileIOSaveStringArrayToFile(FString SaveDirectory, FString JoyfulFileName, TArray<FString> SaveText, bool AllowOverWriting = false);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Load array file", Keywords = "Load array file"), Category = "snzhkhd")
		static bool LoadStringArrayFromFile(TArray<FString>& StringArray, int32& ArraySize, FString FullFilePath = "Enter Full File Path", bool ExcludeEmptyLines = false);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Import Texture2D", Keywords = "Import Texture2D"), Category = "snzhkhd")
		static UTexture2D* ImportFileAsTexture2D(const FString& Filename);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Dynamic Load Object", Keywords = "Dynamic Load Object"), Category = "snzhkhd")
		static FORCEINLINE UObject* LoadObjFromPath(const FName& Path)
		{
			if (Path == NAME_None) return NULL;
			//~

			return Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, *Path.ToString()));
		}

	/*Копирование файла в папку клиента*/

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Save Custom Content", Keywords = "Save Custom Content"), Category = "snzhkhd")
		static bool SaveCustomContent(FString FullFilePath, FString Save_Dir);
		
	/*проверка на  расширение файла*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Is File Extension From", Keywords = "Get File Extension"), Category = "snzhkhd")
		static bool IsFileExtFrom(const FString& FullFilePath, const FString& Extension);
	/*проверка на размер. 1024х1024*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Check Image Size", Keywords = "Get Image Size"), Category = "snzhkhd")
		static bool CheckImageSize(const FString &fn);
		/* попытка сохранить картинку с сервера в файл */
		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Save Texture", Keywords = "Save Texture"), Category = "snzhkhd")
		static FString SaveTexture(UTexture2D* texture, FString Save_Dir);
		

		static void DisplayDirectoryTreeImp(const fs::path& pathToShow, int level, TArray<FString>& FileList);
		static void DisplayDirectoryTree(const fs::path& pathToShow, TArray<FString>& FileList);

		UFUNCTION(BlueprintPure, meta = (DisplayName = "Get All Folder & File", Keywords = "Get All Folder & File"), Category = "snzhkhd")
		static void StartFileManger(FString DriveLabel, TArray<FString>& FileList);

		UFUNCTION(BlueprintPure, meta = (DisplayName = "Get drive label", Keywords = "Get drive label"), Category = "snzhkhd")
		static void GetDiskLabel(TArray<FString>& DriveList);


		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Send PNG", Keywords = "Send PNG"), Category = "snzhkhd")
		static bool SendPNG(FString IPAdress, FString PORT, FString FullPath);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "TCP Send file", Keywords = "TCP Send file"), Category = "snzhkhd")
			static bool TestSendPNG(FString IPAdress, FString PORT, FString FullPath);
		

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Ftp connect To Host _lib", Keywords = "Ftp connect To Host_lib"), Category = "Ftp_lib")
			static bool Ftp_connectToHost(FString ftp_host, FString uid, FString pwd);

		
};
