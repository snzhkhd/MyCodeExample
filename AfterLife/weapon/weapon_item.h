

#pragma once
#include "inventory_item.h"
#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "proj_base.h"
#include "base_damage_type.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "weapon_item.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum EWeaponState
{
	EWS_Active UMETA(DisplayName = "Active"),
	EWS_Inactive UMETA(DisplayName = "Inactive"),
	EWS_Reload UMETA(DisplayName = "Reload"),
	EWS_Fire UMETA(DisplayName = "Fire"),
	EWS_Idle UMETA(DisplayName = "Idle"),
	EWS_Equipping UMETA(DisplayName = "Equipping"),
	EWS_PuttingDown UMETA(DisplayName = "PuttingDown")
};

UENUM(BlueprintType)
enum EWeaponFireType
{
	EWFT_InstantHit UMETA(DisplayName = "InstantHit"),
	EWFT_Projectile UMETA(DisplayName = "Projectile"),
	EWFT_Custom UMETA(DisplayName = "Custom"),
	EWFT_None UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum EZoomState
{
	ZST_NotZoomed UMETA(DisplayName = "NotZoomed"),
	ZST_ZoomingOut UMETA(DisplayName = "ZoomingOut"),
	ZST_ZoomingIn UMETA(DisplayName = "ZoomingIn"),
	ZST_Zoomed UMETA(DisplayName = "Zoomed"),
};


UCLASS()
class AFTERLIFE_API Aweapon_item : public Ainventory_item
{
	GENERATED_BODY()

public:
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	virtual void PreReplication(IRepChangedPropertyTracker & ChangedPropertyTracker) override;

	// Sets default values for this actor's properties
	Aweapon_item();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Zoom")
	uint8 ZoomedFireModeNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Zoom")
	float	ZoomedTargetFOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Zoom")
	float	ZoomedRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Zoom")
	float 	ZoomFadeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Sound")
		USoundCue * FireSound;
	/** Sounds to play when zooming begins/ends/etc. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Sound")
	USoundCue * ZoomInSound;
	/** Sounds to play when zooming begins/ends/etc. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Sound")
	USoundCue * ZoomOutSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Sound")
	USoundCue * WeaponPutDownSnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Sound")
	USoundCue * WeaponEquipSnd;

	UPROPERTY(Replicated)
	bool Firing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Fire")
	int32 ShotCost = 1;

	UPROPERTY()
	uint8 CurrentFireMode;

	UPROPERTY(replicatedUsing = OnRep_AmmoInClip, BlueprintReadOnly, Category = "Settings | Ammo")
	int32 AmmoInClip;

	UFUNCTION()
		virtual void OnRep_AmmoInClip();

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Settings | Ammo")
	int32 AmmoClipSize = 30;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Settings | Ammo")
	int32 AmmoCount = 180;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Settings | Ammo")
	bool bReload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Effect")
	TArray<FName> EffectSockets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Effect")
	FName MuzzleFlashSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Effect")
		UParticleSystem * MuzzleFlashPSC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Effect")
	UParticleSystemComponent* MuzzleFlashComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Effect")
		TSubclassOf<UCameraShake> FireShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TEnumAsByte<EWeaponFireType> WeaponFireTypes = EWeaponFireType::EWFT_None;

	UPROPERTY(Replicated)
		TEnumAsByte<EWeaponState> WeaponState = EWeaponState::EWS_Idle;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Projectile")
	TSubclassOf<class Aproj_base> Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings | Projectile")
	float ProjectileSpawnOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bSuppressSounds;
	/** Holds the amount of time a single shot takes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float FireInterval;
	/** How much of a spread between shots */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float Spread;
	/** How much damage does a given instanthit shot do */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float InstantHitDamage;
	/** momentum transfer scaling for instant hit damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float InstantHitMomentum;
	/** DamageTypes for Instant Hit Weapons */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		TSubclassOf<Ubase_damage_type>  InstantHitDamageTypes;
	
	/** How long does it take to Equip this weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float	EquipTime;
	/** How long does it take to put this weapon down */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float	PutDownTime;
	/** Holds an offest for spawning protectile effects. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FVector	FireOffset;
	/** Set to put weapon down at the end of a state. Typically used to change weapons on state changes (weapon up, stopped firing...) */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "PutDown")
		bool	bWeaponPutDown;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Equip")
		bool	bWeaponEquip;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Effect")
	FVector EffectHitLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SKT_Mesh")
	USkeletalMeshComponent* SKT_Mesh;

