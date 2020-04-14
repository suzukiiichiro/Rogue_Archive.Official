RogueNano
===

A 400 characters long rougelike, written in perl, based on libcurses-perl

See game.pl for minified code and readable.pl for annotated version.

How to run
---
```shell
sudo apt install libcurses-perl
stty cols 80; ./game.pl
```

Features:
---

* Vi-keys movement (diagonal movement is not implemented)
* Random level generation
* Monsters with basic 'AI', you can kill them too
* Infinite levels!
* Only 80x25 terminals supported
* Requires libcurses-perl package to work
* Licensed under WTFPL
