# Jedi Knight Plus Changelog

The next list is a resume of the changes registered on the project. If you want see the details check the [commit history](https://github.com/TriForceX/JediKnightPlus/commits)

The version control structure on this project is `<mayor>.<minor>.<patch>` where:
- **Major:** The project is stable, tested and approved for a public release
- **Minor:** New features added and ready to use or test
- **Patch:** Small changes, code improvements, bug fixes or work in progress

---

## 0.17.0
- Added strafe helper client command
- Added speed meter client command
- Added auto duel challenge option
- Added base dimension option & cvar calculator

## 0.16.2
- Added drain model map entity
- Added cvar for silent macro scan alerts
- Show player illegal bind instead IP on scan alert
- Added duel pass through feature
- Disable jetpack if cvar turned off
- Added infinite ammo for instagib and cheats
- Fixed execute linux server bash

## 0.16.1
- Removed map restart requirement on dimension cvar
- Fixed captures mini scoreboard
- Added force resist anim and custom effects cvar
- Added god option to bots in static mode
- Make private duel dimension optional
- Updated call vote options

## 0.16.0
- Fix dimension settings conflict with jedi master
- Added identify map objects option
- Added 12 and 24 hour clock option
- Added dual saber menu option

## 0.15.4
- Added dual saber command and cvar
- Updated vote display alignment & dual saber running anim
- Updated vote map and callvote checks
- Show map teleports on motd
- Added listdir server command
- Updated race time display & scoreboard extras
- Updated lagometer and clock display
- Updated dimension force powers & fixed chat protect
- Updated custom hats menu options

## 0.15.3
- Fix fx_runner and target_speaker on map fixes cvar
- Fix func_usable check on map fixes cvar
- Allow setviewpos cmd on cheats dimension
- Fixed g2animent spawn and minor refactor
- Fixed func_usable removal & added entity scan test cmd

## 0.15.2
- Clear player deaths on map change or restart
- Added normal dimension option to menus
- Updated server batch files and config
- Updated mod folder name and copyright note
- Minimal server and client tweaks
- Added assets PK3 build on VS projects
- Console commands refactor
- Show client options pop-up only on main menu
- Prevent chat protect on CTF gametype

## 0.15.1
- Fix special move camera check
- Fix emote camera check
- Fix scoreboard extras scale check
- Fix scoreboard icons alignment
- Fix mini scoreboard translated text

## 0.15.0
- Fix some compiling warnings
- Fix private duel end check and duel timer
- Fix engage force duel with no force powers on server
- Added alternative names for some client commands
- Added special movements camera and client menu tweaks
- Added mini keyboard display and extra scoreboard info
- Allow savepos and loadpos on race & cheats dimension
- Added various tweaks for maps related stuff
- Added custom bots AI functions (WIP)
- Added max vote pass option for call votes
- Added server client join sound option
- Added private duel distance option and cvar changes
- Added spectator no-clip movement
- Show client pop-up only if plugin required is enabled
- Added toggle mod rcon command
- Added 2D style player flag
- Updated mod website url
- Added various tweaks and refactored stuff

## 0.14.5
- Changed forcePlugin cvar to pluginRequired
- Added recommended client options pop-up
- Added slider integer input to be used on menus
- Added option to show player names above their head
- Added JKA chat history option to menus
- Improved crosshair names hidden on player labels
- Added missing check for manually disabled map items
- Disable dimension cvar if MVAPI is not available

## 0.14.4
- Added custom map cycle from file only
- Delay bots on custom game type config load
- Prevent carry custom timescale on map change
- Moved spectator follow dimension check
- Allow whois as a server command
- Check map items on custom game type config load

## 0.14.3
- Added teleport chat cooldown on enemy hits
- Removed custom fraglimit for duel gametype
- Added per-gametype settings configs
- Some dimension command changes and related tweaks
- Added custom slow-mo duel end time scale
- Added voteTime check for pause and rand function
- Some client info checks and tweaks

## 0.14.2
- Fixed race timer on pause mode
- Added teleport chat option for unlimited usage
- Added slow-mo duel end workaround
- Added custom fraglimit for duel gametype
- Added faster score plums & fixed seeing through walls
- Remove dual blade when left cheats dimension

## 0.14.1
- Added normal dimension command option
- Rewritten pause game stuff
- Fixed server idle on game type change and pause game
- Fixed plugin check on custom player movements
- Moved button use animation cvar to player movements
- Allow button use animation only on valid targets
- Added menu tweaks by Flendo
- Added extra check for saber toggle on race dimension

## 0.14.0
- Fix dimension flag to a single value
- Removed clientside dimensions code
- Removed serverside dimensions code (trace)
- Moved and cleaned dimension stuff for easier usage
- Added MV shared entity and custom trace for dimensions
- Some general server and client code tweaks
- Removed private duel cvar and added duel end cvar instead
- Added saber only, instagib and cheats dimensions
- Allow G2animent command in cheats dimension
- Fixed weapon default stand & added player movements cvar
- Allow walk and run while using Disruptor in zoom mode
- Added new hat based on Abrum's Kyle pirate model
- Added map default music option
- Show race timer on race dimension

## 0.13.6
- Remove powerups and projectiles when leave guns dimension
- Added check for Sentry Gun and Item Shield in race dimension
- Check effects and weapons sounds through dimensions
- Added custom energy stations to map entities
- Fixed turret chair map entity model
- Added delay for jetpack usage
- Added delay for dimension change
- Added button-use player animation

## 0.13.5
- Moved custom clientside player functions
- Updated custom player animations
- Fix re-routed map spawn function
- Fix re-routed main trace function
- Moved clientside dimension checks
- Added weapon and effects checks for dimensions

## 0.13.4
- Don't render players from other dimensions (Except for spectator)
- Keep missile impact check only for chat protect
- Workaround for missile impact on dimensions (WIP)
- Disable duel glow if dimension is available
- Added shader remap & change music commands

## 0.13.3
- Changed player pass-through method
- Check pass through only on solid players
- Added player pass through check when join guns dimension
- Removed chat dimension and moved to pass through method
- Check player pass through on emote and chat protect only
- Fixed missing check for player pass through

## 0.13.2
- Reduce trace functions code on dimensions
- Allow solid platforms in race dimension

## 0.13.1
- Draw dimension indicator on hud
- Prevent die on race dimension
- Added temporary dimension for future purposes
- Changed temp dimension check to eFlags type
- Fixed wrong check on custom trace functions
- Changed bolt1 assignment to allow multiple values
- Added option to allow or prevent kill when telefrag

## 0.13.0
- Updated whois command to show current dimension
- Fixed wrong check on chat dimension
- Fixed player stuck when leaving chat dimension
- Added guns dimension
- Re-routed main trace function for dimensions
- Prevent stuck when leaving other dimensions
- Always hide non duelers if dimension is enabled
- Added check for missile impact on dimensions (WIP)

## 0.12.4
- Merge updates from mvsdk
- Changed custom models path
- Added server idle reset
- Disable player transparency when using emote menu
- Fixed emote play & stop on menus

## 0.12.3
- Added some race restrictions and damage fixes
- Updated dimension commands
- Don't kill racers on falling
- Added dimension options to main menus

## 0.12.2
- Added jetpack restriction while moving or private duel
- Set player transparency on chat dimension
- Changed jetpack equipment with +use button
- Added JKA jetpack physics style
- Added jetpack fuel main functions

## 0.12.1
- Prevent jetpack usage on race mode
- Added alternative effect for jetpack on ground
- Added jetpack menu options

## 0.12.0
- Added basic support for race maps
- Prevent force powers on racer respawn & save jump level
- Moved say functions to being used also by console commands
- Client commands cleanup
- Added jetpack serverside functions
- Added jetpack clientside functions

## 0.11.6
- Added check for custom teleports chats
- Reorder say commands by preference
- Changed custom player flags & dimensions code cleanup
- Prevent lightsaber usage and emotes in race dimension
- Moved race command into dimension and say commands
- Prevent race mode beign disabled on respawn
- Disable force powers on race mode

## 0.11.5
- Fixed custom model alignment on 1.02
- Fix item position reset on force physics disable
- Don't change gameplay cvar if is the same as the server
- Added example server config & basic launcher
- Moved client & server assets folders
- Revert mod name changes

## 0.11.4
- Updated clientside translations
- Updated mod game version
- Updated general and poll callvote menus
- Updated mod options and callvote on main menu
- Fixed item force physics spawn

## 0.11.3
- Workaround for hitbox height on rolling
- Added option to reload custom configs
- Disable custom model render on 1.02 (Temporary)
- Disable draw clock & lagometer on UI
- Merge updates from mvsdk

## 0.11.2
- Updated client menus (Translation pending)
- Compress size on hats textures
- Some text cleanups
- Added custom hats as user info
- Merge updates from mvsdk

## 0.11.1
- Added reset cvars and commands for menus
- Added flag opacity option
- Added flag custom alignment

## 0.11.0
- Fixed net/lag icon showing through walls
- Enabled 3rd person damage blend
- Clean client G2 instances on gameplay changes
- Added force points on hud
- Added custom chat style (JKA)
- Added custom chat icon (JKA)
- Added custom anims (Only run anim for now)
- Added draw inventory option
- Added scoreboard icons & missing icon
- Added player bounding hit boxes
- Added duel glow option
- Added custom hats
- Added draw bacta model
- Merge updates from mvsdk

## 0.10.5
- Added private duel option
- Added real time clock options
- Updated client mod menu options
- Merge updates from mvsdk

## 0.10.4
- Fixed wrong player model selection
- Fixed player animation bug on gameplay change
- Added server closed password
- Added call vote minimum players cvar

## 0.10.3
- Upscaled main menu mod logo
- Enable 1.02 player angles for ingame changes
- Added custom player models surf files
- Renamed client side source files

## 0.10.2
- Added ingame server list
- Added client version on main menu

## 0.10.1
- Fixed custom duel force usage before it starts

## 0.10.0
- Moved mod session stuff to a custom file
- Keep ignore all chats & duels on session
- Prevent spam on map teleport chats
- Added anti player warping and lag scripts
- Added anti script & macro scanning

## 0.9.9
- Prevent teleport save & load chat spam
- Added option to ignore all duels & chats
- Added whois command

## 0.9.8
- Added push box for teleport chats
- Fixed bot team join on duel gametype

## 0.9.7
- Renamed mod functions
- Moved mod stuff to mod files
- Improved teleport player function
- Added effects and sounds to teleport chats

## 0.9.6
- Updated teleport chat methods
- Added client plugin check

## 0.9.5
- Added teleport chat commands
- Added call vote wait time
- Code tweaks and cleanup

## 0.9.4
- Fixed force duel challenge bugs
- Updated some center prints
- Merge updates from mvsdk

## 0.9.3
- Added force duel challenge
- Prevent some files from loading in duel gametype

## 0.9.2
- Prevent server news on duel gametype

## 0.9.1
- Added random client begin messages
- Added server news messages
- Fixed mod version and code cleanup
- Changed pause game options
- Changed mod urls

## 0.9.0
- Changed emote command usage
- Set emote player custom height
- Updated chat protect options
- Added duel & chat dimensions

## 0.8.4
- Fixed duel armor cvar
- Added ignore chat & duel challenge command
- Added custom call vote system

## 0.8.3
- Server motd only shows once
- Item physics restores original position
- Changed back private duel armor to the original

## 0.8.2
- Added chat protect
- Added welcome message

## 0.8.1
- Prevent duplicated player names
- Fixed wrong callvote condition

## 0.8.0
- Allow force physics on items
- Allow multiple duel challenge
- Allow chat for dueling players
- Display duel challenge end stats
- Added duel chalenge start health/shield
- Added hit damage plums
- Removed old custom server launcher

## 0.7.5
- Fix for gametype not supported maps on callvote
- Disabled emotes on CTF & CTY
- Renamed internal cvars for better recognize

## 0.7.4
- Added gameplay changer as vote option

## 0.7.3
- Fixed limits to drop flag command usage
- Added jk_dropFlagTime cvar
- Added custom client timer actions
- Merge updates from mvsdk

## 0.7.2
- Updated mod header variables
- Updated project website url on menus

## 0.7.1
- Remove non required code in drop flag command

## 0.7.0
- Added drop flag command for CTF & CTY gametypes

## 0.6.0
- Added pause game feature

## 0.5.1
- Renamed cvars prefix and server closed cvars

## 0.5.0
- Added black colors in names
- Added server closed feature
- Added server console commands and gameplay changer

## 0.4.2
- Enabled emotes usage from menus
- Merge updates from mvsdk

## 0.4.1
- Added main mod menu options and emotes menu

## 0.4.0
- Added clientside assets and language settings
- Added modules for cgame and ui + ingame menus fallback

## 0.3.3
- Fixed emotes animations and added 1.02 version restriction
- Enable emotes on 1.03 version and list arranged
- Improved functionallity for special emotes

## 0.3.2
- Changed back all emote ids to its references and few fixes

## 0.3.1
- Added dropPlayer and sendCommand to common functions

## 0.3.0
- Changed emotes description and list on main command
- Updated emote prediction and added combat module
- Added common functions and updated emote prediction flag
- Added help and emote list commands
- Added emotes feature

## 0.1.1
- Updated main modules for server-side client events
- Merge updates from mvsdk

## 0.1.0
- Added mod headers and main modules for cvars and commands
- Initial merge from mvsdk source code as base