# Table "XMLData"

???+ info
    A public table containing all the functions related to gathering XML attributes accross the different xmls with updated values to match the real values.
        
## Functions

### Get·Entity·By·Type·Var·Sub () {: aria-label='Functions' }
#### table GetEntityByTypeVarSub ( [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) type, int variant = 0 , int subtype = 0, bool strict = false) {: .copyable aria-label='Functions' }
Returns a table containing the attributes of the entity on entities2.xml that match the given type and/or variant and/or subtype. The strict parameter determines if it should only return a value when all 3 attributes(type, var and sub) match or return whatever matches the type and take the rest as maybes. All of the attributes are lowercase for ease of access.
???- info "Table usage"
	```lua
	print("Monstro's BossID:", XMLData.GetEntityByTypeVarSub(20).bossid)
	```
___

### Get·Entry·By·Name () {: aria-label='Functions' }
#### table GetEntryByName ( [XMLNode](XMLNode.md) nodetype, string name ) {: .copyable aria-label='Functions' }
Returns a table containing the attributes of the corresponding xml, the matching nodetype(Ex: XMLNode.TRINKET returns trinket nodes from pocketitems.xml) and match the given name parameter. All of the attributes are lowercase for ease of access.
???- info "Table usage"
	```lua
	print("Sad Onion's description:", XMLData.GetEntryByName(XMLNode.ITEM,"The Sad Onion").description)
	```
___

### Get·Entry·From·Entity () {: aria-label='Functions' }
#### table GetEntryFromEntity ( [Entity](Entity.md) entity, bool autoxmlpick = true, bool strict) {: .copyable aria-label='Functions' }
Returns a table containing the attributes of the provided entity. The autoxmlpick parameter determines if only entities2.xml should be used or if it should pick the xml that matches the entitytype(Ex: items.xml for pedestal collectibles) . The strict parameter determines if it should only return a value when the type,variant and subtype attributes match or return whatever matches the type and take the rest as maybes. All of the attributes are lowercase for ease of access.
???- info "Table usage"
	```lua
	print("Player's birthright:", XMLData.GetEntryFromEntity(Isaac.GetPlayer()).birthright)
	```
___




