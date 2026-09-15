---
tags:
  - File
---
# File "babies.xml"

**Resource-Folder**{: .xmlInfo .red}: Using this file in a resource folder of a mod will replace the original file.

**Content-Folder**{: .xmlInfo .green }: Using this file in a content folder will add new co-op babies!

## Babies

With REPENTOGON, you can now include a `babies.xml` in your mod's content folder to add new co-op babies! They will also be available to Buddy in a Box as usual.

```xml
<babies root="gfx/characters/player2/">
	<baby name="REPENTOGON Baby" skin="rgon_baby.png"/>
	<baby name="REPENTOGON Baby 2" skin="rgon_baby_2.png"/>
</babies>
```

A baby's spritesheet in this example would be located at `resources/gfx/characters/player2/rgon_baby.png`.

| Variable-Name | Possible Values | Description |
|:--|:--|:--|
|name|string|The name of the baby.|
|skin|string|Path to the baby's spritesheet, starting from the `root` (see example below).|
|achievement|int or string|Optionally allows you to lock your baby behind an achievement (modded achievements must be specified by name).|
|achievement|boolean|Prevents the baby from appearing naturally.|
