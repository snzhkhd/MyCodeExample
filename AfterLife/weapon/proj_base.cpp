

#include "proj_base.h"

// Sets default values
Aproj_base::Aproj_base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Aproj_base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Aproj_base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void Aproj_base::Init(FVector Direction)
{
	SetActorRotation(Direction.Rotation());
}

