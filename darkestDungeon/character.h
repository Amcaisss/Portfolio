//
// Created by clement on 30/12/24.
//
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "class.h"
#include "item.h"

/**
 * \brief Enumeration representing the status of a character.
 *
 * This enumeration defines the possible statuses a character can have.
 */
typedef enum _status {
    NORMAL,   /**< The character is in a normal state. */
    BLOCKING  /**< The character is in a blocking state. */
} Status;


/**
 * \brief Structure representing a character in the game.
 *
 * This structure defines the attributes and items associated with a character.
 */
typedef struct _character {
    char name[50];       /**< Name of the character. */
    Class *class;        /**< Pointer to the character's class. */
    int lvl;             /**< Level of the character. */
    int att;             /**< Attack points of the character. */
    int def;             /**< Defense points of the character. */
    int hp;              /**< Current health points of the character. */
    int hpmax;           /**< Maximum health points of the character. */
    int rest;            /**< Rest points of the character. */
    int str;             /**< Stress level of the character. */
    int courage;         /**< Courage points of the character. */
    int nbfight;         /**< Number of fights the character has participated in. */
    enum _status status; /**< Current status of the character. */
    Item *item1;         /**< Pointer to the first item equipped by the character. */
    Item *item2;         /**< Pointer to the second item equipped by the character. */
} Character;

/**
 * \brief Initializes a new character with the provided name, courage, and class.
 *
 * This function allocates memory for a new character and initializes its attributes
 * based on the provided name, courage, and class. If memory allocation fails, it
 * returns NULL and prints an error message.
 *
 * \param name Pointer to the name of the character.
 * \param courage Courage points of the character.
 * \param class Pointer to the class of the character.
 * \return Pointer to the newly created character, or NULL if memory allocation fails.
 */
Character *initCharacter(char *name, int courage, Class *class) {
    Character *newCharacter = malloc(sizeof(Character));
    if (!newCharacter) {
        perror("Memory allocation error");
        return NULL;
    }
    strcpy(newCharacter->name, name);
    newCharacter->class = class;
    newCharacter->lvl = 1;
    newCharacter->att = class->att;
    newCharacter->def = class->def;
    newCharacter->hp = class->hpmax;
    newCharacter->hpmax = class->hpmax;
    newCharacter->rest = class->rest;
    newCharacter->str = 0;
    newCharacter->courage = courage;
    newCharacter->status = NORMAL;
    newCharacter->nbfight = 0;
    newCharacter->item1 = NULL;
    newCharacter->item2 = NULL;
    return newCharacter;
}


/**
 * \brief Levels up the character and allows the user to distribute points to various stats.
 *
 * This function increments the character's level by one and allows the user to distribute
 * a specified number of points to the character's attack, defense, HP, or rest stats.
 *
 * \param character Pointer to the character to level up.
 * \param points Number of points to distribute.
 */
void levelUp(Character *character, int points) {
    character->lvl++;
    int option = 1;
    int nbpoints = points;
    while (nbpoints) {
        printf("Choose a stat to increase for %s(%d/%d):\n", character->name, nbpoints, points);
        printf("1. Attack\n");
        printf("2. Defense\n");
        printf("3. HP\n");
        printf("4. Rest\n");
        if (!scanf(" %d", &option)) continue;
        if (option > 4 || option < 1) continue;
        switch (option) {
            case 1:
                character->att++;
                break;
            case 2:
                character->def++;
                break;
            case 3:
                character->hp++;
                character->hpmax++;
                break;
            case 4:
                character->rest++;
                break;
            default:
                printf("Invalid choice\n");
                break;
        }
        nbpoints--;
    }
}


/**
 * \brief Marks the character as dead by setting their HP to 0.
 *
 * This function sets the character's HP to 0 and prints a message indicating that the character is dead.
 *
 * \param character Pointer to the character to mark as dead.
 */
void killCharacter(Character *character) {
    printf("%s is dead\n", character->name);
    character->hp = 0;
}

/**
 * \brief Disables the character due to excessive stress.
 *
 * This function sets the character's stress level to a value that exceeds their
 * maximum stress tolerance, effectively disabling them from further actions.
 *
 * \param character Pointer to the character to disable.
 */
void disableCharacter(Character *character) {
    printf("%s is too stressed to continue.\n", character->name);
    character->str = 100 + character->courage;
}

/**
 * \brief Performs a physical attack on the victim character.
 *
 * This function calculates the damage dealt by the attacker to the victim,
 * taking into account the attacker's and victim's items and defense points.
 * If the damage is less than or equal to zero, it is set to one.
 * The victim's HP is reduced by the calculated damage, and if the victim's
 * HP drops to zero or below, the victim is marked as dead.
 *
 * \param attacker Pointer to the attacking character.
 * \param victim Pointer to the victim character.
 */
