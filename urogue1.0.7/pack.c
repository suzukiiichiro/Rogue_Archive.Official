/*
    pack.c - Routines to deal with the pack.
     
    UltraRogue: The Ultimate Adventure in the Dungeons of Doom
    Copyright (C) 1985, 1986, 1992, 1993, 1995 Herb Chong
    All rights reserved.

    Based on "Advanced Rogue"
    Copyright (C) 1984, 1985 Michael Morgan, Ken Dalka
    All rights reserved.

    Based on "Rogue: Exploring the Dungeons of Doom"
    Copyright (C) 1980, 1981 Michael Toy, Ken Arnold and Glenn Wichman
    All rights reserved.

    See the file LICENSE.TXT for full copyright and licensing information.
*/

/*
    Notes

        The new pack is implemented through the use of bags,
        and items are classed by their types (see rogue.h) which also
        happen to be their display character.
*/

#include <stdlib.h>
#include <ctype.h>
#include "rogue.h"

#define ESCAPE_EXIT(x) if (x == ESCAPE) {after = FALSE; msg(""); return(NULL);}
#define BAD_NEWS    -1
#define BAD_LIST ((struct linked_list *) BAD_NEWS)
#define GOOD_NEWS   0

static char type_list[] = "!?])/=:,";   /* things to inventory */

/*
    swap_top()
        Takes an stacked object and exchanges places with the top
        object. <node> must belong to the <top>'s stacked object list.
*/

void
swap_top(struct linked_list *top, struct linked_list *node)
{
    struct object   *obt, *obn;

    obt = OBJPTR(top);
    obn = OBJPTR(node);

    detach((obt->next_obj), node);  /* Take it out of the stack */
    attach(lvl_obj, node);  /* and put it into the level */
    detach(lvl_obj, top);   /* Remove item from level */

    if (obn->next_obj = obt->next_obj)
        obn->next_obj->l_prev = NULL;

    attach((obn->next_obj), top);
}


/*
    get_all()
        Get as many stacked items as possible.
*/

void
get_all(struct linked_list *top)
{
    struct linked_list  *node;
    struct object   *obt;

    while (top)
    {
        obt = OBJPTR(top);
        node = obt->next_obj;

        rem_obj(top, FALSE);

        if (!add_pack(top, FALSE))
            return;

        top = node;
    }
}


/*
    get_stack()
        Allows the user to chose from a stack of items.
*/

struct linked_list *
get_stack(struct linked_list *item)
{
    struct object   *obj;
    char    buf[2 * LINELEN];
    int i = 0, j;
    struct linked_list  *ll;
mpos = 0;
    obj = OBJPTR(item);

	ll = obj->next_obj;

    sprintf(buf, "You are standing on top of the following items: ");
    add_line(buf);
    sprintf(buf, "%d) -- %s", i, inv_name(obj, TRUE));
    add_line(buf);

    while (ll)
    {
        i++;
        obj = OBJPTR(ll);
        sprintf(buf, "%d) -- %s", i, inv_name(obj, TRUE));
        add_line(buf);
        ll = next(ll);
    }

    end_line();
	
    msg("Which one do you want to pick up? [* for all] ");

    switch(get_string(buf, cw))
    {
        case NORM:
            break;
        case QUIT:  /* pick up nothing */
            msg("");
            return (NULL);
    }

    if (buf[0] == '*')
    {
        get_all(item);
		msg("");
        return(NULL);
    }
    else
    {
        i = atoi(buf);

        if (i)
        {
            obj = OBJPTR(item);
            ll = obj->next_obj;
            j = 1;

            while (ll && (j != i))
            {
                ll = next(ll);
                j++;
            }

            if (ll)
            {
                swap_top(item, ll);
                return(ll);
            }
            else
            {
                debug("Got past last item while picking up.");
                return(item);
            }
        }
        else
            return (item);
    }
}

/*
    add_pack()
        Pick up an object and add it to the pack.  If the argument is
        non-null use it as the linked_list pointer instead of getting it off the
        ground.
*/

