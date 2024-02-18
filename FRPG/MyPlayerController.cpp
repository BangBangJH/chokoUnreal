// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <Blueprint/UserWidget.h>
#include "InventoryUI.h"
#include <NavigationSystem.h>
#include "PlayerCharacter.h"

AMyPlayerController::AMyPlayerController()
{
	bShowMouseCursor = true;
	bReplicates = true;
}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	if (bClickRightMouse && bMoveAble)
	{
		MoveToMouseCursor();
	}
}

void AMyPlayerController::InputRightMouseButtonPressed()
{
	bClickRightMouse = true;
}

void AMyPlayerController::InputRightMouseButtonReleased()
{
	bClickRightMouse = false;
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("RightClick", EInputEvent::IE_Pressed, this, &AMyPlayerController::InputRightMouseButtonPressed);
	InputComponent->BindAction("RightClick", EInputEvent::IE_Released, this, &AMyPlayerController::InputRightMouseButtonReleased);
	InputComponent->BindAction("Inventory", EInputEvent::IE_Pressed, this, &AMyPlayerController::OpenInventory);
	InputComponent->BindAction("Test1", EInputEvent::IE_Pressed, this, &AMyPlayerController::TestInventory);
}


void AMyPlayerController::Server_MovePlayer_Implementation(APlayerController* PlayerCtr, FVector Destination)
{
	Multicast_MovePlayer(PlayerCtr, Destination);
}

void AMyPlayerController::Multicast_MovePlayer_Implementation(APlayerController* PlayerCtr, FVector Destination)
{
	MovePlayer(PlayerCtr, Destination);
}

void AMyPlayerController::MovePlayer(APlayerController* PlayerCtr, FVector Destination)
{
	APawn* ControlledPawn = PlayerCtr->GetPawn();
	if (ControlledPawn != nullptr)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerCtr->GetCharacter());
		if (PlayerCharacter->bAttack)
		{
			PlayerCharacter->AttackEnd(); //이동 입력시 캐릭터 공격 종료
		}
		FVector WorldDirection = Destination - ControlledPawn->GetActorLocation(); //캐릭터, Destination 벡터값으로 방향 계산
		WorldDirection.Z = 0;
		if(WorldDirection.Length() <= 150.f)
		{
			StopMovement();
			WorldDirection.GetUnsafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection * 50.0f); //전진벡터 값으로 이동, 값을 곱해줘야 가까울때도 빠르게 이동
		}
		else
		{
			UNavigationSystemV1* NavSys = PlayerCtr ? FNavigationSystem::GetCurrent<UNavigationSystemV1>(PlayerCtr->GetWorld()) : nullptr;
			const FVector AgentNavLocation = PlayerCtr->GetNavAgentLocation();
			const ANavigationData* NavData = NavSys->GetNavDataForProps(PlayerCtr->GetNavAgentPropertiesRef(), AgentNavLocation);
			FPathFindingQuery Query(PlayerCtr, *NavData, AgentNavLocation, Destination);
			UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
			if (NavSystem->TestPathSync(Query)) //네비로 갈 수 있으면
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(PlayerCtr, Destination); //네비메쉬 이동
			}
			else
			{
				WorldDirection.GetUnsafeNormal();
				ControlledPawn->AddMovementInput(WorldDirection * 50.0f); //전진벡터 값으로 이동, 값을 곱해줘야 가까울때도 빠르게 이동
			}
		}

	}

}



void AMyPlayerController::MoveToMouseCursor()
{
	FHitResult Hit;
	bool bHit = GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	if (bHit)
	{
		Server_MovePlayer(this, Hit.Location);
	}
}

void AMyPlayerController::OpenInventory()
{
	if (!UI_Iventory) // 인벤토리 UI 생성 확인, 없으면 생성
	{
		if (UI_Inventory_Class)
		{
			UI_Iventory = CreateWidget(GetWorld(), UI_Inventory_Class);
			if (UI_Iventory)
			{
				UI_Iventory->AddToViewport();
			}
		}
	}
	else
	{
		// 생성 이후 껐다 켰다
		if (UI_Iventory->GetVisibility() != ESlateVisibility::Hidden)
		{
			UI_Iventory->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			UI_Iventory->SetVisibility(ESlateVisibility::Visible);
		}

	}
}

void AMyPlayerController::TestInventory()
{
	if (UI_Iventory)
	{
		UInventoryUI* InvenUI = Cast<UInventoryUI>(UI_Iventory);
		InvenUI->TestItem();
	}
}

void AMyPlayerController::MoveUnable()
{
	StopMovement();
	bMoveAble = false;
}

void AMyPlayerController::MoveAble()
{
	bMoveAble = true;
}


