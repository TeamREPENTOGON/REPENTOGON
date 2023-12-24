---
tags:
  - File
---
# File "entities2.xml"

**Resource-Folder**{: .xmlInfo .red }: Using this file in a resource folder of a mod will replace the original file.

**Content-Folder**{: .xmlInfo .green }: Using this file in a content folder will add new entities.

| Variable-Name | Possible Values | Description |
|:--|:--|:--|
| customtags | string | Possible values: ['familiarignorebffs', 'familiarcantakedamage', 'familiarblockprojectiles']. |

## Tags explanation

| Tag-Name | Description |
|:--|:--|
| familiarignorebffs | The BFFS item will no longer affect this familiar. |
| familiarcantakedamage | Familiars with baseHP above 0 will be able to take damage and die. |
| familiarblockprojectiles | The familiar destroys projectiles on contact. |

???+ note "More Info"
    For more information about this xml, check the original docs entry [here](https://wofsauge.github.io/IsaacDocs/rep/xml/entities2.html). 
