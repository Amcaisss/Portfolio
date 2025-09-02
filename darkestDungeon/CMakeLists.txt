cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(CharacterProject C)

# Définir le standard C
set(CMAKE_C_COMPILER "/usr/bin/gcc")

# Ajouter les fichiers sources
add_executable(CharacterProject character.c chain_test.c main.c
        main.c)

# Inclure les répertoires d'en-têtes
include_directories(${PROJECT_SOURCE_DIR})