//
// Created by clement on 04/01/25.
//
#include <stdio.h>
#include "character.h"
#include "chain.h"
#include <time.h>


/**
 * \brief Initializes the chain of characters with predefined characters and their classes.
 *
 * This function inserts a set of predefined classes into the given class chain and then
 * creates characters associated with these classes, adding them to the character chain.
 *
 * \param characterChain Pointer to the chain of characters to initialize.
 * \param classChain The chain of classes to use for character creation.
 */
void initCharactersChain(Chain *characterChain, Chain classChain) {
    Class *warrior = insertTailNode(&classChain, initClass("Warrior", 8, 6, 24, 6))->data;
    Class *mage = insertTailNode(&classChain, initClass("Mage", 6, 4, 22, 8))->data;
    Class *archer = insertTailNode(&classChain, initClass("Archer", 7, 5, 23, 7))->data;
    Class *assassin = insertTailNode(&classChain, initClass("Assassin", 8, 4, 20, 8))->data;
    Class *tank = insertTailNode(&classChain, initClass("Tank", 7, 9, 28, 6))->data;

    /* Create characters */
    insertTailNode(characterChain, initCharacter("Thorin", 15, warrior));
    insertTailNode(characterChain, initCharacter("Aragorn", 20, warrior));
    insertTailNode(characterChain, initCharacter("Boromir", 13, warrior));
    insertTailNode(characterChain, initCharacter("Gandalf", 10, mage));
    insertTailNode(characterChain, initCharacter("Merlin", 11, mage));
    insertTailNode(characterChain, initCharacter("Saruman", 9, mage));
    insertTailNode(characterChain, initCharacter("Legolas", 0, archer));
    insertTailNode(characterChain, initCharacter("Robin", 12, archer));
    insertTailNode(characterChain, initCharacter("Hawkeye", 5, archer));
    insertTailNode(characterChain, initCharacter("Ezio", 10, assassin));
    insertTailNode(characterChain, initCharacter("Altair", 11, assassin));
    insertTailNode(characterChain, initCharacter("Connor", 3, assassin));
    insertTailNode(characterChain, initCharacter("Garen", 5, tank));
    insertTailNode(characterChain, initCharacter("Braum", 8, tank));
    insertTailNode(characterChain, initCharacter("Darius", 15, tank));
    insertTailNode(characterChain, initCharacter("Lancelot", 18, warrior));
    insertTailNode(characterChain, initCharacter("Morgana", 14, mage));
    insertTailNode(characterChain, initCharacter("Katniss", 7, archer));
    insertTailNode(characterChain, initCharacter("Aveline", 9, assassin));
    insertTailNode(characterChain, initCharacter("Leona", 12, tank));
}



/**
 * \brief Initializes the chain of items with predefined items.
 *
 * This function inserts a set of predefined items into the given chain.
 *
 * \param items Pointer to the chain of items to initialize.
 */
void initItemsChain(Chain *items) {
    insertHeadNode(items,
                   initItem("Excalibur", 6, 0, 0, 0, 7));
    insertHeadNode(items,
                   initItem("Ritta", 10, -3, 0, 0, 7));
    insertHeadNode(items,
                   initItem("Sixth Eye", 0, 0, 3, 3, 7));
    insertHeadNode(items,
                   initItem("Dagger", 4, 0, 0, 0, 7));
    insertHeadNode(items,
                   initItem("Shield", 0, 5, 0, 0, 7));

    insertHeadNode(items,
                   initItem("Magic Wand", 8, 0, 0, 0, 14));
    insertHeadNode(items,
                   initItem("Battle Axe", 12, -2, 0, 0, 14));
    insertHeadNode(items,
                   initItem("Dragon Armor", 0, 8, 0, 0, 14));
    insertHeadNode(items,
                   initItem("Healing Potion", 0, 0, 5, 0, 14));
    insertHeadNode(items,
                   initItem("Boots of Speed", 0, 0, 0, 5, 14));

    insertHeadNode(items,
                   initItem("Sword of Destiny", 15, 0, 0, 0, 21));
    insertHeadNode(items,
                   initItem("Hammer of Thor", 18, -5, 0, 0, 21));
    insertHeadNode(items,
                   initItem("Phoenix Feather", 0, 0, 10, 0, 21));
    insertHeadNode(items,
                   initItem("Cloak of Invisibility", 0, 0, 0, 10, 21));
    insertHeadNode(items,
                   initItem("Ring of Power", 10, 10, 0, 0, 21));
}


