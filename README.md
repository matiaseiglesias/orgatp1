# orgatp1

compilar normal: gcc -Wall -g -o tp.exe ant_engine.c artist_ant.c
compilar con assembly: gcc -Wall -g -D ASSEMBLY -I/root/orgatp1 -o tp.exe paint.S ant_engine.c artist_ant.c palette_size.S ant_advance.S
