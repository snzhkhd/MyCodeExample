

#include "weapon_item.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

Aweapon_item::Aweapon_item()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SKT_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DefaultMesh"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
//		= SKT_Mesh;
	SKT_Mesh->SetupAttachment(RootComponent);
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	NetPriority = 3.0f;
	NetUpdateFrequency = 100.f;
	if (bDebugFire)
	{
		AmmoInClip = AmmoClipSize;
	}
}
void Aweapon_item::PreReplication(IRepChangedPropertyTracker & ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);
}
void Aweapon_item::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME_CONDITION_NOTIFY(Aweapon_item, AmmoInClip, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME(Aweapon_item, AmmoInClip);
	DOREPLIFETIME(Aweapon_item, AmmoClipSize);
	DOREPLIFETIME(Aweapon_item, AmmoCount);
	DOREPLIFETIME(Aweapon_item, bReload);
	
	DOREPLIFETIME(Aweapon_item, bWeaponPutDown);
	DOREPLIFETIME(Aweapon_item, bWeaponEquip);

	DOREPLIFETIME(Aweapon_item, EffectHitLocation);
	DOREPLIFETIME(Aweapon_item, WeaponState);

	DOREPLIFETIME(Aweapon_item, Firing);
	
	DOREPLIFETIME(Aweapon_item, FireTimer);
	DOREPLIFETIME(Aweapon_item, PutDown);
	DOREPLIFETIME(Aweapon_item, Equipping);
	DOREPLIFETIME(Aweapon_item, WeaponRange);
	
}

// Called when the game starts or when spawned
void Aweapon_item::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void Aweapon_item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void Aweapon_item::OnRep_AmmoInClip()
{
	if (!Instigator) return;
	if (CanFire())
	{
		if (Role < ROLE_Authority)
		{
			PlayFireEffects(CurrentFireMode, FVector::ZeroVector);
			return;
//			ServerFireAmmunition();
		}
//		CallWeaponFire();
	}
}


void Aweapon_item::StartFire(uint8 FireModeNum)
{
	if (GetNetMode() == NM_Client) return;
	if (Instigator != NULL)
	{
		ConsumeAmmo(1);
		//BeginFire(FireModeNum);
	}
}

void Aweapon_item::BeginFire(uint8 FireModeNum)
{
	SetPendingFire(FireModeNum);
}

void Aweapon_item::StopFire(uint8 FireModeNum)
{
	EndFire(FireModeNum);
}

void Aweapon_item::EndFire(uint8 FireModeNum)
{
	Active();
	StopFireEffects(1);
	ClearPendingFire(FireModeNum);	
}

void Aweapon_item::ForceEndFire()
{
	EndFire(0);
	EndFire(1);
}

void Aweapon_item::SendToFiringState(uint8 FireModeNum)
{
	SetCurrentFireMode(FireModeNum);
}

void Aweapon_item::SetCurrentFireMode(uint8 FiringModeNum)
{
	CurrentFireMode = FiringModeNum;	
}

void Aweapon_item::FireModeUpdated(uint8 FiringMode)
{
}

void Aweapon_item::ServerFireAmmunition_Implementation()
{
	if (GetNetMode() == NM_Client) return;
	UE_LOG(LogTemp, Warning, TEXT("ServerFireAmmunition -> GetNetMode == SERVER "));
	if (!CanFire()) return;
	WeaponFiring();
//	TimeWeaponFiring(1);
	switch (WeaponFireTypes)
	{
	case EWFT_InstantHit:
		InstantFire();
		break;

	case EWFT_Projectile:
		ProjectileFire();
		break;

	case EWFT_Custom:
		CustomFire();
		break;
	}
}

bool Aweapon_item::ServerFireAmmunition_Validate()
{
	return true;
}
void Aweapon_item::FireAmmunition()
{
	if (Role < ROLE_Authority)
	{
		PlayFireEffects(CurrentFireMode,FVector::ZeroVector);
//		UE_LOG(LogTemp, Warning, TEXT("FireAmmunition -> GetNetMode == CLIENT"));
//		ServerFireAmmunition();
		return;
	}	
	return;
}