/**
 * \brief Prints the details of all characters in the chain.
 *
 * This function iterates through the character chain and prints the details
 * of each character, including their index in the chain.
 *
 * \param characterChain The chain of characters to print.
 */
void printCharactersDetails(Chain characterChain) {
    int index = 0;
    while (characterChain) {
        Character *character = characterChain->data;
        printf("%d.", index);
        showCharacter(character);
        characterChain = characterChain->next;
        printf("\n");
        index++;
    }
    printf("\n");
}

/**
 * \brief Prints the names of characters in the chain.
 *
 * This function iterates through the character chain and prints the index and name of each character.
 *
 * \param characterChain The chain of characters to print.
 */
void printChainOfStructsWithName(Chain characterChain) {
    int index = 0;
    while (characterChain) {
        Character *character = characterChain->data;
        printf("%d. %s", index, character->name);
        characterChain = characterChain->next;
        index++;
        printf("\n");
    }
}

/**
 * \brief Allows the player to choose a character from the chain.
 *
 * This function prints the details of all available characters and prompts the player
 * to enter the index of the character they want to choose. The chosen character is then
 * removed from the chain and returned.
 *
 * \param characterChain Pointer to the chain of available characters.
 * \return Pointer to the node of the chosen character, or NULL if no characters are available.
 */
Node *choiceCharacter(Chain *characterChain) {
    if (!(*characterChain)) {
        printf("No more characters available\n");
        return NULL;
    }
    printCharactersDetails(*characterChain);
    printf("Enter the index of the character you want to choose : \n");
    int index;
    while (!scanf(" %d", &index) || index < 0 || index >= chainSize(*characterChain)) {
        printf("Invalid index, try again !\n");
        while (getchar() != '\n');
    }
    Node *characterNode = popNodeAt(characterChain, index);
    return characterNode;
}

/**
 * \brief Randomly selects a specified number of characters from the character chain and adds them to a new chain.
 *
 * \param newChain Pointer to the chain where selected characters will be added.
 * \param characterChain Pointer to the chain of available characters.
 * \param nbCharacter Number of characters to select.
 */
void randomCharacter(Chain *newChain, Chain *characterChain, int nbCharacter) {
    for (; nbCharacter; nbCharacter--) {
        Node *node = popRandomNode(characterChain);
        insertTailNode(newChain, node->data);
    }
}

/**
 * \brief Allows the player to choose an action for their character during a fight.
 *
 * This function prompts the player to select an action for their character to perform
 * during a fight. The available actions include performing a physical attack, restoring HP,
 * blocking an attack, or viewing the character's details.
 *
 * \param character Pointer to the player's character.
 * \param ennemy Pointer to the enemy character.
 */
void choseActionFight(Character *character, Character *ennemy) {
    printf("What do you want to do with %s ?\n", character->name);
    printf("[1] Physical Attack\n");
    printf("[2] Restaure HP\n");
    printf("[3] Block Attack\n");
    printf("[4] Resume\n");
    int choice;
    while (scanf(" %d", &choice) != 1 || choice < 1 || choice > 5) {
        printf("Invalid choice, try again\n");
        while (getchar() != '\n');
    }
    switch (choice) {
        case 1:
            physicalAttack(character, ennemy);
            break;
        case 2:
            restaureHP(character);
            break;
        case 3:
            blockAtt(character);
            break;
        case 4:
            showCharacter(character);
            choseActionFight(character, ennemy);
            break;
        default:
            printf("Invalid choice\n");
            choseActionFight(character, ennemy);
    }
}


/**
 * \brief Automatically selects and performs an action for the enemy character during a fight.
 *
 * This function determines the most suitable action for the enemy character to take based on the current state of the fight.
 * It prioritizes attacking the player character with the highest priority, which is calculated based on the difference between
 * the character's maximum HP and current HP, as well as their stress level.
 *
 * \param playerChain Pointer to the chain of player characters.
 * \param ennemy Pointer to the enemy character.
 */
