gcc -g -c ../src/backend/folders.c -o ../dist/folders.o &
gcc -g -c ../src/backend/diff.c -o ../dist/diff.o &
gcc -g -c ../src/backend/files.c -o ../dist/files.o &
gcc -g -c ../src/ui/controls.c -o ../dist/controls.o &
gcc -g -c ../src/ui/ui.c -o ../dist/ui.o &

wait

gcc -g ../src/main.c ../dist/folders.o ../dist/diff.o ../dist/files.o ../dist/controls.o ../dist/ui.o -lncurses -o ../dist/a.out
