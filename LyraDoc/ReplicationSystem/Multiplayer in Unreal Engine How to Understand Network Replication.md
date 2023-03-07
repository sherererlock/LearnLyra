# Multiplayer in Unreal Engine: How to Understand Network Replication

One of Unreal Engine's most compelling features is its built-in networking support. To borrow a phrase from the American auto industry: with Unreal, multi player comes standard. In a multi player game, you have multiple instances of the same game running in tandem - that could be different processes on the same 
machine, or different computers on the same local network, or players on different continents playing over the Internet. In any case, those different game instances have to talk to one another and build a consistent picture of a shared world.

In Unreal's network model, players connect to a server, and the server maintains the authoritative state of the world. When something changes on the server, those changes will be propagated down to clients as needed, as part of a process called replication. Unreal's replication system is a first-class feature that's naturally integrated with game code. You can easily develop a multiplayer game, and you're not managing client connections, opening sockets, or sending packets; and you're not dealing with serialization, encoding and byte order, or time stamping, reordering, and routing - you can just say "I want this property replicated", and it's replicated. Replication can seem pretty complex at first glance, but once you understand the basic concepts, it's really not that difficult. So I'd like to help you wrap your head around Unreal's replication system, how it works, and how to use it.

First, let's talk about net mode. The net mode is a property of the World, and it can be any of these four values. You can differentiate these net modes based on three simple questions: For one, is the game playable? Does our GameInstance have a LocalPlayer, and are we processing that player's input and rendering the world into a viewport? Next, are we a server? In other words, does our GameInstance have the canonical, authoritative copy of the World, with a GameMode actor in it?  And finally, if we're the server: are we open for remote connection attempts? Can other players join in and play as clients?

As we saw in a previous video, when you start up your game, you get a GameInstance object that's tied to the lifetime of the process, and then the game browses to a URL - either a server address or a map name. That causes the game to load a map, which gives it a World, and the World's net mode will differ depending on how your game instance has started up. If your game instance has connected to a remote server, then its World will have the Client net mode. The game is playable for the local player, but it's updating its World at the behest of the server. If your game instance has loaded up a map locally, then the World's net mode will be Standalone. In effect, your single game instance is both the server and the client, but since it's running in a single-player configuration, it's not open for clients to connect to. If you load up a map locally but add the Listen option, then you'll be running as a listen server. This is basically the same thing as standalone, but other instances of the game can connect as clients. Finally, you can run a dedicated server. This is an instance of the game that has no LocalPlayer and no viewport: it's just a server-only console application that players can connect to as clients.

So there are three different networking scenarios that an Unreal game can run in. For a single-player game, you have one GameInstance, and its World is running in Standalone mode. For a multiplayer game, you have multiple processes, each with its own GameInstance and its own copy of the World. One of those processes is either a ListenServer or a DedicatedServer, and the rest are Clients. So that's how we might get ourselves into a multiplayer game in the first place.

Once we have a multiplayer game running, Unreal's replication system works behind the scenes to make sure that all of these different instances of the game are in sync: they all build their own picture of their shared world, and the replication system makes sure they all agree about what's happening in that world. In order to make that happen, the replication system relies on three important classes: NetDriver, NetConnection, and Channel. In this example, we've got a dedicated server and two clients. Each of these three processes has its own Engine object. When we boot up our server, it creates a NetDriver, and it starts listening for messages from remote processes. When we boot up a client, it creates its own NetDriver, which sends a connection request to the server. Once a server and client NetDriver have made contact, that establishes a NetConnection within each NetDriver. The server has one NetConnection for each remote player that's connected. Each client has a single NetConnection, which represents its connection to the server. Within every NetConnection, there are a number of different Channels. Typically, a connection will have a ControlChannel and a VoiceChannel, and then each connection will have a series of ActorChannels, one for each actor that's currently being replicated across that connection.

This illustrates a key fact of the replication system: namely, that replication happens on the level of actors. If you need an actor to stay in sync over the network, then you configure that actor to be eligible for replication. When an actor that's eligible for replication is considered relevant to a particular player, then the server will open an ActorChannel in that player's NetConnection, and the server and client will use that channel to exchange information about that actor. 

