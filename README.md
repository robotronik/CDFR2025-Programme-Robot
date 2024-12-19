# cdfr2025-Programme-Robot

Bienvenue dans le projet cdfr2025-Programme-Robot ! Ce projet vise à développer un programme pour contrôler un robot dans le cadre du CDFR 2025.

## Description

Ce programme est conçu pour permettre au robot d'accomplir différentes tâches, telles que la navigation, la collecte de données, etc. Le code est structuré de manière modulaire et extensible pour faciliter l'ajout de fonctionnalités supplémentaires.

## Fonctionnalités

- **Navigation**: Le robot peut se déplacer dans son environnement en utilisant des algorithmes de navigation.
- **Collecte de données**: Le robot peut collecter et stocker des données provenant de capteurs embarqués.
- **Communication**: Le programme prend en charge la communication avec d'autres systèmes ou dispositifs.

## Prérequis

Avant d'exécuter le programme, assurez-vous d'avoir installé les dépendances suivantes :

```bash
sudo apt-get install make gcc g++ libi2c-dev python3-venv libasio-dev nlohmann-json3-dev gdb gdb-multiarch
```

Les dépendances requises pour compiler sur arm (RasbPi)

```bash
sudo apt-get install g++-aarch64-linux-gnu gcc-aarch64-linux-gnu
sudo dpkg --add-architecture arm64
sudo apt-get update
sudo apt remove libi2c-dev
wget http://ports.ubuntu.com/pool/universe/i/i2c-tools/libi2c-dev_4.3-4_arm64.deb
sudo dpkg --force architecture --force-depends --install ./libi2c-dev_4.3-4_arm64.deb
sudo apt-get install -f
sudo apt-get install libi2c-dev:arm64 libasio-dev:arm64 nlohmann-json3-dev:arm64
dpkg -L libi2c-dev
```

Package config path should be : (Make)
```bash
PKG_CONFIG_PATH = /usr/lib/aarch64-linux-gnu/pkgconfig
# Use pkg-config to set flags
CFLAGS += $(shell pkg-config --cflags libi2c)
LDFLAGS += $(shell pkg-config --libs libi2c)
```

## Installation

1. Ne clonez pas ce dépôt seul !! Clonez le dépôt CDFR : 
```bash
git clone git@github.com:robotronik/CDFR.git --recursive
```
2. puis rendez vous ici dans CDFR2025

```bash
cd informatique/CDFR2025-Programme-Robot/ 
```
3. Pour finir, vous pouvez passer sur la branche main et pull avant de travailler pour vous mettre à jour.
```bash
git checkout main
git pull 
```

## Compilation

Simplement lancer la commande pour compiler le programme

```bash
make
```

Pour lancer les tests faire

```bash
make tests
```

Et finalement, pour nettoyer faire

```bash
make clean
```

## Compilation pour RasbPi

Assurez vous d'avoir les dépendances requieses pour compiler sur arm.
Simplement lancer la commande pour compiler le programme pour ARM et l'installer sur le RasbPi.
Pour se connecter en ssh au RasbPi sans avoir à utiliser de mot de passe, utiliser
ssh-copy-id pi@192.168.1.47

```bash
make deploy
```

Pour nettoyer faire

```bash
make clean-all
```


## Debugging on a Raspberry Pi with VS Code

Connect your PC to the same Wi-Fi as the Raspberry Pi.
Press F5 in VS Code to start debugging.
You can now set breakpoints and use all the debugging tools available in VS Code for remote debugging on the Raspberry Pi.

## Accessibilité au site (restAPI)

Soyez sur le même internet local que le robot.
Le programme doit être lancer depis n'importe quel pc.
Allez sur cette adresse ds un navigateur : http://raspberrypi.local:8080

## Ecran tactile sur le robot

Lancer les commandes pour démmarer chromium sur la page locale du robot
```bash
export DISPLAY=:0
sudo startx /usr/bin/chromium-browser --noerrdialogs --kiosk http:0.0.0.0:8080/robot --incognito --disable-extensions --no-sandbox
```