void Aweapon_item::CallWeaponFire()
{
	if (GetNetMode() == NM_Client) return;
	UE_LOG(LogTemp, Warning, TEXT("ServerFireAmmunition -> GetNetMode == SERVER "));
	if (!CanFire()) return;
	WeaponFiring();
	//	TimeWeaponFiring(1);
	switch (WeaponFireTypes)
	{
	case EWFT_InstantHit:
		InstantFire();
		break;

	case EWFT_Projectile:
		ProjectileFire();
		break;

	case EWFT_Custom:
		CustomFire();
		break;
	}
}

FRotator Aweapon_item::GetAdjustedAim(FVector StartFireLoc)
{
	FRotator R;
	if (Instigator != NULL)
	{
		R = Instigator->GetBaseAimRotation();
		return AddSpread(R);
	}
	return FRotator::ZeroRotator;
}

float Aweapon_item::GetTraceRange()
{
	return WeaponRange;
}

Acpp_character* Aweapon_item::GetTraceOwner()
{
	return Cast<Acpp_character>(GetInstigator());
//	return (Instigator != NULL) ? Instigator : this;
}

FHitResult Aweapon_item::CalcWeaponFire(FVector StartTrace, FVector EndTrace)
{
	FHitResult HitResult;
	if (bDebugFire)
	{
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Emerald, true, -1, 0, 10);
	}	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Instigator);
	QueryParams.bTraceAsyncScene = true;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECollisionChannel::ECC_Camera, QueryParams))
	{
		
	}
	return HitResult;
}
void Aweapon_item::ServerInstantFire_Implementation()
{
	InstantFire();
	return;
}

bool Aweapon_item::ServerInstantFire_Validate()
{
	return true;
}

void Aweapon_item::InstantFire()
{
	if (GetNetMode() == NM_Client) return;
	if (!Instigator) return;
	FVector	StartTrace, EndTrace;
	FHitResult RealImpact;
	StartTrace = Instigator->GetPawnViewLocation();
	for (int i = 0; i < ShotCost; i++)
	{
		FRotator Rot = Instigator->GetViewRotation();//GetAdjustedAim(StartTrace);
		EndTrace = StartTrace + Rot.Vector() * GetTraceRange();
		RealImpact = CalcWeaponFire(StartTrace, EndTrace);
		SetFlashLocation(RealImpact.Location);
		ProcessInstantHit(CurrentFireMode, RealImpact, 1);
	}

}

void Aweapon_item::ProcessInstantHit(uint8 FiringMode, FHitResult Impact, int32 NumHits)
{
	if (Impact.Actor.IsValid())
	{
		if (bDebugFire)
		{
			if(Impact.Actor != NULL)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Hit Actor = "), Impact.Actor->GetName()));
		}
		if (Cast<Acpp_character>(Impact.Actor)->IsValidLowLevelFast())
		{
			UE_LOG(LogTemp, Warning, TEXT("Cast Acpp_character success"));
//			Acpp_character* HitActor = Cast<Acpp_character>(Impact.Actor);
			
			if (InstantHitDamage > 0)
			{
				TSubclassOf<UDamageType> const ValidDamageTypeClass = InstantHitDamageTypes ? InstantHitDamageTypes : TSubclassOf<Ubase_damage_type>(Ubase_damage_type::StaticClass());
				FDamageEvent DamageEvent(ValidDamageTypeClass);
				Impact.Actor->TakeDamage(InstantHitDamage, DamageEvent, Instigator->GetController(),this);
			}
				
			//	Impact.Actor->Apply
				
			//	Cast<Acpp_character>(Impact.Actor)->ApplyDamageMomentum(InstantHitDamage, FDamageEvent::FDamageEvent(), Instigator, this);
			return;			
		}
		UE_LOG(LogTemp, Warning, TEXT("Cast Acpp_character fail"));
	}
}

