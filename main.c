#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct origin {
    char * name;
    struct origin * next;
} Origins;

typedef struct relation {
    char * name;
    struct origin * origins;
    struct relation * next;
} Relations;

typedef struct entity {
    char * name;
    struct relation * relations;
    struct entity * next;
} Entities;

typedef Origins * origins_pointer;
typedef Relations * relations_pointer;
typedef Entities * entities_pointer;


entities_pointer addent(char const input[], entities_pointer firstEntity);
entities_pointer delent(char const input[], entities_pointer firstEntity);
entities_pointer addrel(char const input[], entities_pointer firstEntity);
entities_pointer delrel(char const input[], entities_pointer firstEntity);


/**
 * Main method. It initializes the pointer to the beginning of the list only once.
 * Then, it reads an input line and calls the relative method based on what word the line begins with.
 * Eventually, if the line begins with "end", it stops reading and terminates.
 * @return
 */
int main() {
    entities_pointer entitiesList = NULL;                   //Pointer to the beginning of the list of entities

    char input[256];                                        //Too much: could it be adjusted to the length read?

    do {
        gets(input);                                        //It reads the input line

        if(strstr(input, "addent") == input)                //If the command begins with "addent"
            entitiesList = addent(input, entitiesList);
        else if(strstr(input, "delent") == input)           //If the command begins with "delent"
            entitiesList = delent(input, entitiesList);
        else if(strstr(input, "addrel") == input)           //If the command begins with "addrel"
            entitiesList = addrel(input, entitiesList);
        else if(strstr(input, "delrel") == input)           //If the command begins with "delrel"
            entitiesList = delrel(input, entitiesList);




    } while (input[0] != 'e');                              //Maybe check like the others

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

    while (input[i] != '\0') {          //It initializes newEntity with the name received
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
        firstEntity->relations = NULL;
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
            ptr->relations = NULL;
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

    while (input[i] != '\0') {          //It initializes newEntity with the name received
        newEntity[j] = input[i];
        i++;
        j++;
    }

    newEntity[j] = input[i];            //Copies the '\0'

    //Now newEntity contains the new entity name, like "diego" (with "" and the final '\0').
    //If the name is not present in the list (so ptr points to NULL), it does nothing.
    //Otherwise, if the name is present (so ptr does not point to NULL), it looks for every occurrence of "newEntity"
    //in the entities list, and deletes it in the origins list inside the relations list, if present.
    //Eventually, it links the precedent node to the next node and it deletes the node with the searched name.

    entities_pointer ptr = firstEntity;
    entities_pointer prec_ptr = firstEntity;
    int countNodes = 0;
    while (ptr != NULL && strcmp(ptr->name, newEntity) != 0) {
        prec_ptr = ptr;
        ptr = ptr->next;
        countNodes++;
    }

    //If ptr is not NULL, it search in the entities list for every node that is not "newEntity". In that node, it search
    //in its relations list for an origin name that has the same name as "newEntity". At that point, it removes that origin node.

    if(ptr != NULL) {
        entities_pointer ptrRemove = firstEntity;
        while(ptrRemove != NULL && strcmp(ptrRemove->name, newEntity) != 0) {
            relations_pointer relationsRemove = ptrRemove->relations;
            relations_pointer relationsRemovePrec = ptrRemove->relations;
            int countRelations = 0;

            while(relationsRemove != NULL) {
                origins_pointer originsRemove = relationsRemove->origins;
                origins_pointer originsRemovePrec = relationsRemove->origins;
                int countOrigins = 0;

                while(originsRemove != NULL && strcmp(originsRemove->name, newEntity) != 0) {
                    originsRemovePrec = originsRemove;
                    originsRemove = originsRemove->next;
                    countOrigins++;
                }

                if(originsRemove != NULL && countOrigins == 0) {
                    relationsRemove->origins = originsRemove->next;
                    free(originsRemove);
                }
                else if(originsRemove != NULL) {
                    originsRemovePrec->next = originsRemove->next;
                    free(originsRemove);
                }


                if(relationsRemove->origins == NULL && countRelations == 0) {
                    ptrRemove->relations = relationsRemove->next;
                    relations_pointer relationToDel = relationsRemove;
                    relationsRemove = relationsRemove->next;
                    free(relationToDel);
                }
                else if(relationsRemove->origins == NULL) {
                    relationsRemovePrec->next = relationsRemove->next;
                    relations_pointer relationToDel = relationsRemove;
                    relationsRemove = relationsRemove->next;
                    free(relationToDel);
                    countRelations++;
                }
            }
            ptrRemove = ptrRemove->next;
        }
    }

    //Now that every origin node with the same name as "newEntity" is deleted in every other entity node, it deletes the entity "newEntity".

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


entities_pointer addrel(char const input[], entities_pointer firstEntity) {
    int i = 7;                              //It's the beginning index of the new origin id received
    int originIDLength = 0;

    while (input[i] != ' ') {               //To get the size of the new origin id
        originIDLength++;
        i++;
    }

    char originID[originIDLength + 1];      //It will contain the new origin id, plus the '\0'

    i = 7;
    int j = 0;

    while (input[i] != ' ') {               //It initializes originID with the origin id received
        originID[j] = input[i];
        i++;
        j++;
    }

    originID[j] = '\0';                     //Adds the '\0'


    i++;                                    //Index i now points to the first " of the dest id received in input
    int destIDStart = i;
    int destIDLength = 0;
    while (input[i] != ' ') {               //To get the size of the new dest id
        destIDLength++;
        i++;
    }

    char destID[destIDLength + 1];          //It will contain the new dest id, plus the '\0'

    i = destIDStart;
    j = 0;

    while (input[i] != ' ') {               //It initializes destID with the dest id received
        destID[j] = input[i];
        i++;
        j++;
    }

    destID[j] = '\0';                       //Adds the '\0'


    i++;                                    //Index i now points to the first " of the rel id received in input
    int relIDStart = i;
    int relIDLength = 0;
    while (input[i] != '\0') {              //To get the size of the new rel id
        relIDLength++;
        i++;
    }

    char relID[relIDLength + 1];            //It will contain the new rel id, plus the '\0'

    i = relIDStart;
    j = 0;

    while (input[i] != '\0') {              //It initializes relID with the rel id received
        relID[j] = input[i];
        i++;
        j++;
    }

    relID[j] = '\0';                        //Adds the '\0'


    //It looks for the entity node with the same name as "destID"

    entities_pointer ptr = firstEntity;
    while (ptr != NULL && strcmp(ptr->name, destID) != 0) {
        ptr = ptr->next;
    }

    //If that entity is not present, it exits.
    //If that entity is present and it has no relations yet, it initializes the new nodes with the new relID and originID inside.
    //If that entity is present and it already has relations, it looks for the relation node with relID.
    //If relID is not present, it adds it with originID inside.
    //If relID is already present, it simply adds the new originID to that relation, if not already present.

    if(ptr != NULL) {                                                                                       //If the entity is present
        if(ptr->relations == NULL) {                                                                        //If the entity has no relations yet
            ptr->relations = (relations_pointer) malloc(sizeof(Relations));
            ptr->relations->name = (char *) malloc((relIDLength + 1) * sizeof(char));
            strcpy(ptr->relations->name, relID);
            ptr->relations->next = NULL;
            ptr->relations->origins = NULL;

            ptr->relations->origins = (origins_pointer) malloc(sizeof(Origins));
            ptr->relations->origins->name = (char *) malloc((originIDLength + 1) * sizeof(char));
            strcpy(ptr->relations->origins->name, originID);
            ptr->relations->origins->next = NULL;
        }
        else {                                                                                              //Else if the entity already has any relation
            relations_pointer ptrRelations = ptr->relations;
            relations_pointer ptrRelationsPrec = ptr->relations;

            while(ptrRelations != NULL && strcmp(ptrRelations->name, relID) != 0) {
                ptrRelationsPrec = ptrRelations;
                ptrRelations = ptrRelations->next;
            }

            if(ptrRelations == NULL) {                                                                      //If the requested relation is not present in that entity
                ptrRelations = (relations_pointer) malloc(sizeof(Relations));
                ptrRelationsPrec->next = ptrRelations;
                ptrRelations->next = NULL;
                ptrRelations->name = (char *) malloc((relIDLength + 1) * sizeof(char));
                strcpy(ptrRelations->name, relID);
                ptrRelations->origins = (origins_pointer) malloc(sizeof(Origins));
                ptrRelations->origins->name = (char *) malloc((originIDLength + 1) * sizeof(char));
                strcpy(ptrRelations->origins->name, originID);
                ptrRelations->origins->next = NULL;
            }
            else {                                                                                          //Else if the requested relation is already present in that entity
                origins_pointer originToAdd = ptrRelations->origins;
                origins_pointer originPrec = ptrRelations->origins;
                while(originToAdd != NULL && strcmp(originToAdd->name, originID) != 0) {
                    originPrec = originToAdd;
                    originToAdd = originToAdd->next;
                }

                if(originToAdd == NULL) {                                                                   //If the origin is not present, it is added
                    originToAdd = (origins_pointer) malloc(sizeof(Origins));
                    originPrec->next = originToAdd;
                    originToAdd->next = NULL;
                    originToAdd->name = (char *) malloc((originIDLength + 1) * sizeof(char));
                    strcpy(originToAdd->name, originID);
                }
            }

        }
    }

    return firstEntity;
}


entities_pointer delrel(char const input[], entities_pointer firstEntity) {
    int i = 7;                              //It's the beginning index of the new origin id received
    int originIDLength = 0;

    while (input[i] != ' ') {               //To get the size of the new origin id
        originIDLength++;
        i++;
    }

    char originID[originIDLength + 1];      //It will contain the new origin id, plus the '\0'

    i = 7;
    int j = 0;

    while (input[i] != ' ') {               //It initializes originID with the origin id received
        originID[j] = input[i];
        i++;
        j++;
    }

    originID[j] = '\0';                     //Adds the '\0'


    i++;                                    //Index i now points to the first " of the dest id received in input
    int destIDStart = i;
    int destIDLength = 0;
    while (input[i] != ' ') {               //To get the size of the new dest id
        destIDLength++;
        i++;
    }

    char destID[destIDLength + 1];          //It will contain the new dest id, plus the '\0'

    i = destIDStart;
    j = 0;

    while (input[i] != ' ') {               //It initializes destID with the dest id received
        destID[j] = input[i];
        i++;
        j++;
    }

    destID[j] = '\0';                       //Adds the '\0'


    i++;                                    //Index i now points to the first " of the rel id received in input
    int relIDStart = i;
    int relIDLength = 0;
    while (input[i] != '\0') {              //To get the size of the new rel id
        relIDLength++;
        i++;
    }

    char relID[relIDLength + 1];            //It will contain the new rel id, plus the '\0'

    i = relIDStart;
    j = 0;

    while (input[i] != '\0') {              //It initializes relID with the rel id received
        relID[j] = input[i];
        i++;
        j++;
    }

    relID[j] = '\0';                        //Adds the '\0'


    //It looks for the entity node with the same name as "destID"

    entities_pointer ptr = firstEntity;
    while (ptr != NULL && strcmp(ptr->name, destID) != 0) {
        ptr = ptr->next;
    }

    //If that entity is not present, it exits.
    //If that entity is present it checks if it has relID in its relations list. If not, it exits.
    //If relID is present, it checks if the originID of that relation is the same as the originID in input. If not, it exits.
    //If it is the same, it remove that origin in that relation.

    if(ptr != NULL) {
        relations_pointer relPointer = ptr->relations;
        relations_pointer relPointerPrec = ptr->relations;
        int counterRel = 0;
        while(relPointer != NULL && strcmp(relPointer->name, relID) != 0) {
            relPointerPrec->next = relPointer;
            relPointer = relPointer->next;
            counterRel++;
        }

        if(relPointer != NULL) {
            origins_pointer originPointer = relPointer->origins;
            origins_pointer originPointerPrec = relPointer->origins;
            int countOrigins = 0;
            while(originPointer != NULL && strcmp(originPointer->name, originID) != 0) {
                originPointerPrec->next = originPointer;
                originPointer = originPointer->next;
                countOrigins++;
            }

            if(originPointer != NULL && countOrigins == 0) {
                relPointer->origins = originPointer->next;
                free(originPointer);
            }
            else if(originPointer != NULL) {
                originPointerPrec->next = originPointer->next;
                free(originPointer);
            }

            if(relPointer->origins == NULL && counterRel == 0) {
                ptr->relations = relPointer->next;
                free(relPointer);
            }
            else if(relPointer->origins == NULL) {
                relPointerPrec->next = relPointer->next;
                free(relPointer);
            }
        }
    }
    return firstEntity;
}