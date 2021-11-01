

#include "AL_HUD.h"


void AAL_HUD::BeginPlay()
{
	Super::BeginPlay();
}


FVector AAL_HUD::GetActorLoc(FVector InVector)
{
	FVector ScreenLocation, NewHUDLocation;
	float Scaling;

	ScreenLocation = InVector - MapCenter;

	if (ScreenLocation.SizeSquared() > FMath::Square(0.55 * RadarRange))
	{
		ScreenLocation = FVector(0.55*RadarRange * ScreenLocation.Normalize());
	}
	Scaling = RadarWidth / RadarRange;
	NewHUDLocation.X = MapCenter.X + (FVector::DotProduct(ScreenLocation, CenterPos));	//(FVector::DotProduct(ScreenLocation, MapRotX)* Scaling);
	NewHUDLocation.Y = MapCenter.Y + (FVector::DotProduct(ScreenLocation, CenterPos));
	NewHUDLocation.Z = 0.0;
	return NewHUDLocation;
}

FVector AAL_HUD::UpdateHUDLocation(FVector InLocation)
{
	FVector ScreenLocation, NewHUDLocation;
	float Scaling;

	ScreenLocation = InLocation - ActualMapCenter;

	if (ScreenLocation.SizeSquared() > FMath::Square(0.55 * RadarRange))
	{
		ScreenLocation = FVector(0.55*RadarRange * ScreenLocation.Normalize());		
	}
	Scaling = RadarWidth / RadarRange; 
	NewHUDLocation.X = MapCenter.X + (FVector::DotProduct(ScreenLocation, MapRotX)* Scaling);
	NewHUDLocation.Y = MapCenter.Y + (FVector::DotProduct(ScreenLocation, MapRotY)* Scaling);
	NewHUDLocation.Z = 0.0;
	return NewHUDLocation;
}

void AAL_HUD::ChangeMapRotation()
{
	FVector Z;

	if (CurrentMapRotYaw == MapRot.Yaw)
	{
		return;
	}
	
	GetAxes(MapRot, MapRotX, MapRotY, Z);
	CurrentMapRotYaw = MapRot.Yaw;
	return;
}

void AAL_HUD::DrawRing(UTexture * UL, UTexture * UR, UTexture * LL, UTexture * LR)
{
		// UL
		DrawTexture(UL, XPos, YPos, hw, hh, 0, MapDefaultSize, MapDefaultSize, MapDefaultSize, FLinearColor::White, EBlendMode::BLEND_Masked, 1, false, 0.f, FVector2D::ZeroVector);
		// UR
		DrawTexture(UR, XPos + hw, YPos, hw, hh, 0, MapDefaultSize, MapDefaultSize, MapDefaultSize, FLinearColor::White, EBlendMode::BLEND_Masked, 1, false, 0.f, FVector2D::ZeroVector);
		// LL
		DrawTexture(LL, XPos, YPos + hh, hw, hh, 0, MapDefaultSize, MapDefaultSize, MapDefaultSize, FLinearColor::White, EBlendMode::BLEND_Masked, 1, false, 0.f, FVector2D::ZeroVector);
		// LR
		DrawTexture(LR, XPos + hw, YPos + hh, hw, hh, 0, MapDefaultSize, MapDefaultSize, MapDefaultSize, FLinearColor::White, EBlendMode::BLEND_Masked, 1, false, 0.f, FVector2D::ZeroVector);
}

void AAL_HUD::SetWorldCentr(FVector Location)
{
	WorldCentr = Location;
}

void AAL_HUD::InitSetting()
{
	
	if ((PC = Cast<APlayerController>(GetOwner()))->IsValidLowLevelFast())
	{
		PC->GetViewportSize(clipX, clipY);
		
		AdjustedViewportHeight = clipY;
		ScaleY = AdjustedViewportHeight / clipY;// 768;
		Height = MapDefaultSize * ScaleY;
		Width = MapDefaultSize * ScaleY;

		XPos = clipX - (clipX * (1.0 - MapPosition.X)) - Width;	//	XPos = clipX - (clipX * (1.0 - MapPosition.X)) - Width;
		YPos = (AdjustedViewportHeight * MapPosition.Y);

		MapCenter = FVector(Height /2 , Width/2, 0.f);
	}
	else
	{
		AdjustedViewportHeight = MapCenter.Y;
		ScaleY = AdjustedViewportHeight / 768;
		Height = MapDefaultSize * ScaleY;
		Width = MapDefaultSize * ScaleY;

		XPos = MapCenter.X - (MapCenter.X * (1.0 - MapCenter.X)) - Width;
		YPos = (AdjustedViewportHeight * MapCenter.Y);
		UE_LOG(LogTemp, Warning, TEXT("PC not valid"));
	}
	MapSize = (MapCenter.X / AdjustedViewportHeight);
	ScreenLocation = (GetOwner() != NULL) ? GetOwner()->GetActorLocation() : GetOwner()->GetActorLocation();
	PlayerYaw = GetOwner()->GetActorRotation().Yaw *-1;
	RadarRange = FMax(RadarRange, 2.75 * (MapCenter - MapSize).Size2D());
	MapRot.Yaw = (MapYaw * 182.04444444);
	ActualMapCenter = MapCenter;
	RadarWidth = Width;

	// Draw the Ring
	hw = Width * 0.5;
	hh = Height * 0.5;


	PlayerLocation = FVector(XPos, YPos, 0.f) + MapCenter;
	MapRot.Yaw = PlayerYaw + 16384;// +16384;
}

