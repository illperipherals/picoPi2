-----
#### This is pico2wave like prog. 


#### v0.0.1

You can compile picoPi2Wav to output text into .wav file
of
you can compile picoPi2Std to output it directly in STD

maybe like this:

./picoPi2Std | aplay -f S16_LE -r 16000

It can save some IOPS-&-MMC-life time/duration


-----

#### First we need one library "libsvoxpico.so"

cd into ../lib/ directroy en run && make

```bash
cd lib  
make  

```

"Copying the library to /usr/lib/"

```bash

cp libsvoxpico.so /usr/lib/
```

#### Configuration 

For configuration (LANG, TYPE, etc)

cd into tts and run ./confiruge.py

It will geve nice menu:

Just select what you want:

#### CONF-SCREEN

```bash

|--------------------------------------------------------------------------------|
       Welcome to picoPi2
|--------------------------------------------------------------------------------|
| Usage: ./configure.py [ --make | --lang-tts | lang-bin ]                       |
|                                                                                |
|    --help:             Display this help                                       |
|    --make:             Compile                                                 |
|    --lang-tts:         Select TTS language     (Default USA)                   |
|                            [GBR, USA, FRA, DEU, SPA, ITA]                      |
|    --lang-bin-dir:     Binary languages directory     (Default "../lang/")     |
|    --2wav:             Binary wil output sound to '.wav' file. (Default)       |
|    --2std:             Binary wil output sound to std.                         |
|    --version:          Print current Version                                   |
|--------------------------------------------------------------------------------|
| Sample:                                                                        |
|                                                                                |
|    [./configure.py --make ] -> Compile (default setting)                       |
|    [./configure.py --2wav ] picoPi2Wav -w file.wav 'this' && aplay file.wav    |
|    [./configure.py ] picoPi2Stdout | aplay                                     |
|    [./configure.py --make lang-tts SPA] -> Compile Spanish TTS                 |
|    [./configure.py --make lang-bin "/path/to/"] -> Force to use this dir       |
|--------------------------------------------------------------------------------|
|                                                                                |
| Version: v0.0.1                                                                |
| Dev-Page: https://github.com/ch3ll0v3k/picoPi2                                 |
|                                                                                |
|--------------------------------------------------------------------------------|
```



#### CONFIRM-SCREEN

```bash

|--------------------------------------------------------------------------------|
| This SETTING wil be used:                                                      |
|                                                                                |
|    --make:             "True"                                                  |
|    --lang-tts:         "USA"                                                   |
|    --lang-bin-dir:     "../lang/"                                              |
|    --2wav:             "True"                                                  |
|     Binary name:       "./picoPi2Wav"                                          |
|                                                                                |
|--------------------------------------------------------------------------------|
|                                                                                |
 Correct SETTINGS (Y/N) $>

```
