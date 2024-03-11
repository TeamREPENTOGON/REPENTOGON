---
tags:
  - File
---
# File "backdrops.xml"

**Resource-Folder**{: .xmlInfo .red}: Using this file in a resource folder of a mod will replace the original file.

**Content-Folder**{: .xmlInfo .green }: Using this file in a content folder will add new backdrops animations.

With REPENTOGON, you have the ability to add new backdrops in an organic way, they should work just as vanilla backdrops do with the same xml structure.   
Take in mind that, in the same way as other content xmls, ids are automatically assigned and shouldnt be specified. If you do specify an id for a xml node, it will be ignored.    
To get the Id of a backdrop animation in lua, you can use Isaac.GetBackdropIdByName, you can then set modded backdrops using the Room.SetBackdropType () function.

The following custom attributes can be defined for backdrop in the backdrops xml.

| Variable-Name | Possible Values | Description |
|:--|:--|:--|
|reftype|int|Defines the id of the backdrop to use as a base for the custom backdrop. This is useful to grab elements from preexistent backdrops, like running water and the such|

???+ note "More Info"
    For more information about this xml, check the original docs entry [here](https://wofsauge.github.io/IsaacDocs/rep/xml/backdrops.html).