void autoActionFight(Chain *playerChain, Character *ennemy) {
    if (ennemy->hp <= 0) {
        printf("This ennemy is already dead\n");
        return;
    }
    int targetIndex = -1;
    int maxPriority = targetIndex;
    for (int i = 0; i < chainSize(*playerChain); i++) {
        Character *character = getNodeAt(*playerChain, i)->data;
        if (character->hp <= 0) {
            continue;
        }
        int priotity = (character->hpmax - character->hp) + character->str;
        if (priotity > maxPriority) {
            maxPriority = priotity;
            targetIndex = i;
        }
    }

    if (targetIndex == -1) {
        printf("All characters are dead or invalid\n");
        return;
    }

    Character *target = getNodeAt(*playerChain, targetIndex)->data;

    if (ennemy->hp < ennemy->hpmax / 3 && ennemy->rest > 0) {
        if (rand() % 2)
            restaureHP(ennemy);
        else
            blockAtt(ennemy);
    } else if (target->str >= 50) {
        psychologicalAttack(ennemy, target);
    } else {
        physicalAttack(ennemy, target);
    }
}

/**
 * \brief Checks if any character in the player's chain is still enabled (alive).
 *
 * This function iterates through the player's chain of characters and checks if any character
 * has HP greater than 0, indicating that they are still alive and able to fight.
 *
 * \param playerChain The chain of player characters to check.
 * \return 1 if at least one character is alive, 0 if all characters are incapacitated.
 */
int teamIsEnable(Chain playerChain) {
    while (playerChain) {
        Character *character = playerChain->data;
        if (character->hp > 0) {
            return 1;
        }
        playerChain = playerChain->next;
    }
    return 0;
}

/**
 * \brief Conducts a fight between the player's team and an enemy character.
 *
 * This function manages the fight loop where the player's team and the enemy character take turns
 * to perform actions until either the enemy is defeated or all player characters are incapacitated.
 *
 * \param playerChain Pointer to the chain of player characters.
 * \param ennemy Pointer to the enemy character.
 * \return 1 if the player wins the fight, 0 if the player loses.
 */
int fight(Chain *playerChain, Character *ennemy) {
    while (ennemy->hp > 0 && teamIsEnable(*playerChain)) {
        Chain playerChainCopy = *playerChain;
        autoActionFight(playerChain, ennemy);
        while (playerChainCopy) {
            Character *character = playerChainCopy->data;
            playerChainCopy = playerChainCopy->next;
            if (character->hp <= 0 || character->str >= 100 + character->courage) continue;

            choseActionFight(character, ennemy);
        }
    }
    if (ennemy->hp <= 0) {
        printf("You win the fight with %s.\n", ennemy->name);
        free(ennemy);
        return 1;
    }
    printf("You lose the fight with %s.\n", ennemy->name);
    return 0;
}

/**
 * \brief Selects characters for the fight.
 *
 * This function allows the player to add characters to the fight chain.
 * The player is prompted to add characters until they choose to stop or
 * until at least one character is added.
 *
 * \param newChain Pointer to the chain where selected characters will be added.
 * \param allCharacters Pointer to the chain of all available characters.
 */
void selectFightCharacters(Chain *newChain, Chain *allCharacters) {
    int count = 0;
    char c;
    do {
        printf("Do you want to add a character? (Y/N)\n");
        if (!scanf(" %c", &c)) continue;
        if (c != 'N' && c != 'n' && (c != 'Y' && c != 'y')) {
            printf("Invalid choice\n");
            continue;
        }
        if (c == 'y' || c == 'Y') {
            Node *character = choiceCharacter(allCharacters);
            if (!character || ((Character *) character->data)->hp <= 0) {
                return;
            }
            insertTailNode(newChain, character->data);
            count++;
        }
    } while ((c != 'N' && c != 'n') || !count);
}


