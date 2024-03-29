#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * List of origins of a specific relation.
 * Every node contains the origin name, which is the entity name where the relation comes from.
 */
typedef struct origin {
    char * name;
    struct origin * next;
} Origins;

/**
 * List of the monitored relations that enter an entity node, added with "addrel" and removed with "delrel".
 * Every node contains the relation name, a list of origins of that relation and how many origins are there.
 */
typedef struct relation {
    char * name;
    struct origin * origins;
    int originsNum;
    struct relation * next;
} Relations;

/**
 * List of the monitored entities, added with "addent" and removed with "delent".
 * Every node contains the entity name and a list of relations that enter the node.
 */
typedef struct entity {
    char * name;
    struct relation * relations;
    struct entity * next;
} Entities;

/**
 * List of the entity name with the most instances of a relation coming in. Every node contains the entity name.
 */
typedef struct destID {
    char * destName;
    struct destID * next;
} DestIDs;

/**
 * List used to store every information useful for the method "report".
 * Every node contains a relation name, a list of the entity names with the most instances of that relation coming in,
 * and the number of instances of the relation coming in.
 */
typedef struct report {
    char * relID;
    struct destID * destID;
    int num;
    struct report * next;
} Reports;


typedef Origins * origins_pointer;
typedef Relations * relations_pointer;
typedef Entities * entities_pointer;
typedef DestIDs * destinations_pointer;
typedef Reports * reports_pointer;

int originsSize = sizeof(Origins);
int relationsSize = sizeof(Relations);
int entitiesSize = sizeof(Entities);
int destSize = sizeof(DestIDs);
int reportsSize = sizeof(Reports);


entities_pointer addent(char const input[], entities_pointer firstEntity);
entities_pointer delent(char const input[], entities_pointer firstEntity);
entities_pointer addrel(char const input[], entities_pointer firstEntity);
entities_pointer delrel(char const input[], entities_pointer firstEntity);
void report(entities_pointer firstEntity);


/**
 * Main method. It initializes the pointer to the beginning of the list only once.
 * Then, it reads an input line and calls the relative method based on what word the line begins with.
 * Eventually, if the line begins with "end", it stops reading and terminates.
 * @return 0 when the process ends
 */
