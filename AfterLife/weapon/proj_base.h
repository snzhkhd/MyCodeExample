

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "proj_base.generated.h"

UCLASS()
class AFTERLIFE_API Aproj_base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aproj_base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(FVector Direction);
};
