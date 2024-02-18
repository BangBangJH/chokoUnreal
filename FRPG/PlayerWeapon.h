// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerWeapon.generated.h"

DECLARE_DELEGATE_OneParam(FDele_Single_Attack, AActor*&)

UCLASS()
class FRPG_API APlayerWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UCapsuleComponent* CollisionMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Weapon_OnCollision();
	void Weapon_OffCollision();

	FDele_Single_Attack Dele_Attack; //공격 함수 델리게이트

private:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
