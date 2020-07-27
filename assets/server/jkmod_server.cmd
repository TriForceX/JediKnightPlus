echo off

goto(){
# Linux
cd ..
./jk2mvded +set net_port 28070 +set dedicated 2 +set fs_game JediKnightPlus +exec jkmod_server.cfg
}

goto $@
exit

:(){
:: Windows
cd ..
jk2mvded.exe +set net_port 28070 +set dedicated 2 +set fs_game JediKnightPlus +exec jkmod_server.cfg

exit