#!/usr/bin/perl
# Copyright © 2017 Olga Ustuzhanina <me@laserbat.pw>
# This work is free. You can redistribute it and/or modify it under the
# terms of the Do What The Fuck You Want To Public License, Version 2,
# as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

# Please excuse English used in these annotations. I wrote them a long time ago
# in a big hurry, and my English was way wore back then.

# Curses module, used for terminal I/O
use Curses;
 
# Important variables:
# @w - indexes of every tile on the map.
# @m - offsets for movement keys.
# $v - position of stairs
# $p - position of player
# @l - level
 
# Cell types:
# 0 - "#" - wall
# 1 - "." - floor
# 2 - ">" - stairs
# 3 - reserved
# 4 - "B" - monster
 
# The level is 1D array, it is indexed by one variable. The index in it can be
# converted to x, y coordinates by using division and modulo 80.
# x = i % 80
# y = i / 80
 
# To convert backwards:
# i = y * 80 + k
 
for(
      @w = 0..1920; # Used for indexing the map.
      @m = (-1, clear, 80, -80, 1); # Directions, calls clear every update
      $l[$v] = 2 # Put stairs in its place.
   ){
 
   # Render level.
 
   # Since (x ^ y = 0) => (x != y)
   # Xor is used to check that current cell isn't occupied by player.
   # B in that array is same thing as "B" but uses less characters.
   addch $_ ^ $p ? ("#", ".", ">", 0, B)[$l[$_]] : "@" for @w;
 
   # Read input from used and move.
 
   # Puts in variable $z new possible position of player.
   # Movement is done by vi-keys so it expects keys h, j, k, l
   # h has index 104 in ASCII table, j's index is 106 (that's why second element
   # of @m is zero), k - 107, l - 108.
   # $m[index - 104] is offset for new position on the map.
   # x & 3 is zero when x is 4 or 0. It checks if new position is occupied by
   # a wall or a monster.
 
   $p = $z if $l[$z = $p + $m[ord(getch) - 104]] & 3;
 
   # Since monsters are represented by value 4, and 4 % 3 == 1 (floor tile)
   # it creates a floor tile in place where monster used to be when player
   # moves into a montser.
 
   $l[$z] %= 3;
 
   # Player stepped on stairs.
 
   # x & 2 is not zero when x is 2. 2 represents stairs.
   # This code generates new level when player steps on stairs.
   if($l[$p] & 2){
         # Generate level.
 
         @l = 0 * initscr; # initscr returns some data so 0 * is needed to
         # create empty level.
 
         # map is used to repeat code 100 times.
         map{
            # Checks if current cell ($_) is inside a circle-like curve
            # defined by formula x^2 + abs(y) < 6
            # $n is offset of this curve from center.
            ($_ % 80 - $n % 80)**2 + abs($_ - $n) / 80 < 6?
 
               # If it is, roll a 100 sided virtual die and check if result is
               # 0 or 1. If it is put a monster in this cell and update $p
               # if it isn't put a floor tile here.
 
               # 4 % ~($p = $_) is (almost?) always 4 and updates $p with $_.
               # It will be useful later.
               # $p here is not related to position of player.
               $l[$_] = rand 99 < 2 ? 4 % ~($p = $_) : 1
 
               : 0 # Do nothing.
 
            for @w, # The code above is repeated for ever cell on
            # the map, $_ is position of cell being worked on currently.
 
            $n = $p + $n % 4 # Set the new center of the curve to place where
            # the last monster was put and add ($n % 4) to make it look a bit
            # more random.
         }  0..99;
 
         # Do nothing untill random values for $v (position of stairs) and $p
         # (position of player) are found and are both on unocuppied tiles.
         1 until $l[$v = rand @w] & $l[$p = rand @w]
      }
 
      $l[$_] & 4? # Checks if cell is 4 (monster)
         # $n is the new position of current monster.
 
         # This code checks if new position is floor ($l[$n] == 1)
         # and if it's not occupied by player ($n ^ $p, xor being used as !=)
         # if it isn't it changes current cell to 9 | something
         # Which will be reset to 4 later.
 
         # If it is, it sets $l[$n] to 9 | something and current cell is reset
         # to 1 by the second part of this expression.
         $l[$l[$n] == 1 && $n ^ $p? $n : $_]=9 |
         ($n = $_ + $m[rand 4 + ($l[$_]=1)])
      : 0 # Do nothing.
 
      for @w; # For every cell on the map.
 
      # For every cell on the level, cells that are bigger than 3 with 4.
      # Keep other cells same.
      map $_ = $_ > 3? 4 : $_, @l
}
