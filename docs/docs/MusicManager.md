---
tags:
  - Class
---
# Class "MusicManager"

## Modified Functions
### Crossfade () {: aria-label='Modified Functions' }
#### void Crossfade ( [Music](https://wofsauge.github.io/IsaacDocs/rep/enums/Music.html) MusicId, float FadeRate = 0.08 ) {: .copyable aria-label='Modified Functions' }
Now validates the Music ID to prevent crashes.

___
### Fadein () {: aria-label='Modified Functions' }
#### void Fadein ( [Music](https://wofsauge.github.io/IsaacDocs/rep/enums/Music.html) MusicId, float Volume = 1, float Volume = 0.08 ) {: .copyable aria-label='Modified Functions' }
Now validates the Music ID to prevent crashes.

___
### Play () {: aria-label='Modified Functions' }
#### void Play ( [Music](https://wofsauge.github.io/IsaacDocs/rep/enums/Music.html) MusicId, int Volume = -1 ) {: .copyable aria-label='Modified Functions' }
Now validates the Music ID to prevent crashes.

___

## Functions

### GetCurrentJingleID () {: aria-label='Functions' }
#### [Music](https://wofsauge.github.io/IsaacDocs/rep/enums/Music.html) GetCurrentJingleID ( ) {: .copyable aria-label='Functions' }
Returns the id of the currently playing jingle, or 0 if no jingle is playing/current jingle is fading out.

___
### GetCurrentPitch () {: aria-label='Functions' }
#### float GetCurrentPitch ( ) {: .copyable aria-label='Functions' }

___
### PlayJingle () {: aria-label='Functions' }
#### void PlayJingle ( [Music](https://wofsauge.github.io/IsaacDocs/rep/enums/Music.html) MusicId, int Duration = 140 ) {: .copyable aria-label='Functions' }

___
### SetCurrentPitch () {: aria-label='Functions' }
#### void SetCurrentPitch ( float Pitch ) {: .copyable aria-label='Functions' }

___
### StopJingle () {: aria-label='Functions' }
#### void StopJingle ( ) {: .copyable aria-label='Functions' }
Force currently playing jingle to immediately fade out.
___