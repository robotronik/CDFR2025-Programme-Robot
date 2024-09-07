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
sudo apt-get install make gcc g++ libi2c-dev python3-venv
```
Et la bibliothèque [pigpio](https://abyz.me.uk/rpi/pigpio/index.html)


## Installation

1. Clonez le dépôt sur votre machine locale avec votre clée SSH

   ```bash
   git clone git@github.com:robotronik/CDFR2025-Programme-Robot.git --recursive
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