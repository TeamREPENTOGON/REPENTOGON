---
tags:
  - File
---
# File "cutscenes.xml"

**Resource-Folder**{: .xmlInfo .red}: Using this file in a resource folder of a mod will replace the original file.

**Content-Folder**{: .xmlInfo .green }: Using this file in a content folder will add new cutscenes.

With REPENTOGON, you have the ability to add new cutscenes in an organic way, they should work just as vanilla cutscenes do with the same xml structure.   
Take in mind that, in the same way as other content xmls, ids are automatically assigned and shouldnt be specified. If you do specify an id for a xml node, it will be ignored.    
To get the Id of a cutscene in lua, you can use Isaac.GetCutsceneIdByName, you can also play modded cutscenes from the console command "cutscene" to test them out.

The following custom attributes can be defined for cutscenes in the cutscenes xml.

| Variable-Name | Possible Values | Description |
|:--|:--|:--|
|name|string|used for the GetXByName functions (when this is not present, the filename of the gfx attribute is used instead)|
|music|string or int|used for the anm2 cutscenes, its available in vanilla but in rgon it now allows to set it to a modded music name|

???+ note "More Info"
    For more information about this xml, check the original docs entry [here](https://wofsauge.github.io/IsaacDocs/rep/xml/cutscenes.html).
