# Jedi Knight Plus

https://jkmod.github.io

> Game modification for **Jedi Knight II: Jedi Outcast Multiplayer**. The main goal of this mod is to reunite useful stuff and add custom ones to the game environment, specially making players ideas come true adding new features as well porting some from other mods and games.

<div align="center">

[![version](https://img.shields.io/badge/Current%20Version-0.12.4-brightgreen.svg)](https://github.com/TriForceX/JediKnightPlus/releases)
[![Wiki](https://img.shields.io/badge/Setup%20%26%20Guides-Wiki-blueviolet.svg)](https://github.com/TriForceX/JediKnightPlus/wiki)
[![Discord](https://img.shields.io/badge/Discord%20Chat-Join-7289DA.svg?logo=discord&longCache=true&style=flat)](https://discord.gznetwork.com/jkmod)
[![stars](https://img.shields.io/github/stars/TriForceX/JediKnightPlus.svg?label=Project%20Stars)](https://github.com/TriForceX/JediKnightPlus/stargazers)
[![issues](https://img.shields.io/github/issues/TriForceX/JediKnightPlus.svg?label=Issues%20%26%20Requests&color=red)](https://github.com/TriForceX/JediKnightPlus/issues)
[![license](https://img.shields.io/github/license/TriForceX/JediKnightPlus.svg?label=License&color=yellow)](#license)

</div>

# Information

- Mod configuration and **download** [here](https://github.com/TriForceX/JediKnightPlus/releases)
- Read more information and guides at **wiki** [here](https://github.com/TriForceX/JediKnightPlus/wiki)
- See version changes history in the **changelog** [here](https://github.com/TriForceX/JediKnightPlus/blob/master/CHANGELOG.md)
- Join the mod **Discord** chat server for more support [here](https://discord.gznetwork.com/jkmod)

# About
I started to code the first version of this mod in 2005. Back then it was a private mod and only few people had access. So as a personal task and due to some people request i decided to re-code the mod into a public version, just as a legacy.

The next list is all *features* from the original **Jedi Knight Plus** private mod _(2005 & 2008 versions)_ which need to be ported into the new source code. _(Each completed task will be marked with a check)_

---

### Server Side
**Description:** General server features _(No client plugin required)_

- [x] Gameplay changer between 1.02, 1.03 and 1.04 no matter the server version
- [ ] Improved bots AI (Artificial intelligence)
- [ ] Custom weapon features (Damage, ammo speed, firing rate, etc...)
- [ ] Player accounts system (Save data such as kills, deaths, etc...)
- [ ] Ranking system based on player skills
- [ ] Chat censor filters
- [x] Chat protection
- [x] Allow black color in player names
- [x] Prevent duplicated player names
- [x] Teleport chat commands (Map areas, save & load, etc...)
- [x] Welcome message on client begin (MOTD)
- [x] Server news messages displayed constantly
- [ ] Custom ingame modes (Instant kill, survival, last man standing, etc...)
- [ ] Custom player private chat rooms
- [x] Show damage plums on hits
- [ ] Clan system (Tag protection, member role, etc...)
- [x] Over 50 player emotes (Sit, kiss, surrender, etc...)
- [x] Chat protect feature (Players can't kill during chat)
- [ ] Dual blade lightsaber gameplay (Fixed damages, custom combos, etc...)
- [ ] Custom melee weapon
- [x] Items get affected by force physics (Push, pull, etc...)
- [x] Enable multiple duel challenges
- [x] Custom duel challenge modes (Full force, no force,etc...)
- [x] Custom duel challenge start health and armor
- [x] Custom duel stats (Health, armor and body hits)
- [x] Enable player chat and duel challenge ignore
- [x] Enable chat in duel game type
- [x] Closed server feature (Allow by password)
- [x] Alternative dimensions (Duel, chat, race, etc...)
- [x] Custom call vote system
- [ ] Improved slow-mo duel end functionally
- [x] Drop flag command for CTF & CTY game types
- [x] Random client begin messages
- [x] Prevent player warping or lag scripts
- [x] Custom server idle reset

---

### Administration
**Description:** Ingame server management features for administrators

- [ ] Custom user roles (Administrator, moderator, builder, etc...)
- [ ] Ingame player statistics (Banned, kicked, reports, scripters, etc...)
- [ ] Helpful commands for players (Teleport, weapons, force powers, etc...)
- [ ] Punish commands for players (Slay, silence, sleep, rename, lock, etc...)
- [ ] Ability to control bots actions (Fake chat, movements, emotes, etc...)
- [ ] Spawn unlimited items and weapons
- [ ] Private administrator chat room
- [ ] Team management (Lock, force, shuffle, etc...)
- [x] Pause mode to instantly stop everyone on the server during game

---

### Map Modding
**Description:** Ingame maps modification tools

- [ ] Map auto fix to convert most of single player maps to be playable
- [ ] Over 15 custom entities for maps (Including entities from Single Player)
- [ ] Add models (GLM or MD3 format)
- [ ] Add effects (EFX format)
- [ ] Add any entity based on classname and parameters
- [ ] Add pre-made useful utilities (NPC spawner, message consoles, assister, etc...)
- [ ] Change any shader or texture
- [ ] Modify entities properties
- [ ] Delete entities
- [ ] Clone entities
- [ ] Save feature to get all changes automatically stored in a loadable file
- [x] Support for defrag & race maps

---

### Client Side
**Description:** Additional features using the clientside plugin

- [x] Atmospheric effects (Rain, snow, etc...)
- [ ] Extra info for scoreboard (Wins, losses, flag captures, etc...)
- [ ] On-screen keyboard display to see key presses on screen
- [x] Jedi Academy chat display mode
- [x] Two chat icons (Jedi Outcast & Jedi Academy)
- [x] Fixed 2D aspect ratio for non 4:3 screens
- [ ] Fire lightsaber style, to add real fire effects to the lightsaber
- [x] Jetpack to fly around the map
- [ ] Grapple hook to climb on the walls
- [ ] Fully drivable vehicles (Tie Fighter, X-Wing, AT-ST, etc...)
- [x] Ingame clock showing the real time hours, minutes and seconds
- [x] Ingame server list to join another server without disconnect
- [ ] Speed-o-meter to display player speed ingame and best record
- [ ] Strafe jumping helper/efficiency meter
- [ ] Custom lightsaber hilt support
- [x] Enabled 3rd person damage blend
- [x] Show player bounding hit boxes
- [ ] Two alternative huds (Jedi Academy and basic lite)
- [ ] Alternative FPS and server time style
- [x] Alternative force points display on hud
- [x] Display bacta indicator on belt (3D model)
- [ ] Display current weapon in back (3D model)
- [x] Enable custom item indicator
- [ ] Enable push/pull force effect color to match with the color of lightsaber
- [ ] Enable the color for the second blade with dual blade lightsabers
- [x] Custom saber trail speed/length
- [ ] Custom ingame duel challenge music
- [x] Custom flag transparency (For CTF & CTY game type)
- [x] Custom flag alignment (Experimental)
- [x] Custom player hats
- [x] Show player icons on scoreboard
- [x] Server emotes menu for usage and binding
- [x] Unlocked some client commands and cvars (Camera range, distance, etc...)
- [x] Enable first person lightsaber option on menus
- [ ] Enable first person turret chair view
- [x] Enable 23 custom BaseJK player skins by surf files
- [x] Increased some limits on menus (Fov, camera range, name length, etc...)
- [x] Enable invisible private duels
- [x] Macro scanning feature (anti script)
- [x] Smooth client dimensions

---

### Cheats
**Description:** Some features available when cheats are enabled

- [ ] Free backstabs & DFA combos usage
- [ ] Use force grip on items
- [ ] NPC spawn like in single player game

---

# Special Thanks

The following people supported and helped me directly on this project.

- [DeathSpike](https://github.com/Deathspike) _(Client plugin support)_
- Lee Oates _(Anti-script base code)_
- Nerevar _(Ingame effects, serverlist and santa hat)_
- Kaiser / TVpath _(General modding support)_
- [Rich Whitehouse](https://www.richwhitehouse.com) _(Melee weapon from Hydroball)_
- NTxC / Reality~ _(1.02 gameplay support)_
- Tox Laximus _(Bots AI support)_
- [McMonkey](https://github.com/mcmonkey4eva) _(Map modding support)_
- JediDog~ _(1.03 gameplay support)_
- [NewAge](https://github.com/erfg12) _(Admin management support)_
- Mr.Wonko _(JK2 base improvements support)_
- [Xycaleth](https://github.com/xycaleth) _(LeagueMod tweaks)_
- [Daggolin](https://github.com/Daggolin) _(Coding support and gameplay fixes)_
- [Ouned / Twitch](https://github.com/ouned) _(Engine coding support)_
- [Kameleon](https://github.com/KevinFoged) _(3D model draw angles fix)_
- [Fau](https://github.com/aufau) _(2D aspect ratio fix & SaberMod tweaks)_
- [Bucky](https://github.com/Bucky21659) _(EternalJK2 tweaks)_
- BarryHit _(Support and testing)_
- Player _(Support and testing)_
- [Flendo](https://github.com/FlendoJK2) _(Support and testing)_
- Padaget _(vVv mod tweaks)_
- [VideoP / Loda](https://github.com/videoP) _(JAPro tweaks and cosmetics)_
- Noodle _(User interface & modeling support)_
- QK-Lite / Effect.ve _(General coding support)_
- [MVSDK / JK2MV](https://github.com/mvdevs) _(Gameplay changer and MVAPI)_
- x[Dsk]x Clan and 1.03 community _(Support and testing)_
- =AJC= Clan and 1.04 community _(Support and testing)_

# Contact

**Jedi Knight Plus Mod developed by Tr!Force**  
Work copyrighted (C) with holder attribution 2005 - 2020

Visit the mod page: https://jkmod.github.io

Feel free to write me if you got questions regarding the mod\
E-Mail: triforce@gznetwork.com

# License

> This mod is based on *MVSDK* and is licensed under GPLv2 as free software. Some files in `assets` directory are modified assets from the original, non-free JK2 1.04 release and licensed under *JK2 Editing Tools 2.0* EULA. As stated in the main description, most parts of this mod are from other mods and games, i tried to keep all credits on [special thanks](#special-thanks).

> LCC 4.1 is Copyright (c) 1991-1998 by AT&T, Christopher W. Fraser and David R. Hanson, and available under a non-copyleft license. You can find it in code/tools/lcc/COPYRIGHT. LCC version bundled with this SDK comes from ioquake3 and it has been slightly modified by its developers.

> Remaining parts of JK2 SDK GPL are licensed under GPLv2 as free software. Read LICENSE.txt and README-raven.txt to learn more. According to the license, among other things, you are obliged to distribute full source code of your mod alongside of it, or at least a written offer to ship it (eg a HTTP download link inside a .pk3 file). Moreover, any mod using patches from this repository **must** be released under GPLv2 or a compatible license.

> Q3ASM is Copyright (c) id Software and ioquake3 developers.