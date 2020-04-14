#!/usr/bin/perl
# Copyright © 2017 Olga Ustuzhanina <me@laserbat.pw>
# This work is free. You can redistribute it and/or modify it under the
# terms of the Do What The Fuck You Want To Public License, Version 2,
# as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

# Folowing code is a complete and working roguelike game, in 400 characters.
# Requires 80x24 terminal and libcurses-perl module to be installed.
# See 'readable.pl' for annotated version of this code.
use Curses;for(@w=0..1920;@m=(-1,clear,80,-80,1);$l[$v]=2){addch$_^$p?("#",".",">",0,B)[$l[$_]]:"@"for@w;$p=$z if$l[$z=$p+$m[ord(getch)-104]]&3;$l[$z]%=3;if($l[$p]&2){@l=0*initscr;map{($_%80-$n%80)**2+abs($_-$n)/80<6?$l[$_]=rand 99<2?4%~($p=$_):1:0for@w,$n=$p+$n%4}0..99;1until$l[$v=rand@w]&$l[$p=rand@w]}$l[$_]&4?$l[$l[$n]==1&&$n^$p?$n:$_]=9|($n=$_+$m[rand 4+($l[$_]=1)]):0for@w;map$_=$_>3?4:$_,@l}
