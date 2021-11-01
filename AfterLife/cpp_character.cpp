

#include "cpp_character.h"
//#include "weapon/weapon_item.h"
#include "components/PlayerStatistic_comp.h"
// Sets default values
Acpp_character::Acpp_character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
void Acpp_character::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(Acpp_character, PassengerNum);
}
// Called when the game starts or when spawned
void Acpp_character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Acpp_character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void Acpp_character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void Acpp_character::CallDeathEvent()
{
	DeathEvent();
}

void Acpp_character::CallLostMission()
{
	OnMissionLost();
}

void Acpp_character::CallWinMission()
{
	OnMissionWin();
}

void Acpp_character::DeathEvent_Implementation()
{
	UPlayerStatistic_comp* stat = FindComponentByClass<UPlayerStatistic_comp>();
	if (stat)
	{
		stat->AddDeath();
	}
}

void Acpp_character::OnMissionLost_Implementation()
{
	UPlayerStatistic_comp* stat = FindComponentByClass<UPlayerStatistic_comp>();
	if (stat)
	{
		stat->OnMissionEnd(false);
		stat->CalcStat();
	}
}

void Acpp_character::OnMissionWin_Implementation()
{
	UPlayerStatistic_comp* stat = FindComponentByClass<UPlayerStatistic_comp>();
	if (stat)
	{
		stat->OnMissionEnd(true);
		stat->CalcStat();
	}
}

void Acpp_character::SetPassengerNum(int32 Num)
{
	PassengerNum = Num;
}

bool Acpp_character::IsPassenger()
{
	if(PassengerNum > -1) 
		return true;
	else
		return false;
}


