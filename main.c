#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct entity {
    char * name;
    struct entity * next;
} Entities;

typedef Entities * entities_pointer;


entities_pointer addent(char const input[], entities_pointer firstEntity);
entities_pointer delent(char const input[], entities_pointer firstEntity);


/**
 * Main method. It initializes the pointer to the beginning of the list only once.
 * Then, it reads an input line and calls the relative method based on what word the line begins with.
 * Eventually, if the line begins with "end", it stops reading and terminates.
 * @return
 */
int main() {
    entities_pointer entitiesList = NULL;                   //pointer to the beginning of the list of entities

    char input[256];                                        //too much: could it be adjusted to the length read?

    do {
        gets(input);                                        //reads the input line

        if(strstr(input, "addent") == input)                //if the command begins with "addent"
            entitiesList = addent(input, entitiesList);
        else if(strstr(input, "delent") == input)           //if the command begins with "delent"
            entitiesList = delent(input, entitiesList);




    } while (input[0] != 'e');                              //maybe check like the others

    return 0;
}


/**
 * Method called when the command "addent" <entity_name> is received.
 * If <entity_name> is not present in the entity list, then the method adds it (initializing the list or adding a new node).
 * If <entity_name> is already present in the entity list, then the method does nothing.
 * @param input string
 * @param firstEntity (pointer to the beginning of the entities list)
 * @return firstEntity, modified if necessary
 */
entities_pointer addent(char const input[], entities_pointer firstEntity) {
    int i = 7;                          //It's the beginning index of the new entity name received
    int nameLength = 0;

    while (input[i] != '\0') {          //To get the size of the new entity name
        nameLength++;
        i++;
    }

    char newEntity[nameLength + 1];     //It will contain the new entity name, plus the '\0'

    i = 7;
    int j = 0;

    while (input[i] != '\0') {          //It initialize newEntity with the name received
        newEntity[j] = input[i];
        i++;
        j++;
    }

    newEntity[j] = input[i];            //Copies the '\0'

    //Now newEntity contains the new entity name, like "diego" (with "" and the final '\0').
    //If there aren't nodes yet, it initializes the first entity, and its next attribute points to NULL.
    //Otherwise, it looks in every node: if the name is not there, ptr points to NULL (end of list), so it creates a new node there.
    //If the name is already there, ptr does not point to NULL (the name is present) so we do nothing.

    if (firstEntity == NULL) {
        firstEntity = (entities_pointer) malloc(sizeof(Entities));
        firstEntity->name = (char *) malloc((nameLength + 1) * sizeof(char));
        strcpy(firstEntity->name, newEntity);
        firstEntity->next = NULL;
    }
    else {
        entities_pointer ptr = firstEntity;
        entities_pointer prec_ptr = firstEntity;
        while (ptr != NULL && strcmp(ptr->name, newEntity) != 0) {
            prec_ptr = ptr;
            ptr = ptr->next;
        }

        if (ptr == NULL) {
            ptr = (entities_pointer) malloc(sizeof(Entities));              //Creates the new node
            prec_ptr->next = ptr;                                           //Links the last existing node to the new one
            ptr->name = (char *) malloc((nameLength + 1) * sizeof(char));
            strcpy(ptr->name, newEntity);
            ptr->next = NULL;
        }

    }

    return firstEntity;
}

/**
 * Method called when the command "delent" <entity_name> is received.
 * If <entity_name> is present in the entity list, then the method deletes the node.
 * If <entity_name> is not present in the entity list (or the list is empty), then the method does nothing.
 * @param input
 * @param firstEntity
 * @return
 */
entities_pointer delent(char const input[], entities_pointer firstEntity) {
    int i = 7;                          //It's the beginning index of the new entity name received
    int nameLength = 0;

    while (input[i] != '\0') {          //To get the size of the new entity name
        nameLength++;
        i++;
    }

    char newEntity[nameLength + 1];     //It will contain the new entity name, plus the '\0'

    i = 7;
    int j = 0;

    while (input[i] != '\0') {          //It initialize newEntity with the name received
        newEntity[j] = input[i];
        i++;
        j++;
    }

    newEntity[j] = input[i];            //Copies the '\0'

    //Now newEntity contains the new entity name, like "diego" (with "" and the final '\0').
    //If the name is not present in the list (so ptr points to NULL), it does nothing.
    //Otherwise, if the name is present (so ptr does not point to NULL), it links the precedent node to the next node,
    //and it deletes the node with the searched name.

    entities_pointer ptr = firstEntity;
    entities_pointer prec_ptr = firstEntity;
    int countNodes = 0;
    while (ptr != NULL && strcmp(ptr->name, newEntity) != 0) {
        prec_ptr = ptr;
        ptr = ptr->next;
        countNodes++;
    }

    if(ptr != NULL && countNodes != 0) {                //If ptr is not NULL and the node to remove is not the first
        prec_ptr->next = ptr->next;
        free(ptr);
    }
    else if(ptr != NULL) {                              //If ptr is not NULL and the node to remove is the first
        firstEntity = ptr->next;                        //The head is now changed: if it was the only node, now it is NULL (empty)
        free(ptr);
    }

    return firstEntity;
}