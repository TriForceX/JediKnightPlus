# Jedi Knight Plus

<img align="left" width="110" height="110" src="https://user-images.githubusercontent.com/16083854/94480608-ed2bb780-01ac-11eb-824c-00ca03f95edc.png">

https://jkplus.github.io

Game modification for **Jedi Knight II: Jedi Outcast Multiplayer**. The main goal of this mod is to reunite useful stuff and add custom features to the game environment, specially making players ideas come true adding new features and porting some from other mods and games.

<div align="center">

[![version](https://img.shields.io/github/v/release/TriForceX/JediKnightPlus?include_prereleases&label=Current%20Version&color=brightgreen&style=flat)](https://github.com/TriForceX/JediKnightPlus/releases)
[![wiki](https://img.shields.io/badge/Setup%20%26%20Guides-Wiki-blueviolet.svg?style=flat)](https://github.com/TriForceX/JediKnightPlus/wiki)
[![discord](https://img.shields.io/discord/539434081771847680.svg?logo=discord&logoColor=white&label=Discord&color=5865F2&style=flat)](https://discord.gznetwork.com/jkplus)
[![stars](https://img.shields.io/github/stars/TriForceX/JediKnightPlus.svg?label=Project%20Stars&style=flat)](https://github.com/TriForceX/JediKnightPlus/stargazers)
[![issues](https://img.shields.io/github/issues/TriForceX/JediKnightPlus.svg?label=Issues%20%26%20Requests&color=red&style=flat)](https://github.com/TriForceX/JediKnightPlus/issues)
[![license](https://img.shields.io/github/license/TriForceX/JediKnightPlus.svg?label=License&color=yellow&style=flat)](#license)

</div>

---

# Information

- Mod configuration and **download** [here](https://github.com/TriForceX/JediKnightPlus/releases)
- Read more information and guides at **wiki** [here](https://github.com/TriForceX/JediKnightPlus/wiki)
- See version changes history in the **changelog** [here](https://github.com/TriForceX/JediKnightPlus/blob/master/CHANGELOG.md)
- Join the mod **Discord** chat server for more support [here](https://discord.gznetwork.com/jkplus)

# About

I started to code the first version of this mod in 2005. Back then it was a private mod and only few people had access. So as a personal task and due to some people request i decided to re-code the mod and turn it into a public version, just as a legacy.

<p align="center">

<img width="200" height="150" src="https://github.com/TriForceX/JediKnightPlus/assets/16083854/5710fa0c-b97e-4e3b-ac7f-e3995c996b16">
<img width="200" height="150" src="https://github.com/TriForceX/JediKnightPlus/assets/16083854/c5944f99-e29c-4339-bd59-94d8127e1900">
<img width="200" height="150" src="https://github.com/TriForceX/JediKnightPlus/assets/16083854/516af691-3737-4f9b-9489-52976ca4bc86">
<img width="200" height="150" src="https://github.com/TriForceX/JediKnightPlus/assets/16083854/0d7596a6-c718-42c7-b697-a7cd9297b8b1">
<img width="200" height="150" src="https://github.com/TriForceX/JediKnightPlus/assets/16083854/d4bb8cf6-89ef-40fa-808b-400540ca28de">
<img width="200" height="150" src="https://github.com/TriForceX/JediKnightPlus/assets/16083854/e94aa3e0-a9f8-4270-8be7-710e44e0fc7b">
<img width="200" height="150" src="https://github.com/TriForceX/JediKnightPlus/assets/16083854/65a95166-19b8-4613-bc71-8261544df711">
<img width="200" height="150" src="https://github.com/TriForceX/JediKnightPlus/assets/16083854/fdd8da07-c6d4-40b3-8f6f-a5e37443b653">

</p>

The next list is all *features* from the original **Jedi Knight Plus** private mod _(2005 & 2008 versions)_ which need to be ported into the new source code, theres also a few new ones from people requests. _(Each completed task will be marked with a check)_.

---

# Mod Features

<details><summary> Server Settings</summary><br>

List of general server features <i>(No client plugin required)</i>

- [x] Gameplay changer between 1.02, 1.03 and 1.04 no matter the server version
- [ ] Custom weapon features (Damage, ammo speed, firing rate, etc...)
- [ ] Player accounts system (Save data such as kills, deaths, etc...)
- [ ] Ranking system (Based on player skills)
- [ ] SQLite database for server data storage
- [x] Jetpack (Quake 3 & Jedi Academy style)
- [x] Anti stuck feature (Prevent kills when player spawn inside another player)
- [ ] Chat censor filters
- [x] Chat protection (3 different methods)
- [x] Allow black color in player names
- [x] Prevent duplicated player names
- [x] Teleports chat commands (To map coords, players, by save & load, etc...)
- [x] Misc chat commands (Status, toggle options, etc...)
- [x] Custom server closed feature (With password required)
- [x] Custom server idle feature (Load custom config after reset)
- [x] Welcome message on client begin (MOTD with custom timer)
- [x] Custom global sound on client begin
- [x] Server news messages displayed constantly (With server time or date)
- [ ] Custom game modes (Instant kill, survival, last man standing, and more...)
- [x] Custom player private rooms (With configurable settings)
- [x] Alternate taunt2 command (Play custom sounds)
- [x] Show damage & score plums on player hits
- [ ] Clan system (Tag protection, member roles, etc...)
- [x] Over 60 player emotes (Sit, kiss, surrender, hug, etc...)
- [x] Dual blade lightsaber gameplay (Fixed damages, custom combos, and more...)
- [x] Custom items force physics (Reacts to push, pull, and others...)
- [x] Custom configuration files (Per gametype, teleports, map cycle, and more...)
- [x] Player ignore feature (Chat, duel challenge and emotes)
- [x] Enable multiple chat in duel game type
- [x] Custom call vote system (Custom control, results display, and more...)
- [x] Alternate slow-mo duel end feature (Custom time scale)
- [x] Auto player status tracking feature
- [x] Drop flag command for CTF & CTY game types
- [x] Random client begin messages
- [x] Force client plugin usage (Optional)
- [x] Prevent player warping or lag scripts
- [x] Improved custom map cycle
- [ ] Drivable vehicles (Tie Fighter, X-Wing, AT-ST, etc...)
- [ ] Custom melee weapon
- [ ] Portal gun (Experimental)

---

</details>

<details><summary> Server Management</summary><br>

List of general server management features

- [ ] Custom user roles (Administrator, moderator, builder, etc...)
- [ ] Ingame player statistics (Banned, kicked, cheaters, etc...)
- [x] Helpful commands for players (Teleport, weapons, force powers, etc...)
- [ ] Punish commands for players (Slay, silence, sleep, rename, lock, etc...)
- [x] Ability to control bots actions (Fake chat, movements, emotes, etc...)
- [ ] Spawn unlimited items and weapons
- [ ] Private administrator chat room
- [x] Team management (Lock, force, shuffle, etc...)
- [x] Toggle mod bitvalue cvars
- [x] List custom server files and folders
- [x] Reload config stuff (Teleport chats, server news, dimensions, and more...)
- [x] Change any shader or texture
- [x] Change map background music
- [x] Force player dimension
- [x] Alternate status command (Mod info shown)
- [x] Change global gameplay
- [x] Pause mode to instantly stop everyone on the server during game

---

</details>

<details><summary> Server Dimensions</summary><br>

List of server dimensions features <i>(No client plugin required)</i>

- [x] Alternate dimensions (Private, guns only, race defrag, cheats, and more...)
- [x] Default base dimension
- [x] Special client commands for certain dimensions
- [x] Private rooms with custom settings (Force disable, weapons, and more...)
- [x] Private duels alternate dimensions
- [x] Custom dimensions random spawns
- [ ] Custom dimensions scoring tracking
- [x] Custom dimensions settings from config file

---

</details>

<details><summary> Private Duels</summary><br>

List of private duel features <i>(No client plugin required)</i>

- [x] Dueling players pass-through feature
- [x] Auto duel challenge feature
- [x] Allow multiple duel challenges
- [x] Custom private duel distance
- [x] Custom private duel start emote
- [x] Custom duel challenge modes (Full force, saber only, and more...)
- [x] Custom duel challenge start health and armor
- [x] Announce private duel results (Remaning health, armor and given hits)

---

</details>

<details><summary> Bots Settings</summary><br>

List of bots related features <i>(No client plugin required)</i>

- [x] Improved bots AI (Artificial intelligence)
- [ ] Custom bots saber combos (backstabs, dfa, force kicks, and more...)
- [x] Custom bots actions (Enhanced bot_forGimmick for practicing and testing)
- [x] Client bots control (Control bots actions through spectator)
- [x] Private room bots (Ability to invite and control bots on private roms)

---

</details>

<details><summary> Map Modding</summary><br>

List of maps modification features

- [x] Various map fixes (Including single player maps to be playable)
- [x] New custom map entities and fixed some BaseJK entities
- [x] Custom map default music (For maps without background music)
- [x] Support for defrag & race maps
- [ ] Add models (GLM or MD3 format)
- [ ] Add effects (EFX format)
- [ ] Add any entity based on classname and parameters
- [ ] Add pre-made useful utilities (NPC spawner, message consoles, assister, etc...)
- [ ] Create and modify entities
- [ ] Delete entities
- [ ] Clone entities
- [x] Scan entities
- [ ] Save feature to get all changes automatically stored in a loadable file
- [x] Custom shader remapping

---

</details>

<details><summary> Client Plugin</summary><br>

List of additional client-side plugin features

- [ ] Atmospheric effects (Rain, snow, etc...)
- [x] Extra info for scoreboard (Kills, deaths, and more...)
- [x] On-screen keyboard display (Player actions and movements)
- [x] Jedi Academy chat display mode (With chat history)
- [x] Two chat icons (Jedi Outcast & Jedi Academy)
- [x] Fixed 2D aspect ratio for non 4:3 screens
- [x] Recommended client options pop-up for new players
- [ ] Fire lightsaber style (Real FX effects)
- [x] Jetpack 3D model (With linked FX effects)
- [x] Special movements camera (On some lightsaber combos)
- [x] Show player labels on head (2 different methods)
- [ ] Grapple hook feature
- [x] Real time clock (12hrs and 24hrs)
- [x] Server list menu (Join another server without disconnect)
- [x] Custom speedometer (Over 9 different methods)
- [x] Strafe jumping helper/efficiency meter
- [ ] Custom lightsaber hilts support
- [x] Enabled 3rd person damage blend
- [x] Show player bounding hit boxes
- [ ] Alternate huds (Simple enhanced and Jedi Academy)
- [x] Alternate FPS and server time display
- [x] Show force points on hud
- [x] Display bacta indicator on belt (3D model)
- [ ] Display current weapon in back (3D model)
- [x] Display race timer on race dimension (Defrag)
- [x] Enable useable map items indicator on hud
- [ ] Enable push/pull force effect color based on lightsaber color
- [x] Allow custom second blade color from dual lightsaber
- [x] Custom saber trail speed/length
- [ ] Custom duel challenge music
- [x] Custom flag transparency (For CTF & CTY game type)
- [x] Custom flag alignment (Experimental)
- [x] Custom player hats and cosmetics
- [x] Show player icons on scoreboard
- [x] Server emotes menu for usage and binding
- [x] Custom private duel end animation
- [x] Toggle private duel glow
- [x] Smooth dueling players pass-through
- [x] Fixed force rage recovery icon (Borderless)
- [x] Enable first person lightsaber option on menus
- [ ] Enable first person turret chair view
- [x] Enable over 20 BaseJK hidden player skins
- [x] Increased some limits on menus (Fov, camera range, name length, etc...)
- [x] Macro scanning feature (Anti-script)
- [x] Allow walk and run while using zoom on Disruptor weapon
- [x] Many BaseJK language text updates and fixes
- [ ] Show server maps on callvote menus

---

</details>

<details><summary> Cheats</summary><br>

List of cheats enabled features

- [ ] Free backstabs & DFA combos usage
- [ ] Use force grip on items
- [x] NPC spawn like in single player game

</details>

---

# Special Thanks

The following people supported and helped me directly on this project.

Name | Mod & Game | Notes
:------------: | :------------: | :------------
[DeathSpike](https://github.com/Deathspike) | DS-Online | Client plugin and emotes base
Lee Oates | Duelers Mod | Anti-script & macro scan base
Nerevar | Cam Mod | Some ingame effects and serverlist
Kaiser / TVpath | -- | General modding support
[Rich Whitehouse](https://www.richwhitehouse.com) |  HydroBall | Melee weapon server-side
NTxC / Reality~ | NT's Fix | 1.02 gameplay support
Tox Laximus | uL Mod | Bots AI support
[McMonkey](https://github.com/mcmonkey4eva) | Twifire | Map modding support and fixes
JediDog~ | JD Fix | 1.03 gameplay support
[NewAge](https://github.com/erfg12) | Clan Mod | Admin system support
Mr.Wonko | -- | JK2 base improvements and support
[Xycaleth](https://github.com/xycaleth) | League Mod | Team gameplay ideas
[Daggolin](https://github.com/Daggolin) | -- | Coding support and gameplay fixes
[Ouned / Twitch](https://github.com/ouned) | JK2MF | Engine coding support and info
[Kameleon](https://github.com/KevinFoged) | -- | 3D model draw angles fix
[Fau](https://github.com/aufau) | Saber Mod | 2D ratio fix and misc references
[Bucky](https://github.com/Bucky21659) | EternalJK2 | Speedometer and strafehelper base
[EternalCodes](https://github.com/eternalcodes) | EternalJK | Misc ideas and references
Abrum | -- | Pirate hat base model
BarryHit | -- | Support and testing
Player | -- | Support and testing
[Flendo](https://github.com/FlendoJK2) | -- | Custom modded SP maps
Padaget | vVv Mod | Misc ideas and references
[VideoP / Loda](https://github.com/videoP) | JAPro | Defrag support and references
Noodle | -- | UI and modeling support
Smoo | -- | Support and testing
[Tayst](https://github.com/taysta) | TaystJK | Some cosmetics models
QK-Lite / Effect.ve | -- | General coding support
[MVSDK Devs](https://github.com/mvdevs) | JK2MV | Gameplay changer and MVSDK API
x[Dsk]x Clan | JK2 1.03 | Support and testing
=AJC= Clan | JK2 1.04 | Support and testing

---

# Contact

**Jedi Knight Plus Mod developed by Tr!Force**  
Work copyrighted (C) with holder attribution 2005 - 2024

Visit the mod page: https://jkplus.github.io

Feel free to write me if you got questions regarding the mod\
E-Mail: triforce@gznetwork.com

# License

> This mod is based on *MVSDK* and is licensed under GPLv2 as free software. Some files in `assets` directory are modified assets from the original, non-free JK2 1.04 release and licensed under *JK2 Editing Tools 2.0* EULA. As stated in the main description, most parts of this mod are modified or upgraded parts from other mods or games, i tried to keep all credits on [special thanks](#special-thanks).

> LCC 4.1 is Copyright (c) 1991-1998 by AT&T, Christopher W. Fraser and David R. Hanson, and available under a non-copyleft license. You can find it in code/tools/lcc/COPYRIGHT. LCC version bundled with this SDK comes from ioquake3 and it has been slightly modified by its developers.

> Remaining parts of JK2 SDK GPL are licensed under GPLv2 as free software. Read LICENSE.txt and README-raven.txt to learn more. According to the license, among other things, you are obliged to distribute full source code of your mod alongside of it, or at least a written offer to ship it (eg a HTTP download link inside a .pk3 file). Moreover, any mod using patches from this repository **must** be released under GPLv2 or a compatible license.

> Q3ASM is Copyright (c) id Software and ioquake3 developers.
