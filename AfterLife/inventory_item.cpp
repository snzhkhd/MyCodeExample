

#include "inventory_item.h"
#include "Engine/World.h"
// Sets default values
Ainventory_item::Ainventory_item()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void Ainventory_item::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Ainventory_item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void Ainventory_item::DropFrom(FVector StartLocation, FVector StartVelocity)
{
	Ainventory_item * DropItem;
	FActorSpawnParameters SpawnInfo;
	DropItem = GetWorld()->SpawnActor<Ainventory_item>(GetClass(), StartLocation, findLook(Instigator->K2_GetActorLocation(), StartLocation), SpawnInfo);
	DropItem->SyncReplicatedPhysicsSimulation();
	DropItem->SetActorRelativeLocation(StartVelocity);
	DropItem->SetOwner(NULL);
}

FRotator Ainventory_item::findLook(FVector Start, FVector End)
{
	FVector result = Start - End;
	return result.Rotation();
}

