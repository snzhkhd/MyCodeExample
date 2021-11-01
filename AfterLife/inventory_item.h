

#pragma once
#include "Kismet/KismetMathLibrary.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "cpp_character.h"
#include "inventory_item.generated.h"

UCLASS()
class AFTERLIFE_API Ainventory_item : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Ainventory_item();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY()
	Acpp_character * OwnerPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FString ItemName;

	void GiveTo(Acpp_character Player);

	void ItemRemovedFromInv();

	void DropFrom(FVector StartLocation, FVector StartVelocity);

	FRotator findLook(FVector Start, FVector End);
};