/**
 * \brief Manages the changes in the sanitarium, allowing the player to add or remove characters.
 *
 * This function allows the player to remove characters from the sanitarium and add characters to the sanitarium.
 * The player can remove characters from the sanitarium if there are any characters in the sanitarium.
 * The player can add characters to the sanitarium if there are at least 2 characters in the player's chain
 * and less than 2 characters in the sanitarium.
 *
 * \param playerCharacters Pointer to the chain of player characters.
 * \param sanitarium Pointer to the chain of characters in the sanitarium.
 */
void sanitariumChanges(Chain *playerCharacters, Chain *sanitarium) {
    if (!*playerCharacters) {
        return;
    }
    printCharactersDetails(*sanitarium);
    char c;
    if (*sanitarium) {
        while (c != 'N' && c != 'n') {
            printf("Do you want to remove a character from the sanitarium? (Y/N)\n");
            while (!scanf(" %c", &c)) {
                printf("Invalid choice\n");
                while (getchar() != '\n');
            }
            if (c == 'y' || c == 'Y') {
                Node *remove = choiceCharacter(sanitarium);
                insertNode(playerCharacters, remove);
            }
        }
    }
    c = 0;
    while (c != 'N' && c != 'n' && chainSize(*playerCharacters) >= 2 &&
           chainSize(*sanitarium) < 2) {
        printf("Do you want to add a character to the sanitarium? (Y/N)\n");
        while (!scanf(" %c", &c)) {
            printf("Invalid choice\n");
            while (getchar() != '\n');
        }
        if (c == 'y' || c == 'Y') {
            Node *remove = choiceCharacter(playerCharacters);
            insertNode(sanitarium, remove);
        }
    }
}

/**
 * \brief Advances the game to the next turn.
 *
 * This function updates the status of each character in the fighters chain,
 * including increasing their stress, incrementing their number of fights,
 * and leveling them up if they are still alive. It also resets the defense
 * of characters who were blocking in the previous turn and adds coins to the player's total.
 *
 * \param fighters Pointer to the chain of characters currently fighting.
 * \param coins Pointer to the player's coin count.
 */
void nextTurn(const Chain *fighters, int *coins) {
    while (*fighters) {
        Character *character = (*fighters)->data;
        character->str = character->str + 25 >= 100 + character->courage
                             ? 100 + character->courage
                             : character->str + 25;
        character->nbfight += 1;
        if (character->status == BLOCKING) {
            character->def -= 5;
            character->status = NORMAL;
        }
        if (character->hp > 0)
            levelUp(character, 6);

        fighters = &(*fighters)->next;
    }
    *coins += 10;
}

/**
 * \brief Manages the changes in the tavern, allowing the player to add or remove characters.
 *
 * This function allows the player to remove characters from the tavern and add characters to the tavern.
 * The player can remove characters from the tavern if there are any characters in the tavern.
 * The player can add characters to the tavern if there are at least 2 characters in the player's chain
 * and less than 2 characters in the tavern.
 *
 * \param playerCharacters Pointer to the chain of player characters.
 * \param tavern Pointer to the chain of characters in the tavern.
 */
void tavernChanges(Chain *playerCharacters, Chain *tavern) {
    if (!*playerCharacters) return;
    char c;
    while (c != 'N' && c != 'n' && *tavern) {
        printf("Do you want to remove a character from the tavern? (Y/N)\n");
        while (!scanf(" %c", &c)) {
            printf("Invalid choice\n");
            while (getchar() != '\n');
        }
        if (c == 'y' || c == 'Y') {
            Node *remove = choiceCharacter(tavern);
            insertNode(playerCharacters, remove);
        }
    }
    c = 0;
    while (c != 'N' && c != 'n' && chainSize(*playerCharacters) >= 2 &&
           chainSize(*tavern) < 2) {
        printf("Do you want to add a character to the tavern? (Y/N)\n");
        while (!scanf(" %c", &c)) {
            printf("Invalid choice\n");
            while (getchar() != '\n');
        }
        if (c == 'y' || c == 'Y') {
            Node *remove = choiceCharacter(playerCharacters);
            insertNode(tavern, remove);
        }
    }
}


/**
 * \brief Reduces stress and increases rest points for characters in the tavern.
 *
 * This function iterates through the tavern chain and reduces the stress of each character by 25 points,
 * ensuring that stress does not go below 0. It also increases the rest points by 5.
 *
 * \param tavern Pointer to the chain of characters in the tavern.
 */