void AAL_HUD::DrawPlayerIcon(UTexture * Icon)
{
	DrawTexture(Icon, PlayerLocation.X-8, PlayerLocation.Y, 16, 16, 0, 16, 1, 1, FLinearColor::White, EBlendMode::BLEND_Translucent, 1, false, PlayerYaw*-1, FVector2D(0.5, 0.5));

//	DrawTexture(Icon, XPos + MapCenter.X, YPos + MapCenter.Y, 16, 16, 0, 16, 1, 1, FLinearColor::White, EBlendMode::BLEND_Translucent, 1, false, -1, FVector2D::ZeroVector);
}
void AAL_HUD::DrawObjectiv(UTexture * ObjectivIcon)
{
	if (Objectiv.Num() > -1)
	{
		for (int i = 0; i < Objectiv.Num();i++)
		{
			FVector2D ObjLoc = ConvertMapToMinimap(ConvertWorldToMap(Objectiv[i].ObjectibLocation));
			float Dot = hw / 347;
			FVector Dir = FVector(Objectiv[i].ObjectibLocation.X, 0.f, 0.f);				//FVector((ObjLoc.X * 5), 0.f, 0.f) + FVector(ObjLoc, 0.f);
			FVector Location = CenterPos - 0.98 * hh * Dir;
			Location.X -= 29.5*Dot;
			Location.Y -= 33.5*Dot;
			DrawTexture(ObjectivIcon, Location.X + XPos, Location.Y + YPos, 18, 18, 0, 18, 1, 1, FLinearColor::White, EBlendMode::BLEND_Translucent, 1, false, 0, FVector2D::ZeroVector);
//			GEngine->AddOnScreenDebugMessage(0, 0, FColor::Magenta, FString::Printf(TEXT("Objectiv Draw")));
		}
	}

}
void AAL_HUD::AddObective(FVector Objects)
{
	FObectivStruct str;
	str.ObjectibLocation = Objects;
	str.ObjectivID = Objectiv.Num() + 1;
	Objectiv.Add(str);
}
void AAL_HUD::DeleteObjectiv(FVector Location)
{
//	Objectiv.IndexOfByKey(Location);
	/*for (int i = 0; i < Objectiv.Num();i++)
	{
		if (Objectiv.IndexOfByKey(Location) != INDEX_NONE)
		{
			Objectiv.Remove(Objectiv[i]);
			return;
		}
	}*/

}


FVector2D AAL_HUD::MoveMap(FVector2D Loc)
{
	FVector2D PlayerLoc = ConvertMapToMinimap(ConvertWorldToMap(GetOwner()->GetActorLocation()));
	return Loc - PlayerLoc;
}


void AAL_HUD::DrawNorthIcon(UTexture * Icon, bool NoTextur)
{
	// North indicator
	DotScale = hw / 347;
	NorthDir = MapRotY;
	NorthDir.X *= -1.0;
	NorthLocation = CenterPos - 0.98 * hh * NorthDir;
	NorthLocation.X -= 29.5*DotScale;
	NorthLocation.Y -= 33.5*DotScale;
//	NorthLocation.Rot
//	NorthLocation = MapCenter + NorthLocation.RotateAngleAxis(-1, GetOwner()->GetActorForwardVector());
//	NorthLocation = MapCenter + NorthLocation.RotateAngleAxis(NorthDir.X, GetOwner()->GetActorForwardVector());
	if (!NoTextur)
	{
		DrawTexture(Icon, NorthLocation.X + XPos+ MapCenter.X, NorthLocation.Y + YPos+ MapCenter.Y, 18, 18, 0, 18, 1, 1, FLinearColor::White, EBlendMode::BLEND_Translucent, 1, false, 0, FVector2D::ZeroVector);

//		DrawTexture(Icon, XPos + 5, YPos +5, 32, 32, 0, 32, 1, 1, FLinearColor::White, EBlendMode::BLEND_Translucent, 1, false, PlayerYaw*-1, FVector2D(3, 3));
	}
	else
	{
		DrawText("N", FLinearColor::Blue, NorthLocation.X + XPos, NorthLocation.Y + YPos);
	}
}