int main() {
    entities_pointer entitiesList = NULL;                                       //Pointer to the beginning of the list of entities

    char input[256];                                                            //Too much: could it be adjusted to the length read?

    do {
        fgets(input, 256, stdin);                                     //It reads the input line

        if(input[0] == 'a' && input[1] == 'd' && input[2] == 'd') {
            if(input[3] == 'e'/* && input[4] == 'n' && input[5] == 't'*/)           //If the command begins with "addent"
                entitiesList = addent(input, entitiesList);
            else if(input[3] == 'r'/* && input[4] == 'e' && input[5] == 'l'*/)      //If the command begins with "addrel"
                entitiesList = addrel(input, entitiesList);
        }
        else if(input[0] == 'd' && input[1] == 'e' && input[2] == 'l') {
            if(input[3] == 'e'/* && input[4] == 'n' && input[5] == 't'*/)           //If the command begins with "delent"
                entitiesList = delent(input, entitiesList);
            else if(input[3] == 'r'/* && input[4] == 'e' && input[5] == 'l'*/)      //If the command begins with "delrel"
                entitiesList = delrel(input, entitiesList);
        }
        else if(input[0] == 'r'/* && input[1] == 'e' && input[2] == 'p' && input[3] == 'o' && input[4] == 'r' && input[5] == 't'*/)
            report(entitiesList);                                               //If the command begins with "report"

    } while (input[0] != 'e');

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
    char * newEntity = NULL;                                        //It will contain the new entity name, plus the '\0'
    int i = 7;

    for(; input[i] != '\n'; i++) {                                  //i = 7 is the beginning index of the new entity name received
        if(i == 7) {
            newEntity = malloc(sizeof(char) + 1);
            newEntity[0] = input[i];
            newEntity[1] = '\0';
        }
        else {
            newEntity = realloc(newEntity, i-7 + sizeof(char) + 1);
            newEntity[i-7] = input[i];
        }
    }

    newEntity[i-7] = '\0';

    //Now newEntity contains the new entity name, like "diego" (with "" and the final '\0').
    //If there aren't nodes yet, it initializes the first entity, and its next attribute points to NULL.
    //Otherwise, it looks in every node: if the name is not there, ptr points to NULL (end of list), so it creates a new node there.
    //If the name is already there, ptr does not point to NULL (the name is present) so we do nothing.


    if (firstEntity == NULL) {
        firstEntity = malloc(entitiesSize);
        firstEntity->name = malloc((i-7 + 1) * sizeof(char));
        strcpy(firstEntity->name, newEntity);
        firstEntity->relations = NULL;
        firstEntity->next = NULL;
    }
    else {
        entities_pointer ptr = firstEntity;
        entities_pointer prec_ptr = firstEntity;
        int strcmpResult = 0;
        while (ptr != NULL && (strcmpResult = strcmp(newEntity, ptr->name)) > 0) {
            prec_ptr = ptr;
            ptr = ptr->next;
        }

        if (ptr == NULL) {
            ptr = malloc(entitiesSize);              //Creates the new node
            prec_ptr->next = ptr;                                           //Links the last existing node to the new one
            ptr->name = malloc((i-7 + 1) * sizeof(char));
            strcpy(ptr->name, newEntity);
            ptr->relations = NULL;
            ptr->next = NULL;
        }
        else if(strcmpResult < 0) {
            entities_pointer newEntityNode = malloc(entitiesSize);
            if(ptr == firstEntity) {
                newEntityNode->next = firstEntity;
                firstEntity = newEntityNode;
            }
            else {
                prec_ptr->next = newEntityNode;
                newEntityNode->next = ptr;
            }
            newEntityNode->name = malloc((i-7 + 1) * sizeof(char));
            strcpy(newEntityNode->name, newEntity);
            newEntityNode->relations = NULL;
        }
    }

    return firstEntity;
}

/**
 * Method called when the command "delent" <entity_name> is received.
 * If <entity_name> is present in the entity list, then the method deletes the node.
 * In this case, every relation which includes <entity_name> is deleted.
 * If <entity_name> is not present in the entity list (or the list is empty), then the method does nothing.
 * @param input string
 * @param firstEntity (pointer to the beginning of the entities list)
 * @return firstEntity, modified if necessary
 */
