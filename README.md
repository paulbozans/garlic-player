# garlic-player - Open Source SMIL Player for Digital Signage

The goal is to create a as most as neccessary SMIL 3.0 compatible and as most as possible platform independent free open source multimedia player to meet Digital Signage needs.
Garlic should be used online with web-based content managment systems like [SmilControl](https://smil-control.com) or other.
Offline usage via command-line is also possible.
Garlic is able to play local and remote (via http) SMIL Files with the features described below.

## Libraries used
 - [Qt5](https://www.qt.io) >= 5.10 (5.12.x recommended)
 - [QtAV](http://www.qtav.org) >= 1.13.0 as alternative to QtMultimedia
 - [Quazip](http://quazip.sourceforge.net) 0.7.3 (included in ./src/ext)
 - [zlib](https://zlib.net) 1.2.11 (included in ./src/ext)

## Current Operating System Supported
 - Linux
 - Windows
 - Android
 - OSX (CI is prepared)
  
  ### Successful tested with:
 - Linux 64Bit: Kde Neon, Kubuntu, Debian, Arch
 - Raspberry Pi: 3 and 4 and with a 64Bit rpi 4 (experimental)
 - Tinker Board: (Linaro OS 1.8) 
 - Android Armv7 (4.4.1, 5.0.1, 5.1.1, 6.0.2, 7.1 and 9)
 - Windows 64Bit: 7, 8 and 10
 - MacOS: High Sierra and Catalina 
 
For Android player-c2qml is needed, cause QtMultimedia-Widgets are not supported.

*garlic.sh [options] SMIL_INDEX*

## Command line options
 - -h, --help          Displays this help.
 - -v, --version       Displays version information.
 - -c, --config		   path to a config.xml
 - -m, --windows-mode  fullscreen, bigscreen (when multi monitor) or windowed mode
 - -z, --windows-size  size of windows (when windows mode) e.g. 980x540 means 980px width and 540px height
 - -s, --screen-select Starts in selected screen number

Arguments:
SMIL_INDEX        Path to SMIL index

Binaries can be downloaded at [garlic-player.com](https://garlic-player.com)

## Features
 - Support for multiple screens (not on Android)
 - Logging
 - 301 redirect support
 - lot's of video formats 
 - Media Cache
 - SMIL3 subset, HTML5, HTML5-Widgets and (depending on device) WebGL support

## actual supported SMIL-Features

### Time container elements
- **seq**
- **par**
- **excl**
- **priorityClass**

#### Attributes for excl/priorityClass time container
- **peers** stop|never|pause|defer
- **higher** stop|pause
- **lower** never|defer

### Media-Elements
- **video**
- **audio**
- **images**
- **html/text**
- **HTML Widgets** 

### other Elements
- **prefetch**
- **meta** for refresh index via http-equiv="Refresh"

#### Attributes for media
- **region**
- **fit** fill|scale|meetbest

### Attributes for media and time container:
- **xml:id** with Fallback for SMIL 2.0 id
- **title** stub
- **class** stub
- **xml:lang** stub
- **dur** with values indefinite, media, Clock values (full+partial clock),  normal (h min s and ms)
- **begin** with Clock values (like dur) and wallcock with periods and repeats. Date is supported only in full ISO 8601 format not short date like 201634W.
- **end** has same features as begin
- **repeatCount** only integer values (float support maybe implemented later)

### Layered multizone layouts elements:
- **root-layout**
- **layout**

### Multizone attributes
- **regionName**
- **top** values in px or %
- **left** values in px or %
- **width** values in px or %
- **height** values in px or %
- **z-index** integer values
- **backgroundColor** values are compatible with CSS

<!-- -->  HTML-like comments in SMIL-File are supported and content inside will ignored

## additional features supported not in w3c standard
 - **shuffle play** in seq container via metadata
 - **automatic refresh** via metatags in SMIL header
 - **Logs/reports** system and inventory-reports, events- and playlogs via subscription meta and delivery by webdav like described in [a-smil.org Reporting](http://www.a-smil.org/index.php/Reporting)
 - **expr-attribute** in media tags for conditional play as described in [a-smil.org](http://www.a-smil.org/index.php/Conditional_play) by [IAdea](http://www.iadea.com/)
 - http downloads and caching
 - caching of remote SMIL index file
 - caching of remote media files
 - refresh via meta tag in cyclic timer periods
 - http 301 redirects for index and media
 - change config.xml during plaback
 - remote update

## HotKeys
 - toogle fullscreen with Ctrl-F
 - toogle bigscreen with Ctrl-B
 - quit app with Ctrl-Q
 - open configuration dialog with Ctrl-S

## future ToDo's
 - interactivity trigger (touch, keyboard, ean-codes)
 - REST Api 

Contact: [Nikolaos Sagiadinos] (mailto:ns@smil-control.com)
