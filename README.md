# UDP VPN

`udp_vpn` es una herramienta escrita en C para establecer un túnel VPN sobre UDP, pensada para ser liviana, eficiente y fácil de integrar en sistemas Linux como servicio de sistema (`systemd`).

## 🚀 Características

- Comunicación punto a punto usando UDP
- Archivo de configuración en `/etc/udp_vpn/config.cfg`
- Instalación limpia mediante paquete `.deb`
- Servicio gestionado automáticamente por `systemd`
- Proyecto compilable con Makefile

## 📦 Instalación

### Opción A: Desde paquete `.deb`

1. Descarga el paquete desde la sección [Releases](https://github.com/ghgv/udp_vpn/releases)
2. Instala:


sudo dpkg -i udp_vpn_0.5.deb

3. Verifica el servicio

sudo systemctl status vpn

El programa para cliente busca automáticamente el archivo en el cual:

<configuration>
	<server>cloudware.com.co</server>
	<port>5000</port>
	<timer>10</timer>
	<target>10.0.0.1</target>
</configuration>

El server es el dominio al cual se conecta. Port es el puerto del servidor y la IP es la IP del tunel UDP del lado del servidor.


El paquete .deb instala automáticamente el archivo:

/etc/systemd/system/vpn.service

sudo systemctl start vpn       # Inicia el servicio
sudo systemctl stop vpn        # Detiene el servicio
sudo systemctl enable vpn      # Inicia automáticamente al arrancar

🧹 Desinstalación

Solo el binario:

sudo dpkg -r vpn

📁 Estructura del proyecto

.
├── src/                 → Código fuente en C
├── include/             → Archivos de encabezado (.h)
├── bin/                 → Ejecutables generados
├── etc/udp_vpn/         → Configuración por defecto
├── systemd/             → vpn.service
├── Makefile             → Compilación y empaquetado

📄 Licencia
MIT License

🤝 Autor
German Gonzalez – https://github.com/ghgv