---
tags:
  - File
---
# File "stringtable.sta" ( Localization / Translations / Renaming )

**Content-Folder**{: .xmlInfo .green }: Using this file in a content folder will allow addition or replacement of translation strings.

REPENTOGON adds stringtable support, which can be used to add or modify translation strings (item/enemy names, etc).

This can be used to replace vanilla strings (such as to rename vanilla items), or add translations for your mod's own strings.

Please follow this guide to ensure future compatability.

## File Locations

There are two stringtable files: `stringtable.sta` and `stringtable_pc.sta`.

`stringtable.sta` is where the vast majority of relevant strings are located (items/enemy names, etc).

You may place your mod's `stringtable.sta` file in one of these locations:

- `mods/your-mod-name/content-repentogon/stringtable.sta`
- `mods/your-mod-name/content/stringtable.sta`

`stringtable_pc.sta` contains only a small number of system-related strings, such as the game exit confirmation message.

For translations of vanilla strings, you may place your `stringtable_pc.sta` file in one of these locations:

- `mods/your-mod-name/content-repentogon/stringtable_pc.sta`
- `mods/your-mod-name/content/stringtable_pc.sta`

## Minimium Example

Use this minimium template to create your mod's `stringtable.sta` file.

```xml
<?xml version="1.0" encoding="UTF-8"?>
<stringtable>
    <info version="1.0" rgon_sta_version="0"/>
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

Please do not modify the top portions such as the contents of the `info` and `languages` tags! These are required for the stringtable to load properly.

Every `key` string you add needs **EXACTLY** 8 strings, which matches the order of the languages defined at the top. That is, the first string is the English string, followed by the Japanese string, and so on.

## String Categories

The following are the most relevant string categories for modding purposes:

| Category Name | Relevant Modded XML | Description |
|:--|:--|:--|
| "Default" | N/A | Generic category used for various things, such as game inputs, transformations, system prompts/messages, and miscellaneous UI text. |
| "Items" | [items.xml](items.md) | `name` and `description` of collectibles and trinkets. |
| "PocketItems" | [pocketitems.xml](pocketitems.md) | `name`/`hud`/`description` of cards and pills. |
| "Players" | [players.xml](players.md) | `name` and `birthright` descriptions for players. |
| "Entities" | [entities2.xml](entities.md) | `name` for enemies, viewable in the besiary. |
| "Curses" | [curses.xml](curses.md) | `name` for curses. |
| "Challenges" | [challenges.xml](challenges.md) | `name` and `lockeddesc` for modded challenges only. Vanilla does not use this category, since vanilla challenge names are pngs. |
| "Minibosses" | N/A | Vanilla miniboss names, such as the sins. |
| "Stages" | N/A | Vanilla stage names. |

You can use any custom string as your category name, these are just the ones used naturally by the game.

## Replacing Existing Strings

To replace an existing string (such as the name of an item), such as one from vanilla, add an entry under the appropriate category, with the correct `key` of the string you'd like to replace.

Fill in the string for the language(s) you wish to replace the string for. Any languages that you leave empty will retain their original string and will not be overwritten.

The following example renames "The D6" to "The Epic D6", in English and Spanish only. For other languages, the name of the item is unchanged.

```xml
    <category name="Items">
        <key name="THE_D6_NAME">
            <string>The Epic D6</string>
            <string></string>
            <string></string>
            <string></string>
            <string></string>
            <string></string>
            <string>El D6 Épico</string>
            <string></string>
        </key>
    </category>
```

This replacement can work for the `stringtable.sta` of other mods as well, though your mod must load after the other mod in order for this to work.

## Adding New Strings

To add entirely new strings, such as ones for your mod's items and other named content, add an entry other the appropriate category, with a distinct `key` string (including an abbreviation of your mod's name can help prevent overlaps).

```xml
    <category name="Items">
        <key name="MYMOD_MY_COOL_ITEM_NAME">
            <string>My Item</string>
            <string>My Item</string>
            <string>My Item</string>
            <string>My Item</string>
            <string>My Item</string>
            <string>My Item</string>
            <string>My Item</string>
            <string>My Item</string>
        </key>
        <key name="MYMOD_MY_COOL_ITEM_DESC">
            <string>Wow!</string>
            <string>Wow!</string>
            <string>Wow!</string>
            <string>Wow!</string>
            <string>Wow!</string>
            <string>Wow!</string>
            <string>Wow!</string>
            <string>Wow!</string>
        </key>
    </category>
```

For new strings like these, you must provide a string for every language, as there is no automatic fallback to English. If you aren't supplying translations, it is fine to just copy and paste the English strings to all languages initially. You could add translations in the future, or they could even be supplied by another mod once you've provided the base stringtable!

You can also add strings under any arbitrary custom category name, if you wish.

## Using Translation Strings in your MOD

To use translation strings for your mod's content, pick a distinct key string for your supported string attribute (typically names/descriptions) and set the attribute to that key, prefixed by a `#`.

Not every single string supports translations. For the most part they are ones that the vanilla game supported, or are at least rendered somewhere as dynamic text.

For example, "null" items do not support translations since their names are never rendered anywhere.

### XMLs

Here is an [items.xml](items.md) passive collectible example, corresponding to the example above:

```xml
    <passive name="#MYMOD_MY_COOL_ITEM_NAME" description="#MYMOD_MY_COOL_ITEM_DESC" gfx="test.png" />
```

From then on, in any other XML or code where you are expected to use the name of your item, use the string key instead like `#MYMOD_MY_COOL_ITEM_NAME`.

For example, here is a modded challenge specifying the starting player/item/card by name, using their keys.

Note that the tainted version of the character is specified using the `-Tainted-` suffix. This still works when using translation string keys.

```xml
    <challenge name="#MYMOD_MY_CHALLENGE" lockeddesc="#MYMOD_LOCKED_DESC" endstage="1" playertype="#MYMOD_MY_PLAYER_NAME-Tainted-" startingitems="#MYMOD_MY_COOL_ITEM_NAME" startingcard="#MYMOD_MY_COOL_CARD" />
```

### Lua Code

In lua code, such as for any appropriate `Isaac.GetXByName` function, again use the key.

```xml
local MY_COOL_ITEM = Isaac.GetItemIdByName("#MYMOD_MY_COOL_ITEM_NAME")
```

Some functions may accept the local or English name and return the ID, but this behaviour is inconsistent. Please use the key everywhere.

You can get the string for a given category+key in the current game language via [Isaac.GetString()](../Isaac.md#getstring).

### Content ANM2s

In general, if you need to use the name of something like a card (`content/gfx/ui_cardfronts.anm2`) or a player, you can again use the key string such as `#MYMOD_MY_CARD` as the animation name.

However, for players, you can instead choose to have a separate animation for each of their localized names. If an animation exists that matches the player's current localized name, that animation will be used. If not, English is tried as a fallback, then finally the raw string key (`#MYMOD_MY_PLAYER_NAME`). This allows you to localize the menu/name gfx used for your character. This applies to many content folder anm2s since most of them are player related, though not all of them necessitate localization (such as `characterportraits`).