Aproj_base * Aweapon_item::ProjectileFire()
{
	if (GetNetMode() == NM_Client) return NULL;
	FVector StartTrace, EndTrace, RealStartLoc, AimDir;
	FActorSpawnParameters SpawnInfo;
	FHitResult TestImpact;
	Aproj_base *SpawnedProjectile;
//	
	IncrementFlashCount();
	StartTrace = Instigator->GetPawnViewLocation();	
	AimDir =(GetAdjustedAim(StartTrace)).Vector();
	RealStartLoc = GetPhysicalFireStartLoc(AimDir);
	if (StartTrace != RealStartLoc)
	{
		EndTrace = StartTrace + AimDir * GetTraceRange();
		TestImpact = CalcWeaponFire(StartTrace, EndTrace);

		AimDir = TestImpact.Location - RealStartLoc;
	}
	SpawnedProjectile = GetWorld()->SpawnActor<Aproj_base>(GetProjectileClass(), GetActorLocation(), GetActorRotation(), SpawnInfo);
	if (SpawnedProjectile->IsValidLowLevelFast())
	{
		SpawnedProjectile->Init(AimDir);
		return  SpawnedProjectile;
	}
	UE_LOG(LogTemp, Warning, TEXT("Projectile class not valid"));
	return  NULL;
}

void Aweapon_item::CustomFire()
{
}

FVector Aweapon_item::GetMuzzleLoc()
{
	if (Instigator != NULL)
	{
		return Instigator->GetPawnViewLocation() + (FireOffset +  Instigator->GetViewRotation().Vector());
	}
	UE_LOG(LogTemp, Warning, TEXT("Instigator not valid"));
	return FVector::ZeroVector;
}

FVector Aweapon_item::GetPhysicalFireStartLoc(FVector AimDir)
{
	return FVector();
}

bool Aweapon_item::TryPutDown()
{
	bWeaponPutDown = true;
	return true;
}

void Aweapon_item::Inactive()
{
	WeaponState = EWS_Inactive;
	Firing = false;
}

void Aweapon_item::Active()
{
	WeaponState = EWS_Active;
	Firing = false;
}

void Aweapon_item::WeaponFiring()
{
	UE_LOG(LogTemp, Warning, TEXT("WeaponFiring-----------------------------------------"));
	WeaponState = EWS_Fire;
	Firing = true;
}

void Aweapon_item::HandleFinishedFiring()
{
	WeaponState = EWS_Idle;
	Firing = false;
}

bool Aweapon_item::ShouldRefire()
{
	if (!HasAmmo(CurrentFireMode))
	{
		return false;
	}

	// refire if owner is still willing to fire
	return StillFiring(CurrentFireMode);
}

bool Aweapon_item::StillFiring(uint8 FireMode)
{
	return (PendingFire(FireMode));
}

void Aweapon_item::WeaponEquipping()
{
	bWeaponEquip = true;
	WeaponState = EWS_Equipping;
}

void Aweapon_item::WeaponPuttingDown()
{
	PutDownWeapon();
	WeaponState = EWS_PuttingDown;
}

void Aweapon_item::PutDownEnd()
{
	bWeaponPutDown = false;
}

void Aweapon_item::EquippingEnd()
{	
	Activate();
}

bool Aweapon_item::CanFire()							
{
	if (Firing)
	{
		UE_LOG(LogTemp, Warning, TEXT("Firing true"));
		return false;
	}
	if (WeaponFireTypes == EWS_Inactive || WeaponFireTypes == EWeaponFireType::EWFT_None)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponFireTypes == Inactive or None"));
		return false;
	}
	if (IsReload())
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponFireTypes == Inactive or None"));
		return false;
	}

	if (WeaponFireTypes == EWeaponFireType::EWFT_Projectile)
	{
		return  AmmoInClip > 0 && Projectile != NULL;
	}
	if (FireTimer.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("FireTimer is activ"));
		return false;
	}
	else 
	{
		return AmmoInClip > 0;
	}
	return false;
}

bool Aweapon_item::CanReload()
{
	return AmmoInClip < AmmoClipSize && AmmoCount != 0;
}

bool Aweapon_item::IsReload()
{
	return bReload;
}

void Aweapon_item::Reload()
{

	if (AmmoInClip == 0)
	{
		if (AmmoCount >= AmmoClipSize)
		{
			AmmoInClip = AmmoClipSize;
			AmmoCount = AmmoCount - AmmoClipSize;
			return;
		}
		AmmoInClip = AmmoClipSize;
		AmmoCount = 0;
		return;
	}
	AmmoCount = AmmoCount + AmmoInClip;
	if (AmmoCount >= AmmoClipSize)
	{
		AmmoInClip = AmmoClipSize;
		AmmoCount = AmmoCount - AmmoClipSize;
		return;
	}
	AmmoInClip = AmmoClipSize;
	AmmoCount = 0;
	return;

}

