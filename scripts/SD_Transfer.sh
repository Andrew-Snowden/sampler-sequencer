#!/bin/bash

sudo mkdir mnt && sudo mount /dev/sdb4 ./mnt/ && sudo rm -rf ./mnt/a7-main.uimg && sudo cp ./build/a7-main.uimg ./mnt/ && sudo umount mnt && sudo rm -rf mnt
