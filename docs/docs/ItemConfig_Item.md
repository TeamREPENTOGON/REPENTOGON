---
tags:
  - Class
---
# Class "ItemConfigItem"

## Functions

### AddCustomCacheTag () {: aria-label='Functions' }
#### void AddCustomCacheTag ( string Tag ) {: .copyable aria-label='Functions' }
Adds the provided string to the table of custom cache tags. See [items.xml](xml/items.md) for more information on `customcache`.

___
### AddCustomTag () {: aria-label='Functions' }
#### void AddCustomTag ( string Tag ) {: .copyable aria-label='Functions' }
Adds the provided string to the table of custom tags. See [items.xml](xml/items.md) for more information on `customtags`.

___
### GetCustomCacheTags () {: aria-label='Functions' }
#### table GetCustomCacheTags ( ) {: .copyable aria-label='Functions' }
Returns a table containing all the strings specified in the item's `customcache` attribute in [items.xml](xml/items.md). Tags are always provided in all lowercase. See [items.xml](xml/items.md) for more information on `customcache`.

___
### GetCustomTags () {: aria-label='Functions' }
#### table GetCustomTags ( ) {: .copyable aria-label='Functions' }
Returns a table containing all the strings specified in the item's `customtags` attribute in [items.xml](xml/items.md). Tags are always provided in all lowercase. See [items.xml](xml/items.md) for more information on `customtags`.

___
### HasCustomCacheTag () {: aria-label='Functions' }
#### boolean HasCustomCacheTag ( string tag ) {: .copyable aria-label='Functions' }
Returns true if the items has the provided string specified in its `customcache` attribute in [items.xml](xml/items.md). Capitalization does not matter. See [items.xml](xml/items.md) for more information on `customcache`.

___
### HasCustomTag () {: aria-label='Functions' }
#### boolean HasCustomTag ( string tag ) {: .copyable aria-label='Functions' }
Returns true if the items has the provided string specified in its `customtags` attribute in [items.xml](xml/items.md). Capitalization does not matter. See [items.xml](xml/items.md) for more information on `customtags`.

___
### RemoveCustomCacheTag () {: aria-label='Functions' }
#### void RemoveCustomCacheTag ( string Tag ) {: .copyable aria-label='Functions' }
Removes the provided string from the table of custom cache tags. See [items.xml](xml/items.md) for more information on `customcache`.

___
### RemoveCustomTag () {: aria-label='Functions' }
#### void RemoveCustomTag ( string Tag ) {: .copyable aria-label='Functions' }
Removes the provided string from the table of custom tags. See [items.xml](xml/items.md) for more information on `customtags`.

___
