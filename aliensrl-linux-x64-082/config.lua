-- ----------------------------------------------------------------------
--  This is the AliensRL initialization file. Modify at your own risk :). 
--  If you mess up something overwrite with a new config.lua.
-- ----------------------------------------------------------------------

sound     = true
music     = true
run_delay = 10

ascii_mode     = false
ascii_width    = 80
ascii_height   = 30

display_width      = 1024
display_height     = 768
fullscreen         = false
gradual_light      = true
gradual_brightness = 1.0

display_hints    = true
clear_messages   = false
message_coloring = true
message_buffer   = 1000

messages = {
	["Exploration*"] 		   = YELLOW,
	["You have no more *!"]    = RED,
	["Your armor is *!"]       = LIGHTRED,
}

keybindings = {
	["LEFT"]         = COMMAND_WALKWEST,
	["RIGHT"]        = COMMAND_WALKEAST,
	["UP"]           = COMMAND_WALKNORTH,
	["DOWN"]         = COMMAND_WALKSOUTH,
	["PGUP"]         = COMMAND_WALKNE,
	["PGDOWN"]       = COMMAND_WALKSE,
    ["HOME"]         = COMMAND_WALKNW,
	["END"]          = COMMAND_WALKSW,

	["SHIFT+LEFT"]   = COMMAND_RUNWEST,
	["SHIFT+RIGHT"]  = COMMAND_RUNEAST,
	["SHIFT+UP"]     = COMMAND_RUNNORTH,
	["SHIFT+DOWN"]   = COMMAND_RUNSOUTH,
	["SHIFT+PGUP"]   = COMMAND_RUNNE,
	["SHIFT+PGDOWN"] = COMMAND_RUNSE,
    ["SHIFT+HOME"]   = COMMAND_RUNNW,
	["SHIFT+END"]    = COMMAND_RUNSW,

	["ESCAPE"]       = COMMAND_ESCAPE,
	["CENTER"]       = COMMAND_WAIT,
	["PERIOD"]       = COMMAND_WAIT,
	["ENTER"]        = COMMAND_OK,
	["TAB"]          = COMMAND_TARGET,
	["G"]            = COMMAND_PICKUP,
	["F"]            = COMMAND_FIRE,
	["U"]            = COMMAND_USE,
	
	["S"]            = COMMAND_SIDEARM,
	["P"]            = COMMAND_PRIMARY,
	["H"]            = COMMAND_HEAVY,

	["R"]            = COMMAND_RELOAD,
	["D"]            = COMMAND_DROP,

	["1"]            = COMMAND_SLOT1,
	["2"]            = COMMAND_SLOT2,
	["3"]            = COMMAND_SLOT3,
	["4"]            = COMMAND_SLOT4,

	["SPACE"]        = COMMAND_ACT,

	["L"]            = COMMAND_LOOK,

	["F10"]          = COMMAND_SCREENSHOT,    	
}