entities_pointer delent(char const input[], entities_pointer firstEntity) {
    char * newEntity = NULL;     //It will contain the new entity name, plus the '\0'
    int i = 7;

    for(; input[i] != '\n'; i++) {      //i = 7 is the beginning index of the new entity name received
        if(i == 7) {
            newEntity = malloc(sizeof(char) + 1);
            newEntity[0] = input[i];
            newEntity[1] = '\0';
        }
        else {
            newEntity = realloc(newEntity, i-7 + sizeof(char) + 1);
            newEntity[i-7] = input[i];
        }
    }

    newEntity[i-7] = '\0';

    //Now newEntity contains the new entity name, like "diego" (with "" and the final '\0').
    //If the name is not present in the list (so ptr points to NULL), it does nothing.
    //Otherwise, if the name is present (so ptr does not point to NULL), it looks for every occurrence of "newEntity"
    //in the entities list, and deletes it in the origins list inside the relations list, if present.
    //Eventually, it links the precedent node to the next node and it deletes the node with the searched name.

    entities_pointer ptr = firstEntity;
    entities_pointer prec_ptr = firstEntity;
    int countNodes = 0;
    int strcmpResult = 0;
    while(ptr != NULL && ((strcmpResult = strcmp(ptr->name, newEntity)) < 0)) {
        prec_ptr = ptr;
        ptr = ptr->next;
        countNodes++;
    }

    //If ptr is not NULL, it search in the entities list for every node that is not "newEntity". In that node, it search
    //in its relations list for an origin name that has the same name as "newEntity". At that point, it removes that origin node.

    entities_pointer ptrRemove = NULL;
    int nodeToDelete = 0;

    if(ptr != NULL && strcmpResult == 0) {
        for (ptrRemove = firstEntity; ptrRemove != NULL; ptrRemove = ptrRemove->next) {
            relations_pointer relationsRemove = ptrRemove->relations;
            relations_pointer relationsRemovePrec = ptrRemove->relations;
            int countRelations = 0;

            while (relationsRemove != NULL) {
                origins_pointer originsRemove = relationsRemove->origins;
                origins_pointer originsRemovePrec = relationsRemove->origins;
                int countOrigins = 0;

                while (originsRemove != NULL && strcmp(originsRemove->name, newEntity) != 0) {
                    originsRemovePrec = originsRemove;
                    originsRemove = originsRemove->next;
                    countOrigins++;
                }

                if (originsRemove != NULL && countOrigins == 0) {
                    relationsRemove->origins = originsRemove->next;
                    free(originsRemove);
                    relationsRemove->originsNum--;
                } else if (originsRemove != NULL) {
                    originsRemovePrec->next = originsRemove->next;
                    free(originsRemove);
                    relationsRemove->originsNum--;
                }


                if (relationsRemove->origins == NULL && countRelations == 0) {
                    ptrRemove->relations = relationsRemove->next;
                    relations_pointer relationToDel = relationsRemove;
                    relationsRemove = relationsRemove->next;
                    free(relationToDel);
                } else if (relationsRemove->origins == NULL) {
                    relationsRemovePrec->next = relationsRemove->next;
                    relations_pointer relationToDel = relationsRemove;
                    relationsRemove = relationsRemove->next;
                    free(relationToDel);
                    countRelations++;
                } else {
                    relationsRemovePrec = relationsRemove;
                    relationsRemove = relationsRemovePrec->next;
                    countRelations++;
                }
            }
        }
    }

    //Now that every origin node with the same name as "newEntity" is deleted in every other entity node, it deletes the entity "newEntity".

    if(ptr != NULL && strcmpResult == 0 && countNodes == 0) {                //If ptr is not NULL and the node to remove is not the first
        firstEntity = ptr->next;                        //The head is now changed: if it was the only node, now it is NULL (empty)
        free(ptr);
    }
    else if(ptr != NULL && strcmpResult == 0) {                              //If ptr is not NULL and the node to remove is the first
        prec_ptr->next = ptr->next;
        free(ptr);
    }

    return firstEntity;
}

/**
 * Method called when the command "addrel" <entity_name_origin> <entity_name_destination> <relation_name> is received.
 * This method looks for <entity_name_destination> node in the entities list. Then, if <relation_name> is not present
 * in the relations list of that entity, it is added with <entity_name_origin> as the origin name.
 * If <entity_name_origin> or <entity_name_destination> are not present in the entity list, then the method does nothing.
 * @param input string
 * @param firstEntity (pointer to the beginning of the entities list)
 * @return firstEntity, modified if necessary
 */