void tavernAction(const Chain *tavern) {
    while (*tavern) {
        Character *character = (*tavern)->data;
        character->str = character->str - 25 < 0 ? 0 : character->str - 25;
        character->rest += 5;
        tavern = &(*tavern)->next;
    }
}

/**
 * \brief Restores HP and rest points for characters in the sanitarium.
 *
 * This function iterates through the sanitarium chain and restores HP and rest points
 * for each character. HP is increased by 7 points, but not exceeding the maximum HP.
 * Rest points are also increased by 7.
 *
 * \param sanitarium Pointer to the chain of characters in the sanitarium.
 */
void sanitariumAction(const Chain *sanitarium) {
    while (*sanitarium) {
        Character *character = (*sanitarium)->data;
        character->hp = character->hp + 7 > character->hpmax ? character->hpmax : character->hp + 7;
        character->rest += 7;
        sanitarium = &(*sanitarium)->next;
    }
}

/**
 * \brief Displays the items available in the market.
 *
 * This function iterates through the market chain and prints the details of each item.
 *
 * \param market Pointer to the chain of items available in the market.
 */
void displayMarket(Chain *market) {
    Node *current = *market;
    int index = 1;

    printf("\n--- Market Items ---\n");
    while (current != NULL) {
        Item *item = current->data;
        printf("%d. ", index);
        showItem(item);
        current = current->next;
        index++;
    }
}

/**
 * \brief Assigns an item to a character chosen by the player.
 *
 * \param characters Pointer to the chain of player characters.
 * \param item Pointer to the item to be assigned.
 */
void assignItemToCharacter(Chain *characters, Item *item) {
    Node *current = choiceCharacter(characters);
    insertNode(characters, current);

    if (!current) {
        printf("Invalid choice. Returning to the market.\n");
        return;
    }

    // Assign the item to the selected character
    Character *selectedCharacter = current->data;
    if (!selectedCharacter->item1) {
        selectedCharacter->item1 = item;
        printf("Assigned %s to %s.\n", item->name, selectedCharacter->name);
    } else if (selectedCharacter->item2 == NULL) {
        selectedCharacter->item2 = item;
        printf("Assigned %s to %s.\n", item->name, selectedCharacter->name);
    } else {
        printf("%s already has two items equipped. Cannot assign more items.\n", selectedCharacter->name);
    }
}

/**
 * \brief Purchases an item from the market and assigns it to a character.
 *
 * \param market Pointer to the chain of items available in the market.
 * \param playerGold Pointer to the player's gold count.
 * \param characters Pointer to the chain of player characters.
 */
void purchaseItem(Chain *market, int *playerGold, Chain *characters) {
    int choice;
    displayMarket(market);

    printf("\nYou have %d gold.\n", *playerGold);
    printf("Enter the number of the item you wish to purchase (or 0 to exit): ");

    if (scanf("%d", &choice) != 1 || choice < 0) {
        printf("Invalid input. Exiting the market.\n");
        while (getchar() != '\n'); // Clear input buffer
        return;
    }

    if (choice == 0) {
        printf("Exiting the market.\n");
        return;
    }

    Node *current = *market;
    int index = 1;

    // Traverse the list to find the chosen item
    while (current && index < choice) {
        current = current->next;
        index++;
    }

    if (!current) {
        printf("Invalid choice. Please try again.\n");
        return;
    }

    // Retrieve the selected item
    Item *selectedItem = current->data;

    // Check if the player can afford the item
    if (*playerGold >= selectedItem->price) {
        *playerGold -= selectedItem->price;
        printf("You purchased %s for %d gold!\n", selectedItem->name, selectedItem->price);

        // Remove the item from the market
        popNodeAt(market, choice - 1);

        // Assign the item to a character
        assignItemToCharacter(characters, selectedItem);
    } else {
        printf("You don't have enough gold to purchase this item.\n");
    }
}

/**
 * \brief Handles market interactions, allowing the player to view items, purchase items, or exit the market.
 *
 * \param market Pointer to the chain of items available in the market.
 * \param playerGold Pointer to the player's gold count.
 * \param characters Pointer to the chain of player characters.
 */
