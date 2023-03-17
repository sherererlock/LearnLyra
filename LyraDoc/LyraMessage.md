## Lyra Game play Message系统

#### 使用方法

1. 定义消息Payload结构体

   ```C++
   USTRUCT(BlueprintType) // 可以在蓝图中识别到
   struct FLyraPlayerResetMessage
   {
   	GENERATED_BODY()
   
   	UPROPERTY(BlueprintReadOnly)
   	AActor* OwnerPlayerState = nullptr; // 具体参数
   };
   ```

   

2. 调用蓝图节点Listen For Gameplay Messages,处理接收到消息后的具体逻辑

   ![image-20230309113652124](D:\URPRJ\LearLyraGit\LearnLyra\LyraDoc\pic\ListenForGameplayMessages.png)

3. 调用Broadcast Message节点触发消息

   C++调用示例

   ```c++
   	FLyraPlayerResetMessage Message;
   	Message.OwnerPlayerState = CurrentActorInfo->OwnerActor.Get();
   	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
   	MessageSystem.BroadcastMessage(FLyraGameplayTags::Get().GameplayEvent_Reset, Message);
   ```

   蓝图调用示例

![image-20230309114243514](D:\URPRJ\LearLyraGit\LearnLyra\LyraDoc\pic\BroadcastMessage.png)

#### 源码分析

类图

```mermaid
classDiagram
class FGameplayMessageListenerData{
	TFunction ReceivedCallback;	
	EGameplayMessageMatch MatchType;
	UScriptStruct ListenerStructType;
	int HandleID;
}

class FChannelListenerList{
	TArray~FGameplayMessageListenerData~ Listeners;
	int HandleID;
}

class FGameplayMessageListenerHandle{
	FGameplayTag Channel;
	int ID;
	FDelegateHandle StateClearHandle;
	+Unregister();
}

class UGameplayMessageSubsystem{
	TMap~FGameplayTag, FChannelListenerList~ ListenerMap;
	+FGameplayMessageListenerHandle FGameplayMessageListenerHandle RegisterListener(Channel, Callback, MatchType);
	+void UnregisterListener(FGameplayMessageListenerHandle Handle);
	+void BroadcastMessage(FGameplayTag Channel, const FMessageStructType& Message)
}

class FAsyncGameplayMessageDelegate{
	+UAsyncAction_ListenForGameplayMessage* ProxyObject;
	+FGameplayTag Channel;
}

class CallbackFunction{
	+ FGameplayTag Channel;
	+ UScriptStruct* PayloadType;
	+ const void* payload;
	+ UAsyncAction_ListenForGameplayMessage* Action;
}

class UAsyncAction_ListenForGameplayMessage{
	UWorld* WorldPtr;
	FGameplayTag ChannelToReigster;
	UScriptStruct* MessageStructType;
	FGameplayMessageMatch MessageMatchType;
	FGameplayMessageListenerHandle ListenerHandle;
	+ bool GetPayload();
	+ void Activate()
	+ void SetReadyToDestroy()
	+ void HandleMessageReceived(FGameplayTag Channel, UScriptStruct* StructType, const void* Payload);
}

UGameplayMessageSubsystem *-- FChannelListenerList
FChannelListenerList *-- FGameplayMessageListenerData
FGameplayMessageListenerData o-- CallbackFunction 
CallbackFunction *-- UAsyncAction_ListenForGameplayMessage
UAsyncAction_ListenForGameplayMessage *-- FAsyncGameplayMessageDelegate
UAsyncAction_ListenForGameplayMessage *-- FGameplayMessageListenerHandle 

```

------

## Subsystem

```mermaid
classDiagram
class FSubsystemCollectionBase{
	TMap~UClass*, USubsystem*~ SubsystemMap;
	UClass* BaseType;
	UObject* Outer;
	
	+ void Initialize(UObject* NewOuter);
	+ void Deinitialize();
	+ virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	+ const UClass* GetBaseType() const
}

template~typename TBaseType~
class FSubsystemCollection{
	+ TSubsystemClass* GetSubsystem(const TSubclassOf<TSubsystemClass>& SubsystemClass) const;
}

FSubsystemCollectionBase <|-- FSubsystemCollection
```

