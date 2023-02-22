

## Death流程

赋予技能

```C++
void ALyraPlayerState::OnExperienceLoaded(const ULyraExperienceDefinition* /*CurrentExperience*/)
{
    void ALyraPlayerState::SetPawnData(const ULyraPawnData* InPawnData)
    {
        for (const ULyraAbilitySet* AbilitySet : PawnData->AbilitySets)
		{
			if (AbilitySet)
			{
				AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
			}
		}
    }
}
```

死亡流程

```c++
//OnOutOfHealth绑定
void ULyraHealthComponent::InitializeWithAbilitySystem(ULyraAbilitySystemComponent* InASC);

// 调用
void ULyraHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    OnOutOfHealth.Broadcast(Instigator, Causer, Data.EffectSpec, Data.EvaluatedData.Magnitude);
    {
        void ULyraHealthComponent::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser)
        {
            // 以GameplayEvent_Death Tag触发技能
            Payload.EventTag = FLyraGameplayTags::Get().GameplayEvent_Death;
            AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
        }
    }
}

void ULyraGameplayAbility_Death::ActivateAbility()
{
    void ULyraGameplayAbility_Death::StartDeath()
    {
        void ULyraHealthComponent::StartDeath()
        {
            OnDeathStarted.Broadcast(Owner);
            GA_AutoRespawn::OnDeathStarted;
		   {
    
		   }
            void ALyraCharacter::OnDeathStarted(AActor*)
            {
                // 禁止输入、禁用碰撞和移动
                void ALyraCharacter::DisableMovementAndCollision();
            }
        }
    }
    
    void UGameplayAbility::ActivateAbility()
    {
        // 设置摄像机， 触发GameCue、调用EndAbility
        K2_ActivateAbilityFromEvent()
        {
            
        }
    }
   
}

void ULyraGameplayAbility_Death::EndAbility()
{
	void ULyraGameplayAbility_Death::FinishDeath();
    {
        void ULyraHealthComponent::FinishDeath()
        {
            OnDeathFinished.Broadcast(Owner);
            {
                void ALyraCharacter::OnDeathFinished(AActor*)
                {
                    void ALyraCharacter::DestroyDueToDeath()
                    {
                        // 清理工作
                        void ALyraCharacter::UninitAndDestroy()
                        {
                            // 
                            DetachFromControllerPendingDestroy();
                        }
                    }
                }
            }
        }
    }
}

```

重生流程

```C++
void ULyraAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
    void ULyraGameplayAbility::OnPawnAvatarSet();
    {
        GA_AutoRespawn::OnPawnAvatarSet();
        {
            BindDeathListener()
            {
                OnDeathStarted
            }
        }
    }
}

```

------
