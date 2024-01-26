---
tags:
  - File
---
# File "challenges.xml"

**Resource-Folder**{: .xmlInfo .red}: Using this file in a resource folder of a mod will replace the original file.

**Content-Folder**{: .xmlInfo .green }: Using this file in a content folder will add new challenges.

With REPENTOGON, some attributes have specially mod support capatibilites added to them, see below:

| Variable-Name | Possible Values | Description |
|:--|:--|:--|
|unlockachievement|string|Sets the achievement(preferably a modded one using the name of the achievement) to unlock when the challenge is completed|
|startingitems|string|This now supports modded items by using the names of the items instead of the ids. Same format as in vanilla, a comma separated list of values|
|startingitems2|string|This now supports modded items by using the names of the items instead of the ids. Same format as in vanilla, a comma separated list of values|
|startingtrinkets|string|This now supports modded trinkets by using the names of the trinkets instead of the ids. Same format as in vanilla, a comma separated list of values|
|playertype|string|This now supports modded characters by using the names of the items instead of the ids|


???+ note "More Info"
    For more information about this xml, check the original docs entry [here](https://wofsauge.github.io/IsaacDocs/rep/xml/players.html).