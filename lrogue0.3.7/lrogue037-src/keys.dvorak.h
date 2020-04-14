/*
 * keys.dvorak.h
 *
 * Created by Miguel G. C.
 */

#ifndef _KEYS_DVORAK_H_
#define _KEYS_DVORAK_H_

#define ROGUE_KEY_NORTH           'c'
#define ROGUE_KEY_SOUTH           'w'
#define ROGUE_KEY_EAST            'n'
#define ROGUE_KEY_WEST            'h'
#define ROGUE_KEY_NORTHEAST       'r'
#define ROGUE_KEY_NORTHWEST       'g'
#define ROGUE_KEY_SOUTHEAST       'v'
#define ROGUE_KEY_SOUTHWEST       'm'

#define ROGUE_KEY_NORTH_SHIFT     (ROGUE_KEY_NORTH+('A'-'a'))
#define ROGUE_KEY_SOUTH_SHIFT     (ROGUE_KEY_SOUTH+('A'-'a'))
#define ROGUE_KEY_EAST_SHIFT      (ROGUE_KEY_EAST+('A'-'a'))
#define ROGUE_KEY_WEST_SHIFT      (ROGUE_KEY_WEST+('A'-'a'))
#define ROGUE_KEY_NORTHEAST_SHIFT (ROGUE_KEY_NORTHEAST+('A'-'a'))
#define ROGUE_KEY_NORTHWEST_SHIFT (ROGUE_KEY_NORTHWEST+('A'-'a'))
#define ROGUE_KEY_SOUTHEAST_SHIFT (ROGUE_KEY_SOUTHEAST+('A'-'a'))
#define ROGUE_KEY_SOUTHWEST_SHIFT (ROGUE_KEY_SOUTHWEST+('A'-'a'))

#define ROGUE_KEY_NORTH_CTRL      (ROGUE_KEY_NORTH-('a'-1))
#define ROGUE_KEY_SOUTH_CTRL      (ROGUE_KEY_SOUTH-('a'-1))
#define ROGUE_KEY_EAST_CTRL       (ROGUE_KEY_EAST-('a'-1))
#define ROGUE_KEY_WEST_CTRL       (ROGUE_KEY_WEST-('a'-1))
#define ROGUE_KEY_NORTHEAST_CTRL  (ROGUE_KEY_NORTHEAST-('a'-1))
#define ROGUE_KEY_NORTHWEST_CTRL  (ROGUE_KEY_NORTHWEST-('a'-1))
#define ROGUE_KEY_SOUTHEAST_CTRL  (ROGUE_KEY_SOUTHEAST-('a'-1))
#define ROGUE_KEY_SOUTHWEST_CTRL  (ROGUE_KEY_SOUTHWEST-('a'-1))

#define ROGUE_KEY_INSTRUCTIONS    '?'
#define ROGUE_KEY_REST            't'
#define ROGUE_KEY_SEARCH          's'
#define ROGUE_KEY_INVENTORY       'i'
#define ROGUE_KEY_FIGHT           'f'
#define ROGUE_KEY_FIGHT_TO_DEATH  'F'
#define ROGUE_KEY_EAT             'e'
#define ROGUE_KEY_QUAFF           'q'
#define ROGUE_KEY_READ            'l'
#define ROGUE_KEY_MOVE            '.'
#define ROGUE_KEY_DROP            'd'
#define ROGUE_KEY_PUT_ON_RING     'P'
#define ROGUE_KEY_REMOVE_RING     'Y'
#define ROGUE_KEY_DROP_CHECK      '>'
#define ROGUE_KEY_CHECK_UP        '<'
#define ROGUE_KEY_INV_WEAPON      ')'
#define ROGUE_KEY_INV_ARMOR       ']'
#define ROGUE_KEY_INV_RINGS       '='
#define ROGUE_KEY_ID_TRAP         '^'
#define ROGUE_KEY_SINGLE_INV      'I'
#define ROGUE_KEY_TAKE_OFF        'K'
#define ROGUE_KEY_WEAR            'U'
#define ROGUE_KEY_WIELD           'u'
#define ROGUE_KEY_CALL            'x'
#define ROGUE_KEY_ZAPP            'z'
#define ROGUE_KEY_THROW           'j'
#define ROGUE_KEY_PICK_UP         ','
#define ROGUE_KEY_MULTIMOVE       ']'

#define ROGUE_KEY_REMESSAGE       '\020'
#define ROGUE_KEY_VERSION         '\002'
#define ROGUE_KEY_QUIT            'Q'
#define ROGUE_KEY_SAVE_GAME       'S'
#define ROGUE_KEY_WHATISIT        '/'
#define ROGUE_KEY_NOP             ' '

#define ROGUE_KEY_CANCEL          '\033'
#define ROGUE_KEY_LIST            '*'
#define ROGUE_KEY_SAVE_SCREEN     'X'
#define ROGUE_KEY_REFRESH         '\014'
#define ROGUE_KEY_FLUSH           '\032'


#define ROGUE_KEY_WIZARDIZE       '\030'
#define ROGUE_KEY_WIZ_INVENTORY   '\011'
#define ROGUE_KEY_WIZ_MAGIC_MAP   '\023'
#define ROGUE_KEY_WIZ_SHOW_TRAPS  '\013'
#define ROGUE_KEY_WIZ_SHOW_OBJS   '\017'
#define ROGUE_KEY_SHOW_AV_HP      '\001'
#define ROGUE_KEY_WIZ_NEW_OBJ     '\177'
#define ROGUE_KEY_WIZ_SHOW_MONST  '\012'

#endif	/* _KEYS_DVORAK_H_ */