If an Actor is being replicated to a client, there are three important things that can happen as a result. First, the lifetime of the Actor is kept in sync between the server and the client. If the server spawns a replicated actor, the client will be notified so it can spawn its own copy. If the actor is destroyed on the server, it'll automatically be destroyed on the client as well.  Then there are replicated properties. If an actor has a property that's flagged for replication, then when that property is changed on the server, the new value will be propagated to the client. Finally, there are RPCs: remote procedure calls. If you designate a function as a Multicast RPC, then when you call that function on the server, the server will send a message to every client to whom that Actor is currently being replicated, indicating that the client should call that function on its copy of the actor. You can also declare Client and Server RPCs, which let you send messages back and forth between the server and the single client who owns the actor.

Ownership is another important concept for  Actor replication. Every Actor can have another Actor designated as its Owner. Typically, you set the Owner on spawn, but you can also call SetOwner at runtime. The PlayerController class has special importance when it comes to Ownership. Basically, each of our NetConnections represents a Player, and once the player is fully logged into the game, it has a PlayerController actor associated with it. From the server's point of view, the connection owns that PlayerController, and by extension, the connection owns any actors that can trace their ownership back to the PlayerController. PlayerControllers automatically own the Pawn that they're possessing. So let's say each of our PlayerControllers has a Pawn, and each Pawn has spawned a Weapon actor that it owns. The server can look at either one of these Weapon actors, follow its chain of Owner references back to the PlayerController, and figure out that that Weapon actor belongs to a particular client connection.

So let's look more closely at how you configure an actor to be replicated. In order for an Actor to ever be considered for replication, it needs to have its bReplicates flag set to true. Typically you'd just set this value in the constructor, or for a Blueprint, you'd check the "Replicates" box in the Class Defaults. But you can also turn replication on or off at runtime.
So long story short, if an actor has its bReplicates flag enabled, then it's eligible for replication: the server can open an ActorChannel within any given NetConnection in order to replicate that actor to the corresponding client.
An actor's relevancy determines which connections that will happen for and at what times. When an actor is eligible for replication, every so often the server's net driver will check that actor against each client connection to determine if it's relevant to that client. Some actors are bAlwaysRelevant: this means that as long as they're eligible for replication, the server will replicate them to all clients at all times. For example, the GameState and PlayerState actors are always relevant. 
Ownership plays an important role in relevancy. An Actor that's owned by (or instigated by) a particular player will always be considered relevant for the corresponding client. Some actors, like PlayerControllers, are configured to only be relevant to their owner, so they'll never replicate to non-owning clients. You can also configure an Actor so that it inherits relevancy from its Owner. If none of these special flags are set, and the client in question doesn't own the Actor,
then the default behavior works like this: if the Actor is Hidden and its root component has collision disabled, it won't be considered relevant. Otherwise, relevancy is based on distance from the player that corresponds to the client connection. If the squared distance from the actor to the player is less than NetCullDistanceSquared, then the actor is relevant to that player. 
These rules aren't hard-coded: you can override the IsNetRelevantFor function for any actor class to provide your own custom rules. 

Once an actor is being replicated, its update frequency and priority will determine how often the server sends updates to clients for whom that actor is relevant. Setting NetUpdateFrequency will dictate how many times per second the server will check an actor and potentially send new data to clients if anything's changed. Keep in mind that real-world networks have extremely variable latency and bandwidth can quickly become a limiting factor, so even if your actor is set to update 60 times per second, you're not going to see perfectly smooth results on the other end.

The server's NetDriver employs some simple load balancing in order to mitigate bandwidth saturation. At any point in time, the NetDriver has a finite amount of bandwidth to work with, so it sorts relevant actors according to priority, and then it runs network updates until it's used up its available bandwidth. Actors that are closer to the player are weighted with a higher priority, and actors that haven't been updated in a while will have a higher priority as well, so that every actor eventually works its way to the front of the priority list. Setting an Actor's NetPriority property will apply an additional scale to that weight, so, for example, you could dictate that an important actor should be updated 5 times more often than it would be otherwise.