void Aweapon_item::HolderDied()
{
	ForceEndFire();
}

bool Aweapon_item::IsActiveWeapon()
{
	return WeaponState != EWS_Inactive;
}

bool Aweapon_item::DoOverrideNextWeapon()
{
	return false;
}

bool Aweapon_item::DoOverridePrevWeapon()
{
	return false;
}

bool Aweapon_item::CanThrow()
{
	return bCanThrow;
}

void Aweapon_item::WeaponThrown()
{
	Inactive();
	ForceEndFire();
	DetachWeapon();
}

int32 Aweapon_item::GetPendingFireLength()
{
	return 0;
}

bool Aweapon_item::IsFiring()
{
	return Firing;
}

void Aweapon_item::ConsumeAmmo(uint8 FireModeNum)
{
	AddAmmo(-ShotCost);	
}

int32 Aweapon_item::AddAmmo(int32 Amount)
{
	AmmoInClip = AmmoInClip + Amount;
	return AmmoInClip;
}

bool Aweapon_item::HasAmmo(uint8 FireModeNum)
{
	return (AmmoInClip > 0);	
}

bool Aweapon_item::PendingFire(uint8 FireMode)
{
	return false;
}

void Aweapon_item::SetPendingFire(uint8 FireMode)
{

}

void Aweapon_item::ClearPendingFire(uint8 FireMode)
{
}

TSubclassOf<class Aproj_base> Aweapon_item::GetProjectileClass()
{
	return Projectile;
}

FRotator Aweapon_item::AddSpread(FRotator BaseAim)
{
	FRotator rot;

	rot.Yaw = BaseAim.Yaw + FMath::RandRange(-1, 1);
	rot.Pitch = BaseAim.Pitch + FMath::RandRange(-1, 1);
	rot.Roll = BaseAim.Roll + FMath::RandRange(-1, 1);

	return rot;
	//float CurrentSpread, RandY, RandZ;

	//CurrentSpread = Spread;
	//if (CurrentSpread == 0)
	//{
	//	return BaseAim;
	//}
	//else
	//{	
	//	// Add in any spread.		
	//	BaseAim = Instigator->GetBaseAimRotation();
	//	GetViewAxes(X, Y, Z);
	//	RandY = FMath::SRand() - 0.5;
	//	RandZ = FMath::Sqrt(0.5 - FMath::Square(RandY)) * (FMath::SRand() - 0.5);
	//	return (X + RandY * CurrentSpread * Y + RandZ * CurrentSpread * Z).Rotation();
	//}
}

float Aweapon_item::MaxRange()
{
	return WeaponRange;	
}

void Aweapon_item::WeaponPlaySound(USoundCue *Sound)
{
	if (!Sound )
	{
		UE_LOG(LogTemp, Warning, TEXT("Sound not valid"));
		return;
	}
	if (!Instigator)
	{
		UE_LOG(LogTemp, Warning, TEXT("Instigator not valid"));
		return;
	}
	UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation());	
	
}

void Aweapon_item::PlayFireEffects(uint8 FireModeNum, FVector optional_HitLocation)
{
	
	
	PlayFiringSound();
	// Start muzzle flash effect
	CauseMuzzleFlash();
	if (GetInstigatorController()->IsValidLowLevelFast())
	{
		if (GetInstigatorController()->IsLocalPlayerController())
		{
			ShakeView();
		}
		return;
	}	
	return;
}

void Aweapon_item::StopFireEffects(uint8 FireModeNum)
{
	StopMuzzleFlash();
}

void Aweapon_item::PlayFiringSound()
{
	WeaponPlaySound(FireSound);
}

void Aweapon_item::CauseMuzzleFlash()
{										///   MuzzleFlashPSC, Mesh, MuzzleFlashSocket,  NULL, NULL, NULL, EAttachLocation::SnapToTarget, true, EPSCPoolMethod::ManualRelease
	MuzzleFlashComp = UGameplayStatics::SpawnEmitterAttached(MuzzleFlashPSC, SKT_Mesh, MuzzleFlashSocket, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true, EPSCPoolMethod::AutoRelease);
	if (!MuzzleFlashComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Muzzle Flash class not valid"));				
	}
//	MuzzleFlashPSC.OnSpaw
}

void Aweapon_item::StopMuzzleFlash()
{
	if (MuzzleFlashPSC)
	{
		MuzzleFlashPSC->BeginDestroy();
	}
}

