---
date: 2023-12-31
---
# FAQ

REPENTOGON works very differently from a traditional mod. We suspect there will inevitably be some confusion about its inner workings and how it will interact with future game updates.

This FAQ is broken down into different sections for both standard end-users and mod developers.

## End-Users

### **Q: What is REPENTOGON?**
REPENTOGON is a script extender for The Binding of Isaac: Repentance. The project started in late January 2023 due to a "perfect storm" of events: at the time, we believed there to only be one update for Repentance left, several modders were frustrated with the lack of API updates, and we were pointed towards the direction of an open source version of *LibZHL*, the framework _Kilburn created to develop *Antibirth*, laying dormant in the source code of *FTL: Hyperspace*, a similar mod for FTL.

That version of LibZHL turned out not to work for us because it had extensive changes made to it to no longer suit our needs as far as Isaac goes. With the express permission of _Kilburn we used the original version directly from *Antibirth*, and have made significant changes to it in order to further increase its flexibility, power, and ease of use. 

REPENTOGON's goal is to enhance the game's API to allow modders to drop many of the hacks they need to do in order to change basic game functionality, and to further allow for things that were previously impossible. We anticipate that REPENTOGON will become a hard dependency for many mods in the future, both big and small.

We also have some small quality-of-life tweaks and bugfixes, but these are all completely optional. Many of these are on by default but can be disabled in our options menu. This is bound to the same key that traditionally would open the debug console.

### **Q: My antivirus says REPENTOGON is malicious! Is it safe?**
**Yes.** Any antivirus that flags us is reporting a false-positive. Some of the techniques we use to make this work are the same a (very primitive) virus could use to inject itself into a running program as well. 

We are a fully open source project and *always* will be. The code can be audited at any time to show that we are not malicious.

### **Q: Will REPENTOGON break when the game updates?**
**Yes, unavoidably, *and this is not the fault of Nicalis!*** Instead, this is an unavoidable side effect of how REPENTOGON works. *Please do not review bomb future updates or harass Nicalis developers because they break REPENTOGON.* This will only serve to harm the project.

We will do our best to adapt to future updates, but this will take time. We suspect we can get REPENTOGON running on a new update quickly, but it will take time to ensure that REPENTOGON hasn't broken in a subtle way. We will need to extensively retest our changes.

In the future, if Nicalis were to add *debug symbols* into the game, this would significantly reduce the time needed to adapt to future updates. Isaac has never shipped with these on Windows despite shipping with them on most other platforms, so we feel the exclusion of these on Windows is doing more harm than good, but we nonetheless respect Nicalis's decision to not include them and ask that end-users do not harass Nicalis about getting these included.

### **Q: How do I disable REPENTOGON?**
You will need to add `-repentogonoff`/`-repentogoff`/`-repentogone` as a launch option in order to make the game load without REPENTOGON.

### **Q: Does REPENTOGON work on Linux / Steam Deck?**
**Yes!** We do not have a native Linux build as there's no point to since there's no native Linux version of Repentance, but it works properly under Proton. On Steam Deck this is the default setup.

You will need to add `WINEDLLOVERRIDES=dsound=n,b %command%` as a launch option in order to make the game load our DLL. We aim to make an installer to automate this process.

### **Q: Does REPENTOGON work on Mac?**
**It's complicated.** If you're using the version of Repentance from the App Store, no. We have not researched this version and it likely doesn't support mods anyways. Running the Windows version in Wine should work, although you'll need to add the same launch option mentioned in the previous question.

### **Q: Does REPENTOGON support Rebirth, Afterbirth, or Afterbirth+?**
**No, and we never will.** It doesn't make much sense to support those DLCs as they are legacy versions of the game. Rebirth and Afterbirth don't have Lua mod support and most major mods have moved on from Afterbirth+. We will solely focus on Repentance.

## Mod Developers

### **Q: How do I detect if REPENTOGON is in use?**
We define a `REPENTOGON` global, much like Repentance defines a `REPENTANCE` global. You can use this to prompt the player to download REPENTOGON or to have a pathway in your mod that runs if REPENTOGON is enabled.

### **Q: How does REPENTOGON speed up the API?**
Multiple ways! By far the biggest impact is the move to Lua 5.4, which has a new *generational* garbage collector that works much better than the standard incremental garbage collector for the purposes of Isaac. The generational garbage collector does many tiny sweeps, whereas incremental performs less frequent large sweeps. This means garbage collection stutter is much, much less noticeable. In early REPENTOGON testing, generational GC's results were indistinguishable from not running GC at all on a high-end computer.

The port to 5.4 also necessitated rewriting three of the most frequently used API functions- `Isaac.GetRoomEntities`, `Isaac.FindByType`, and `Isaac.FindInRadius`- as these all violate Lua spec and rely on undefined behavior, and as such didn't work going to 5.4. Our reimplementations of these functions are indistinguishable from the originals from an API perspective, but perform twice (!) as fast.

Additionally, the base API is made using LuaBridge, which is a *very* slow Lua wrapper. Our new classses and functions are all made in traditional Lua instead. We'd like to fully eliminate LuaBridge in the future, which would create further performance improvements down the road. Don't rely on this, though! REPENTOGON allows for direct control over game functionality that previously had to be reimplemented, please use those. Ideally, mods transitioning to REPENTOGON will inherently increase game performance over time.

We've also made smaller changes like caching many of the commonly used classes like Game, which avoids a costly hop back to C++.

We briefly looked into adopting [LuaJIT](https://github.com/LuaJIT/LuaJIT) which would be the performance holy grail when combined with FFI, which allows calling C functions *directly* from Lua, but this would necessitate a full API rewrite. LuaJIT doesn't have bitwise operator support so we're currently stuck, but it's on the [LuaJIT 3.0 roadmap](https://github.com/LuaJIT/LuaJIT/issues/1092). If and when this is implemented we will look into this, but we will not roll this out unless we achieve full compatibility with the existing mod ecosystem.

### **Q: Will REPENTOGON get updates to add more features in the future?**
Of course! REPENTOGON is *not* finished, far from it. The only reason we're releasing now is because if we don't set an arbitrary release date, we'll *never* release. REPENTOGON is a playground for us, with each core developer focusing on their personal goals first and foremost. Many of us are no longer interested in working on Isaac mods themselves, but have had a blast working on REPENTOGON, and that's not stopping any time soon.

### **Q: REPENTOGON doesn't have a feature that I'd like. Is it possible to add X?**
It's complicated. REPENTOGON is the result of nearly a year of reverse engineering. We've hit most of the low hanging fruit at this point. There's still stuff we'd like to fully investigate, like complete control over level generation, but this will take time.

Many of the issues we have on the repository are pipe dream wishes more than anything. We'd love to tackle everything there but some of it is going to take a *lot* of effort, potentially entire reimplementations of existing game functionality. Feel free to show your support for some of these, though- the more people that are interested, the more we'll prioritize a specific issue.
