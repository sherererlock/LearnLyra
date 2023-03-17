### UE中Mantle系统的实现

在UE的CharacterMovementComponent中有一个属性是`MaxStepHeight`,它表示角色可以走上去的平台最大高度，高于这个值的平台可以跳上去，除此之外UE的CharacterMovementComponent并没有提供其余的任何方式来让角色站上去，这就是需要实现的Mantle系统。

这个问题可以通过直接播放一个RootMotion动画来解决，通过抬高人物Root骨骼的方式让角色登上平台，但这样做的问题也显而易见，无法适应高度不同的平台。所以我们需要用程序的方式来解决这个问题。

首先我们需要确定一些配置来辅助我们的计算，可以用下面的数据结构表示

```c++
USTRUCT(BlueprintType)
struct FMantleSetting
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float MaxLedgeHeight; // 可以爬上去的最高位置
	UPROPERTY(EditAnywhere)
	float MinLedgeHeight; // 可以爬上去的最低位置
	UPROPERTY(EditAnywhere)
	float ReachDistance; // 障碍物距离角色多大距离时可以攀爬
	UPROPERTY(EditAnywhere)
	float ForwardTraceRadius; // 向前探测时球体的半径
	UPROPERTY(EditAnywhere)
	float DownwardTraceRadius; //向下探测时球体的半径
};
```

#### 第一部分 检测可以攀登的平台

当角色碰到障碍物时，我们需要一种方式来检测该障碍物是否可以攀爬上去，从而决定是否需要进行下面的步骤。检测的过程分为三步：

第一步我们需要使用胶囊体向角色移动的方向探测是否碰到了障碍物，该胶囊体的属性如下：

半高：`(MaxLedgeHeight - MinLedgeHeight) / 2`
半径：`ForwardTraceRadius`

接下来确定检测的起始位置。开始位置的高度（即Z坐标）是`(MaxLedgeHeight - MinLedgeHeight) / 2`,在XY坐标上向着角色移动反方向的平移-30的距离，覆盖角色前后更多的位置，结束位置的高度与开始位置一样,在XY坐标上，向着角色移动方向平移`ReachDistance`的距离，示意图如下。如果检测到障碍物，则进行第二部

第二步，我们需要确定平台高度在`MaxLedgeHeight`和`MinLedgeHeight`之间，计算出具体的高度。我们用一个半径为`DownwardTraceRadius`的球体从上到下检测平台。第一步检测过程中我们可以获取到障碍物朝着角色反方向的法线和与障碍物的碰撞点，我们可以在利用这两个值，确定在XY平面上我们检测的的位置，我们将这个点确定在与障碍物的碰撞点向着障碍物里面15厘米的位置，这是在XY平面上的位置，高度上，我们从`MaxLedgeHeight + MaxLedgeHeight`开始一直检测到`MaxLedgeHeight - DownwardTraceRadius`为之。示意图如下。如果检测到碰撞点，则进行下一步。

第三步 这一步我们需要确认角色是否可以站立在平台上。我们使用与角色胶囊体半径大小一样的球体从上到下检测，模拟人物胶囊体站立在该平台上的情景，如果检测到碰撞，则说明无法站立。示意图如下。

#### 第二部分 做攀爬前的准备工作

