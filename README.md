# CDFR2025-Programme-Robot

Bienvenue dans le projet CDFR2025-Programme-Robot ! Ce projet vise à développer un programme pour contrôler un robot dans le cadre du CDFR 2025.

## Description

Ce programme est conçu pour permettre au robot d'accomplir différentes tâches, telles que la navigation, la collecte de données, etc. Le code est structuré de manière modulaire et extensible pour faciliter l'ajout de fonctionnalités supplémentaires.

## Fonctionnalités

- **Navigation**: Le robot peut se déplacer dans son environnement en utilisant des algorithmes de navigation.
- **Collecte de données**: Le robot peut collecter et stocker des données provenant de capteurs embarqués.
- **Communication**: Le programme prend en charge la communication avec d'autres systèmes ou dispositifs.

## Prérequis

Avant d'exécuter le programme, assurez-vous d'avoir installé les dépendances suivantes :

```bash
sudo apt-get install make gcc g++ python3-venv libasio-dev
```

Les dépendances supplémentaires requises pour compiler sur arm (RasbPi)

```bash
sudo apt-get install g++-aarch64-linux-gnu
```
no need to install libi2c-dev_4.3-2

Pour debug

```bash
sudo apt install gdbserver
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

Assurez vous d'avoir les dépendances requises pour compiler sur arm.
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

Sur un nouveau Raspi, il faut configurer avec
sudo raspi-config
et activer l'I2C et la communicaton série dans Interface Options

Pour voir le service (useful commands)
```bash
journalctl -b -u programCDFR
journalctl -u programCDFR -f

systemctl list-units --type=service

sudo systemctl daemon-reload
sudo systemctl restart programCDFR
```

## Debugging on a Raspberry Pi with VS Code

Connect your PC to the same Wi-Fi as the Raspberry Pi.
Change the address in launch.json and task.json to match the robot address.
Press F5 in VS Code to start debugging.
You can now set breakpoints and use all the debugging tools available in VS Code for remote debugging on the Raspberry Pi.

## Accessibilité au site (restAPI)

Soyez sur le même internet local que le robot. Le programme doit être lancé.
Allez sur cette adresse dans un navigateur : http://raspitronik.local:8080

## Ecran tactile sur le robot

Lancer les commandes pour démmarer chromium sur la page locale du robot
```bash
sudo apt-get install xorg openbox chromium-browser
sudo apt install xorg openbox -y
export DISPLAY=:0
sudo startx /usr/bin/chromium-browser --noerrdialogs --kiosk http:0.0.0.0:8080/robot --incognito --disable-extensions --no-sandbox
ou
/usr/bin/chromium-browser --kiosk http:0.0.0.0:8080/robot --incognito --disable-extensions
```

Configuration de lecran long
```bash
sudo nano /boot/firmware/config.txt
```
et mettre 
```bash
dtoverlay=vc4-kms-dsi-waveshare-panel,8_8_inch
```


If you’re running the Raspberry Pi OS with its default desktop, you can add your command to the autostart file so it launches once the X session starts.

Edit the Autostart File:
Open (or create if it doesn’t exist) the file:

```bash
/home/pi/.config/lxsession/LXDE-pi/autostart
```
Add Your Command:

```bash
@/usr/bin/chromium-browser --kiosk http://0.0.0.0:8080/robot --incognito --disable-extensions
```
Save and Reboot

## Actions and Actuators

(in code, theyre called banner, stocks, columns, platforms and tribunes)

Defenies dans constante.h
- Stepper 1 - Elevateur à planches
- Stepper 2 - Elevateur à étages
- Stepper 3 - Rail du bas
- Stepper 4 - Rail du haut
- Servo 1 - Push étage tribune
- Servo 2 - Souleveur à planches gauche
- Servo 3 - Souleveur à planches droite
- Servo 4 - Pinces
- servo 5 - Banière

## RGB Light signals

**SOLID:**  
- 🟢 Green: The robot has finished the match  

**BLINKING:**  
- 🔴 Red: The program has failed. Restart the robot.  
- 🔵 Blue: The robot is ready to start as blue  
- 🟡 Yellow: The robot is ready to start as yellow  
- 🟣 Purple: The robot is in manual control mode  

**RAINBOW:**  
- 🌈 The robot is waiting for user input

## Check List Match
- Position robot
- Ascensseur position basse
- Ascensseur pince position basse
- Couleur choisie (vérfie on live table)
- Stratégie choisie
- Prêt démarrage