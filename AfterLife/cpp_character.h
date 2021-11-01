#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "weapon/weapon_item.h"

#include "cpp_character.generated.h"


UCLASS()
class AFTERLIFE_API Acpp_character : public ACharacter
{
	GENERATED_BODY()

public:
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	// Sets default values for this character's properties
	Acpp_character();
//	class Aweapon_item * Aweapon_item_instance;
	class UPlayerStatistic_comp * APlayerStatistic_comp_instance;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CallDeathEvent", Keywords = "CallDeathEvent"), Category = "DeathEvent")
	void CallDeathEvent();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CallEndMission", Keywords = "CallEndMission"), Category = "Mission")
	void CallLostMission();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CallWinMission", Keywords = "CallWinMission"), Category = "Mission")
	void CallWinMission();

	UFUNCTION(BlueprintNativeEvent, Category = "GamePlayEvent | Death")
		void DeathEvent();
	virtual void DeathEvent_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "GamePlayEvent | Death")
		void OnMissionLost();
	virtual void OnMissionLost_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "GamePlayEvent | Death")
		void OnMissionWin();
	virtual void OnMissionWin_Implementation();


	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Vehicle")
	int32 PassengerNum = -1;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Passenger Num", Keywords = "Set Passenger Num"), Category = "Vehicle")
	void SetPassengerNum(int32 Num);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "IsPassenger", Keywords = "IsPassenger"), Category = "Vehicle")
		bool IsPassenger();
};


