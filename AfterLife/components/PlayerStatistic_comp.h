

#pragma once
#include "Net/UnrealNetwork.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "cpp_character.h"
#include "PlayerStatistic_comp.generated.h"


UENUM(BlueprintType)
enum ETreathLevel
{
	ETL_GREEN UMETA(DisplayName = "GREEN"),
	ETL_BRONZE UMETA(DisplayName = "BRONZE"),
	ETL_IRIDIUM UMETA(DisplayName = "IRIDIUM"),
	ETL_SILVER UMETA(DisplayName = "SILVER"),
	ETL_PLATINUM UMETA(DisplayName = "PLATINUM"),
	ETL_GOLD UMETA(DisplayName = "GOLD"),
	ETL_CHEATER UMETA(DisplayName = "CHEATER")
};

USTRUCT(BlueprintType)
struct FStatStruct
{
	GENERATED_USTRUCT_BODY()

	FStatStruct()
	{
		MidPercentHit = 0.f;
		MidKillDistance = 0.f;
		KillCount = 0;
		DeathCount = 0;
		AssistCount = 0;
		WinMissionCount = 0;
		LostMissionCount = 0;
		TreathLevel = ETreathLevel::ETL_GREEN;
	}
	UPROPERTY(BlueprintReadOnly)
		float MidPercentHit;

	UPROPERTY(BlueprintReadOnly)
		float MidKillDistance;

	UPROPERTY(BlueprintReadOnly)
		int32 KillCount;

	UPROPERTY(BlueprintReadOnly)
		int32 DeathCount;

	UPROPERTY(BlueprintReadOnly)
		int32 AssistCount;

	UPROPERTY(BlueprintReadOnly)
		int32 WinMissionCount;

	UPROPERTY(BlueprintReadOnly)
		int32 LostMissionCount;

	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte<ETreathLevel> TreathLevel;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERLIFE_API UPlayerStatistic_comp : public UActorComponent
{
	GENERATED_BODY()

public:	

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	virtual void PreReplication(IRepChangedPropertyTracker & ChangedPropertyTracker) override;


	// Sets default values for this component's properties
	UPlayerStatistic_comp();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "TreathLevel")
		FStatStruct StatStr;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	

	UPROPERTY()
	TArray<float> AllPercentHit;

	UPROPERTY()
	TArray<float> AllKillDistance;
public:

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Stat", Keywords = "Get Stat"), Category = "Statistic")
	FStatStruct GetStat();

	void OnMissionEnd(bool Result);

	void AddHit(FHitResult HIT);

	void AddKill(float Distance);

	void AddAssist();

	void AddDeath();

	void CalcStat();
};
