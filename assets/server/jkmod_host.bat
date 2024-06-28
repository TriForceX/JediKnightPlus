@echo off
:: JK+ Server launcher (Windows)

set jk_binary=jk2mvded.exe
set jk_dedicated=2
set jk_net_port=28070
set jk_mod=JediKnightPlus
set jk_start_map=ffa_bespin
set jk_config=jkmod_server.cfg

cd ..
%jk_binary% +set dedicated %jk_dedicated% +set net_port %jk_net_port% +set fs_game %jk_mod% +set nextmap map %jk_start_map% +exec %jk_config%