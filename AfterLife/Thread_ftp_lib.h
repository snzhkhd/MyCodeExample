

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AsynkTask_Interface.h"
#include "Async/AsyncWork.h"
#include "Thread_ftp_lib.generated.h"

/**
 * 
 */

using namespace std;

class UploadAsyncTask : public FNonAbandonableTask
{
public:
	UObject* Object;
	UploadAsyncTask(UObject* Object)
	{
		this->Object = Object;
	}
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UploadAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
	void DoWork()
	{
		IAsynkTask_Interface::Execute_UploadTask(Object);
	}
};

class DownloadAsyncTask : public FNonAbandonableTask
{
public:
	UObject* Object;
	DownloadAsyncTask(UObject* Object)
	{
		this->Object = Object;
	}
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(DownloadAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
	void DoWork()
	{
		IAsynkTask_Interface::Execute_DownloadTask(Object);
	}
};

class UploadSkinAsyncTask : public FNonAbandonableTask
{
public:
	UObject* Object;
	UploadSkinAsyncTask(UObject* Object)
	{
		this->Object = Object;
	}
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UploadSkinAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
	void DoWork()
	{
		IAsynkTask_Interface::Execute_UploadSkinTask(Object);
	}
};

class DownloadSkinAsyncTask : public FNonAbandonableTask
{
public:
	UObject* Object;
	DownloadSkinAsyncTask(UObject* Object)
	{
		this->Object = Object;
	}
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(DownloadSkinAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
	void DoWork()
	{
		IAsynkTask_Interface::Execute_DownloadSkinTask(Object);
	}
};


UCLASS()
class AFTERLIFE_API UThread_ftp_lib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

		UFUNCTION(BlueprintCallable)
		static void CallUploadAsyncTask(UObject* Object)
		{
			(new FAutoDeleteAsyncTask<UploadAsyncTask>(Object))->StartBackgroundTask();
		}
		
		UFUNCTION(BlueprintCallable)
		static void CallDownloadAsyncTask(UObject* Object)
		{
			(new FAutoDeleteAsyncTask<DownloadAsyncTask>(Object))->StartBackgroundTask();
		}


		UFUNCTION(BlueprintCallable)
			static void CallUploadSkinAsyncTask(UObject* Object)
		{
			(new FAutoDeleteAsyncTask<UploadSkinAsyncTask>(Object))->StartBackgroundTask();
		}

		UFUNCTION(BlueprintCallable)
			static void CallDownloadSkinAsyncTask(UObject* Object)
		{
			(new FAutoDeleteAsyncTask<DownloadSkinAsyncTask>(Object))->StartBackgroundTask();
		}



};

