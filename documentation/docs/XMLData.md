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
### GetEntryById () {: aria-label='Functions' }
#### table GetEntryById ( [XMLNode](enums/XMLNode.md) NodeType, int Idx ) {: .copyable aria-label='Functions' }
Returns a table containing the attributes of the corresponding xml, the matching NodeType(Ex: XMLNode.TRINKET returns trinket nodes from pocketitems.xml) and match the given unique id.
???- info "Table usage"
	```lua
	print("Sad Onion's description:", XMLData.GetEntryByName(XMLNode.ITEM, 1).description)
	```
???+ note "child nodes"
    Child nodes are returned as tables alongside the rest of the attributes. For example, if you want to access the samples of a sound entry, you can use `soundentry.sample[1]`.
???+ note "id?"
    The Id usually matches the actual id of the node in question, with the exception of cases like the entities.xml where ids are not unique, on those cases, the id is the order of the node and wont correspond with the actual id. On the cases of XMLs without ids, its just the order again.
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
