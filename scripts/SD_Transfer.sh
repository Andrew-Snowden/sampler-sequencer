#!/bin/bash

sudo mkdir mnt && sudo mount /dev/sdb4 ./mnt && sudo rm -rf ./mnt/a7-main.uimg && sudo rm -rf ./mnt/main.elf && sudo cp ./build/corea7/a7-main.uimg ./mnt/ && sudo cp ./bin/m4/main.elf ./mnt/ && sudo umount mnt && sudo rm -rf mnt
