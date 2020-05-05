# PuPPlugin for BAM (template)
You can replace any FP texture with another. That another texture may be updated every frame and contain video.

### Limitations
- Game light have impact how texture looks.
- No mipmaps generated for texture with video. So, high res video may look bad.

### How to run & test
1. In BAM dir you will find **BAM.dll** **v.284**. This version is required. Copy to **C:\Games\Future Pinball\BAM** dir.
2. After build  copy **PuPPlugin.dll** to **C:\Games\Future Pinball\BAM\Plugins** dir.
3. In BAM dir is **puptest.fpt**. 
You can test with it. After run you can change Einstein poster with keys: **F,G,H,J** to different texture.
With **K** you will see animated bars (simple video placeholder).


# Features
### 1. Script
If you need to communicate with FP script, add new command you can do it.
If you don't, you may just remove **BAM_GetCOM** function from **main.cpp**.
In **PuPCOM.idl** you will find 4 command:
- **Msg** _"text"_  - Message box...
- **GetTextureId** _"texturename"_ - argument is FP texture name, it returns Id (OpenGL texture id)
- **ReplaceTexture** _orgTextureId_, _dstTextureId_ - replace _orgTextureId_ with _dstTextureId_.
- **ReplaceTextureWithVideo** _targetTextureId_, _videoStreamId_ - used to play video stream in place of texture
- **ShowVideo** _"texturename"_, _"windowname"_, useRegEx [optional, default = **FALSE**]  - play video stream in place of texture
- **StopVideo** _"texturename"_ - stops video

In FP you can have acces to new commands:
```
Dim PP 
Set PP = xBAM.Get("PuPPlugin")
T0 = PP.GetTextureId("einstein poster")
PP.ReplaceTextureWithVideo T0, 1
```

You can also check if PuPPlugin is present:
```
If PP Is Nothing Then
	AddDebugText "PuP Plugin is missing"
Else
    ' Do somthing with PP
End If
```

### 2. Usage cases:
1. Initialization
```
Dim PP 
Set PP = xBAM.Get("PuPPlugin")
```
2. Play Video from PUPSCREEN2 on "einstein poster" texture
```
PP.ShowVideo "einstein poster", "PUPSCREEN2"
```
3. Show notepad add
```
PP.ShowVideo "einstein poster", "Notepad", true
```
**Note:** last param: **useRegeEx = true** allows to select any window with "Notepad" in title, like "*** new document - Notepad"
4. Show same video on 2 textures: "einstein poster" & "p1-beachwood"
```
PP.ShowVideo "einstein poster", "PUPSCREEN2"
PP.ShowVideo "p1-beachwood", "PUPSCREEN2"
```
Yes. You will see same on 2 different places.
5. Show 3 different videos on 3 textures:
```
PP.ShowVideo "einstein poster", "PUPSCREEN1"
PP.ShowVideo "p1-beachwood", "PUPSCREEN2"
PP.ShowVideo "flipper-t1-yellow-red", "Notepad", true
```
6. Stop video on selected texture:
```
PP.StopVideo "einstein poster"
```

### 3. Video
In **PuPVideo.cpp** you can find how to update texture and replace any FP texture with somthine else.

### 4. ToDo
- Add shaders to replace FP/BAM draw. Right now replaced texture will be just texture over FP object and game lights will have impact how it looks.