entities_pointer addrel(char const input[], entities_pointer firstEntity) {
    char * originID = NULL;     //It will contain the new originID, plus the '\0'
    int i = 7;

    for(; input[i] != ' '; i++) {      //i = 7 is the beginning index of the new originID received
        if(i == 7) {
            originID = malloc(sizeof(char) + 1);
            originID[0] = input[i];
            originID[1] = '\0';
        }
        else {
            originID = realloc(originID, i-7 + sizeof(char) + 1);
            originID[i-7] = input[i];
        }
    }

    originID[i-7] = '\0';
    int originIDLength = i - 7;

    int destIDStart = ++i;                                    //Index i now points to the first " of the dest id received in input
    char * destID = NULL;

    for(; input[i] != ' '; i++) {
        if(i == destIDStart) {
            destID = malloc(sizeof(char) + 1);
            destID[0] = input[i];
            destID[1] = '\0';
        }
        else {
            destID = realloc(destID, i-destIDStart + sizeof(char) + 1);
            destID[i-destIDStart] = input[i];
        }
    }

    destID[i-destIDStart] = '\0';

    int relIDStart = ++i;                                    //Index i now points to the first " of the relID received in input
    char * relID = NULL;

    for(; input[i] != '\n'; i++) {
        if(i == relIDStart) {
            relID = malloc(sizeof(char) + 1);
            relID[0] = input[i];
            relID[1] = '\0';
        }
        else {
            relID = realloc(relID, i-relIDStart + sizeof(char) + 1);
            relID[i-relIDStart] = input[i];
        }
    }

    relID[i-relIDStart] = '\0';
    int relIDLength = i - relIDStart;

    //It looks for the entities nodes with the same name as "destID" and "originID"

    entities_pointer ptr = firstEntity;
    entities_pointer destPtr = NULL;
    int originIDFound = 0;
    int destIDFound = 0;
    int strcmpDest = 0;
    int strcmpOr = 0;
    while (ptr != NULL) {
        if(strcmp(ptr->name, destID) == 0) {
            destIDFound = 1;
            destPtr = ptr;
        }
        if(strcmp(ptr->name, originID) == 0)
            originIDFound = 1;
        if(destIDFound == 1 && originIDFound == 1)
            break;
        else if((originIDFound == 0 && strcmpOr > 0) || (destIDFound == 0 && strcmpDest > 0))
            return firstEntity;
        else
            ptr = ptr->next;
    }

    //If at least one of the two entities is not present, it exits.
    //If that entity is present and it has no relations yet, it initializes the new nodes with the new relID and originID inside.
    //If that entity is present and it already has relations, it looks for the relation node with relID.
    //If relID is not present, it adds it with originID inside.
    //If relID is already present, it simply adds the new originID to that relation, if not already present.

    if(originIDFound && destIDFound) {                                          //If the entities are present
        if(destPtr->relations == NULL) {                                                                        //If the entity has no relations yet
            destPtr->relations = malloc(relationsSize);
            destPtr->relations->next = NULL;
            destPtr->relations->name = malloc((relIDLength + 1) * sizeof(char));
            strcpy(destPtr->relations->name, relID);
            destPtr->relations->origins = NULL;
            destPtr->relations->origins = malloc(originsSize);
            destPtr->relations->origins->name = malloc((originIDLength + 1) * sizeof(char));
            strcpy(destPtr->relations->origins->name, originID);
            destPtr->relations->originsNum = 1;
            destPtr->relations->origins->next = NULL;
        }
        else {                                                                                              //Else if the entity already has any relation
            relations_pointer ptrRelations = destPtr->relations;
            relations_pointer ptrRelationsPrec = destPtr->relations;
            int strcmpResult = 0;

            while(ptrRelations != NULL && (strcmpResult = strcmp(relID, ptrRelations->name)) > 0) {
                ptrRelationsPrec = ptrRelations;
                ptrRelations = ptrRelations->next;
            }

            if(ptrRelations == NULL) {                                                                      //If the requested relation is not present in that entity and it needs to be inserted at the end
                ptrRelations = malloc(relationsSize);
                ptrRelations->next = NULL;
                ptrRelations->name = malloc((relIDLength + 1) * sizeof(char));
                strcpy(ptrRelations->name, relID);
                ptrRelations->origins = malloc(originsSize);
                ptrRelations->origins->name = malloc((originIDLength + 1) * sizeof(char));
                strcpy(ptrRelations->origins->name, originID);
                ptrRelations->originsNum = 1;
                ptrRelations->origins->next = NULL;
                ptrRelationsPrec->next = ptrRelations;
            }
            else if(ptrRelations != NULL && strcmpResult < 0) {                                            //If the requested relation is not present but it needs to be inserted in the middle
                relations_pointer newRelation = malloc(relationsSize);
                if(ptrRelations == destPtr->relations) {
                    newRelation->next = destPtr->relations;
                    destPtr->relations = newRelation;
                }
                else {
                    newRelation->next = ptrRelations;
                    ptrRelationsPrec->next = newRelation;
                }
                newRelation->name = malloc((relIDLength + 1) * sizeof(char));
                strcpy(newRelation->name, relID);
                newRelation->origins = malloc(originsSize);
                newRelation->origins->name = malloc((originIDLength + 1) * sizeof(char));
                strcpy(newRelation->origins->name, originID);
                newRelation->originsNum = 1;
                newRelation->origins->next = NULL;
            }
            else if(ptrRelations != NULL && strcmpResult == 0) {                                          //Else if the requested relation is already present in that entity
                origins_pointer originToAdd = ptrRelations->origins;
                origins_pointer originPrec = ptrRelations->origins;
                int strcmpOrigin = 0;
                while(originToAdd != NULL && (strcmpOrigin = strcmp(originID, originToAdd->name)) > 0) {
                    originPrec = originToAdd;
                    originToAdd = originToAdd->next;
                }

                if(originToAdd == NULL) {                                                                   //If the origin is not present and it needs to be inserted at the end
                    originToAdd = malloc(originsSize);
                    originToAdd->next = NULL;
                    originToAdd->name = malloc((originIDLength + 1) * sizeof(char));
                    strcpy(originToAdd->name, originID);
                    originPrec->next = originToAdd;
                    ptrRelations->originsNum++;
                }
                else if(originToAdd != NULL && strcmpOrigin < 0) {                                          //Else if the origin is not present but it needs to be inserted in the middle
                    origins_pointer newOrigin = malloc(originsSize);
                    if(originToAdd == ptrRelations->origins) {
                        newOrigin->next = ptrRelations->origins;
                        ptrRelations->origins = newOrigin;
                    }
                    else {
                        newOrigin->next = originToAdd;
                        originPrec->next = newOrigin;
                    }
                    newOrigin->name = malloc((originIDLength + 1) * sizeof(char));
                    strcpy(newOrigin->name, originID);
                    ptrRelations->originsNum++;
                }
            }

        }
    }

    return firstEntity;
}

