---
tags:
  - File
---
# File "stringtable.sta"

**Content-Folder**{: .xmlInfo .green }: Using this file in a content folder will add new strings.

REPENTOGON add stringtable support. Since it's an extension, please follow this guide, to ensure future compatability.

## Mod File Location

REPENTOGON loads `stringtable.sta` from one of these location:

- `mods/your-mod-name/content-repentogon/stringtable.sta`
- `mods/your-mod-name/content-dlc3/stringtable.sta`
- `mods/your-mod-name/content/stringtable.sta`

REPENTOGON loads `stringtable_pc.sta` from one of these location:

- `mods/your-mod-name/content-repentogon/stringtable_pc.sta`
- `mods/your-mod-name/content-dlc3/stringtable_pc.sta`
- `mods/your-mod-name/content/stringtable_pc.sta`

REPENTOGON supports add/overwrite key strings in `stringtable.sta` with other `stringtable.sta`, or overwrite items in `stringtable_pc.sta` with other `stringtable_pc.sta`.

In current version, the sta load order in different mods is not defined.

## Minimium Example
Use this minimium template.
```xml
<?xml version="1.0" encoding="UTF-8"?>
<stringtable>
    <info version="1.0" modified="Tue Nov 19 17:07:06 2024" modifiedby="XXX" rgon_sta_version="0"/>
    <languages>
        <language id="21" index="0" name="Key"/>
        <language id="0" index="1" name="English"/>
        <language id="2" index="2" name="Japanese"/>
        <language id="11" index="3" name="Korean"/>
        <language id="13" index="4" name="Chinese (Simple)"/>
        <language id="10" index="5" name="Russian"/>
        <language id="5" index="6" name="German"/>
        <language id="4" index="7" name="Spanish"/>
        <language id="3" index="8" name="French"/>
    </languages>
    <category name="YOUR_CATEGORY">
        <key name="YOUR_KEY_NAME">
            <string>I'm english.</string>
            <string>I'm english.</string>
            <string>I'm english.</string>
            <string>I'm english.</string>
            <string>I'm english.</string>
            <string>I'm english.</string>
            <string>I'm english.</string>
            <string>I'm english.</string>
        </key>
    </category>
</stringtable>
```
### Fallback behavior
As a modder, you need copy and paste your english text to other languages, so players in that language can see your english strings instead of an empty string. Repentogon only automatically apply a fallback fix for the vanilla game's `sta` files. Modders should do it manually in case they do want an empty string.

### Merge behavior
You can overwrite an existing string if your `category/key` exactly match an exist one. If you leave a `string` with empty value, and it already have existing value, your empty value will be ignored instead of overwriting the existing value.

### Languages and orders
Every `key` tags need **EXACTLY** 8 strings, which match the order of game defines. 

Your sta file still works if some keys have more or less than 8 strings, but do NOT rely on this feature. 

### `info` tag
The `info` tag is required, and the attribute `rgon_sta_version` is required to be `0`, or your sta file will be refused to load. This is for future compat. See `log.txt` if something goes wrong.

### `languages` tags
It's highly recommand to keep the `languages` tag, and keep it exactly match the current game's key. They're ignored by repentogon but please keep this for future compatability.
