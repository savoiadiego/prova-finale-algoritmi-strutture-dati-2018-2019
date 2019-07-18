#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct entity {
    char * name;
    struct entity * next;
} Entities;

typedef Entities * entities_pointer;


void addent(char const input[], entities_pointer firstEntity);



int main() {

    entities_pointer entitiesList = NULL;          //pointer to the beginning of the list of entities


    char input[256];                    //too much: could it be adjusted to the length read?

    do {

        gets(input);                    //reads the input line

        if(strstr(input, "addent"))
            addent(input, entitiesList);




    } while (input[0] != 'e');

    return 0;
}


void addent(char const input[], entities_pointer firstEntity) {
    int i = 7;                      //it's the beginning of the new entity name
    int nameLength = 0;

    while (input[i] != '\0') {       //to get the size of the new entity name
        nameLength++;
        i++;
    }

    char newEntity[nameLength + 1];     //it will contain the new entity name, plus the '\0'

    i = 7;
    int j = 0;

    while (input[i] != '\0') {
        newEntity[j] = input[i];
        i++;
        j++;
    }

    newEntity[j] = input[i];        //copies the '\0'

    //now newEntity contains the new entity name, like "diego" (with "")
    //if there aren't nodes, we initialize the first entity, and its next points to NULL
    //otherwise, we look in every node: if the name is not there, ptr points to NULL (end of list), so we create a new node there
    //if the name is already there, ptr does not point to NULL (the name is present) so we do nothing.

    if (firstEntity == NULL) {
        firstEntity = (entities_pointer) malloc(sizeof(Entities));
        firstEntity->name = (char *) malloc((nameLength + 1) * sizeof(char));
        strcpy(firstEntity->name, newEntity);
    } else {
        entities_pointer ptr = firstEntity;
        entities_pointer prec_ptr = firstEntity;
        while (ptr != NULL && strcmp(ptr->name, newEntity) != 0) {
            prec_ptr = ptr;
            ptr = ptr->next;
        }

        if (ptr == NULL) {
            ptr = (entities_pointer) malloc(sizeof(Entities));          //creates the new node
            prec_ptr->next = ptr;                                       //link the last existing node to the new one
            ptr->name = (char *) malloc((nameLength + 1) * sizeof(char));
            strcpy(ptr->name, newEntity);
        }

    }
}