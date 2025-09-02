//
// Created by clement.
//
#pragma once
#include <stdio.h>
#include <stdlib.h>

/**
 * \brief Structure representing a node in a linked list.
 *
 * This structure defines a node in a linked list, which contains a pointer to the data
 * and a pointer to the next node in the list.
 */
typedef struct _node {
    void *data; /**< Pointer to the data stored in the node */
    struct _node *next; /**< Pointer to the next node in the list */
} Node, *Chain;

/**
 * \brief Creates a new node with the provided data.
 *
 * This function allocates memory for a new node and initializes its data pointer
 * with the provided value. If memory allocation fails, it returns NULL.
 *
 * \param data Pointer to the data to be stored in the node.
 * \return Pointer to the newly created node, or NULL if memory allocation fails.
 */
Node *createNode(void *data) {
    Node *node = malloc(sizeof(Node));
    if (!node) {
        perror("Memory allocation error");
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    return node;
}

/**
 * \brief Inserts a new node at the head of the chain.
 *
 * This function creates a new node with the provided data and inserts it at the head of the chain.
 * If memory allocation for the new node fails, it returns NULL.
 *
 * \param chain Pointer to the chain where the node will be inserted.
 * \param data Pointer to the data to be stored in the new node.
 * \return Pointer to the newly created node, or NULL if memory allocation fails.
 */
Node *insertHeadNode(Chain *chain, void *data) {
    Node *node = createNode(data);
    if (!node) return NULL;
    node->next = *chain;
    *chain = node;
    return node;
}

/**
 * \brief Inserts an existing node at the head of the chain.
 *
 * This function inserts an existing node at the head of the specified chain.
 * The node's next pointer is updated to point to the current head of the chain,
 * and the chain pointer is updated to point to the new head node.
 *
 * \param chain Pointer to the chain where the node will be inserted.
 * \param node Pointer to the node to be inserted.
 * \return Pointer to the inserted node.
 */
Node *insertNode(Chain *chain, Node *node) {
    node->next = *chain;
    *chain = node;
    return node;
}


/**
 * \brief Inserts a new node at the end of the chain.
 *
 * This function creates a new node with the provided data and inserts it at the end of the chain.
 * If the chain is empty, it inserts the new node at the head of the chain.
 * If memory allocation for the new node fails, it returns NULL.
 *
 * \param chain Pointer to the chain where the node will be inserted.
 * \param data Pointer to the data to be stored in the new node.
 * \return Pointer to the newly created node, or NULL if memory allocation fails.
 */
Node *insertTailNode(Chain *chain, void *data) {
    if (!*chain) {
        return insertHeadNode(chain, data);
    }
    Node *node = createNode(data);
    if (!node) return NULL;
    Node *current = *chain;
    while (current->next) current = current->next;
    current->next = node;
    return node;
}

/**
 * \brief Inserts a new node at a specific position in the chain.
 *
 * This function creates a new node with the provided data and inserts it at the specified position
 * in the chain. If the index is 0, the node is inserted at the head of the chain. If the index is
 * greater than the length of the chain, the node is inserted at the end of the chain.
 *
 * \param chain Pointer to the chain where the node will be inserted.
 * \param data Pointer to the data to be stored in the new node.
 * \param index Position where the new node will be inserted (0 to insert at the head).
 * \return Pointer to the newly created node, or NULL if memory allocation fails or the index is invalid.
 */
Node *insertNodeAt(Chain *chain, void *data, int index) {
    if (index < 0) return NULL;
    if (!index) return insertHeadNode(chain, data);
    Node *current = *chain;
    while (current->next && index--) current = current->next;
    Node *node = createNode(data);
    if (!node) return NULL;
    node->next = current->next;
    current->next = node;
    return node;
}

/**
 * \brief Removes and returns the head node from the chain.
 *
 * This function removes the head node from the specified chain and returns its address.
 * If the chain is empty, it returns NULL.
 *
 * \param chain Pointer to the chain from which the head node will be removed.
 * \return Pointer to the removed node, or NULL if the chain is empty.
 */
Node *popHeadNode(Chain *chain) {
    if (!*chain) return NULL;
    Node *node = *chain;
    *chain = node->next;
    return node;
}

/**
 * \brief Removes and returns the address of the node at the tail of the chain.
 *
 * \param chain Pointer to the chain from which the tail node will be removed.
 * \return Pointer to the removed node, or NULL if the chain is empty.
 */
Node *popTailNode(Chain *chain) {
    if (!*chain) return NULL;
    if (!(*chain)->next) return popHeadNode(chain);
    Node *current = *chain;
    while (current->next->next) current = current->next;
    Node *node = current->next;
    current->next = NULL;
    return node;
}

/**
 * \brief Removes and returns the node at a specific position in the chain.
 *
 * This function removes the node at the specified position in the chain and returns its address.
 * If the index is 0, the head node is removed. If the index is greater than the length of the chain,
 * the function returns NULL.
 *
 * \param chain Pointer to the chain from which the node will be removed.
 * \param index Position of the node to remove (0 to remove the head).
 * \return Pointer to the removed node, or NULL if the chain is empty or the index is invalid.
 */
Node *popNodeAt(Chain *chain, int index) {
    if (index < 0) return NULL;
    if (!index) return popHeadNode(chain);
    Node *current = *chain;
    while (current->next && --index) current = current->next;
    if (!current->next) return NULL;
    Node *node = current->next;
    current->next = node->next;
    return node;
}

/**
 * \brief Frees all nodes in the chain and sets the chain to NULL.
 *
 * This function iterates through the chain, freeing each node's memory.
 * After all nodes are freed, the chain pointer is set to NULL.
 *
 * \param chain Pointer to the chain to be freed.
 */
void freeChain(Chain *chain) {
    Node *current = *chain;
    while (current) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    *chain = NULL;
}

/**
 * \brief Calculates the size of the chain.
 *
 * This function iterates through the chain and counts the number of nodes.
 *
 * \param chain The chain whose size is to be calculated.
 * \return The number of nodes in the chain.
 */
int chainSize(Chain chain) {
    int result = 0;
    while (chain) {
        result++;
        chain = chain->next;
    }
    return result;
}

/**
 * \brief Retrieves the node at a specific position in the chain.
 *
 * This function returns the node at the specified index in the chain.
 * If the index is invalid (negative or out of bounds), it returns NULL.
 *
 * \param chain The chain from which the node will be retrieved.
 * \param index The position of the node to retrieve (0 for the head).
 * \return Pointer to the node at the specified position, or NULL if the index is invalid.
 */
Node *getNodeAt(Chain chain, int index) {
    if (index < 0) return NULL;
    while (chain && index--) chain = chain->next;
    return chain;
}

/**
 * \brief Retrieves a random node from the chain.
 *
 * This function selects a random node from the given chain and returns its address.
 * If the chain is empty, it returns NULL.
 *
 * \param chain The chain from which the random node will be retrieved.
 * \return Pointer to the random node, or NULL if the chain is empty.
 */
Node *getRandomNode(Chain chain) {
    if (!chain) return NULL;
    int index = rand() % chainSize(chain);
    return getNodeAt(chain, index);
}

/**
 * \brief Removes and returns a random node from the chain.
 *
 * This function selects a random node from the given chain, removes it,
 * and returns its address. If the chain is empty, it returns NULL.
 *
 * \param chain Pointer to the chain from which the random node will be removed.
 * \return Pointer to the removed node, or NULL if the chain is empty.
 */
Node *popRandomNode(Chain *chain) {
    if (!*chain) return NULL;
    int index = rand() % chainSize(*chain);
    return popNodeAt(chain, index);
}

/**
 * \brief Moves a node from one chain to another.
 *
 * This function moves a specified node from the source chain to the destination chain.
 * If the node is the head of the source chain, it updates the head pointer.
 * Otherwise, it searches for the node in the source chain, removes it, and inserts it at the head of the destination chain.
 *
 * \param chain1 Pointer to the source chain.
 * \param chain2 Pointer to the destination chain.
 * \param node Pointer to the node to be moved.
 */
void deplaceNode(Chain *chain1, Chain *chain2, Node *node) {
    if (*chain1 == node) {
        Chain current = *chain1;
        *chain1 = current->next;
        current->next = *chain2;
        *chain2 = current;
        return;
    }
    Node *current = *chain1;
    while (current->next && current->next != node) current = current->next;
    if (!current->next) return;
    current->next = node->next;
    node->next = *chain2;
    *chain2 = node;
}

/**
 * \brief Moves all nodes from one chain to another.
 *
 * This function moves all nodes from the source chain to the destination chain.
 * It iterates through the source chain, moving each node to the destination chain
 * until the source chain is empty.
 *
 * \param chain1 Pointer to the source chain.
 * \param chain2 Pointer to the destination chain.
 */
void include(Chain *chain1, Chain *chain2) {
    while (*chain1) {
        deplaceNode(chain1, chain2, *chain1);
    }
}
