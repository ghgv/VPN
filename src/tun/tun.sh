ip tuntap add dev tun0 mode tun
ip addr add 10.0.0.2/24 dev tun0
ip link set up dev tun0