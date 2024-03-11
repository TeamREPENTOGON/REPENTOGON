---
tags:
  - Global
  - Class
---
# Global Class "XMLData"

???+ info
	A public table containing all the functions related to gathering XML attributes accross the different XMLs with updated values to match the real values.
    
    You can get this class by using the `XMLData` global table.

    **Note that to call these functions, you must use a `.` (period) instead of a `:` (colon)!**
    
    ???+ example "Example Code"
        ```lua
        local numEntries = XMLData.GetNumEntries(XMLNode.ENTITY)
        ```
        
???+ warning "Warning"
    XML attributes are converted to lowercase when being parsed by REPENTOGON! This eliminates capitializtion inconsistency in vanilla tags, but might result in not being able to find attributes if they're looked up by their name as definined in the XML (e.g `bossID` will return `nil`, so use `bossid` instead.)
        
## Functions

### GetEntityByTypeVarSub () {: aria-label='Functions' }
#### table GetEntityByTypeVarSub ( [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) Type, int Variant = 0 , int SubType = 0, boolean Strict = false) {: .copyable aria-label='Functions' }
Returns a table containing the attributes of the entity on entities2.xml that match the given type and/or variant and/or subtype. The strict parameter determines if it should only return a value when all 3 attributes(type, var and sub) match or return whatever matches the type and take the rest as maybes.
???- info "Table usage"
	```lua
	print("Monstro's BossID:", XMLData.GetEntityByTypeVarSub(20).bossid)
	```
???+ note "child nodes"
    Child nodes are returned as tables alongside the rest of the attributes. For example, if you want to access the samples of a sound entry, you can use `soundentry.sample[1]`.
___
### GetBossColorByTypeVarSub () {: aria-label='Functions' }
#### table GetBossColorByTypeVarSub ( [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) Type, int Variant , int SubType) {: .copyable aria-label='Functions' }
Returns a table containing the attributes of the bosscolor on bosscolors.xml that match the given type variant and subtype. 
???- info "Table usage"
	```lua
	print("Red Monstro's suffix:", XMLData.GetBossColorByTypeVarSub(20,0,1).suffix)
	```
___
### GetEntryById () {: aria-label='Functions' }
#### table GetEntryById ( [XMLNode](enums/XMLNode.md) NodeType, int Idx ) {: .copyable aria-label='Functions' }
Returns a table containing the attributes of the corresponding xml, the matching NodeType(Ex: XMLNode.TRINKET returns trinket nodes from pocketitems.xml) and match the given unique id.
???- info "Table usage"
	```lua
	print("Sad Onion's description:", XMLData.GetEntryById(XMLNode.ITEM, 1).description)
	```
???+ note "child nodes"
    Child nodes are returned as tables alongside the rest of the attributes. For example, if you want to access the samples of a sound entry, you can use `soundentry.sample[1]`.
???+ note "id?"
    The Id usually matches the actual id of the node in question, with the exception of cases like the entities.xml where ids are not unique, on those cases, the id is the order of the node and wont correspond with the actual id. On the cases of XMLs without ids, its just the order again.
___
### GetEntryByOrder () {: aria-label='Functions' }
#### table GetEntryByOrder ( [XMLNode](enums/XMLNode.md) NodeType, int Order ) {: .copyable aria-label='Functions' }
Similar to GetByName or GetById, but it returns the node based on the order in which it appears on the xmls (1 will return the first node, 2 the second one and so on). Useful to iterate through xmls in combination with GetNumEntries, specially for redundant xmls like entities.xml.
???- info "Table usage"
	```lua
	print("Sad Onion's description:", XMLData.GetEntryByOrder(XMLNode.ITEM, 1).description)
	```
???+ note "child nodes"
    Child nodes are returned as tables alongside the rest of the attributes. For example, if you want to access the samples of a sound entry, you can use `soundentry.sample[1]`.
???+ note "id?"
    The Id usually matches the actual id of the node in question, with the exception of cases like the entities.xml where ids are not unique, on those cases, the id is the order of the node and wont correspond with the actual id. On the cases of XMLs without ids, its just the order again.
___
### GetModById () {: aria-label='Functions' }
#### table GetModById ( string modId ) {: .copyable aria-label='Functions' }
Returns a table containing the attributes of the metdata xml of the matching mod id.
???- info "Table usage"
	```lua
	print("Car's mod name:", XMLData.GetModById("2788006730").name)
	```
???+ note "id?"
    The Id usually matches the actual id of the mod in the workshop, with the exception of cases where the mod was downloaded illegally and tampered with or if its an indev mod. If the mod doesnt have an id, then the directory is used as an id.
___
### GetEntryByName () {: aria-label='Functions' }
#### table GetEntryByName ( [XMLNode](enums/XMLNode.md) NodeType, string Name ) {: .copyable aria-label='Functions' }
Returns a table containing the attributes of the corresponding xml, the matching NodeType (Ex: XMLNode.TRINKET returns trinket nodes from pocketitems.xml) and match the given name parameter.
???- info "Table usage"
	```lua
	print("Sad Onion's description:", XMLData.GetEntryByName(XMLNode.ITEM, "The Sad Onion").description)
	```
???+ note "child nodes"
    Child nodes are returned as tables alongside the rest of the attributes. For example, if you want to access the samples of a sound entry, you can use `soundentry.sample[1]`.
___
### GetEntryFromEntity () {: aria-label='Functions' }
#### table GetEntryFromEntity ( [Entity](Entity.md) Entity, boolean AutoXMLPick = true, boolean Strict) {: .copyable aria-label='Functions' }
Returns a table containing the attributes of the provided entity. The `AutoXMLPick` parameter determines if only entities2.xml should be used or if it should pick the xml that matches the [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) (Ex: items.xml for pedestal collectibles) . The strict parameter determines if it should only return a value when the type,variant and subtype attributes match or return whatever matches the type and take the rest as maybes.
???- info "Table usage"
	```lua
	print("Player's birthright:", XMLData.GetEntryFromEntity(Isaac.GetPlayer()).birthright)
	```
???+ note "child nodes"
    Child nodes are returned as tables alongside the rest of the attributes. For example, if you want to access the samples of a sound entry, you can use `soundentry.sample[1]`.
___
### GetNumEntries () {: aria-label='Functions' }
#### int GetNumEntries ( [XMLNode](enums/XMLNode.md) NodeType) {: .copyable aria-label='Functions' }
Returns the number of entries a given XMLNode structure has.
___
