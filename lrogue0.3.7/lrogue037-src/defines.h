/* define.h - defines for Rogue
  The new features I've added have increased the complexity of the game
  such that I think this would be a useful way to turn on or off the
  modifications.  It's also a nice central location for score
  file locations since those can vary widely.

  Hopefully the comments will be explanation enough.

  Version 0.3.7 - Greg Kennedy (kennedy.greg@gmail.com) - removed ROGUE_INSTRPATH
	and COMPAT_SCORES.
  Version 0.3.5 - Greg Kennedy (kennedy.greg@gmail.com) - initial version.
*/

/* Path to the score file.  The default is in the local directory.  Alain
  Williams suggests using "/var/games/RogueScores" or some other central
  location to enable multiple players on one machine.
  You must also give a ROGUE_SAVEPATH if you intend to enable
  ONE_SAVE_PER_PLAYER below.
  As of version 0.3.7 rogue.instr is compiled into the game. */

#define ROGUE_SCOREFILE ".roguescores"
#define ROGUE_SAVEPATH "./"

/* If you'd like to enable the number pad for movement, uncomment this define.
  You can also hit 0 to get inventory, . to rest, and 5 to search.
  You can only use this with Num Lock off; when on, it reverts to old behavior
  (like the number keys on top of the keyboard)
  This is an option because it has the following bugs:
   * when defined, the ESC key must be tapped twice in-game
   * SHIFT+key movement doesn't work for number pad
   * CTRL+key movement only works for left, right, up and down. */

//#define NUMPAD

/* These options change how the high score table is written:
  MAX_SCORES sets the maximum size of the table.
  REPEAT_NAMES allows users to be on the table multiple times (default
   behavior is only to save the top score for each userid)
  Note that changing these options may BREAK COMPATABILITY BETWEEN SCORE FILES.
  If you have an existing high score table, the only safe changes you can make
  here are to increase MAX_SCORES or enable REPEAT_NAMES. */

#define MAX_SCORES 10
#define REPEAT_NAMES

/* Personally I think the game should ONLY be played where your saves are
  deleted when they're loaded. </soapbox> 
  But, if you can't take the challenge, uncomment this define and your save
  games will not be deleted.  This will interact badly with the next option
  (ONE_SAVE_PER_PLAYER) because your one save will never be deleted, so don't
  enable both at the same time. */

// #define SAVE_CHEAT

/* This option enables Nethack-like behavior of allowing only one
  game-in-progress per player, and automatically restoring the save game the
  next time the player logs in (instead of the default "./rogue <savename>"
  to restore your games).  Combined with the next option (ASKNAME), this makes
  a good setup for a multi-user Rogue setup with one user name. */

// #define ONE_SAVE_PER_PLAYER

/* This option disables the user ID check at the beginning and instead
  prompts the user to enter their name.  Useful in situations where you'd
  like a shared username for multiple players solely for playing games. */

#define ASKNAME

/* Choose your keyboard layout (you must define one or the other): */

#define KEYS_QWERTY
// #define KEYS_DVORAK
