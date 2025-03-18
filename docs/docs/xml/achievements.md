---
tags:
  - File
---
# File "achievements.xml"

**Resource-Folder**{: .xmlInfo .red}: Using this file in a resource folder of a mod will replace the original file.

**Content-Folder**{: .xmlInfo .green }: Using this file in a content folder will add new achievements.

With REPENTOGON, you have the ability to add new achievements in an organic way, they should work just as vanilla achievements do with the same xml structure.   
Take in mind that, in the same way as other content xmls, ids are automatically assigned and shouldnt be specified. If you do specify an id for a xml node, it will be ignored.    
To get the Id of an achievement in lua, you can use Isaac.GetAchievementIdByName, you can unlock modded achievements using the PersistentGameData.TryUnlock function. When modded achievements are unlocked, they will play the animation as normal and they will interact with vanilla achievements if several are unlocked at once.
REPENTOGON also tracks modded achievements and saves them internally, so you dont need to handle them on your side. Take in mind that you can lock achievements by using the `lockachievement` console command.

The following custom attributes can be defined for achievements in the achievements xml.

| Variable-Name | Possible Values | Description |
|:--|:--|:--|
|name|string|used for the GetXByName functions (when this is not present, text is used instead)|
|gfxback|string|path of the png sprite to use as a background paper for the achievement|
|hidden|bool|any value other than 'false', preferably 'true', will make it not show the achievement popup on unlock nor display the achievement in the secrets menu|

???+ note "More Info"
    For more information about this xml, check the original docs entry [here](https://wofsauge.github.io/IsaacDocs/rep/xml/achievements.html).