/**
 * Method called when the command "delrel" <entity_name_origin> <entity_name_destination> <relation_name> is received.
 * This method looks for <entity_name_destination> node in the entities list. Then, if <relation_name> is present
 * in the relations list of that entity, it deletes <entity_name_origin> from the origins list of that relations list.
 * If <entity_name_origin>, <entity_name_destination> or <relation_name> are not present in the entity list, the method does nothing.
 * @param input string
 * @param firstEntity (pointer to the beginning of the entities list)
 * @return firstEntity, modified if necessary
 */
entities_pointer delrel(char const input[], entities_pointer firstEntity) {
    char * originID = NULL;     //It will contain the new originID, plus the '\0'
    int i = 7;

    for(; input[i] != ' '; i++) {      //i = 7 is the beginning index of the new originID received
        if(i == 7) {
            originID = malloc(sizeof(char) + 1);
            originID[0] = input[i];
            originID[1] = '\0';
        }
        else {
            originID = realloc(originID, i-7 + sizeof(char) + 1);
            originID[i-7] = input[i];
        }
    }

    originID[i-7] = '\0';

    int destIDStart = ++i;                                    //Index i now points to the first " of the dest id received in input
    char * destID = NULL;

    for(; input[i] != ' '; i++) {
        if(i == destIDStart) {
            destID = malloc(sizeof(char) + 1);
            destID[0] = input[i];
            destID[1] = '\0';
        }
        else {
            destID = realloc(destID, i-destIDStart + sizeof(char) + 1);
            destID[i-destIDStart] = input[i];
        }
    }

    destID[i-destIDStart] = '\0';

    int relIDStart = ++i;                                    //Index i now points to the first " of the relID received in input
    char * relID = NULL;

    for(; input[i] != '\n'; i++) {
        if(i == relIDStart) {
            relID = malloc(sizeof(char) + 1);
            relID[0] = input[i];
            relID[1] = '\0';
        }
        else {
            relID = realloc(relID, i-relIDStart + sizeof(char) + 1);
            relID[i-relIDStart] = input[i];
        }
    }

    relID[i-relIDStart] = '\0';

    //It looks for the entities nodes with the same name as "destID" and "originID"

    entities_pointer ptr = firstEntity;
    entities_pointer destPtr = NULL;
    int originIDFound = 0;
    int destIDFound = 0;
    int strcmpDest = 0;
    int strcmpOr = 0;
    while (ptr != NULL) {
        if(strcmp(ptr->name, destID) == 0) {
            destIDFound = 1;
            destPtr = ptr;
        }
        if(strcmp(ptr->name, originID) == 0)
            originIDFound = 1;
        if(destIDFound == 1 && originIDFound == 1)
            break;
        else if((originIDFound == 0 && strcmpOr > 0) || (destIDFound == 0 && strcmpDest > 0))
            return firstEntity;
        else
            ptr = ptr->next;
    }

    //If at least one of the two entities is not present, it exits.
    //If that entity is present it checks if it has relID in its relations list. If not, it exits.
    //If relID is present, it checks if the originID of that relation is the same as the originID in input. If not, it exits.
    //If it is the same, it remove that origin in that relation.

    if(originIDFound && destIDFound) {
        relations_pointer relPointer = destPtr->relations;
        relations_pointer relPointerPrec = destPtr->relations;
        int counterRel = 0;
        int strcmpResult = 0;
        while(relPointer != NULL && (strcmpResult = strcmp(relID, relPointer->name)) > 0) {
            relPointerPrec = relPointer;
            relPointer = relPointer->next;
            counterRel++;
        }

        if(relPointer != NULL && strcmpResult == 0) {
            origins_pointer originPointer = relPointer->origins;
            origins_pointer originPointerPrec = relPointer->origins;
            int countOrigins = 0;
            int strcmpOrigins = 0;
            while(originPointer != NULL && (strcmpOrigins = strcmp(originID, originPointer->name)) > 0) {
                originPointerPrec = originPointer;
                originPointer = originPointer->next;
                countOrigins++;
            }

            if(strcmpOrigins == 0 && countOrigins == 0) {
                relPointer->origins = originPointer->next;
                free(originPointer);
                relPointer->originsNum--;
            }
            else if(strcmpOrigins == 0) {
                originPointerPrec->next = originPointer->next;
                free(originPointer);
                relPointer->originsNum--;
            }

            if(relPointer->origins == NULL && counterRel == 0) {
                destPtr->relations = relPointer->next;
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

/**
 * Method called when the command "report" is received.
 * This method scans the entities list: for every entity, it scans the inbound relations and for every relations, it stores the name and the number of origins.
 * It adds the relation to a "report" list if that relation is not already present, inserting also the destination and the number of origins.
 * Instead, if the relation is already present in the "report" list, then it compares the numbers of origins: if it is greater than the one already present,
 * then it overwrites the old one, changing also the destination if different.
 * If the numbers are the same, it simply adds the new destination (so it will be a list of lists).
 * After competing the scan of the whole list, it sorts it according to the relation names. Destinations are sorted too, if more than one.
 * Finally, it prints the list.
 * @param firstEntity (pointer to the beginning of the entities list)
 */
void report(entities_pointer firstEntity) {
    reports_pointer reportHead = NULL;
    entities_pointer ptr;
    relations_pointer rel;

    for(ptr = firstEntity; ptr != NULL; ptr = ptr->next) {
        for(rel = ptr->relations; rel != NULL; rel = rel->next) {
            if(reportHead == NULL) {
                reportHead = malloc(reportsSize);
                reportHead->next = NULL;
                reportHead->destID = malloc(destSize);
                reportHead->destID->destName = malloc((strlen(ptr->name) + 1) * sizeof(char));
                strcpy(reportHead->destID->destName, ptr->name);
                reportHead->destID->next = NULL;
                reportHead->relID = malloc((strlen(rel->name) + 1) * sizeof(char));
                strcpy(reportHead->relID, rel->name);
                reportHead->num = rel->originsNum;
            }
            else {
                reports_pointer report = reportHead;
                reports_pointer reportPrec = reportHead;
                int strcmpResult = 0;
                while(report != NULL && ((strcmpResult = strcmp(rel->name, report->relID)) > 0)) {
                    reportPrec = report;
                    report = report->next;
                }

                //Same as above but report is not empty, so if rel is not present, report == NULL and we add it at the end of the list.
                //If rel is already present, report != NULL and there are two cases:
                //If strcmp gave 0, report points at a node with the same relID, so we check num and we do accordingly.
                //If strcmp gave <0, the new relID must be inserted between reportPrec and report.

                if(report == NULL) {            //relID is not present, so we add a new node at the end of the list
                    report = malloc(reportsSize);
                    report->next = NULL;
                    report->destID = malloc(destSize);
                    report->destID->destName = malloc((strlen(ptr->name) + 1) * sizeof(char));
                    strcpy(report->destID->destName, ptr->name);
                    report->destID->next = NULL;
                    report->relID = malloc((strlen(rel->name) + 1) * sizeof(char));
                    strcpy(report->relID, rel->name);
                    report->num = rel->originsNum;
                    reportPrec->next = report;
                }
                else {
                    if(strcmpResult < 0) {          //The new relID must be inserted between reportPrec and report
                        reports_pointer newReport = malloc(reportsSize);
                        if(report == reportHead) {
                            newReport->next = reportHead;
                            reportHead = newReport;
                        }
                        else {
                            reportPrec->next = newReport;
                            newReport->next = report;
                        }
                        newReport->destID = malloc(destSize);
                        newReport->destID->destName = malloc((strlen(ptr->name) + 1) * sizeof(char));
                        strcpy(newReport->destID->destName, ptr->name);
                        newReport->destID->next = NULL;
                        newReport->relID = malloc((strlen(rel->name) + 1) * sizeof(char));
                        strcpy(newReport->relID, rel->name);
                        newReport->num = rel->originsNum;
                    }
                    else {          //The new relID is the same as the one in report node, so we check num and modify accordingly
                        int num = rel->originsNum;
                        if(num == report->num) {            //if num is the same, we add the new destID, sorting them
                            destinations_pointer destTemp = report->destID;
                            destinations_pointer destTempPrec = report->destID;
                            strcmpResult = 0;
                            while(destTemp != NULL && ((strcmpResult = strcmp(ptr->name, destTemp->destName)) > 0)) {
                                destTempPrec = destTemp;
                                destTemp = destTemp->next;
                            }
                            if(destTemp == NULL) {              //If the new destID must be inserted at the end of the list
                                destTemp = malloc(destSize);
                                destTemp->next = NULL;
                                destTemp->destName = malloc((strlen(ptr->name) + 1) * sizeof(char));
                                strcpy(destTemp->destName, ptr->name);
                                destTempPrec->next = destTemp;
                            }
                            else if(strcmpResult < 0) {         //If the new destID must be inserted between destTempPrec and destTemp
                                destinations_pointer newDest = malloc(destSize);
                                if(destTemp == report->destID) {
                                    newDest->next = report->destID;
                                    report->destID = newDest;
                                }
                                else {
                                    destTempPrec->next = newDest;
                                    newDest->next = destTemp;
                                }
                                newDest->destName = malloc((strlen(ptr->name) + 1) * sizeof(char));
                                strcpy(newDest->destName, ptr->name);
                            }
                        }
                        else if(num > report->num) {    //if num is higher than the previous, we substitute the destID
                            report->destID->destName = NULL;
                            free(report->destID->next);
                            report->destID->next = NULL;
                            report->destID->destName = malloc((strlen(ptr->name) + 1) * sizeof(char));
                            strcpy(report->destID->destName, ptr->name);
                            report->num = num;
                        }
                    }
                }
            }
        }
    }

    //After the report list is created, we have to print it.

    if(reportHead == NULL) {
        printf("none\n");
        return;
    }
    else {
        reports_pointer reportPrint = reportHead;
        char *output = NULL;
        int outputLength = 0;
        char *space = " ";
        int spaceLength = strlen(space);
        char *dot = ";";
        int dotLength = strlen(dot);

        int countOutput = 0;

        while (reportPrint != NULL) {
            int relIDLength = strlen(reportPrint->relID);
            char *relName = malloc(relIDLength + spaceLength + 1);
            strcpy(relName, reportPrint->relID);
            strcat(&relName[relIDLength], space);


            char *destinations = NULL;
            int destinationsLength = 0;
            int countDest = 0;
            while (reportPrint->destID != NULL) {
                int destIDLength = strlen(reportPrint->destID->destName);
                char *destID = malloc(destIDLength + 1);
                strcpy(destID, reportPrint->destID->destName);
                if (countDest == 0) {
                    destinations = malloc(destIDLength + 1);
                    strcpy(destinations, destID);
                    destinationsLength = destIDLength;
                } else {
                    destinations = realloc(destinations, destinationsLength + spaceLength + destIDLength + 1);
                    strcat(&destinations[destinationsLength], space);
                    strcat(&destinations[destinationsLength + spaceLength], destID);
                    destinationsLength = destinationsLength + spaceLength + destIDLength;
                }

                countDest++;
                reportPrint->destID = reportPrint->destID->next;
            }

            char num[3];
            sprintf(num, "%d", reportPrint->num);
            int numLength = strlen(num);

            if (countOutput == 0) {
                output = malloc(relIDLength + spaceLength + destinationsLength + spaceLength + numLength + dotLength + 1);
                strcpy(output, relName);
                strcat(&output[relIDLength + spaceLength], destinations);
                strcat(&output[relIDLength + spaceLength + destinationsLength], space);
                strcat(&output[relIDLength + spaceLength + destinationsLength + spaceLength], num);
                strcat(&output[relIDLength + spaceLength + destinationsLength + spaceLength + numLength], dot);
                outputLength = relIDLength + spaceLength + destinationsLength + spaceLength + numLength + dotLength;
            } else {
                output = realloc(output, outputLength + spaceLength + relIDLength + spaceLength + destinationsLength + spaceLength + numLength + dotLength + 1);
                strcat(&output[outputLength], space);
                strcat(&output[outputLength + spaceLength], relName);
                strcat(&output[outputLength + spaceLength + relIDLength + spaceLength], destinations);
                strcat(&output[outputLength + spaceLength + relIDLength + spaceLength + destinationsLength], space);
                strcat(&output[outputLength + spaceLength + relIDLength + spaceLength + destinationsLength + spaceLength], num);
                strcat(&output[outputLength + spaceLength + relIDLength + spaceLength + destinationsLength + spaceLength + numLength], dot);
                outputLength = outputLength + spaceLength + relIDLength + spaceLength + destinationsLength + spaceLength + numLength + dotLength;
            }

            countOutput++;
            reportPrint = reportPrint->next;
            free(relName);
        }
        printf("%s\n", output);
        free(reportHead);
        return;
    }
}