	UPROPERTY()
	UPrimitiveComponent* Mesh;
	/** Can player toss his weapon out? Typically false for default inventory. */
	UPROPERTY()
	bool	bCanThrow;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Timer")
	FTimerHandle FireTimer;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Timer")
		FTimerHandle PutDown;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Timer")
		FTimerHandle Equipping;
/*----------------------------------DEBUG----------------------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		bool bDebugFire = true;

	/** Range of Weapon, used for Traces (InstantFire, ProjectileFire, AdjustAim...) */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Settings")
		float	WeaponRange = 5000;
public:

	void PutDownEnd();

	/*	-> Activate	*/
	void EquippingEnd();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "CanFire", Keywords = "CanFire"), Category = "weapon | Fire")
	bool CanFire();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "CanReload", Keywords = "CanReload"), Category = "weapon | Reload")
	bool CanReload();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "IsReload", Keywords = "IsReload"), Category = "weapon | Reload")
	bool IsReload();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reload", Keywords = "Reload"), Category = "weapon | Reload")
		void Reload();

 /**
  * Pawn holding this weapon as active weapon just died.

	-> ForceEndFire
  */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reload", Keywords = "Reload"), Category = "weapon | Other")
		void HolderDied();

/**
 * Informs if this weapon is active for the player
 *
 * @return	true if this an active weapon for the player
 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "IsActiveWeapon", Keywords = "IsActiveWeapon"), Category = "weapon | Other")
	bool IsActiveWeapon();

	/**
	 * hook to override Next weapon call.
	 * For example the physics gun uses it to have mouse wheel change the distance of the held object.
	 * Warning: only use in firing state, otherwise it breaks weapon switching
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "DoOverrideNextWeapon", Keywords = "DoOverrideNextWeapon"), Category = "weapon | Equiping")
	bool DoOverrideNextWeapon();

/**
 * hook to override Previous weapon call.
 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "DoOverridePrevWeapon", Keywords = "DoOverridePrevWeapon"), Category = "weapon | Equiping")
	bool DoOverridePrevWeapon();

/**
 * Returns true if this item can be thrown out.
 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "CanThrow", Keywords = "CanThrow"), Category = "weapon | Other")
	bool CanThrow();

	/* 
	-> Inactive
	-> ForceEndFire
	-> DetachWeapon	
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "WeaponThrown", Keywords = "WeaponThrown"), Category = "weapon | Other")
		void WeaponThrown();

/*********************************************************************************************
 * Pending Fire / Inv Manager
 *********************************************************************************************/
	int32 GetPendingFireLength();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "IsFiring", Keywords = "IsFiring"), Category = "weapon | Fire")
	bool IsFiring();

	/*		AddAmmo(-ShotCost)		*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Consume Ammo", Keywords = "Consume Ammo"), Category = "weapon | Ammo")
	void ConsumeAmmo(uint8 FireModeNum);

	/*	AmmoClipSize = FMath::Clamp(AmmoClipSize + Amount, 0, AmmoClipSize);	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AddAmmo", Keywords = "AddAmmo"), Category = "weapon | Ammo")
	int32 AddAmmo(int32 Amount);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "HasAmmo", Keywords = "HasAmmo"), Category = "weapon | Ammo")
	bool HasAmmo(uint8 FireModeNum);

	/*		-> Instigator->IsPendingFire	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Pending Fire", Keywords = "Pending Fire"), Category = "weapon | Fire")
	bool PendingFire(uint8 FireMode);

	/*		-> Instigator->SetPendingFire		*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Pending Fire", Keywords = "Set Pending Fire"), Category = "weapon | Fire")
	void SetPendingFire(uint8 FireMode);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Clear Pending Fire", Keywords = "Clear Pending Fire"), Category = "weapon | Fire")
	void ClearPendingFire(uint8 FireMode);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetProjectileClass", Keywords = "GetProjectileClass"), Category = "weapon | Fire")
	TSubclassOf<class Aproj_base> GetProjectileClass();

/**
 * Adds any fire spread offset to the passed in rotator
 * @param Aim the base aim direction
 * @return the adjusted aim direction
 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Add Spread", Keywords = "Add Spread"), Category = "weapon | Fire")
	FRotator AddSpread(FRotator BaseAim);

/**
* Returns the Maximum Range for this weapon
*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Max Range", Keywords = "Get Max Range"), Category = "weapon | Other")
	float MaxRange();


/*********************************************************************************************
* Effects / Mesh / Animations / Sounds
*********************************************************************************************/

	/*		Play Sound At Location	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Weapon Play  Sound", Keywords = "Weapon Play  Sound"), Category = "weapon | Effects")
	void WeaponPlaySound(USoundCue * Sound);

	/*		-> CauseMuzzleFlash -> ShakeView	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Play Fire Effects", Keywords = "Play Fire Effects"), Category = "weapon | Effects")
	void PlayFireEffects(uint8 FireModeNum, FVector optional_HitLocation);


	/*		StopMuzzleFlash	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Stop Fire Effects", Keywords = "Stop Fire Effects"), Category = "weapon | Effects")
	void StopFireEffects(uint8 FireModeNum);

	/*		WeaponPlaySound(FireSound)	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Play Firing Sound", Keywords = "Play Firing Sound"), Category = "weapon | Effects")
	void PlayFiringSound();

	/*		Spawn Emitter Attached	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Cause Muzzle Flash", Keywords = "Cause Muzzle Flash"), Category = "weapon | Effects")
	void CauseMuzzleFlash();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Stop Muzzle Flash", Keywords = "Stop Muzzle Flash"), Category = "weapon | Effects")
	void StopMuzzleFlash();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Shake View", Keywords = "Shake View"), Category = "weapon | Effects")
	void ShakeView();
/*********************************************************************************************
* Timing
*********************************************************************************************/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Fire Interval", Keywords = "Get Fire Interval"), Category = "weapon | Timing")
	float GetFireInterval(uint8 FireModeNum);

	/*		SetTimer(FireTimer	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Time Weapon Firing", Keywords = "Time Weapon Firing"), Category = "weapon | Timing")
	void TimeWeaponFiring(uint8 FireModeNum);

	/*	ClearTimer(FireTimer
		DO TO Fire Again
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RefireCheckTimer", Keywords = "RefireCheckTimer"), Category = "weapon | Timing")
	void RefireCheckTimer();

	/*		-> WeaponPuttingDown	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TimeWeaponPutDown", Keywords = "TimeWeaponPutDown"), Category = "weapon | Timing")
	void TimeWeaponPutDown();

	/*		-> WeaponEquipping		*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TimeWeaponEquipping", Keywords = "TimeWeaponEquipping"), Category = "weapon | Timing")
	void TimeWeaponEquipping();

/**
 * All inventory use the Activate() function when an item is selected for use.
 * For weapons, this function starts the Equipping process. If the weapon is the inactive state,
 * it will go to the 'WeaponEquipping' followed by 'Active' state, and ready to be fired.
 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Activate", Keywords = "Activate"), Category = "weapon | Equiping")
		void Activate();
	/*PutDownWeapon	 bWeaponPutDown = true */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PutDownWeapon", Keywords = "PutDownWeapon"), Category = "weapon | Equiping")
		void PutDownWeapon();


