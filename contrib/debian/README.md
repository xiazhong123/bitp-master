
Debian
====================
This directory contains files used to package bitpcoind/bitpcoin-qt
for Debian-based Linux systems. If you compile bitpcoind/bitpcoin-qt yourself, there are some useful files here.

## bitpcoin: URI support ##


bitpcoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install bitpcoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your bitpcoinqt binary to `/usr/bin`
and the `../../share/pixmaps/bitpcoin128.png` to `/usr/share/pixmaps`

bitpcoin-qt.protocol (KDE)
