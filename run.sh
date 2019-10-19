#!/bin/bash

qemu=qemu-system-i386
image=kernel/myos.bin

$qemu -kernel $image \
    -debugcon stdio\
    -m 1G \
    -d guest_errors


