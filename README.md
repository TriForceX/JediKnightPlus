# Jedi Knight Plus

https://triforce.dev/projects/jkplus

Game modification for **Jedi Knight 2 Multiplayer** *(all game versions support)*. The main goal is to reunite the most useful and innovative stuff to the game environment, specially make players ideas come true adding new features as well porting some from other games.

[![version](https://img.shields.io/badge/Current%20Version-0.7.2-brightgreen.svg)](https://github.com/TriForceX/JediKnightPlus/releases)
[![stars](https://img.shields.io/github/stars/TriForceX/JediKnightPlus.svg?label=Project%20Stars)](https://github.com/TriForceX/JediKnightPlus/stargazers)
[![issues](https://img.shields.io/github/issues/TriForceX/JediKnightPlus.svg?label=Issues%20%26%20Requests&color=red)](https://github.com/TriForceX/JediKnightPlus/issues)
[![license](https://img.shields.io/github/license/TriForceX/JediKnightPlus.svg?label=License&color=yellow)](https://github.com/TriForceX/JediKnightPlus#license)

# Information
> I started to code the first version of this mod in 2005. Back then this was a private mod, only few people had access, i never did a public release. So due to the people request i decided to remake it now (the old one had lots of bugs and trash code). 

**Note:** This mod is under development, some features may be disabled or not finished. 

- Read more information and guides at **Wiki** [here](https://github.com/TriForceX/JediKnightPlus/wiki)
- See version changes history in the **Changelog** [here](https://github.com/TriForceX/JediKnightPlus/blob/master/CHANGELOG.md)
- Join my **Discord** chat server for more support  [here](https://discord.gg/tfJRtzV)

# Server Setup

Before start is highly recommend to use [JK2MV](https://jk2mv.org/downloads) to play the game and host dedicated servers, specially because **JK2MV** has multi-version support, lots of game improvements and fixes. For mod setup follow the next steps:

---

1. Download the lastest mod **release** [here](https://github.com/TriForceX/JediKnightPlus/releases)
2. Go to `GameData` (the directory which holds `jk2mp.exe` or `jk2ded.exe`)
3. Unpack the `JediKnightPlus` folder into `GameData` directory
4. To configure mod options edit the files which starts with `jkplus_<name>.cfg` for example `jkplus_server.cfg`
8. For *Windows* you can launch the server using the file `jkplus_launcher.exe` and follow the steps on it
5. Or you can launch the server in-game using **server setup** or using the **console** in *Windows*, *Linux* or *OSX*
6. For *Windows* use the following parameters: 
   - `C:\<path to jk2>\GameData\jk2mvded.exe +set fs_game JediKnightPlus +exec jkplus_server.cfg`
7. For *Linux* or *OSX* use the following parameters: 
   - `./jk2mvded +set fs_game JediKnightPlus +exec jkplus_server.cfg`
9. You can use any JK2 dedicated server binary on the previous steps: `jk2mp`, `jk2ded` or `jk2mvded`
10. That's it! Your server is now ready to use **Jedi Knight Plus Mod**

# Mod Features

The next list is all features from the original **Jedi Knight Plus** mod which need to be ported, improved or fixed on the new source code. Each finished task will be marked with a check :white_check_mark:

---

### Server Side
Players doesn't need to have the client-side plugin to use this features.

Status | Feature
:------: | :-------
:white_check_mark: | Gameplay changer between 1.02, 1.03 and 1.04 in a single command
:x: | Improved bots AI (Artificial intelligence)
:x: | Configurable weapon features (Damage, ammo speed, firing rate, etc...)
:x: | Player account system (Tracks kills, deaths, etc...)
:x: | Ranking system based on player skills
:x: | Chat censor filters
:white_check_mark: | Allow black color in player names
:x: | In-game chat commands (Teleports, info, etc...)
:x: | Welcome message and news messages showing constantly on the server
:x: | Custom in-game modes (Instant kill, survival, last man standing, etc...)
:x: | Custom player private chat rooms
:x: | Enabled Jedi Crystal and Beer Bacta items
:x: | NPC spawn command like in Single Player
:x: | Improved cheat commands
:x: | Protected server clan tag (With clan password)
:white_check_mark: | Over 50 player emotes (Sit, kiss, surrender, etc...)
:x: | Teleport commands (Save & load position, player to player, etc...)
:x: | Anti lamer feature (Players can't kill during chat)
:x: | Dual blade lightsaber gameplay (Fixed damages, different combos, etc...)
:x: | Items get affected by force physics (Push, pull, etc...)
:x: | Enable multiple private duels on FFA game type
:x: | Multiple private duel modes (Full force, no force, guns, etc...)
:x: | Enable invisible private duels
:x: | Enable chat in Duel game type
:white_check_mark: | Closed server feature (For maintenance or private use without password)
:x: | Alternative player dimensions
:x: | Custom poll call votes
:x: | Improved slow-mo duel end functionally
:white_check_mark: | Drop Flag command for CTF & CTY game types
 
### Client Side
Players need the mod plugin to use this features _(doesn't affect the server gameplay)_

Status | Feature
:------: | :-------
:x: | Atmospheric effects (Rain, snow, sun, etc...)
:x: | Rank icons for the scoreboard (Also show wins, losses, flag captures, etc...)
:x: | Mini keyboard display to see key-presses on-screen
:x: | Jedi Academy chat display mode (Positioned at left bottom)
:x: | Three chat display icons (JK2, Jedi Academy and user customizable)
:x: | Fixed 2D aspect ratio for non 4:3 screens
:x: | Fire lightsaber style, to add real fire effects to the lightsaber
:x: | Fully working jet pack to fly around the map
:x: | Grapple hook to climb against walls
:x: | Fully drivable vehicles (Tie Fighter, X-Wing, AT-ST, etc...)
:x: | In-game clock showing the actual local hours, minutes and seconds
:x: | In-game server list to join another server without disconnect
:x: | Speed-o-meter to display player speed in-game and best record
:x: | Strafe jumping helper/efficiency meter
:x: | Custom lightsaber hilt support
:x: | Two alternative huds (Jedi Academy and basic lite)
:x: | Alternative FPS and server time style
:x: | Alternative Force points display with numbers
:x: | Display bacta indicator (3D model)
:x: | Display current weapon in back (3D model)
:x: | Enable custom item indicator
:x: | Enable push/pull force effect color to match with the color of lightsaber
:x: | Enable the color for the second blade with dual blade lightsabers
:x: | Custom saber trail length
:x: | Custom in-game duel challenge music
:x: | Custom flag transparency (For CTF game type)
:x: | Show player icons on scoreboard
:x: | Custom camera options (Range, distance, etc...)
:x: | Enable first person lightsaber
:x: | Enable first person turret chair view
:x: | Enable custom player skins (Rocket Trooper, Jedi Jan, etc...)
:x: | Basic anti-script feature (Against noob cheaters)
:x: | Increased some limits (Fov, camera range, name length, etc...)
 
### Map Modding
Allows in-game map modifications _(models, effects, shaders and textures)_

Status | Feature
:------: | :-------
:x: | Map auto fix to convert most of single player maps playable
:x: | Over 15 custom entities for maps (Including entities from Single Player)
:x: | Add models in-game (GLM or MD3 format)
:x: | Add effects in-game (EFX format)
:x: | Add any entity based on class name and its parameters
:x: | Add pre-made useful utilities (NPC spawner, print consoles, assister, etc...)
:x: | Change any shader or texture in-game
:x: | Modify entities properties in-game
:x: | Delete entities in-game
:x: | Clone entities in-game
:x: | Save feature to get all changes automatically stored in a loadable file
 
### Administration
Allows server admins to control players and server features in-game

Status | Feature
:------: | :-------
:x: | Custom user roles (Administrator, moderator, builder, etc...)
:x: | In-game player statistics (Banned, kicked, reports, scripters, etc...)
:x: | Helpful commands for players (Teleport, weapons, force powers, etc...)
:x: | Punish commands for bad players (Slay, silence, sleep, rename, lock, etc...)
:x: | Ability to control bots actions (Fake chat, movements, emotes, etc...)
:x: | Spawn unlimited items and weapons
:x: | Private administrator chat room
:x: | Custom call vote with actions
:x: | Team management (Lock, force, shuffle, etc...)
:white_check_mark: | Pause mode to instantly stop everyone on the server during game

# Special Thanks

The following people supported and helped me directly on this project.

- [DeathSpike](https://github.com/Deathspike) _(Client plugin support)_
- Lee Oates _(Anti-script base code)_
- Nerevar _(Ingame effects, atmospherics and santa hat)_
- Kaiser _(Map modding support)_
- [Rich Whitehouse](https://www.richwhitehouse.com) _(Melee weapon from Hydroball)_
- NTxC / Reality~ _(1.02 coding support)_
- Tox Laximus _(Bots AI support)_
- [McMonkey](https://github.com/mcmonkey4eva) _(Map modding support)_
- JediDog~ _(1.03 coding support)_
- [NewAge](https://github.com/erfg12) _(Admin management support)_
- Wonko The Sane _(JK2 base improvements support)_
- [Daggolin](https://github.com/Daggolin) _(Map modding and coding support)_
- [Ouned / Twitch](https://github.com/ouned) _(Engine coding support)_
- Kameleon _(3D model draw angles fix)_
- [Fau](https://github.com/aufau) _(2D aspect ratio fix)_
- [Bucky](https://github.com/Bucky21659) _(EternalJK tweaks and hints)_
- BarryHit _(Coding support and testing)_
- Player _(Support and testing)_
- [Flendo](https://github.com/FlendoJK2) _(Beta testing)_
- vVv _(Pause mode base)_
- [VideoP / Loda](https://github.com/videoP) _(Strafe helper)_
- [MVSDK / JK2MV](https://github.com/mvdevs) _(Gameplay changer and MVAPI)_
- x[Dsk]x Clan and 1.03 community _(Support and testing)_

# Contact

**Jedi Knight Plus Mod developed by Tr!Force**  
Work copyrighted (C) with holder attribution 2005 - 2019

Visit the mod page: https://triforce.dev/projects/jkplus

Feel free to write me if you got questions regarding the mod\
E-Mail: triforce@gznetwork.com

# License

> This mod is based on *MVSDK* and is licensed under GPLv2 as free software. Some files in `assets` directory are modified assets from the original, non-free JK2 1.04 release and licensed under *JK2 Editing Tools 2.0* EULA.

> LCC 4.1 is Copyright (c) 1991-1998 by AT&T, Christopher W. Fraser and David R. Hanson, and available under a non-copyleft license. You can find it in code/tools/lcc/COPYRIGHT. LCC version bundled with this SDK comes from ioquake3 and it has been slightly modified by its developers.

> Remaining parts of JK2 SDK GPL are licensed under GPLv2 as free software. Read LICENSE.txt and README-raven.txt to learn more. According to the license, among other things, you are obliged to distribute full source code of your mod alongside of it, or at least a written offer to ship it (eg a HTTP download link inside a .pk3 file). Moreover, any mod using patches from this repository **must** be released under GPLv2 or a compatible license.

> Q3ASM is Copyright (c) id Software and ioquake3 developers.