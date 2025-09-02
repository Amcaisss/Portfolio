//
// Created by clement on 30/12/24.
//
#pragma once


/**
 * \brief Structure representing a Class.
 *
 * This structure defines the attributes of a class, including its name, attack, defense,
 * maximum health points, and resistance.
 */
typedef struct _class
{
    char *name; /**< Name of the class */
    int att; /**< Attack of the class */
    int def; /**< Defense of the class */
    int hpmax; /**< Maximum health points of the class */
    int rest; /**< Resistance of the class */
} Class;

/**
 * \brief Initializes a new class with the provided attributes.
 *
 * This function allocates memory for a new class and initializes its attributes
 * with the provided values. If memory allocation fails, it returns NULL.
 *
 * \param name Name of the class.
 * \param att Attack of the class.
 * \param def Defense of the class.
 * \param hpmax Maximum health points of the class.
 * \param rest Resistance of the class.
 * \return Pointer to the newly created class, or NULL if memory allocation fails.
 */
Class *initClass(char *name, int att, int def, int hpmax, int rest)
{
    Class *newClass = malloc(sizeof(Class));
    if (!newClass)
    {
        perror("Memory allocation error");
        return NULL;
    }
    newClass->name = name;
    newClass->att = att;
    newClass->def = def;
    newClass->hpmax = hpmax;
    newClass->rest = rest;
    return newClass;
}
