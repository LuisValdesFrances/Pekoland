lcc -c -o main.o main.c collision.o collision.c
lcc -Wf-bo2 -c -o mapTiles.o mapTiles.c
lcc -Wf-bo3 -c -o spriteTiles.o spriteTiles.c
lcc -Wf-bo4 -c -o maps.o maps.c

lcc -Wl-yt1 -Wl-yo8 -o pekoland.gb main.o collision.o mapTiles.o spriteTiles.o maps.o
pause