void physicalAttack(Character *attacker, Character *victim) {
    int damage = attacker->att;
    int def = victim->def;
    if (attacker->item1) {
        damage += attacker->item1->att;
        def += attacker->item1->def;
    }
    if (attacker->item2) {
        damage += attacker->item2->att;
        def += attacker->item2->def;
    }
    damage = (damage - def) * (rand() % 41 + 80)/100;
    if (damage <= 0) {
        damage = 1;
    }
    victim->hp -= damage;
    printf("%s attacks %s for %d damage\n", attacker->name, victim->name, damage);
    if (victim->hp <= 0) {
        killCharacter(victim);
    }
}

/**
 * \brief Performs a psychological attack on the victim character.
 *
 * This function increases the stress level of the victim character based on the attacker's attack points.
 * If the stress level exceeds the victim's maximum stress tolerance, the victim is disabled.
 *
 * \param attacker Pointer to the attacking character.
 * \param victim Pointer to the victim character.
 */
void psychologicalAttack(Character *attacker, Character *victim) {
    int damage = attacker->att;
    if (damage < 0) {
        damage = 1;
    }
    victim->str += damage;
    printf("%s attacks %s for %d stress\n", attacker->name, victim->name, damage);
    if (victim->str >= 100 + victim->courage) {
        disableCharacter(victim);
    }
}

/**
 * \brief Restores the character's HP based on their rest points.
 *
 * This function calculates the total rest points from the character and their items,
 * then adds these points to the character's current HP. If the character's HP exceeds
 * the maximum HP, it is capped at the maximum HP. The character's rest points are then
 * reset to zero. If the character is dead (HP <= 0) or already restored (rest <= 0),
 * the function returns 0 and prints an appropriate message.
 *
 * \param character Pointer to the character to restore.
 * \return 1 if the character's HP was restored, 0 if the character is dead or already restored.
 */
int restaureHP(Character *character) {
    if (character->hp <= 0 || character->rest <= 0) {
        printf("This character is already dead or already restaured\n");
        return 0;
    }
    int restaurePoint = character->rest;
    if (character->item1)
        restaurePoint += character->item1->rest;

    if (character->item2)
        restaurePoint += character->item2->rest;
    character->hp += restaurePoint;
    character->hp = character->hp > character->hpmax ? character->hpmax : character->hp;
    character->rest = 0;
    printf("%s restaures %d hp\n", character->name, restaurePoint);
    return 1;
}

/**
 * \brief Increases the character's defense by 5 points and changes their status to BLOCKING.
 *
 * This function increases the defense points of the specified character by 5 and sets their status to BLOCKING.
 *
 * \param character Pointer to the character whose defense will be increased.
 */
void blockAtt(Character *character) {
    character->def += 5;
    character->status = BLOCKING;
}


/**
 * \brief Displays the character's details.
 *
 * This function prints the details of the specified character, including their name, class, stats,
 * and equipped items. If an item slot is empty, it indicates "No item".
 *
 * \param character Pointer to the character to display.
 */
void showCharacter(Character *character) {
    printf("\n");
    printf("+-------------------+-----------------+\n");
    printf("| Name              | %-15s |\n", character->name);
    printf("+-------------------+-----------------+\n");
    printf("| Class             | %-15s |\n", character->class->name);
    printf("| Attack            | %-15d |\n", character->att);
    printf("| Defense           | %-15d |\n", character->def);
    printf("| HP/HPMax          | %d/%-12d |\n", character->hp, character->hpmax);
    printf("| Rest              | %-15d |\n", character->rest);
    printf("| Stress            | %-15d |\n", character->str);
    printf("| Courage           | %-15d |\n", character->courage);
    printf("| Number of Comb.   | %-15d |\n", character->nbfight);
    printf("+-------------------+-----------------+\n");

    if (character->item1) {
        printf("| Item1             | %-15s |\n", character->item1->name);
        printf("| Item1 Attack      | %-15d |\n", character->item1->att);
        printf("| Item1 Defense     | %-15d |\n", character->item1->def);
        printf("| Item1 HPMax       | %-15d |\n", character->item1->hpmax);
        printf("| Item1 Rest        | %-15d |\n", character->item1->rest);
    } else {
        printf("| Item1             | %-15s |\n", "No item");
    }
    printf("+-------------------+-----------------+\n");

    if (character->item2) {
        printf("| Item2             | %-15s |\n", character->item2->name);
        printf("| Item2 Attack      | %-15d |\n", character->item2->att);
        printf("| Item2 Defense     | %-15d |\n", character->item2->def);
        printf("| Item2 HPMax       | %-15d |\n", character->item2->hpmax);
        printf("| Item2 Rest        | %-15d |\n", character->item2->rest);
    } else {
        printf("| Item2             | %-15s |\n", "No item");
    }
    printf("+-------------------+-----------------+\n");
}