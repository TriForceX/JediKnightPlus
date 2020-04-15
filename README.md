# Jedi Knight Plus

https://triforce.dev/projects/jkplus

> Game modification for **Jedi Knight II: Jedi Outcast Multiplayer**. The main goal is to reunite the most useful and custom stuff to the game environment, specially make players ideas come true adding new features as well porting some from other mods and games.

<div align="center">

[![version](https://img.shields.io/badge/Current%20Version-0.8.3-brightgreen.svg)](https://github.com/TriForceX/JediKnightPlus/releases)
[![Wiki](https://img.shields.io/badge/Setup%20%26%20Guides-Wiki-blueviolet.svg)](https://github.com/TriForceX/JediKnightPlus/wiki)
[![Discord](https://img.shields.io/badge/Discord%20Chat-Join-7289DA.svg?logo=discord&longCache=true&style=flat)](https://discord.gznetwork.com/jkplus)
[![stars](https://img.shields.io/github/stars/TriForceX/JediKnightPlus.svg?label=Project%20Stars)](https://github.com/TriForceX/JediKnightPlus/stargazers)
[![issues](https://img.shields.io/github/issues/TriForceX/JediKnightPlus.svg?label=Issues%20%26%20Requests&color=red)](https://github.com/TriForceX/JediKnightPlus/issues)
[![license](https://img.shields.io/github/license/TriForceX/JediKnightPlus.svg?label=License&color=yellow)](#license)

</div>

# Information

- Mod configuration and **download** [here](https://github.com/TriForceX/JediKnightPlus/wiki/Setup)
- Read more information and guides at **wiki** [here](https://github.com/TriForceX/JediKnightPlus/wiki)
- See version changes history in the **changelog** [here](https://github.com/TriForceX/JediKnightPlus/blob/master/CHANGELOG.md)
- Join the mod **Discord** chat server for more support [here](https://discord.gznetwork.com/jkplus)

# About
I started to code the first version of this mod in 2005. Back then it was a private mod and only few people had access. So as a personal task and due to some people request i decided to re-code the mod into a public version, just as a legacy.

The next list is all *features* from the original **Jedi Knight Plus** private mod _(2005 & 2008 versions)_ which need to be ported into the new source code. _(Each completed task will be marked with a check)_

---

### Server Side
**Description:** General server features _(No client plugin required)_

- [x] Gameplay changer between 1.02, 1.03 and 1.04 in the current match
- [ ] Improved bots AI (Artificial intelligence)
- [ ] Configurable weapon features (Damage, ammo speed, firing rate, etc...)
- [ ] Player account system (Tracks kills, deaths, etc...)
- [ ] Ranking system based on player skills
- [ ] Chat censor filters
- [x] Allow black color in player names
- [x] Prevent duplicated player names
- [ ] In-game chat commands (Teleports, info, etc...)
- [x] Welcome message on client begin
- [ ] Server news messages per each certain amount of time
- [ ] Custom in-game modes (Instant kill, survival, last man standing, etc...)
- [ ] Custom player private chat rooms
- [ ] Enabled Jedi Crystal and Beer Bacta items
- [x] Show damage plums on hits
- [ ] NPC spawn command like in Single Player
- [ ] New and improved cheat commands
- [ ] Clan tag protection for clan members
- [x] Over 50 player emotes (Sit, kiss, surrender, etc...)
- [ ] Teleport commands (Save & load position, player to position, etc...)
- [x] Anti lamer feature (Players can't kill during chat)
- [ ] Dual blade lightsaber gameplay (Fixed damages, different combos, etc...)
- [x] Items get affected by force physics (Push, pull, etc...)
- [x] Enable multiple duel challenge
- [ ] Custom duel challenge modes (Full force, no force, guns, etc...)
- [x] Custom duel stats (Health, armor and hits)
- [x] Custom duel challenge start health and armor
- [ ] Enable invisible private duel challenge
- [x] Enable chat in duel game type
- [x] Passwordless closed server feature (Allow only by IP)
- [ ] Alternative dimensions (Weapon or saber only, custom gameplay, etc...)
- [ ] Custom poll call votes
- [ ] Improved slow-mo duel end functionally
- [x] Drop flag command for CTF & CTY game types

---

### Administration
**Description:** In-game server management features for administrators

- [ ] Custom user roles (Administrator, moderator, builder, etc...)
- [ ] In-game player statistics (Banned, kicked, reports, scripters, etc...)
- [ ] Helpful commands for players (Teleport, weapons, force powers, etc...)
- [ ] Punish commands for players (Slay, silence, sleep, rename, lock, etc...)
- [ ] Ability to control bots actions (Fake chat, movements, emotes, etc...)
- [ ] Spawn unlimited items and weapons
- [ ] Private administrator chat room
- [ ] Custom call vote with actions
- [ ] Team management (Lock, force, shuffle, etc...)
- [x] Pause mode to instantly stop everyone on the server during game

---

### Map Modding
**Description:** In-game maps modification tools

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

---

### Client Side
**Description:** Additional features using the clientside plugin

- [ ] Atmospheric effects (Rain, snow, sun, etc...)
- [ ] Rank icons for the scoreboard (Also show wins, losses, flag captures, etc...)
- [ ] Mini keyboard display to see key-presses on-screen
- [ ] Jedi Academy chat display mode (Positioned at left bottom)
- [ ] Three chat display icons (JK2, Jedi Academy and user customizable)
- [x] Fixed 2D aspect ratio for non 4:3 screens
- [ ] Fire lightsaber style, to add real fire effects to the lightsaber
- [ ] Fully working jet pack to fly around the map
- [ ] Grapple hook to climb against walls
- [ ] Fully drivable vehicles (Tie Fighter, X-Wing, AT-ST, etc...)
- [ ] In-game clock showing the actual local hours, minutes and seconds
- [ ] In-game server list to join another server without disconnect
- [ ] Speed-o-meter to display player speed in-game and best record
- [ ] Strafe jumping helper/efficiency meter
- [ ] Custom lightsaber hilt support
- [ ] Two alternative huds (Jedi Academy and basic lite)
- [ ] Alternative FPS and server time style
- [ ] Alternative Force points display with numbers
- [ ] Display bacta indicator (3D model)
- [ ] Display current weapon in back (3D model)
- [ ] Enable custom item indicator
- [ ] Enable push/pull force effect color to match with the color of lightsaber
- [ ] Enable the color for the second blade with dual blade lightsabers
- [ ] Custom saber trail length
- [ ] Custom in-game duel challenge music
- [ ] Custom flag transparency (For CTF game type)
- [ ] Show player icons on scoreboard
- [x] Server emotes menu for usage and binding
- [ ] Unlockd some client commands and cvars (Camera range, distance, etc...)
- [ ] Enable first person lightsaber
- [ ] Enable first person turret chair view
- [ ] Enable custom player skins (Rocket Trooper, Jedi Jan, etc...)
- [ ] Anti-script feature (Macro scanning)
- [ ] Increased some limits (Fov, camera range, name length, etc...)

---

# Special Thanks

The following people supported and helped me directly on this project.

- [DeathSpike](https://github.com/Deathspike) _(Client plugin support)_
- Lee Oates _(Anti-script base code)_
- Nerevar _(Ingame effects, atmospherics and santa hat)_
- Kaiser / TVpath _(General modding support)_
- [Rich Whitehouse](https://www.richwhitehouse.com) _(Melee weapon from Hydroball)_
- NTxC / Reality~ _(1.02 gameplay support)_
- Tox Laximus _(Bots AI support)_
- [McMonkey](https://github.com/mcmonkey4eva) _(Map modding support)_
- JediDog~ _(1.03 gameplay support)_
- [NewAge](https://github.com/erfg12) _(Admin management support)_
- Mr.Wonko _(JK2 base improvements support)_
- [Daggolin](https://github.com/Daggolin) _(Map modding and coding support)_
- [Ouned / Twitch](https://github.com/ouned) _(Engine coding support)_
- Kameleon _(3D model draw angles fix)_
- [Fau](https://github.com/aufau) _(2D aspect ratio fix & SaberMod tweaks)_
- [Bucky](https://github.com/Bucky21659) _(EternalJK mod tweaks)_
- BarryHit _(Support and testing)_
- Player _(Support and testing)_
- [Flendo](https://github.com/FlendoJK2) _(Support and testing)_
- Padaget _(vVv mod tweaks)_
- [VideoP / Loda](https://github.com/videoP) _(Strafe helper)_
- Noodle _(User interface support)_
- Effect.ve _(General coding support)_
- [MVSDK / JK2MV](https://github.com/mvdevs) _(Gameplay changer and MVAPI)_
- x[Dsk]x Clan and 1.03 community _(Support and testing)_
- =AJC= Clan and 1.04 community _(Support and testing)_

# Contact

**Jedi Knight Plus Mod developed by Tr!Force**  
Work copyrighted (C) with holder attribution 2005 - 2020

Visit the mod page: https://triforce.dev/projects/jkplus

Feel free to write me if you got questions regarding the mod\
E-Mail: triforce@gznetwork.com

# License

> This mod is based on *MVSDK* and is licensed under GPLv2 as free software. Some files in `assets` directory are modified assets from the original, non-free JK2 1.04 release and licensed under *JK2 Editing Tools 2.0* EULA.

> LCC 4.1 is Copyright (c) 1991-1998 by AT&T, Christopher W. Fraser and David R. Hanson, and available under a non-copyleft license. You can find it in code/tools/lcc/COPYRIGHT. LCC version bundled with this SDK comes from ioquake3 and it has been slightly modified by its developers.

> Remaining parts of JK2 SDK GPL are licensed under GPLv2 as free software. Read LICENSE.txt and README-raven.txt to learn more. According to the license, among other things, you are obliged to distribute full source code of your mod alongside of it, or at least a written offer to ship it (eg a HTTP download link inside a .pk3 file). Moreover, any mod using patches from this repository **must** be released under GPLv2 or a compatible license.

> Q3ASM is Copyright (c) id Software and ioquake3 developers.