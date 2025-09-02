//
// Created by clement on 30/12/24.
//
#pragma once



/**
 * \brief Structure representing an Item.
 *
 * This structure defines the attributes of an item, including its name, attack, defense,
 * maximum health points, resistance, a function pointer for its use, durability, and price.
 */
typedef struct _item {
    char name[50]; /**< Name of the item */
    int att; /**< Attack of the item */
    int def; /**< Defense of the item */
    int hpmax; /**< Maximum health points of the item */
    int rest; /**< Resistance of the item */
    int (*onUse)(void *user, void *ennemy); /**< Function called when the item is used */
    int durability; /**< Durability of the item (negative = infinite) */
    int price; /**< Price of the item */
} Item;



/**
 * \brief Initializes a new item with the provided attributes.
 *
 * This function allocates memory for a new item and initializes its attributes
 * with the provided values. If memory allocation fails, it returns NULL.
 *
 * \param name Name of the item.
 * \param att Attack of the item.
 * \param def Defense of the item.
 * \param hpmax Maximum health points of the item.
 * \param rest Resistance of the item.
 * \param price Price of the item.
 * \return Pointer to the newly created item, or NULL if memory allocation fails.
 */
Item *initItem(char *name, int att, int def, int hpmax, int rest, int price) {
    Item *newItem = malloc(sizeof(Item));
    if (!newItem) {
        perror("Memory allocation error");
        return NULL;
    }
    strcpy(newItem->name, name);
    newItem->att = att;
    newItem->def = def;
    newItem->hpmax = hpmax;
    newItem->rest = rest;
    newItem->price = price;
    return newItem;
}

/**
 * \brief Displays the details of an item.
 *
 * This function prints the attributes of the given item in a formatted table.
 *
 * \param item Pointer to the item to be displayed.
 */
void showItem(Item *item) {
    printf("+-------------------+-----------------+\n");
    printf("| Name              | %-15s |\n", item->name);
    printf("+-------------------+-----------------+\n");
    printf("| Attack            | %-15d |\n", item->att);
    printf("| Defense           | %-15d |\n", item->def);
    printf("| HP                | %-15d |\n", item->hpmax);
    printf("| Resistance        | %-15d |\n", item->rest);
    printf("| Price             | %-15d |\n", item->price);
    printf("+-------------------+-----------------+\n");
}