int
add_pack(struct linked_list *item, int print_message)
{
    struct object   *obj, *op;
    int from_floor;

    if (item == NULL)
    {
        from_floor = TRUE;

        if ((item = find_obj(hero.y, hero.x)) == NULL)
        {
            msg("Nothing to pick up.");
            return(FALSE);
        }
    }
    else
        from_floor = FALSE;

    if (from_floor)
    {
        item = get_stack(item);

        if (!item)
            return(FALSE);
    }

    obj = OBJPTR(item);

    /* If it is gold, just add its value to rogue's purse and get rid of */

    if (obj->o_type == GOLD)
    {
        struct linked_list  *mitem;
        struct thing    *tp;

        if (print_message)
        {
            if (!terse)
                addmsg("You found ");

            msg("%d gold pieces.", obj->o_count);
        }

        /*
         * First make sure no greedy monster is after this gold. If
         * so, make the monster run after the rogue instead.
         */

        for (mitem = mlist; mitem != NULL; mitem = next(mitem))
        {
            tp = THINGPTR(mitem);

            if (tp->t_horde==obj)
            {
                tp->t_ischasing = TRUE;
                tp->t_chasee = &player;
                tp->t_horde  = NULL;
            }
        }

        /*
         * This will cause problems if people are able to drop and
         * pick up gold, or when GOLDSTEAL monsters are killed.
         */

        /* Thieves get EXP for gold they pick up */

        if (player.t_ctype == C_THIEF)
        {
            pstats.s_exp += obj->o_count / 4;
            check_level();
        }

        purse += obj->o_count;

        if (from_floor)
            rem_obj(item, TRUE);    /* Remove object from the level */

        return (TRUE);
    }

    /* see if he can carry any more weight */

    if (itemweight(obj) + pstats.s_pack > pstats.s_carry)
    {
        msg("Too much for you to carry.");

        if (print_message)
        {
            msg("%s onto %s", terse ? "Moved" : "You moved",
                inv_name(obj, LOWERCASE));
        }

        return(FALSE);
    }

    /*
     * Link it into the pack. If the item can be grouped, try to find its
     * neighbors and bump the count. A special case is food, which can't
     * be grouped, but an exact match allows the count to get
     * incremented.
     */
	 
    if ((op = apply_to_bag(pack, obj->o_type, bff_group, NULL, obj)) != NULL)
    {
        op->o_count += obj->o_count;    /* add it to the rest */

        if (from_floor)
            rem_obj(item, FALSE);

        pack_report(op, print_message, "You now have ");

        return(TRUE);
    }

    /* Check for and deal with scare monster scrolls */

    if (obj->o_type == SCROLL && obj->o_which == S_SCARE)
        if (obj->o_flags & ISCURSED)
        {
            msg("The scroll turns to dust as you pick it up.");
            rem_obj(item, TRUE);
            return(TRUE);
        }

    /* Check if there is room */

    if (count_bag(pack, (int) obj->o_type, NULL) == max_print())
    {
        msg("You have no room for more %s.", name_type(obj->o_type));

        if (print_message)
        {
            obj = OBJPTR(item);
            msg("%s onto %s.", terse ? "Moved" : "You moved",
                inv_name(obj, LOWERCASE));
        }

        return(FALSE);
    }

    /*
     * finally, add the new item to the bag, and free up the linked list
     * on top of it.
     */

    if (from_floor)
        rem_obj(item, FALSE);

    push_bag(&pack, obj);
    pack_report(obj, print_message, "You now have ");
    ur_free(item);

    return(TRUE);      /* signal success */
}

/*
    pack_report()
        Notify the user about the results of the pack operation and do some
        post processing.
*/

void
pack_report(object *obj, int print_message, char *message)
{
    /* Notify the user */

    if (print_message)
    {
        if (!terse)
            addmsg(message);

        msg("(%c%c) %s.", obj->o_type, print_letters[get_ident(obj)],
            inv_name(obj, !terse));
    }

    if (obj->o_type == ARTIFACT)
    {
        has_artifact |= (1 << obj->o_which);
        picked_artifact |= (1 << obj->o_which);

        if (!(obj->ar_flags & ISUSED))
        {
            obj->ar_flags |= ISUSED;
            pstats.s_exp += arts[obj->o_which].ar_worth / 10;
            check_level();
        }
    }

    updpack();

    return;
}

/*
    inventory()
        list what is in the pack
*/

void
inventory(struct linked_list *container, int type)
{
    int cnt;

    if (type == 0)
    {
        msg("What kind of item <%s> to inventory (* for all)?",  type_list);

        type = wgetch(cw);

        if (type == ESCAPE)
        {
            after = FALSE;
            msg("");
            return;
        }
    }

    /*
     * Get a list of items to print out. If the user selects '*', list
     * them all.
     */

    if (type == '*')
        type = 0;   /* no type passed ->use them all */

    mpos = 0;

    if ((cnt = count_bag(container, type, NULL)) == 0)
        msg("You don't have any %s.", name_type(type));
    else
    {
        apply_to_bag(container, type, NULL, baf_print_item, &type);
        end_line();
        msg("");
    }

    return;
}

/*
    pick_up()
        Add something to characters pack.
*/

void
pick_up(char ch)
{
    switch(ch)
    {
        default:
            debug("Where did you pick that up???");
            break;

        case GOLD:
        case ARMOR:
        case POTION:
        case FOOD:
        case WEAPON:
        case SCROLL:
        case ARTIFACT:
        case RING:
        case STICK:
            add_pack(NULL, MESSAGE);
            break;
    }
}

/*
    get_object()

        Pick something out of a pack for a purpose. The basic idea is to
        list all the possibilities, let the user select one, get that item
        from the container, and pass it back to the calling routine.
*/