That process of periodic, bandwidth-limited network updates mostly applies to replicated properties. If you have a high-priority message that you want to send over the network immediately, that's what RPCs are for. Any UFUNCTION can be designated Client, Server, or NetMulticast to make it an RPC.
If you invoke a Client RPC from the server, then the function implementation will run on the owning client. 
If you invoke a Server RPC from the owning client, then the function implementation will  run on the server.
If you invoke a Multicast RPC from the server, the function implementation will run everywhere: on the server, and then on all clients. 
Unlike Server and Client RPCs, relevancy is a factor  for Multicast RPCs, since non-owning clients may not have an open channel for the actor: in that case, the client simply won't receive the RPC. That means you shouldn't rely on Multicast RPCs to replicate persistent state changes to clients.
RPCs can be declared Reliable or Unreliable. Unreliable RPCs can be dropped if bandwidth is saturated, and they're not guaranteed to arrive, and therefore they're not guaranteed to arrive in order either. Reliable RPCs are guaranteed to arrive, and within a single Actor, reliable RPCs are guaranteed to arrive in the order in which they were called. This reliability is necessary if your function call is critical to gameplay, but if overused it can exacerbate bandwidth saturation
and it can lead to bottlenecks in the event of packet loss.
In C++ the actual body of your function needs to be defined with the "_Implementation" suffix. This is the function that actually runs on the remote process, whereas the function you call locally is an auto-generated thunk that sends the appropriate messages over the network. Server RPCs can also be declared WithValidation, in which case you need to implement a corresponding "_Validate" function that takes all the same arguments and returns a boolean indicating
whether those values are trustworthy. This is a means of cheat detection for cases where the server uses data sent from the client in a way that affects gameplay. Just keep in mind that if a Server RPC fails validation, the client who sent that RPC will be kicked out of the game. So RPCs are sent immediately, and they can be reliable - that makes them useful for certain cases where replicated properties can be comparatively limited.

Server RPCs are the only way to get data from  the client to the server by way of the owning connection, so they have their place. But by and large, RPCs are typically reserved for high-priority, time-critical network code. For example, the Engine's character movement system makes liberal use of RPCs to send position updates back and forth, because movement prediction and correction requires up-to-date information with as little latency as possible.
For everything else, you should rely on replicated properties wherever you can get away with it.
Property replication is the workhorse of Unreal's replication system, and the load balancing and prioritization features we already discussed make it far more scalable. Whereas RPCs are immediate, the key word with property replication is eventually. If you change a replicated property on the server, you can count on all clients eventually being in sync with the server. If an actor changes on the server when a player is too far away for it to be relevant, the change will still stick: eventually, once the actor becomes relevant to that client again, it'll receive the updated property value.
Property replication also respects update frequency and bandwidth limits, no matter what. You can change a replicated property every single frame on the server, and the client will just get the most recent value whenever it happens to be updated. The server's under no obligation to send every single intermediate value.
To enable replication for a property, you can add the Replicated specifier. In C++, you need to take the extra step of defining a GetLifetimeReplicatedProps function in your Actor's .cpp file: you'll want to include UnrealNetwork.h there as well. This function is where you specify which properties should be replicated, and under what conditions. In the simplest case, you just want a property to be replicated to all clients at all times, and you accomplish that by naming the property in a DOREPLIFETIME macro. But you can also specify conditions for replication. For example, you might only need to replicate a property to the owning client, or you might need only non-owning clients to get updates. Or you might have a property that's initialized on spawn but never changes at runtime.
If you need to run some code when a replicated property is updated, then you can declare a RepNotify function and use the ReplicatedUsing specifier. Any time the value changes due to a replication update, the specified notify function will be called on the client.
Note that in Blueprints, changing a property's value on the server will automatically invoke the associated RepNotify function on the server. This isn't the case in C++ - if you want your RepNotify logic to run on the server as well as on clients, then you'll want to manually call the RepNotify function after updating your property value.

There's one final concept that we should look at: and that's an actor's network role. There are a few different roles that an actor can have, but in most cases, you only need to be concerned with a far simpler question: do I have authority over this Actor?  Whenever you're running some code in an Actor class, you can check for authority: if you have authority, you have the final say in updating the state of the actor, either because the game is running in single-player mode,  or your code is being executed by the server, or because the actor only exists on the client. If you don't have authority, your code is running on the client, and the actor is being replicated from the server. In that case, the client's copy of the actor is a proxy for the authoritative version on the server.
If an actor doesn't have authority, then its role is almost always SimulatedProxy. Typically, AutonomousProxy only enters the picture when we're talking about players. A PlayerController is replicated to the owning client is an AutonomousProxy, and the associated Pawn is an AutonomousProxy for that client. For all other clients, the Pawn is replicated as a SimulatedProxy. Autonomous refers to the fact that the client is directly controlling the actor's movement and behavior, even though it doesn't have full authority. But unless you're dealing with player characters, it's usually just a binary question: do I have authority or not? If you are dealing with player-related code, then another important question is whether the player is locally controlled. If a Pawn is locally controlled, then that player corresponds to the  GameInstance where your code is running. If not, it's a remote client's player.

