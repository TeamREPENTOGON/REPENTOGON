---
date: 2023-12-31
links: faq.md
---
# FAQ

REPENTOGON works very differently from a traditional mod. We suspect there will inevitably be some confusion about its inner workings and how it will interact with future game updates.

### **Q: What is REPENTOGON?**
REPENTOGON is a script extender for The Binding of Isaac: Repentance. The project started in late January 2023 due to a "perfect storm" of events: at the time, we believed there to only be one update for Repentance left, several modders were frustrated with the lack of API updates, and we were pointed towards the direction of an open source version of *LibZHL*, the framework _Kilburn created to develop *Antibirth*- laying dormant in the source code of *FTL: Hyperspace*, a similar mod for FTL.

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

### **Q: Does REPENTOGON work on Linux / Steam Deck?**
**Yes!** We do not have a native Linux build as there's no point to since there's no native Linux version of Repentance, but it works properly under Proton. On Steam Deck this is the default setup.

You will need to add `WINEDLLOVERRIDES=dsound=n,b %command%` as a launch option in order to make the game load our DLL. We aim to make an installer to automate this process.

### **Q: Does REPENTOGON work on Mac?**
**It's complicated.** If you're using the version of Repentance from the App Store, no. We have not researched this version and it likely doesn't support mods anyways. Running the Windows version in Wine should work, although you'll need to add the same launch option mentioned in the previous question.

### **Q: Does REPENTOGON support Afterbirth+?**
**No, and we never will.** It doesn't make much sense to support Afterbirth+ as it is a legacy verison of the game and most major mods have moved on. We will solely focus on Repentance.