/**
* Called when the weapon runs out of ammo during firing
	-> ForceEndFire
 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "WeaponEmpty", Keywords = "WeaponEmpty"), Category = "weapon | Ammo")
		void WeaponEmpty();


/**
 * Increment Pawn's FlashCount variable.
* This is used to play weapon fire effects on remote clients.
* Call this on the server and local player.
*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IncrementFlashCount", Keywords = "IncrementFlashCount"), Category = "weapon | Fire")
	void IncrementFlashCount();

/**
* Clear flashCount variable on Pawn. and call WeaponStoppedFiring event.
* Call this on the server and local player.
*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ClearFlashCount", Keywords = "ClearFlashCount"), Category = "weapon | Fire")
	void ClearFlashCount();


/**
* This function sets up the Location of a hit to be replicated to all remote clients.

		if (HitLocation.IsZero())
		ForceEndFire();
 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetFlashLocation", Keywords = "SetFlashLocation"), Category = "weapon | Effects")
	void SetFlashLocation(FVector HitLocation);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ClearFlashLocation", Keywords = "ClearFlashLocation"), Category = "weapon | Effects")
	void ClearFlashLocation();

/**
* AttachWeaponTo is called when it's time to attach the weapon's mesh to a location.
* it should be subclassed.
*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Attach Weapon To", Keywords = "Attach Weapon To"), Category = "weapon | Equiping")
	void AttachWeaponTo(UPrimitiveComponent * MeshCpnt, FName SocketName);

/*Detach weapon components from instigator. Perform any clean up. */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Detach Weapon", Keywords = "Detach Weapon"), Category = "weapon | Equiping")
	void DetachWeapon();