void AAL_HUD::DrawBackGround(UTexture * Textures)
{
	DrawTexture(Textures, XPos, YPos, MapDefaultSize, MapDefaultSize, 0, Textures->MaxTextureSize, 1, 1, FLinearColor::White, EBlendMode::BLEND_Translucent, 1, false, 0.f, FVector2D::ZeroVector);
}

void AAL_HUD::DrawFullMap(UTexture * Map, UTexture * Mask)
{
	if (!GetOwningPawn()->IsValidLowLevelFast()) return;
	FVector2D MapCoord = ConvertMapToMinimap(ConvertWorldToMap(GetOwningPawn()->GetActorLocation()));	//  - FVector2D(WorldCentr)		ConvertWorldToMap(
//	MapCoord /= (MapDefaultSize + MiniMapZoom);
	DrawTexture(Map, XPos, YPos, MapDefaultSize, MapDefaultSize, MapCoord.X - Map->MaxTextureSize - MiniMapZoom, MapCoord.Y - Map->MaxTextureSize - MiniMapZoom, MiniMapZoom, MiniMapZoom, FLinearColor::White, EBlendMode::BLEND_Translucent, 1, true, 0.f, FVector2D::ZeroVector);



//	DrawTexture(Map, XPos, YPos, MapDefaultSize-5, MapDefaultSize-5, 0, MapDefaultSize, MiniMapZoom, MiniMapZoom, FLinearColor::White, EBlendMode::BLEND_Translucent, 1, false, 0.f, FVector2D::ZeroVector);

	DrawTexture(Mask, XPos, YPos, MapDefaultSize, MapDefaultSize, 0, Mask->MaxTextureSize, 1, 1, FLinearColor::White, EBlendMode::BLEND_Masked, 1, false, 0.f, FVector2D::ZeroVector);

}

void AAL_HUD::DrawEnemy(UTexture * EnemyTextur)
{
	if (EnemyLocation.Num() > -1)
	{
		for (int i = 0;i < EnemyLocation.Num(); i++)
		{			
			if (EnemyLocation[i] == GetOwningPawn()) continue;
			if (FVector::Distance(EnemyLocation[i]->GetActorLocation(), WorldCentr) < 1150.f)
			{
				FVector2D loc;
				float PlayerYaw = EnemyLocation[i]->GetControlRotation().Yaw*-1;

				loc = ConvertMapToMinimap(ConvertWorldToMap(EnemyLocation[i]->GetActorLocation()));	// ConvertMapToMinimap(			
				DrawTexture(EnemyTextur, loc.X, loc.Y, 16, 16, 0, 16, 1, 1, FLinearColor::White, EBlendMode::BLEND_Translucent, 1, false, PlayerYaw, FVector2D(0.5, 0.5));
			}
			
		}
	}
	return;
}

void AAL_HUD::DrawFrendly(UTexture * FrendlyTextur)
{
	if (FrendlyLocation.Num() > -1)
	{
		for (int i = 0;i < FrendlyLocation.Num(); i++)
		{
			FVector2D loc;
			loc = ConvertMapToMinimap(ConvertWorldToMap(FrendlyLocation[i]->GetActorLocation())); //GetActorLoc(FrendlyLocation[i]->GetActorLocation().Projection());
			DrawTexture(FrendlyTextur, loc.X, loc.Y, 16, 16, 0, 16, 1, 1, FLinearColor::White, EBlendMode::BLEND_Translucent, 1, false, 0, FVector2D(0.5, 0.5));
		}
	}
	return;
}

void AAL_HUD::AddEnemy(Acpp_character * Enemy)
{
	EnemyLocation.Add(Enemy);
}

void AAL_HUD::AddFrendly(Acpp_character * Frendly)
{
	FrendlyLocation.Add(Frendly);
}

void AAL_HUD::ClearLastEnemy()
{
	EnemyLocation.Remove(EnemyLocation[EnemyLocation.Num()-1]);
}

FVector2D AAL_HUD::ConvertWorldToMap(FVector location)
{
	FVector Result = ScreenLocation - FVector(-MapDefaultSize, MapDefaultSize, 0.f);
	Result -= location;
	return FVector2D(Result.X, -Result.Y);
}

FVector2D AAL_HUD::ConvertMapToMinimap(FVector2D Location)
{
	Location = FVector2D(MapDefaultSize, MapDefaultSize) * FVector2D(Location.X / MapDefaultSize, Location.Y / MapDefaultSize);
	Location /= 10;
	return FVector2D(Location.X + XPos + (MapCenter.Y-33), Location.Y + YPos + (MapCenter.X-25));
}


void AAL_HUD::GetAxes(FRotator A, FVector& X, FVector& Y, FVector& Z)
{
	FRotationMatrix R(A);
	R.GetScaledAxes(X, Y, Z);
	return;
}

float AAL_HUD::FMax(float A, float B)
{
	if (A > B)
		return A;
	else
		return B;
}

void AAL_HUD::DrawHUD()
{
	Super::DrawHUD();
}
