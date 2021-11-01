

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AsynkTask_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAsynkTask_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AFTERLIFE_API IAsynkTask_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Async Task")
	void UploadTask();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Async Task")
	void DownloadTask();


	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Async Task")
	void DownloadSkinTask();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Async Task")
	void UploadSkinTask();
};
