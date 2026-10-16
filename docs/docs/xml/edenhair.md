---
tags:
  - File
---
# File "edenahir.xml"

**Resource-Folder**{: .xmlInfo .red}: Using this file in a resource folder will have NO EFFECT!

**Content-Folder**{: .xmlInfo .green }: Using this file in a content folder will add new eden hairstyles.

## Eden Hair

REPENTOGON allows you to include an `edenhair.xml` file in your content folder to add new eden hairstyles. All available eden hairstyles are equally likely to get picked.

Spritesheets for custom Eden hairstyles must match the layout of the vanilla `edenhair` spritesheets.

```xml
<edenhair root="gfx/characters/costumes/">
	<hair gfx="eden_hair_test_1.png" />
	<hair gfx="eden_hair_test_2.png" />
	<hair gfx="eden_hair_test_3.png" />
</edenhair>
```

A spritesheet in this example would be located at `resources/gfx/characters/costumes/eden_hair_test_1.png`.

| Variable-Name | Possible Values | Description |
|:--|:--|:--|
|gfx|string|Path to the hair spritesheet, starting from the `root` (see example below).|