So, once you have all these concepts in your  head, the most important thing to appreciate about supporting multiplayer is how it adds complexity to the way that you think about your code. You might think that programmers spend all their time furiously typing, like in the movies. But as a programmer, a lot of your time is spent just reasoning about how your code is executed. And it doesn't matter how smart you are - your brain can only fit a few pieces of information in it at a time, so if you have to juggle too many things at once, it's going to be very hard to write good, maintainable code.
A lot of skill and experience just boils down to knowing how to design and implement complex functionality in a form that's relatively simple to reason about.

Supporting multiplayer adds a layer of complexity that you have to manage. If you have an actor that's replicated, then you're not just thinking about what the class is responsible for and what each of its member functions does, you also have to consider where that code is being executed, and how data flows across the network at different times.
That extra complexity can feel overwhelming at first, but it gets easier to manage with experience. So, to wrap up, here are a few tips to keep in mind.
First, it's important to remember that even if an actor is replicated, not every part of it needs to be concerned with networking. If you have an actor with a static mesh component  and a dynamic material instance, neither of those objects is network-aware. You can write the functions that initialize and modify those objects without having to think about networking. Those functions just run independently on each game instance - and you only need to bring replication into the equation to make sure that they're called in a consistent way across all instances of the game.
Most of your game code will ultimately build off of the Unreal GameFramework, which is designed for multiplayer out of the box. When you're overriding these classes' member functions to add your own functionality, you'll want to be aware of whether a function is designed to run only on the server, only on clients, or everywhere.
Using assertions can make it plainly obvious from looking at your code where it's designed to run, and it can also be useful to establish naming conventions for helper functions, such as using a prefix like "Auth" for functions that are only called with authority. 
You can think of your actor's functionality as sort of a cause-and-effect flowchart: on one end, you have events that set things in motion. On the other end, you have the state changes that need to occur as a result. If you have some process that starts and ends in different places on the network, that's when you need to use some form of replication. If the process originates on the client and ends up on the server, then you typically use a Server RPC. If a process originates on the server and ends up having effects on the client, then you need to consider where to replicate data down to clients.
Your code needs to stay on the server for  as long as it's updating authoritative game state, or if players could gain an unfair advantage by viewing or modifying the data client-side. But after that, you generally want to replicate the minimal set of data required to reconstruct the end result, and let the client handle the rest of the process independently.

When your code needs to support networking, it generally fits one of a handful of simple cases. Before you start getting lost in the low-level details, ask yourself if the functionality you're dealing with fits into one of these four buckets:
If something needs to happen only on the server in multiplayer, and also in single-player, put it behind a HasAuthority check.
If something's only necessary when running as a remote client, and therefore shouldn't run in single-player either, check that you don't have authority.
There are some cases where you want to exclude dedicated server specifically: in those cases, check IsRunningDedicatedServer. 
And, if you have purely client-side functionality, it's usually tied to a Pawn or a Controller, so you can check whether the player is locally controlled.
That's about it for an overview.
If you're looking for more information about networking features, the official documentation is a great resource, and if you want more technical details, there are some good places in the Engine source where you can poke around. 
In particular, NetDriver.h has some good information on how the replication system works, and you can see its implementation, along with some useful console variables, in NetworkDriver.cpp. NetDriver is just the Engine-level base class: the actual implementation used for the game network driver is called IpNetDriver, and it's implemented in the OnlineSubsystemUtils plugin, along with IpConnection. If you'd like to mess around with the simple example project seen in this video, you can clone it from GitHub and look through the change history, and it's pretty exhaustively commented. Beyond that, I'd suggest checking  out the ShooterGame example project if you want to see a bigger, more official  example of a multiplayer game. 

Thanks for watching! I hope this video's helped you wrap your head around the replication system - please feel free to let me know if you found it helpful or if you have questions or suggestions, and I'll see you in the next video.