struct object *
get_object(struct linked_list *container, char *purpose, int type, int (*bff_p)(struct object *obj, bag_arg *junk))
/* char    *container;    what container has what we want               */
/* char    *purpose;      a message (verb) to print if we cant find any */
/* char    type;          type (o_type) to pick out (NULL = any)        */
/* int     (*bff_p) ();   bag filter function to test item              */
{
    struct object   *obj_p = NULL;
    char sel_id;   /* selected type and id */
    int  sel_type, response;

    if (container == NULL)
    {
        msg("There isn't anything in there.");
        after = FALSE;
        return(NULL);
    }

    /* Make sure we have at least one item that qualifies! */

    if (apply_to_bag(container, type, bff_p, NULL, NULL) == NULL)
    {
        msg("You seem to have nothing to %s.", purpose);
        after = FALSE;
        return(NULL);
    }

    while (obj_p == NULL)
    {
        if (type == 0)
        {
            msg("What kind of item <%s> do you want to %s (* for list)?", type_list, purpose);

            response = wgetch(cw);
            ESCAPE_EXIT(response);
            msg("");

            if (response == '*')
            {
                add_line("! Potion");
                add_line("? Scroll");
                add_line("= Ring");
                add_line("/ Stick");
                add_line("] Armor");
                add_line(") Weapon");
                add_line(": Food");
                end_line();
                continue;
            }


            if (!is_member(type_list, response)) { beep();
               continue; }


            if (count_bag(container, response, NULL) == 0)
            {
                msg("You don't have any %s.", name_type(response));
                continue;
            }

            type = response;
        }

        while(obj_p == NULL)
        {
            msg("What item do you want to %s (* for list)?", purpose);
            response = wgetch(cw);
            msg("");
            ESCAPE_EXIT(response);

            if (response == '*')
            {
                mpos = 0;
                apply_to_bag(container, type, bff_p, baf_print_item, &type);
                end_line();
                continue;
            }

            sel_type = type;
            sel_id = response;

            obj_p = scan_bag(container, sel_type,unprint_id(&sel_id));
        }
    }

    mpos = 0;
    msg("");
    return(obj_p);
}

/*
    get_item()

        This is only an interim function that serves as an interface to
        the old function get_item and its replacement get_object. It
        assumes a NULL action routine and allocates a linked_list
        structure on top of the object pointer.
*/

struct linked_list *
get_item(char *purpose, int type)
{
    struct object   *obj_p;

    if ((obj_p = get_object(pack, purpose, type, NULL)) == NULL)
        return(NULL);

    return(make_item(obj_p));
}

/*
    del_pack()
        Take something out of the hero's pack and throw it away.
*/

void
del_pack(struct linked_list *what)
{
    rem_pack(OBJPTR(what));
    discard(what);
}

/*
    discard_pack
        take an object from the pack and throw it away (like del_pack,
        but without the linked_list structure)
*/

void
discard_pack(struct object *obj_p)
{
    rem_pack(obj_p);
    throw_away(obj_p);
}

/*
    rem_pack()
        Removes an item from the pack.
*/

void
rem_pack(struct object *obj_p)
{
    cur_null(obj_p);    /* check for current stuff */
    pop_bag(&pack, obj_p);
    updpack();
    return;      /* tell caller an item has been removed */
}

/*
    cur_null()
        This updates cur_weapon etc for dropping things
*/

void
cur_null(struct object *op)
{
    if (op == cur_weapon)
        cur_weapon = NULL;
    else if (op == cur_armor)
        cur_armor = NULL;
    else if (op == cur_ring[LEFT_1])
        cur_ring[LEFT_1] = NULL;
    else if (op == cur_ring[LEFT_2])
        cur_ring[LEFT_2] = NULL;
    else if (op == cur_ring[LEFT_3])
        cur_ring[LEFT_3] = NULL;
    else if (op == cur_ring[LEFT_4])
        cur_ring[LEFT_4] = NULL;
    else if (op == cur_ring[LEFT_5])
        cur_ring[LEFT_5] = NULL;
    else if (op == cur_ring[RIGHT_1])
        cur_ring[RIGHT_1] = NULL;
    else if (op == cur_ring[RIGHT_2])
        cur_ring[RIGHT_2] = NULL;
    else if (op == cur_ring[RIGHT_3])
        cur_ring[RIGHT_3] = NULL;
    else if (op == cur_ring[RIGHT_4])
        cur_ring[RIGHT_4] = NULL;
    else if (op == cur_ring[RIGHT_5])
        cur_ring[RIGHT_5] = NULL;
}

/*
    idenpack()
        Identify all the items in the pack
*/

void
idenpack(void)
{
    apply_to_bag(pack, 0, NULL, baf_identify, NULL);
}

/*
    show_floor()
        Print out the item on the floor.  Used by the move command.
*/

void
show_floor(void)
{
    struct linked_list  *item;
    struct object   *obj;

    item = find_obj(hero.y, hero.x);

    if (item != NULL)
    {
        addmsg("%s onto ", terse ? "Moved" : "You moved");

        obj = OBJPTR(item);

        if (obj->next_obj != NULL)
		    msg("a stack of things.");
		else if (obj->o_type == GOLD)
            msg("%d gold pieces.", obj->o_count);
        else
        {
            addmsg(inv_name(obj, TRUE));
            addmsg(".");
            endmsg();
        }
    }
}
