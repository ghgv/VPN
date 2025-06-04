#!/bin/bash
set -e

# Compilar y empaquetar
make clean
make
dpkg-deb --build udp_vpn_0.5

# Crear release (si ya existe, solo sube el .deb)
gh release upload v0.5 udp_vpn_0.5.deb --clobber
