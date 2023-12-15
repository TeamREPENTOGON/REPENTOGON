---
tags:
  - File
---
# File "costumes2.xml"

**Resource-Folder**{: .xmlInfo .red}: Using this file in a resource folder of a mod will replace the original file.

**Content-Folder**{: .xmlInfo .green }: Using this file in a content folder will add new costumes.

With REPENTOGON, the "id" attribute of a costume with type="none" can now be used to assign that costume to a character or a "null" item/effect from the same mod.

| Variable-Name | Possible Values | Description |
|:--|:--|:--|
|id|int|Assign the same "id" to a type="none" costume and a corresponding "null" item in [items.xml](items.md) to have that "null" item automatically apply the costume, similarly to what could already be done for typical item costumes. See the page for [items.xml](items.md) for more information on "null items".<br /><br />Setting the "id" of a type="none" costume and the "modcostume" attribute of a character in [players.xml](players.md) to the same number will permanantly apply that costume to that character, like the hair costumes of vanilla characters.<br /><br />Note that the id used must be at least 1. This id is not the one that will be used in-game, and is only for associating the costume with other things defined in a mod's xmls.|

???+ note "More Info"
    For more information about this xml, check the original docs entry [here](https://wofsauge.github.io/IsaacDocs/rep/xml/costumes2.html). 