/** Returns the base view aim of the weapon owner */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetViewAxes", Keywords = "GetViewAxes"), Category = "weapon | Other")
	void GetViewAxes(FVector & XAxis, FVector & YAxis, FVector & ZAxis);

/**
* This function can be used by a weapon to override a playercontroller's FOVAngle.  It should
* be overriden in a subclass.
*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Adjust FOV Angle", Keywords = "Adjust FOV Angle"), Category = "weapon | Other")
	float AdjustFOVAngle(float FOVAngle);

/**
* WeaponCalcCamera allows a weapon to adjust the pawn's controller's camera.  Should be subclassed
*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "WeaponCalcCamera", Keywords = "WeaponCalcCamera"), Category = "weapon | Other")
	void WeaponCalcCamera(float fDeltaTime, FVector & out_CamLoc, FRotator & out_CamRot);



/*********************************************************************************************
* Handling the actual Fire Commands
*********************************************************************************************/

/* Weapon Firing Logic overiew:

		 The weapon system here is designed to be a single code path that follows the same flow on both
		 the Authoritive server and the local client.  Remote clients know nothing about the weapon and utilize
		 the WeaponAttachment system to see the end results.


		 1: The InventoryManager (IM) on the Local Client recieves a StartFire call.  It calls StartFire().

		 2: If Local Client is not Authoritive it notifies the server via ServerStartFire().

		 3: Both StartFire() and ServerStartFire() sync up by calling BeginFire().

		 4: BeginFire sets the PendingFire flag for the incoming fire Mode

		 5: BeginFire looks at the current state and if it's in the Active state, it begins the
			firing sequence by transitioning to the new fire state as defined by the FiringStatesArray
			array.  This is done by called SendToFiringState.

		 6: The Firing Logic is handled in the various firing states.  Firing states are responsible for the
			following:
						 a: Continuing to fire if their associated PendingFire is hot
						 b: Transitioning to a new weapon when out of ammo
						 c: Transitioning to the "Active" state when no longer firing


		 The weapon system also receives a StopFire() event from the IM.  When this occurs, the following
		 logic is performed:

		 1: The IM on the Local Client calls StopFire().

		 2: If Weapon Stop fire is not on the Authoritive process, it notifes the server via the
			ServerStopFire() event.

		 3: Both StopFire() and ServerStopFire() sync up by calling EndFire().

		 4: EndFire() clears the PendingFire flag for this outgoing fire mode.


		 Firing states should be identical in their execution, branching outwards as need.  For example,
		 in the default firing state ('WeaponFiring') the function FireAmmunition() occurs in all applicable processes.
		 -> ConsumeAmmo -> AddAmmo -> OnRep_AmmoInClip -> FireAmmunition -> if NM_Client -> PlayFireEffects -> ServerFireAmmunition -> WeaponFiring -> TimeWeaponFiring -> Fire from FireType(Projectile, Instant or Custom)
*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StartFire", Keywords = "StartFire"), Category = "weapon | Fire")
		void StartFire(uint8 FireModeNum);

	/*		-> SetPendingFire	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "BeginFire", Keywords = "BeginFire"), Category = "weapon | Fire")
		void BeginFire(uint8 FireModeNum);

	/*		-> EndFire		*/
 UFUNCTION(BlueprintCallable, meta = (DisplayName = "StopFire", Keywords = "StopFire"), Category = "weapon | Fire")
	void StopFire(uint8 FireModeNum);

	/*		-> ClearPendingFire	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "EndFire", Keywords = "EndFire"), Category = "weapon | Fire")
		void EndFire(uint8 FireModeNum);

	/*		-> EndFire(0) -> EndFire(1)	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ForceEndFire", Keywords = "ForceEndFire"), Category = "weapon | Fire")
		void ForceEndFire();

	/**
	 * Send weapon to proper firing state
	 * Also sets the CurrentFireMode.
	 * Network: LocalPlayer and Server
	 *
	 * @param	FireModeNum Fire Mode.
		-> SetCurrentFireMode
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Send To Firing State", Keywords = "Send To Firing State"), Category = "weapon | Fire")
	void SendToFiringState(uint8 FireModeNum);

	/**
	 * Set current firing mode.
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Current Fire Mode", Keywords = "Set Current Fire Mode"), Category = "weapon | Fire")
	void SetCurrentFireMode(uint8 FiringModeNum);

	/**
	 * Event called when Pawn.FiringMode has been changed.
	 * bViaReplication indicates if this was the result of a replication call.
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Fire Mode Updated", Keywords = "Fire Mode Updated"), Category = "weapon | Fire")
	void FireModeUpdated(uint8 FiringMode);



/**
* FireAmmunition: Perform all logic associated with firing a shot
* - Fires ammunition (instant hit or spawn projectile)
* - Consumes ammunition
* - Plays any associated effects (fire sound and whatnot)
	-> ConsumeAmmo -> PlayFiringSound -> switch (WeaponFireTypes)
*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Fire Ammunition", Keywords = "Fire Ammunition"), Category = "weapon | Fire")
		void FireAmmunition();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFireAmmunition();

	void CallWeaponFire();
	/**
	 * GetAdjustedAim begins a chain of function class that allows the weapon, the pawn and the controller to make
	 * on the fly adjustments to where this weapon is pointing.
		return AddSpread
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetAdjustedAim", Keywords = "GetAdjustedAim"), Category = "weapon | Other")
	FRotator GetAdjustedAim(FVector StartFireLoc);


	/**
	 * Range of weapon
	 * Used for Traces (CalcWeaponFire, InstantFire, ProjectileFire, AdjustAim...)
	 * State scoped accessor function. Override in proper state
	 *
	 * @return	range of weapon, to be used mainly for traces.
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetTraceRange", Keywords = "GetTraceRange"), Category = "weapon | Other")
	float GetTraceRange();

	/** @return the actor that 'owns' this weapon's traces (i.e. can't be hit by them) */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetTraceOwner", Keywords = "GetTraceOwner"), Category = "weapon | Other")
	Acpp_character * GetTraceOwner();



	/**
	 * CalcWeaponFire: Simulate an instant hit shot.
	 * This doesn't deal any damage nor trigger any effect. It just simulates a shot and returns
	 * the hit information, to be post-processed later.
	 *
	 * ImpactList returns a list of ImpactInfo containing all listed impacts during the simulation.
	 * CalcWeaponFire however returns one impact (return variable) being the first geometry impact
	 * straight, with no direction change. If you were to do refraction, reflection, bullet penetration
	 * or something like that, this would return exactly when the crosshair sees:
	 * The first 'real geometry' impact, skipping invisible triggers and volumes.
	 *
	 * @param	StartTrace	world location to start trace from
	 * @param	EndTrace	world location to end trace at
	 * @param	Extent		extent of trace performed
	 * @output	ImpactList	list of all impacts that occured during simulation
	 * @return	first 'real geometry' impact that occured.
	 *
	 * @note if an impact didn't occur, and impact is still returned, with its HitLocation being the EndTrace value.
		-> LineTraceSingleByChannel
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CalcWeaponFire", Keywords = "CalcWeaponFire"), Category = "weapon | Fire")
	FHitResult CalcWeaponFire(FVector StartTrace, FVector EndTrace);


	/**
	 * Performs an 'Instant Hit' shot.
	 * Also, sets up replication for remote clients,
	 * and processes all the impacts to deal proper damage and play effects.
		 -> CalcWeaponFire -> SetFlashLocation -> ProcessInstantHit
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "InstantFire", Keywords = "InstantFire"), Category = "weapon | Fire")
	void InstantFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInstantFire();

	/**
	 * Processes a successful 'Instant Hit' trace and eventually spawns any effects.
	 * Network: LocalPlayer and Server
	 * @param FiringMode: index of firing mode being used
	 * @param Impact: hit information
	 * @param NumHits (opt): number of hits to apply using this impact
	 * 			this is useful for handling multiple nearby impacts of multihit weapons (e.g. shotguns)
	 *			without having to execute the entire damage code path for each one
	 *			an omitted or <= 0 value indicates a single hit
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ProcessInstantHit", Keywords = "ProcessInstantHit"), Category = "weapon | Fire")
	void ProcessInstantHit(uint8 FiringMode, FHitResult Impact, int32 NumHits);


	/**
	 * Fires a projectile.
	 * Spawns the projectile, but also increment the flash count for remote client effects.
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ProjectileFire", Keywords = "ProjectileFire"), Category = "weapon | Fire")
	Aproj_base * ProjectileFire();
	

	/**
	 * If the weapon isn't an instant hit, or a simple projectile, it should use the tyoe EWFT_Custom.  In those cases
	 * this function will be called.  It should be subclassed by the custom weapon.
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CustomFire", Keywords = "CustomFire"), Category = "weapon | Fire")
	void CustomFire();


	/**
	 * This function returns the world location for spawning the visual effects
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetMuzzleLoc", Keywords = "GetMuzzleLoc"), Category = "weapon | Effect")
	FVector GetMuzzleLoc();

	/**
 * This function returns the world location for spawning the projectile, pulled in to the Pawn's collision along the AimDir direction.
 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetPhysicalFireStartLoc", Keywords = "GetPhysicalFireStartLoc"), Category = "weapon | Fire")
	FVector GetPhysicalFireStartLoc(FVector AimDir);

/**
 * Put Down current weapon
 *returns true if the weapon can be put down.
 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "TryPutDown", Keywords = "TryPutDown"), Category = "weapon | Equiping")
	bool TryPutDown();


/*********************************************************************************************
* State Inactive
* Default state for a weapon. It is not active, cannot fire and resides in player inventory.
	-> WeaponState == EWS_Inactive
*********************************************************************************************/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Inactive", Keywords = "Inactive"), Category = "weapon | Weapon State")
		void Inactive();


