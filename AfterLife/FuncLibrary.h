

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <string>
#include <MD5.h>
#include <cstdio>
#include <sstream>
#include "FuncLibrary.generated.h"

using namespace std;
/**
 * 
 */
UCLASS()
class AFTERLIFE_API UFuncLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get MD5", Keywords = "Get MD5"), Category = "snzhkhd")
	static FString MD5(FString To_MD5);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Delete File", Keywords = "Delete File"), Category = "snzhkhd")
	static bool DeleteFile(FString FullPathToFile);


	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TCP Load Char", Keywords = "TCP Load Char"), Category = "snzhkhd")
	static bool LoadChar(FString Srt, FString &Name, FString &fraction, FString &money, FString &first_wpn, FString &Second_wpn, FString &grenade, FString &current_car, TArray<FString> &CharMods, TArray<FString> &CarMods, TArray<FString> &FirsWpnMod, TArray<FString> &Inventory);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "String Seperation", Keywords = "String Seperation"), Category = "snzhkhd")
	static TArray<FString> FStringSeperation(FString Sourse, FString divider);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PNG to Byte", Keywords = "PNG to Byte"), Category = "snzhkhd")
	static FString PNGtoByte(FString FullPathToFile);
};
