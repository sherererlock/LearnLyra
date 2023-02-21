# UE重要函数执行顺序

### GameModule

```c++
UWorld::InitWorld()
{
    //创建物理场景
	CreatePhysicsScene();
    
    // 生成Actor
    UWorld::SpawnActor();
    {
        AActor::PostSpawnInitialize();
        {
            AActor::RegisterAllComponents();
            {
                UActorComponent::RegisterComponentWithWorld();
                {
                    UActorComponent::ExecuteRegisterEvents();
                    {
                        // 注册调用事件
                        UActorComponent::OnRegister();
                        {
                            
                        }
                        // 创建渲染线程资源
                        UActorComponent::CreateRenderState_Concurrent();
                        {
                            
                        }
                        
                        // 创建物理资源
                       	UActorComponent::CreatePhysicsState(/*bAllowDeferral=*/true);
                        {
                            
                        }
                    }
                }
            }
        }
    }
}

FEngineLoop::Tick();
{
    UWorld::SpawnPlayActor();
    {
        AController::Possess();
        {
            APlayerController::OnPossess();
            {
                ALyraCharacter::PossessedBy();
                {
					
                }     
                
                ClientRestart();
                {
                    APawn::PawnClientRestart();
                    {
                        APawn::SetupPlayerInputComponent();
                    }
                }
            }
        }
    }
    
    UWorld::BeginPlay();
    {
        AActor::BeginPlay();
        {
            UActorComponent::BeginPlay()
        }
    }
    
    UWorld::Tick();
    {
        Actor::Tick();
        {
            ComponentTick();
        }
            
    }
}
```