void market(Chain *market, int *playerGold, Chain *characters) {
    int choice;

    while (1) {
        printf("\n--- Market Menu ---\n");
        printf("1. View items\n");
        printf("2. Purchase an item\n");
        printf("3. Exit the market\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting the market.\n");
            while (getchar() != '\n'); // Clear input buffer
            break;
        }

        switch (choice) {
            case 1:
                displayMarket(market);
                break;
            case 2:
                purchaseItem(market, playerGold, characters);
                break;
            case 3:
                printf("Thank you for visiting the market!\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}


/**
 * \brief Starts the game by initializing characters, selecting fighters, and managing the game loop.
 *
 * This function sets up the game by randomly selecting enemy characters, allowing the player to choose their characters,
 * and then managing the game loop where fights occur, characters level up, and various actions are taken in the sanitarium,
 * tavern, and market.
 *
 * \param playerCharacters Pointer to the chain of player characters.
 * \param ennemyCharacters Pointer to the chain of enemy characters.
 * \param allCharacters Pointer to the chain of all available characters.
 * \param items Pointer to the chain of items available in the market.
 * \param tavern Pointer to the chain of characters in the tavern.
 * \param sanitarium Pointer to the chain of characters in the sanitarium.
 * \param coins Pointer to the player's coin count.
 */
void startGame(Chain *playerCharacters, Chain *ennemyCharacters, Chain *allCharacters, Chain *items, Chain *tavern,
               Chain *sanitarium, int *coins) {
    // Randomly select 5 enemy characters
    randomCharacter(ennemyCharacters, allCharacters, 10);
    printf("You will have to choose 2 characters to fight the enemies.\n");

    // Allow the player to choose 2 characters
    for (int i = 0; i < 2; i++) {
        Node *player = choiceCharacter(allCharacters);
        if (!player) perror("Doesn't have enough characters to play");
        insertTailNode(playerCharacters, player->data);
    }

    printf("Your team is:\n");
    printCharactersDetails(*playerCharacters);

    // Main game loop
    for (int i = 0; *ennemyCharacters && *playerCharacters; *ennemyCharacters = (*ennemyCharacters)->next, i++) {
        Character *ennemy = (*ennemyCharacters)->data;
        Chain fightCharacters = NULL;
        printf("Now, you will fight %s.\n", ennemy->name);
        printf("Choose characters to fight (max 3):\n");

        // Select characters for the fight
        selectFightCharacters(&fightCharacters, playerCharacters);

        // Conduct the fight
        int hasWin = fight(&fightCharacters, ennemy);
        if (!hasWin) {
            printf("You lose the game...\n");
            return;
        }

        // Proceed to the next turn
        nextTurn(&fightCharacters, coins);
        include(&fightCharacters, playerCharacters);

        // Add a new character to the team every 2 fights
        if (!(i % 2) && i >= 2) {
            printf("A new character wants to join your team.\n");
            insertNode(playerCharacters, choiceCharacter(allCharacters));
        }

        // Perform actions in the sanitarium and tavern
        sanitariumAction(sanitarium);
        tavernAction(tavern);
        sanitariumChanges(playerCharacters, sanitarium);
        tavernChanges(playerCharacters, tavern);

        // Visit the market
        market(items, coins, playerCharacters);
        i++;
    }

    printf("You win.\n");
}


int main(void) {
    //init
    srand(time(NULL));
    Chain allCharacters = NULL;
    Chain playerCharacters = NULL;
    Chain ennemyCharacters = NULL;
    Chain items = NULL;
    Chain tavern = NULL;
    Chain sanitarium = NULL;
    Chain classChain = NULL;
    int coins = 0;
    initCharactersChain(&allCharacters, classChain);
    initItemsChain(&items);

    //game
    startGame(&playerCharacters, &ennemyCharacters, &allCharacters, &items, &tavern, &sanitarium, &coins);
    //Free


    freeChain(&items);
    freeChain(&allCharacters);
    freeChain(&classChain);
    freeChain(&playerCharacters);
    freeChain(&playerCharacters);
    freeChain(&tavern);
    freeChain(&sanitarium);
    return 0;
}
