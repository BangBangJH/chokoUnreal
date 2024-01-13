// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameNetworkManager.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Common/TcpSocketBuilder.h"
#include "Sockets.h"

AMyGameNetworkManager::AMyGameNetworkManager()
{
    ClientSocket = nullptr;
}

AMyGameNetworkManager::~AMyGameNetworkManager()
{
    CloseConnection();
}

bool AMyGameNetworkManager::ConnectToServer()
{
    FIPv4Address ServerIpv4;
    FIPv4Address::Parse(this->ServerIP, ServerIpv4);
    FIPv4Endpoint Endpoint(ServerIpv4, this->ServerPort);

    ClientSocket = FTcpSocketBuilder(TEXT("UE5TCPClient"))
        .AsBlocking()
        .AsReusable()
        .Build();

    if (ClientSocket && ClientSocket->Connect(*Endpoint.ToInternetAddr()))
    {
        // 연결 성공
        UE_LOG(LogTemp, Warning, TEXT("SSSSSSSSSSSSSSSSSS."));
        return true;
    }
    else
    {
        // 연결 실패
        UE_LOG(LogTemp, Error, TEXT("FFFFFFFFFFFFFFFFF."));
        UE_LOG(LogTemp, Error, TEXT("FFFFFFFFFFFFFFFFF."));
        UE_LOG(LogTemp, Error, TEXT("FFFFFFFFFFFFFFFFF."));
        return false;
    }

    return ClientSocket && ClientSocket->Connect(*Endpoint.ToInternetAddr());
}

void AMyGameNetworkManager::SendData(const FString& DataToSend)
{
    if (!ClientSocket) return;

    // 문자열을 UTF-8로 인코딩
    FTCHARToUTF8 Convert(DataToSend);
    int32 BytesSent = 0;
    ClientSocket->Send((uint8*)Convert.Get(), Convert.Length(), BytesSent);
}

void AMyGameNetworkManager::CloseConnection()
{
    if (ClientSocket)
    {
        ClientSocket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ClientSocket);
        ClientSocket = nullptr;
    }
}

bool AMyGameNetworkManager::ReceiveData(FString& OutReceivedData)
{
    if (!ClientSocket) return false;

    // 수신 버퍼 준비
    TArray<uint8> ReceivedData;
    uint32 Size;
    while (ClientSocket->HasPendingData(Size))
    {
        ReceivedData.Init(0, FMath::Min(Size, 65507u));

        int32 Read = 0;
        ClientSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);

        if (Read > 0) {
            // 수신된 데이터를 FString으로 변환
            FString ReceivedString = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceivedData.GetData())));
            OutReceivedData += ReceivedString;
            UE_LOG(LogTemp, Warning, TEXT("Received data: %s"), *ReceivedString);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Read < 0"));
        }
    }

    return true;
}