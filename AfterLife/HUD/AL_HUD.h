

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "cpp_character.h"
#include "Engine/Canvas.h"
#include "Math/Vector2D.h"
#include "GameFramework/HUD.h"
#include "Kismet/KismetMathLibrary.h"
#include "AL_HUD.generated.h"


USTRUCT(BlueprintType)
struct FObectivStruct
{
	GENERATED_USTRUCT_BODY()

	FObectivStruct()
	{
		ObjectibLocation = FVector::ZeroVector;
		ObjectivID = 0;
	}
	UPROPERTY(BlueprintReadOnly)
	FVector ObjectibLocation;

	UPROPERTY(BlueprintReadOnly)
	int32 ObjectivID;
};



/**
 * 
 */
UCLASS()
class AFTERLIFE_API AAL_HUD : public AHUD
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	APlayerController * PC;

	UPROPERTY()
	FVector ActualMapCenter = FVector::ZeroVector;

	UPROPERTY()
	FVector CenterPos = FVector::ZeroVector;

	UPROPERTY()
	FVector MapRotX = FVector::ZeroVector;

	UPROPERTY()
	FVector MapRotY = FVector::ZeroVector;

	UPROPERTY()
	float RadarWidth;

	UPROPERTY()
	float RadarRange;

	UPROPERTY()
	int32 CurrentMapRotYaw;

	UPROPERTY()
	int32 MapYaw;

	UPROPERTY()
	FVector2D MapPosition = FVector2D(0.99,0.05);

	void GetAxes(FRotator A, FVector& X, FVector& Y, FVector& Z);

	float FMax(float A, float B);

public:

	virtual void DrawHUD();

	/** Location which is used as center for minimap (onslaught) */
	UPROPERTY()
	FVector MapCenter = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map | Setting")
	float MapDefaultSize = 250;


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map | Setting")
	//	UTexture * Full_Map = NULL;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map | Setting")
	//	UTexture * BackGround = NULL;


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map | Setting")
	//UTexture * NorthIcon = NULL;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map | Setting")
	//UTexture * PlayerIcon = NULL;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map | Setting")
	//UTexture * UL_Map = NULL;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map | Setting")
	//UTexture * UR_Map = NULL;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map | Setting")
	//UTexture * LL_Map = NULL;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map | Setting")
	//UTexture * LR_Map = NULL;

//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Zoom")

	FVector GetActorLoc(FVector InVector);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UpdateHUDLocation", Keywords = "UpdateHUDLocation"), Category = "MAP")
	FVector UpdateHUDLocation(FVector InLocation);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ChangeMapRotation", Keywords = "ChangeMapRotation"), Category = "MAP")
	void ChangeMapRotation();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DrawRing", Keywords = "DrawRing"), Category = "MAP")
	void DrawRing(UTexture * UL, UTexture * UR, UTexture * LL, UTexture * LR);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map | Setting")
	float MiniMapZoom = 0.5;

	UPROPERTY(BlueprintReadOnly, Category = "Map | Setting")
	TArray<Acpp_character*> EnemyLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Map | Setting")
	TArray<Acpp_character*> FrendlyLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Map | Setting")
	TArray<FObectivStruct> Objectiv;

	UPROPERTY(BlueprintReadOnly, Category = "Map | Setting")
	FVector WorldCentr;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetWorldCentr", Keywords = "SetWorldCentr"), Category = "MAP")
	void SetWorldCentr(FVector Location);

	UPROPERTY()
	FVector ScreenLocation;
	UPROPERTY()
	FVector NorthDir;
	UPROPERTY()
	FVector NorthLocation;
	UPROPERTY()
	FVector PlayerLocation;
	UPROPERTY()
	FRotator MapRot;
	UPROPERTY()
	int32 PlayerYaw;
	UPROPERTY()
	int32 clipX;
	UPROPERTY()
	int32 clipY;
	UPROPERTY()
	float hw;
	UPROPERTY()
	float hh;
	UPROPERTY()
	float DotScale;
	UPROPERTY()
	float XPos;
	UPROPERTY()
	float YPos;
	UPROPERTY()
	float Width;
	UPROPERTY()
	float Height;
	UPROPERTY()
	float MapSize;
	UPROPERTY()
	float ScaleY;
	UPROPERTY()
	float AdjustedViewportHeight;





	UFUNCTION(BlueprintCallable, meta = (DisplayName = "InitSetting", Keywords = "InitSetting"), Category = "MAP")
	void InitSetting();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DrawPlayerIcon", Keywords = "DrawPlayerIcon"), Category = "MAP")
	void DrawPlayerIcon(UTexture * Icon);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DrawNorthIcon", Keywords = "DrawNorthIcon"), Category = "MAP")
	void DrawNorthIcon(UTexture * Icon, bool NoTextur);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DrawBackGround", Keywords = "DrawBackGround"), Category = "MAP")
	void DrawBackGround(UTexture * Textures);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DrawFullMap", Keywords = "DrawFullMap"), Category = "MAP")
	void DrawFullMap(UTexture * Map, UTexture * Mask);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DrawEnemy", Keywords = "DrawEnemy"), Category = "MAP")
	void DrawEnemy(UTexture * EnemyTextur);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DrawFrendly", Keywords = "DrawFrendly"), Category = "MAP")
	void DrawFrendly(UTexture * FrendlyTextur);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AddEnemy", Keywords = "AddEnemy"), Category = "MAP")
	void AddEnemy(Acpp_character * Enemy);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AddFrendly", Keywords = "AddFrendly"), Category = "MAP")
	void AddFrendly(Acpp_character * Frendly);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ClearLastEnemy", Keywords = "ClearLastEnemy"), Category = "MAP")
	void ClearLastEnemy();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DrawObjectiv", Keywords = "DrawObjectiv"), Category = "MAP")
	void DrawObjectiv(UTexture * ObjectivIcon);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AddObective", Keywords = "AddObective"), Category = "MAP")
	void AddObective(FVector Objects);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DeleteObjectiv", Keywords = "DeleteObjectiv"), Category = "MAP")
	void DeleteObjectiv(FVector Location);

	inline FVector2D MoveMap(FVector2D Loc);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ConvertWorldToMap", Keywords = "ConvertWorldToMap"), Category = "MAP")
	FVector2D ConvertWorldToMap(FVector location);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ConvertMapToMinimap", Keywords = "ConvertMapToMinimap"), Category = "MAP")
	FVector2D ConvertMapToMinimap(FVector2D Location);
};
