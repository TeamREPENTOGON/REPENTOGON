---
tags:
  - File
---
# File "giantbook.xml"

**Resource-Folder**{: .xmlInfo .red}: Using this file in a resource folder of a mod will replace the original file.

**Content-Folder**{: .xmlInfo .green }: Using this file in a content folder will add new giantbook animations.

With REPENTOGON, you have the ability to add new giantbook animations in an organic way, they should work just as vanilla giantbook animations do with the same xml structure.   
Take in mind that, in the same way as other content xmls, ids are automatically assigned and shouldnt be specified. If you do specify an id for a xml node, it will be ignored.    
To get the Id of a Giant Book animation in lua, you can use Isaac.GetGiantBookIdByName, you can then play modded giant book animations using the ItemOverlay.Show function.

???+ note "More Info"
    For more information about this xml, check the original docs entry [here](https://wofsauge.github.io/IsaacDocs/rep/xml/giantbook.html).