# Jedi Knight Plus (JK+)
https://jkplus.github.io

Game modification for **Jedi Knight 2 Multiplayer** with useful and innovative features. *(all game versions support)*

# About
The main goal is to reunite the most useful and innovative stuff to the game environment, specially make players ideas come true, as well porting some features from Jedi Academy or another games, and of course making new ones. 

> I started to code the first version of this mod in 2005 when i was 15 years old (i learned to code with this). Back then this was a private mod, only few people had access, i never did a public release. So due to the people request i decided to remake it now (the old one had lots of bugs and trash code). 

# Status & Download

Note: This mod is under development, some features may be disabled or not finished. If you found a bug, problem or an error, please contact me at triforce@jk2mp.com

Phase | Version | Download
:------------: | :------------: | :-------------:
Develop | 1.0-rev2 | Not available
Release | Undefined | Not available

# Mod & Server Setup

First of all is highly recommend to use [JK2MV](https://jk2mv.org/downloads) to play the game and host dedicated servers, specially due the multi version support and lots of game improvements and fixes. The mod installation is simple, follow the next steps:

1. Go to `GameData` (the directory which holds `jk2mp.exe` or `jk2ded.exe`)

2. Unpack the `JediKnightPlus` folder into `GameData` directory
   
3. To configure mod options edit the files which starts with `jkplus_<name>.cfg` like `jkplus_server.cfg`

4. You can launch the server in game server setup or directly with the command prompt in *Windows* or *Unix*

5. For *Windows* use the following parameters: 
   - `C:\<path to jk2>\GameData\jk2mvded.exe +set fs_game JediKnightPlus +exec jkplus_server.cfg`

6. For *Unix* use the following parameters: 
   - `./jk2mvded +set fs_game JediKnightPlus +exec jkplus_server.cfg`

7. Also for *Windows* you can launch the server using the file `jkplus_launcher.exe`

8. If you are using `Base JK2` the steps are the same, just use `jk2mp` instead `jk2mvmp` or `jk2ded` instead `jk2mvded`

9. That's it! Your server is now ready to use *Jedi Knight Plus Mod*

# Features List

The next list is all features the old Jedi Knight Plus mod already has, which need to be ported, improved or fixed for the new source code.

###### 1. Server Side Features

Status | Feature
:------: | :-------
Done ✓ | Gameplay changer between 1.02, 1.03 and 1.04 in a single command
✗ | Improved bots AI (Artificial intelligence)
✗ | Configurable weapon features (Damage, ammo speed, firing rate, etc...)
✗ | Player account system (Tracks kills, deaths, etc...)
✗ | Ranking system based on player skills
✗ | Chat censor filters
Done ✓ | Allow black color in player names
✗ | In-game chat commands (Teleports, info, etc...)
✗ | Welcome message and news messages showing constantly on the server
✗ | Custom in-game modes (Instant kill, survival, last man standing, etc...)
✗ | Custom player private chat rooms
✗ | Enabled Jedi Crystal and Beer Bacta items
✗ | NPC spawn command like in Single Player
✗ | Improved cheat commands
✗ | Protected server clan tag (With clan password)
Done ✓ | Over 50 player emotes (Sit, kiss, surrender, etc...)
✗ | Teleport commands (Save & load position, player to player, etc...)
✗ | Anti lamer feature (Players can't kill during chat)
✗ | Dual blade lightsaber gameplay (Fixed damages, different combos, etc...)
✗ | Basic anti-script feature (Against noob cheaters)
✗ | Items get affected by force physics (Push, pull, etc...)
✗ | Enable multiple private duels on FFA game type
✗ | Multiple private duel modes (Full force, no force, guns, etc...)
✗ | Enable invisible private duels
✗ | Enable chat in Duel game type
Done ✓ | Closed server feature (For maintenance or private use without password)
✗ | Alternative player dimensions
✗ | Custom poll call votes
✗ | Improved slow-mo duel end functionally
 
###### 2. Client Side Features

Status | Feature
:------: | :-------
✗ | Atmospheric effects (Rain, snow, sun, etc...)
✗ | Rank icons for the scoreboard (Also show wins, losses, flag captures, etc...)
✗ | Mini keyboard display to see key-presses on-screen
✗ | Jedi Academy chat display mode (Positioned at left bottom)
✗ | Three chat display icons (JK2, Jedi Academy and user customizable)
✗ | Fixed 2D aspect ratio for non 4:3 screens
✗ | Fire lightsaber style, to add real fire effects to the lightsaber
✗ | Fully working jet pack to fly around the map
✗ | Grapple hook to climb against walls
✗ | Fully drivable vehicles (Tie Fighter, X-Wing, AT-ST, etc...)
✗ | In-game clock showing the actual local hours, minutes and seconds
✗ | In-game server list to join another server without disconnect
✗ | Speed-o-meter to display player speed in-game and best record
✗ | Strafe jumping helper/efficiency meter
✗ | Custom lightsaber hilt support
✗ | Two alternative huds (Jedi Academy and basic lite)
✗ | Alternative FPS and server time style
✗ | Alternative Force points display with numbers
✗ | Display bacta indicator (3D model)
✗ | Display current weapon in back (3D model)
✗ | Enable custom item indicator
✗ | Enable push/pull force effect color to match with the color of lightsaber
✗ | Enable the color for the second blade with dual blade lightsabers
✗ | Custom saber trail length
✗ | Custom in-game duel challenge music
✗ | Custom flag transparency (For CTF game type)
✗ | Show player icons on scoreboard
✗ | Custom camera options (Range, distance, etc...)
✗ | Enable first person lightsaber
✗ | Enable first person turret chair view
✗ | Enable custom player skins (Rocket Trooper, Jedi Jan, etc...)
✗ | Increased some limits (Fov, camera range, name length, etc...)
 
###### 3. Map Modding Features

Status | Feature
:------: | :-------
✗ | Map auto fix to convert most of single player maps playable
✗ | Over 15 custom entities for maps (Including entities from Single Player)
✗ | Add models in-game (GLM or MD3 format)
✗ | Add effects in-game (EFX format)
✗ | Add any entity based on class name and its parameters
✗ | Add pre-made useful utilities (NPC spawner, print consoles, assister, etc...)
✗ | Change any shader or texture in-game
✗ | Modify entities properties in-game
✗ | Delete entities in-game
✗ | Clone entities in-game
✗ | Save feature to get all changes automatically stored in a loadable file
 
###### 4. Administration Features

Status | Feature
:------: | :-------
✗ | Custom user roles (Administrator, moderator, builder, etc...)
✗ | In-game player statistics (Banned, kicked, reports, scripters, etc...)
✗ | Helpful commands for players (Teleport, weapons, force powers, etc...)
✗ | Punish commands for bad players (Slay, silence, sleep, rename, lock, etc...)
✗ | Ability to control bots actions (Fake chat, movements, emotes, etc...)
✗ | Spawn unlimited items and weapons
✗ | Private administrator chat room
✗ | Custom call vote with actions
✗ | Team management (Lock, force, shuffle, etc...)
✗ | Pause mode to instantly stop everyone on the server during game

# To-Do Stuff & Public Requests

There is the list of unfinished stuff and people's approved requests. As i go forward, i will move the items to the upper list of mod features:

- Add Unlagged Feature
- Enable lightsaber physics on dead corpses
- Possibility to do force powers on NPC's
- Add Telefrag (Show the camera on the last killer)
- In-game screenshot viewer
- Add Light-Amp Goggles item from single player
- Add Flashlight feature
- Enable voice commands (Jedi Academy style)
- Add Hold The Point game type
- Add Man Hunt or Captains game type
- Add Jedi Academy Radar for team games
- In-game MP3 player
- Portal Gun teleport weapon mechanics

# Special Thanks

The people below helped me to solve problems and help in this project by giving me advice or helping me directly.

- DeathSpike (Client plugin support)
- Lee Oates (Anti-script base code)
- Nerevar (Ingame effects, atmospherics and santa hat)
- Kaiser (Map modding support)
- Rich Whitehouse (Melee weapon from Hydroball)
- NTxC / Reality~ (1.02 coding support)
- Tox Laximus (Bots AI support)
- McMonkey (Map modding support)
- JediDog~ (1.03 coding support)
- NewAge (Admin management support)
- Wonko The Sane (JK2 base improvements support)
- Daggolin (Map modding and coding support)
- Ouned / Twitch (Engine coding support)
- Kameleon (3D model draw angles fix)
- Fau & Bucky (2D aspect ratio fix)
- BarryHit (Coding support and testing)
- Player (Support and testing)
- Flendo (Beta testing)
- VideoP / Loda (Strafe helper)
- MVSDK / JK2MV (Gameplay changer and MVAPI)
- x[Dsk]x Clan and 1.03 community (Support and testing)

# Contact

**Jedi Knight Plus Mod developed by Tr!Force**  
Work copyrighted (C) with holder attribution 2005 - 2018

Visit the home page: https://jkplus.github.io

Feel free to write me if you got questions regarding the mod  
E-mail: triforce@jk2mp.com

# License

This mod is based on *MVSDK* and is licensed under GPLv2 as free software. Some files in `assets` directory are modified assets from the original, non-free JK2 1.04 release and licensed under *JK2 Editing Tools 2.0* EULA.

LCC 4.1 is Copyright (c) 1991-1998 by AT&T, Christopher W. Fraser and David R. Hanson, and available under a non-copyleft license. You can find it in code/tools/lcc/COPYRIGHT. LCC version bundled with this SDK comes from ioquake3 and it has been slightly modified by its developers.

Remaining parts of JK2 SDK GPL are licensed under GPLv2 as free software. Read LICENSE.txt and README-raven.txt to learn more. According to the license, among other things, you are obliged to distribute full source code of your mod alongside of it, or at least a written offer to ship it (eg a HTTP download link inside a .pk3 file). Moreover, any mod using patches from this repository **must** be released under GPLv2 or a compatible license.

Q3ASM is Copyright (c) id Software and ioquake3 developers.