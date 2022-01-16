#!/bin/sh
# JK+ Server launcher (Linux/OSX)

jk_binary=jk2mvded
jk_dedicated=2
jk_net_port=28070
jk_config=jkmod_server.cfg

cd ..
./$jk_binary +set dedicated $jk_dedicated +set net_port $jk_net_port +set fs_game JediKnightPlus +exec $jk_config
