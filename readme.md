# Quickstart

## Dependencies (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install build-essential nasm qemu-system-x86 gcc-multilib make
```

## Build

```bash
make rebuild
```

## Run in QEMU

```bash
qemu-system-x86_64 -cpu host -drive format=raw,file=build/disk.img
```

*(`-cpu host` is recommended here so AVX instructions pass through to the VM without `#UD` exceptions).*

## Write to USB Stick

1. Identify your USB drive:
```bash
lsblk

```


2. Write the image (replace `sdX` with your actual device name, e.g., `sdb`):
```bash
sudo dd if=build/disk.img of=/dev/sdX bs=4M status=progress oflag=sync
sudo sync

```

> **Warning:** Always target the base device (e.g., `/dev/sdb`), **never** a partition (e.g., `/dev/sdb1`). Writing to the wrong device can permanently destroy your system installation.