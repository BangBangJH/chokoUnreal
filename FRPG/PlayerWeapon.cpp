// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeapon.h"
#include <Components/CapsuleComponent.h>

// Sets default values
APlayerWeapon::APlayerWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	CollisionMesh = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionMesh"));
	CollisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &APlayerWeapon::OnOverlapBegin);
	CollisionMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerWeapon::Weapon_OnCollision()
{
	CollisionMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	UE_LOG(LogTemp, Log, TEXT("OnCollision"));
}

void APlayerWeapon::Weapon_OffCollision()
{
	CollisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UE_LOG(LogTemp, Log, TEXT("OffCollision"));
}

void APlayerWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Dele_Attack.IsBound())
	{
		Dele_Attack.Execute(OtherActor);
		UE_LOG(LogTemp, Log, TEXT("Attack"));
	}

}

