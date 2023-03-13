## Time-steps and Achieving Smooth Motion in Unity

在Unity社区中，争论最激烈的话题之一是如何从游戏中消除运动卡顿的问题。这个问题对所有引擎都是通用的，而且直接来自于你的引擎使用的Timestep。没有适用于所有情况的单一解决方案，但肯定有次优的做法。许多开发者在某一点上遇到了运动卡顿的问题，但要获得帮助可能会很困难。关于Unity中的时间步长，有很多令人惊讶的错误信息。Unity论坛上的许多答案虽然正确，但并不全面，而且留下了完全解决这个问题所需的理解上的差距。本文旨在更深入地解决这个问题，解释Unity中的时间步骤，它们在哪里以及为什么会导致卡顿，并提出一个有助于解决这个问题的解决方案。本文提供了一个资产包，以及一个解决方案的演示。![Eww! Is this type of stutter familiar?](D:\URPRJ\LearLyraGit\LearnLyra\LyraDoc\pic\movementjerky.gif)

上面的图片是一个简单的运动卡顿的例子。显然，这不是我们在游戏中想要的行为。这很容易自己复现。

1. 创建一个新的项目

2. 导入默认的第一人称角色控制器并把它放在一个新的场景中
3. 放置一些对象，并绕着其中一个对象旋转。

去掉头部的晃动，使用游戏手柄可以更容易观察到这种效果。一般来说，在移动时，环顾四周会出现特别严重的卡顿。

现在看看这个案例，我在这里应用了我们后面要讨论的技术。将第一个例子与第二个例子相比较，你应该看到在流畅度上有明显的区别

![Nice and smooth. Much better!](D:\URPRJ\LearLyraGit\LearnLyra\LyraDoc\pic\movesmooth.gif)

在解决这个问题之前，了解Unity的更新周期是很重要的。特别是，我们需要检查Update和FixedUpdate方法背后的逻辑。下面是Unity关于执行顺序的文档的一小段摘录，可以在这里找到：https://docs.unity3d.com/Manual/ExecutionOrder.html

特别是，如果你以前没有的话，你要仔细检查更新顺序部分，因为它与本文的其余部分高度相关。