/*********************************************************************************************
* State Active
* A Weapon this is being held by a pawn should be in the active state.  In this state,
* a weapon should loop any number of idle animations, as well as check the PendingFire flags
* to see if a shot has been fired.
	-> WeaponState == EWS_Active
 *********************************************************************************************/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Active", Keywords = "Active"), Category = "weapon | Weapon State")
		void Active();

/*********************************************************************************************
* state WeaponFiring
* This is the default Firing State.  It's performed on both the client and the server.
		-> WeaponState == EWS_Fire
*********************************************************************************************/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Weapon Firing", Keywords = "Weapon Firing"), Category = "weapon | Weapon State")
		void WeaponFiring();

	/*	-> WeaponState == EWS_Idle;*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "HandleFinishedFiring", Keywords = "HandleFinishedFiring"), Category = "weapon | Weapon State")
		void HandleFinishedFiring();


	/**
	 * Check if current fire mode can/should keep on firing.
	 * This is called from a firing state after each shot is fired
	 * to decide if the weapon should fire again, or stop and go to the active state.
	 * The default behavior, implemented here, is keep on firing while player presses fire
	 * and there is enough ammo. (Auto Fire).
	 *
	 * @return	true to fire again, false to stop firing and return to Active State.
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ShouldRefire", Keywords = "ShouldRefire"), Category = "weapon | Fire")
	bool ShouldRefire();

	/**
	 * This function returns true if the weapon is still firing in a given mode
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "StillFiring", Keywords = "StillFiring"), Category = "weapon | Fire")
	bool StillFiring(uint8 FireMode);


/**
* State WeaponEquipping
* The Weapon is in this state while transitioning from Inactive to Active state.
* Typically, the weapon will remain in this state while its selection animation is being played.
* While in this state, the weapon cannot be fired.
*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Weapon Equipping", Keywords = "Weapon Equipping"), Category = "weapon | Weapon State")
		void WeaponEquipping();

/**
* State WeaponPuttingDown
* Putting down weapon in favor of a new one.
* Weapon is transitioning to the Inactive state.
	->PutDownWeapon
*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Weapon Putting Down", Keywords = "Weapon Putting Down"), Category = "weapon | Weapon State")
		void WeaponPuttingDown();

/**
* State WeaponRealod	
*/
	void SetReloadState();
};
