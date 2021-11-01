

#include "PlayerStatistic_comp.h"

// Sets default values for this component's properties
UPlayerStatistic_comp::UPlayerStatistic_comp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bReplicates = true;
	// ...
}
void UPlayerStatistic_comp::PreReplication(IRepChangedPropertyTracker & ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);
}
void UPlayerStatistic_comp::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//	DOREPLIFETIME_CONDITION_NOTIFY(Aweapon_item, AmmoInClip, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME(UPlayerStatistic_comp, StatStr);
}
// Called when the game starts
void UPlayerStatistic_comp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerStatistic_comp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FStatStruct UPlayerStatistic_comp::GetStat()
{
	return StatStr;
}

void UPlayerStatistic_comp::OnMissionEnd(bool Result)
{
	if (Result)
		StatStr.LostMissionCount++;
	else
		StatStr.WinMissionCount++;
}

void UPlayerStatistic_comp::AddHit(FHitResult HIT)
{
	if (Cast<Acpp_character>(HIT.Actor)->IsValidLowLevelFast())
	{
		AllPercentHit.Add(1.f);
	}
	else
		AllPercentHit.Add(0.f);
}

void UPlayerStatistic_comp::AddKill(float Distance)
{
	AllKillDistance.Add(Distance);
	StatStr.KillCount++;
}

void UPlayerStatistic_comp::AddAssist()
{
	StatStr.AssistCount++;
}

void UPlayerStatistic_comp::AddDeath()
{
	StatStr.DeathCount++;
}

void UPlayerStatistic_comp::CalcStat()
{
	int64 l_Hit = 0, l_dist = 0;
	for (int i = 0; i < AllPercentHit.Num(); i++)
	{
		l_Hit = l_Hit + AllPercentHit[i];
	}
	
	StatStr.MidPercentHit = l_Hit;
	for (int i = 0; i < AllKillDistance.Num(); i++)
	{
		l_dist = l_dist + AllKillDistance[i];
	}
	StatStr.MidKillDistance = l_dist;
}

