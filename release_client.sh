#!/bin/bash
set -e

VERSION="0.5"
PKG="udp-vpn-client"
ARCH="amd64"
REPO="ghgv/VPN"

BIN_NAME="bin/$PKG"
SERVICE_SRC="systemd/system/$PKG.service"
PKG_DIR="${PKG}-${VERSION}"
DEB_NAME="${PKG}-${VERSION}-${ARCH}.deb"

echo "🔧 Compilando $PKG..."
make


echo "📦 Empaquetando $PKG"



cp "$BIN_NAME" "$PKG_DIR/usr/local/bin/"

cat > "$PKG_DIR/DEBIAN/control" <<EOF
Package: $PKG
Version: $VERSION
Architecture: $ARCH
Maintainer: Tu Nombre <tu@email.com>
Description: VPN UDP - Cliente
EOF

cat > "$PKG_DIR/DEBIAN/postinst" <<EOF
#!/bin/bash
set -e
systemctl daemon-reload
systemctl enable $PKG
systemctl start $PKG
EOF

chmod +x "$PKG_DIR/DEBIAN/postinst"


echo "$PKG_DIR"
dpkg-deb --build "$PKG_DIR"
echo "$PKG_DIR"
mv "$PKG_DIR.deb" "$DEB_NAME"
echo "✅ Generado $DEB_NAME"

echo "🚀 Subiendo a GitHub Release..."
gh release create "v$VERSION" "$DEB_NAME" --repo "$REPO" --title "Client v$VERSION" --notes "Release de $PKG"