void Aweapon_item::ShakeView()
{
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(FireShake, 1.f);
}

float Aweapon_item::GetFireInterval(uint8 FireModeNum)
{
	return FireInterval;
}

void Aweapon_item::TimeWeaponFiring(uint8 FireModeNum)
{
	UE_LOG(LogTemp, Warning, TEXT("TimeWeaponFiring start"));

//	FTimerDelegate TimerDel;
	/*
	
	//Binding the function with specific variables
	TimerDel.BindUFunction(this, FName("MyUsefulFunction"), MyInt, MyFloat);
	//Calling MyUsefulFunction after 5 seconds without looping
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 5.f, false);
	
	*/	
	if (!FireTimer.IsValid())
	{
//		FTimerDelegate FireTimerDelegate;
//		FireTimerDelegate.BindUFunction(this, FName("RefireCheckTimer"));
//		GetWorldTimerManager().SetTimer(Equipping, FireTimerDelegate, false, FireInterval);
		GetWorldTimerManager().SetTimer(FireTimer, this, &Aweapon_item::RefireCheckTimer, FireInterval, false);
	}
//		GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &Aweapon_item::RefireCheckTimer, FireInterval, false);
	
}

void Aweapon_item::RefireCheckTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("TimeWeaponFiring end"));
	StopFire(1);
	if (GetWorldTimerManager().IsTimerActive(FireTimer))
	{
		GetWorldTimerManager().ClearTimer(FireTimer);
	}
	if (!Instigator) return;
	if (Cast<Acpp_character>(Instigator))
	{
		if (CanFire())
		{
			UE_LOG(LogTemp, Warning, TEXT("CanFire = true"));
		//	StartFire(1);
		}
		
		// DO TO Fire Again
	}
	UE_LOG(LogTemp, Warning, TEXT("Cast<Acpp_character> fail"));
}

void Aweapon_item::TimeWeaponPutDown()
{
	if (!PutDown.IsValid())
	{
		WeaponPuttingDown();
		GetWorld()->GetTimerManager().SetTimer(PutDown, this, &Aweapon_item::PutDownEnd, PutDownTime, false);
	}
}

void Aweapon_item::TimeWeaponEquipping()
{
	if (!Equipping.IsValid())
	{
		WeaponEquipping();
		
		GetWorld()->GetTimerManager().SetTimer(Equipping, this, &Aweapon_item::EquippingEnd, EquipTime, false);
	}
}

void Aweapon_item::Activate()
{
	bWeaponEquip = false;
}

void Aweapon_item::PutDownWeapon()
{
	bWeaponPutDown = true;
}

void Aweapon_item::WeaponEmpty()
{
	if (IsFiring())
	{
		ForceEndFire();
	}
}

void Aweapon_item::IncrementFlashCount()
{
}

void Aweapon_item::ClearFlashCount()
{
}

void Aweapon_item::SetFlashLocation(FVector HitLocation)
{
	if (HitLocation.IsZero())
		ForceEndFire();
	else
	{
		EffectHitLocation = HitLocation;
	}
		
}

void Aweapon_item::ClearFlashLocation()
{
}

void Aweapon_item::AttachWeaponTo(UPrimitiveComponent * MeshCpnt, FName SocketName)
{
	if (Instigator)
	{
		SetActorEnableCollision(false);
		AttachToComponent(MeshCpnt, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}
}

void Aweapon_item::DetachWeapon()
{
//	SetActorEnableCollision(false);
	//Instigator->AttachToComponent(MeshCpnt, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
}

void Aweapon_item::GetViewAxes(FVector & XAxis, FVector & YAxis, FVector & ZAxis)
{
	if (Instigator)
	{
		FVector out = Instigator->GetPawnViewLocation();
		XAxis = FVector(out.X);
		YAxis = FVector(out.Y);
		ZAxis = FVector(out.Z);
	}
	
	
}

float Aweapon_item::AdjustFOVAngle(float FOVAngle)
{
	return FOVAngle;
}

void Aweapon_item::WeaponCalcCamera(float fDeltaTime, FVector & out_CamLoc, FRotator & out_CamRot)
{
}

void Aweapon_item::SetReloadState()
{
	WeaponState = EWeaponState::EWS_Reload;
}