![Unity Update Order. Source:&nbsp;https://docs.unity3d.com/Manual/ExecutionOrder.htmlGet to know this flowchart!](D:\URPRJ\LearLyraGit\LearnLyra\LyraDoc\pic\ExcutionOrder.jpeg)

这个图表概述了在Unity中单帧中调用方法的顺序。我们将关注的部分是FixedUpdate和Update方法，分别用绿色和红色标出。

Unity实现了所谓的半固定时间步长。这意味着主游戏循环可以使用一个可变的时间步长以任何帧率运行，在Unity中称为deltaTime，它管理着一个使用固定时间步长的内部循环。这有一些优点；主要是能够以硬件允许的速度运行视觉更新，同时将核心游戏模拟锁定在一个恒定的速率上。但也有缺点，包括容易出现上述的卡顿现象。以下是Unity的更新循环结构的简化表示，它可能是这样的。

```c++
float currentSimulationTime = 0;
float lastUpdateTime = 0;

while (!quit) // variable steps
{
    while (currentSimulationTime  < Time.time) // fixed steps
    {		
        FixedUpdate();
        Physics.SimulationStep(currentState, Time.fixedDeltaTime);
        currentSimulationTime += Time.fixedDeltaTime;
    }
    Time.deltaTime = Time.time - lastUpdateTime;
    Update();
    Render();
    lastUpdateTime = Time.time;
}
```

Update 方法对你来说可能已经很熟悉了。在Unity处理完输入后，在屏幕渲染前，每一帧都会调用Monobehaviors。如果VSync被禁用，当一帧完成后，Unity将立即开始下一帧，从而获得尽可能高的帧速率。由于硬件和每一帧的复杂性不同，帧率永远不会是恒定的。即使启用了VSync，使Unity试图匹配一个特定的帧率，它也不是真正的恒定。正因为如此，Update可以在每秒内被调用任意次数。

每次物理模拟进行时，都会在Monobehaviors上调用FixedUpdate。Unity把这些调用当作是固定的时间间隔，即使在实际时间中，多个模拟步骤不是以均匀的时间间隔计算的。这样做是因为游戏物理学，特别是加速运动，在均匀的时间间隔下是最准确和稳定的。这个固定的时间步长在Unity中被称为 fixedDeltaTime。默认情况下，它的值为0.02，这意味着在游戏的每一秒钟都有50个物理步骤和FixedUpdate调用。这样，我们可以认为FixedUpdate与帧率无关，因为它每秒被调用的次数相同，即使渲染的帧率很低或很高。需要强调的是，FixedUpdate和物理循环是同步的，而不是在一个单独的线程上发生。

在这一点上，让我们观察一些更新时间的例子。

![img](D:\URPRJ\LearLyraGit\LearnLyra\LyraDoc\pic\updateandfixedupdate_ideal.png)

上面是50个FixedUpdates和每秒60帧时的时间安排。然而，这并不是完全可以实现的，因为帧率是变化的，所以下面是一个更真实的，虽然是夸张的时间线。请注意，有些帧是紧密相连的，中间没有物理步骤，这通常发生在渲染复杂度低的时候。 另一方面，其他的更新在它们之间有很长的停顿，每一帧之间有许多物理步骤的计算，这通常发生在加载资产时。这意味着，即使你试图用固定步骤的数量来匹配帧率，也不能保证它们是一致的。

![Update Timings2.png](D:\URPRJ\LearLyraGit\LearnLyra\LyraDoc\pic\updateandfixedupdate_real.png)

利用我们对Unity的时间步长的了解，我们现在可以理解下面这个案例。在这个场景中，有一个球体和摄像机都在围绕一个支点运行。左边球体的变换被设置在Update循环中，而摄像机的变换被设置在FixedUpdate循环中，右边的是Update。左边有明显的抖动，而右边则保持平稳。

![Camera moves in FixedUpdate on the left, Update on the right.&nbsp;](https://images.squarespace-cdn.com/content/v1/525ed7ade4b0924d2f499223/1470770667452-ZNVTVQQ4ADRDUN67H7H7/image-asset.gif?format=750w)

由于Update的调用速度与FixedUpdate不同，球体经常移动而摄像机保持静止。这导致球体相对于摄像机的移动不一致，产生了卡顿。放慢速度，可以观察到这种行为。

![Same example as above, 5% speed.](https://images.squarespace-cdn.com/content/v1/525ed7ade4b0924d2f499223/1470770742461-6Q7C5C0B5A7RI7WW2PYC/image-asset.gif?format=750w)

所以，我们可以看到这个卡顿的根本原因是在Update中移动一些对象，而在FixedUpdate中移动其他对象。简单的解决方法是在Update或FixedUpdate中移动所有的变换。

然而，这就是事情变得棘手的地方。在Unity开发者中常见的答案是将大部分游戏和运动逻辑放在Update中，只在偶尔的物理任务中使用FixedUpdate。虽然这有很多优点，包括使用简单，但由于很多原因，这是有问题的。也许最重要的是，你的游戏玩法是依赖于帧率的。这就为大量影响游戏性的错误和不一致的行为打开了大门。此外，它阻止了确定性，例如几乎整个实时战略流派，都依赖于此。当你需要一个物体的加速运动时，它也会带来问题，比如一个角色控制器的重力。对于这些对象应该使用FixedUpdate，但由于其他对象是在Update中移动的，你会得到卡顿（参见标准资产第一人称角色控制器，观察这个确切的问题）。

因此，一个常见的、有时也是必要的替代方案是将所有的状态和游戏逻辑放在一个固定的时间段内，如FixedUpdate，并在Update中严格处理视觉效果和输入逻辑。然而，这并不是没有挑战。首先，你可能希望你的物理步骤以不同的速度发生，这取决于你的游戏逻辑。这可以通过实现你自己的独立于FixedUpdate的固定时间步长循环来解决，该循环以你希望的任何速度进行。这样做相当简单，而且可以给你很大的控制权，允许进行微调优化。接下来，当只在FixedUpdate中读取时，输入可能会被遗漏，因为在FixedUpdate之间可能会出现多个帧，而只有最后一帧的输入会存到下一个FixedUpdate中。这尤其影响到按钮的Up和Down事件，因为它们只在一帧内有效。解决这个问题的方法是通过存储每一帧的输入，直到它们在下一个FixedUpdate中被全部处理。将这种行为整合到你使用的任何输入控制器中是一种相当无缝的方式，并将缓冲保持在一个地方。

然而，一个更大的问题是，FixedUpdate的调用频率通常低于客户端的帧率，所以移动的物体不会像屏幕渲染那样频繁地更新它们的位置。这使得游戏有些不稳定，尽管是一致的。有多种方法可以解决这个问题，主要是使用插值和外推法来填补FixedUpdate之间的帧数，使运动变得平滑。插值，将一个物体从一个游戏状态平滑地移动到下一个状态，这很方便，因为它可以应用于大多数物体，并且很容易工作，但确实引入了一个值固定的DeltaTime的延迟。然而，这个延迟是被普遍接受的，很多游戏，甚至是Twitch射击游戏之类的，都允许这个延迟以获得流畅性。外推法，预测下一个更新周期内物体的位置，避免了延迟，但本质上更难实现无缝工作，而且有性能成本。

![Camera and sphere moving in FixedUpdate, using interpolation on the right side only.](https://images.squarespace-cdn.com/content/v1/525ed7ade4b0924d2f499223/1470770844632-BSTFXXSQZI4XNVO40DRL/image-asset.gif?format=750w)

上面是另一个演示插值的比较。在左边，摄像机和球体都在FixedUpdate中设置了它们的变换。右边是相同的，但是内插在FixedUpdate步骤之间平滑地移动变换。注意到在这两种情况下两个物体都保持对齐，但在右边，更频繁的变换更新减少了卡顿。

那么，如何在Unity中完成这种插值呢？我做了一个资产包，其中包含一个类似于我过去使用的设置，你可以在这里得到。

此外，你可以在这里抓取用于创建上述例子的构建。

该设置基于三个脚本工作，在导入包后的 "Assets / Scripts / FixedInterpolation "目录下找到。这些脚本有完整的注释，但这里提供了更紧凑的版本，下面有简要的描述。

1. InterpolationController - 存储最近两个固定步骤的时间戳，并通过与更新期间的时间进行比较，生成一个全局插值因子。该脚本必须连接到场景中的一个游戏对象。

   ```c#
   using UnityEngine;
   using System.Collections;
   
   public class InterpolationController : MonoBehaviour
   {
       private float[] m_lastFixedUpdateTimes;
       private int m_newTimeIndex;
   
       private static float m_interpolationFactor;
       public static float InterpolationFactor {
           get { return m_interpolationFactor; }
       }
   
       public void Start() {
           m_lastFixedUpdateTimes = new float[2];
           m_newTimeIndex = 0;
       }
   
       public void FixedUpdate()  {
           m_newTimeIndex = OldTimeIndex();
           m_lastFixedUpdateTimes[m_newTimeIndex] = Time.fixedTime;
       }
   
       public void Update() {
           float newerTime = m_lastFixedUpdateTimes[m_newTimeIndex];
           float olderTime = m_lastFixedUpdateTimes[OldTimeIndex()];
   
           if (newerTime != olderTime) {
               m_interpolationFactor = (Time.time - newerTime) / (newerTime - olderTime);
           } else {
               m_interpolationFactor = 1;
           }
       }
       
       private int OldTimeIndex() {
           return (m_newTimeIndex == 0 ? 1 : 0);
       }
   }
   ```

2. InterpolatedTransform - 在最近的两个固定步骤之后为一个对象存储变换，并使用全局插值因子在它们之间插值对象。它还确保对象在当前固定步骤执行之前被放回它最后一个固定步骤的位置，而不是它最后被内插到的位置。这意味着任何移动变换的脚本都是从正确的状态开始工作的。如果你传送一个对象并想防止插值，在移动对象后调用 ForgetPreviousTransforms 方法。这个脚本应该被附加到任何在FixedUpdate期间移动的对象上。

   ```c#
   using UnityEngine;
   using System.Collections;
   
   [RequireComponent(typeof(InterpolatedTransformUpdater))]
   public class InterpolatedTransform : MonoBehaviour
   {
       private TransformData[] m_lastTransforms;
       private int m_newTransformIndex;
   
       void OnEnable() {
           ForgetPreviousTransforms();
       }
   
       public void ForgetPreviousTransforms() {
           m_lastTransforms = new TransformData[2];
           TransformData t = new TransformData(
                                   transform.localPosition,
                                   transform.localRotation,
                                   transform.localScale);
           m_lastTransforms[0] = t;
           m_lastTransforms[1] = t;
           m_newTransformIndex = 0;
       }
   
       void FixedUpdate() {
           TransformData newestTransform = m_lastTransforms[m_newTransformIndex];
           transform.localPosition = newestTransform.position;
           transform.localRotation = newestTransform.rotation;
           transform.localScale = newestTransform.scale;
       }
   
       public void LateFixedUpdate() {
           m_newTransformIndex = OldTransformIndex();
           m_lastTransforms[m_newTransformIndex] = new TransformData(
                                                       transform.localPosition,
                                                       transform.localRotation,
                                                       transform.localScale);
       }
   
       void Update() {
           TransformData newestTransform = m_lastTransforms[m_newTransformIndex];
           TransformData olderTransform = m_lastTransforms[OldTransformIndex()];
   
           transform.localPosition = Vector3.Lerp(
                                       olderTransform.position, 
                                       newestTransform.position, 
                                       InterpolationController.InterpolationFactor);
           transform.localRotation = Quaternion.Slerp(
                                       olderTransform.rotation, 
                                       newestTransform.rotation, 
                                       InterpolationController.InterpolationFactor);
           transform.localScale = Vector3.Lerp(
                                       olderTransform.scale, 
                                       newestTransform.scale,
                                       InterpolationController.InterpolationFactor);
       }
   
       private int OldTransformIndex() {
           return (m_newTransformIndex == 0 ? 1 : 0);
       }
   
       private struct TransformData {
           public Vector3 position;
           public Quaternion rotation;
           public Vector3 scale;
   
           public TransformData(Vector3 position, Quaternion rotation, Vector3 scale) {
               this.position = position;
               this.rotation = rotation;
               this.scale = scale;
           }
       }
   }
   
   ```

3. InterpolatedTransformUpdater - 用于在其他脚本的FixedUpdates之前和之后调用InterpolatedTransform的几个方法。必须放在也有InterpolatedTransform的对象上。

   ```
   using UnityEngine;
   using System.Collections;
   
   public class InterpolatedTransformUpdater : MonoBehaviour
   {
       private InterpolatedTransform m_interpolatedTransform;
       
       void Awake() {
           m_interpolatedTransform = GetComponent<InterpolatedTransform>();
       }
   	
    void FixedUpdate() {
           m_interpolatedTransform.LateFixedUpdate();
       }
   }
   ```

   ![The required execution order.](https://images.squarespace-cdn.com/content/v1/525ed7ade4b0924d2f499223/1470771300766-IQLQSALS873RMD1K7EKN/image-asset.png?format=500w)

为了使这些脚本发挥作用，必须按上述方式指定执行顺序。此外，任何附加了InterpolatedTransform的对象必须只在FixedUpdate中移动，因为在Update中进行的任何变换都会覆盖插值。此外，你应该确保在必要时对任何输入进行缓冲。虽然有很多方法可以做到这一点，但一个好的解决方案是在你自己的输入控制器中建立输入缓冲，减少其他地方的复杂性。最后，请记住，虽然这个简单的设置是令人惊讶的功能，但对于一个准备生产的系统来说，还有许多改进的地方，例如外推法。

总之，把你所有的游戏逻辑放在Update或FixedUpdate中。不要混合和匹配时间段，除非你愿意咬紧牙关，接受一些卡顿。此外，非常值得考虑将所有的游戏状态放在FixedUpdate中，将Update专门用于用户输入、视觉效果和游戏状态间的插值。虽然这需要改变你的游戏结构，但这是一个成熟的设计结构，有很多优点。

## RECOMMENDED READING

**More on variable, fixed, and semi-fixed timesteps:** 

- http://gafferongames.com/game-physics/fix-your-timestep/
- http://www.koonsolo.com/news/dewitters-gameloop/

**A case where placing all game state information in fixed timesteps only is necessary:**

- https://blog.forrestthewoods.com/synchronous-rts-engines-and-a-tale-of-desyncs-9d8c3e48b2be#.kt2uk2a89

**Unity docs:**

- https://docs.unity3d.com/Manual/ExecutionOrder.html

原文网址

https://www.kinematicsoup.com/news/2016/8/9/rrypp5tkubynjwxhxjzd42s3o034o8

视频地址

https://www.youtube.com/watch?v=qtV8JEIq0Ng