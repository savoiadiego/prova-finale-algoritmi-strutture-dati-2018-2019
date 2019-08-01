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

typedef struct destID {
    char * destName;
    struct destID * next;
} DestIDs;

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
    entities_pointer entitiesList = NULL;                   //Pointer to the beginning of the list of entities

    char input[256];                                        //Too much: could it be adjusted to the length read?

    do {
        fgets(input, 257, stdin);                                                 //It reads the input line

        if(input[0] == 'a' && input[1] == 'd' && input[2] == 'd') {
            if(input[3] == 'e' && input[4] == 'n' && input[5] == 't')                       //If the command begins with "addent"
                entitiesList = addent(input, entitiesList);
            else if(input[3] == 'r' && input[4] == 'e' && input[5] == 'l')                  //If the command begins with "addrel"
                entitiesList = addrel(input, entitiesList);
        }
        else if(input[0] == 'd' && input[1] == 'e' && input[2] == 'l') {
            if(input[3] == 'e' && input[4] == 'n' && input[5] == 't')                       //If the command begins with "delent"
                entitiesList = delent(input, entitiesList);
            else if(input[3] == 'r' && input[4] == 'e' && input[5] == 'l')                  //If the command begins with "delrel"
                entitiesList = delrel(input, entitiesList);
        }
        else if(input[0] == 'r' && input[1] == 'e' && input[2] == 'p' && input[3] == 'o' && input[4] == 'r' && input[5] == 't')
            report(entitiesList);                                                           //If the command begins with "report"

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
    char * newEntity = NULL;     //It will contain the new entity name, plus the '\0'
    int i = 7;

    for(; input[i] != '\n'; i++) {      //i = 7 is the beginning index of the new entity name received
        if(i == 7) {
            newEntity = (char *) malloc(sizeof(char) + 1);
            newEntity[0] = input[i];
            newEntity[1] = '\0';
        }
        else {
            newEntity = realloc(newEntity, strlen(newEntity) + sizeof(char) + 1);
            newEntity[i-7] = input[i];
        }
    }

    newEntity[i-7] = '\0';

    //Now newEntity contains the new entity name, like "diego" (with "" and the final '\0').
    //If there aren't nodes yet, it initializes the first entity, and its next attribute points to NULL.
    //Otherwise, it looks in every node: if the name is not there, ptr points to NULL (end of list), so it creates a new node there.
    //If the name is already there, ptr does not point to NULL (the name is present) so we do nothing.

    if (firstEntity == NULL) {
        firstEntity = (entities_pointer) malloc(sizeof(Entities));
        firstEntity->name = (char *) malloc((i-7 + 1) * sizeof(char));
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
            ptr->name = (char *) malloc((i-7 + 1) * sizeof(char));
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
            newEntity = (char *) malloc(sizeof(char) + 1);
            newEntity[0] = input[i];
            newEntity[1] = '\0';
        }
        else {
            newEntity = realloc(newEntity, strlen(newEntity) + sizeof(char) + 1);
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

                relationsRemovePrec = relationsRemove;
                relationsRemove = relationsRemovePrec->next;
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
            originID = (char *) malloc(sizeof(char) + 1);
            originID[0] = input[i];
            originID[1] = '\0';
        }
        else {
            originID = realloc(originID, strlen(originID) + sizeof(char) + 1);
            originID[i-7] = input[i];
        }
    }

    originID[i-7] = '\0';

    int destIDStart = ++i;                                    //Index i now points to the first " of the dest id received in input
    char * destID = NULL;

    for(; input[i] != ' '; i++) {
        if(i == destIDStart) {
            destID = (char *) malloc(sizeof(char) + 1);
            destID[0] = input[i];
            destID[1] = '\0';
        }
        else {
            destID = realloc(destID, strlen(destID) + sizeof(char) + 1);
            destID[i-destIDStart] = input[i];
        }
    }

    destID[i-destIDStart] = '\0';

    int relIDStart = ++i;                                    //Index i now points to the first " of the relID received in input
    char * relID = NULL;

    for(; input[i] != '\n'; i++) {
        if(i == relIDStart) {
            relID = (char *) malloc(sizeof(char) + 1);
            relID[0] = input[i];
            relID[1] = '\0';
        }
        else {
            relID = realloc(relID, strlen(relID) + sizeof(char) + 1);
            relID[i-relIDStart] = input[i];
        }
    }

    relID[i-relIDStart] = '\0';

    //It looks for the entity node with the same name as "destID"

    entities_pointer ptr = firstEntity;
    while (ptr != NULL && strcmp(ptr->name, destID) != 0) {
        ptr = ptr->next;
    }

    //It also checks if "originID" is present in the monitored entities

    entities_pointer ptrOrigin = firstEntity;
    while(ptrOrigin != NULL && strcmp(ptrOrigin->name, originID) != 0) {
        ptrOrigin = ptrOrigin->next;
    }

    //If at least one of the two entities is not present, it exits.
    //If that entity is present and it has no relations yet, it initializes the new nodes with the new relID and originID inside.
    //If that entity is present and it already has relations, it looks for the relation node with relID.
    //If relID is not present, it adds it with originID inside.
    //If relID is already present, it simply adds the new originID to that relation, if not already present.

    if(ptr != NULL && ptrOrigin != NULL) {                                                                  //If the entity is present
        if(ptr->relations == NULL) {                                                                        //If the entity has no relations yet
            ptr->relations = (relations_pointer) malloc(sizeof(Relations));
            ptr->relations->name = (char *) malloc((strlen(relID) + 1) * sizeof(char));
            strcpy(ptr->relations->name, relID);
            ptr->relations->next = NULL;
            ptr->relations->origins = NULL;

            ptr->relations->origins = (origins_pointer) malloc(sizeof(Origins));
            ptr->relations->origins->name = (char *) malloc((strlen(originID) + 1) * sizeof(char));
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
                ptrRelations->name = (char *) malloc((strlen(relID) + 1) * sizeof(char));
                strcpy(ptrRelations->name, relID);
                ptrRelations->origins = (origins_pointer) malloc(sizeof(Origins));
                ptrRelations->origins->name = (char *) malloc((strlen(originID) + 1) * sizeof(char));
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
                    originToAdd->name = (char *) malloc((strlen(originID) + 1) * sizeof(char));
                    strcpy(originToAdd->name, originID);
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
            originID = (char *) malloc(sizeof(char) + 1);
            originID[0] = input[i];
            originID[1] = '\0';
        }
        else {
            originID = realloc(originID, strlen(originID) + sizeof(char) + 1);
            originID[i-7] = input[i];
        }
    }

    originID[i-7] = '\0';

    int destIDStart = ++i;                                    //Index i now points to the first " of the dest id received in input
    char * destID = NULL;

    for(; input[i] != ' '; i++) {
        if(i == destIDStart) {
            destID = (char *) malloc(sizeof(char) + 1);
            destID[0] = input[i];
            destID[1] = '\0';
        }
        else {
            destID = realloc(destID, strlen(destID) + sizeof(char) + 1);
            destID[i-destIDStart] = input[i];
        }
    }

    destID[i-destIDStart] = '\0';

    int relIDStart = ++i;                                    //Index i now points to the first " of the relID received in input
    char * relID = NULL;

    for(; input[i] != '\n'; i++) {
        if(i == relIDStart) {
            relID = (char *) malloc(sizeof(char) + 1);
            relID[0] = input[i];
            relID[1] = '\0';
        }
        else {
            relID = realloc(relID, strlen(relID) + sizeof(char) + 1);
            relID[i-relIDStart] = input[i];
        }
    }

    relID[i-relIDStart] = '\0';

    //It looks for the entity node with the same name as "destID"

    entities_pointer ptr = firstEntity;
    while (ptr != NULL && strcmp(ptr->name, destID) != 0) {
        ptr = ptr->next;
    }

    //It also checks if "originID" is present in the monitored entities

    entities_pointer ptrOrigin = firstEntity;
    while(ptrOrigin != NULL && strcmp(ptrOrigin->name, originID) != 0) {
        ptrOrigin = ptrOrigin->next;
    }

    //If at least one of the two entities is not present, it exits.
    //If that entity is present it checks if it has relID in its relations list. If not, it exits.
    //If relID is present, it checks if the originID of that relation is the same as the originID in input. If not, it exits.
    //If it is the same, it remove that origin in that relation.

    if(ptr != NULL && ptrOrigin != NULL) {
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


//Scorri la lista delle entities: per ogni entity scorri le relazioni in ingresso. Per ogni relazione salvi nome e numero di origins.
//In una nuova lista "report", se la relazione non è già presente, allora la aggiungi con destinatario e numero di origins.
//Se invece è già presente, confronti il numero di origins e se è maggiore di quello già presente, allora sostituisci numero ed eventualmente destinatario, se diverso.
//Se il numero è identico, allora si aggiunge il nuovo destinatario (che quindi è una lista dentro la lista).
//Una volta completato lo scorrimento di tutta la lista, bisogna ordinarla in base al nome della relazione.
//Bisogna inoltre ordinare anche i destinatari di ogni relazione, se più di uno.
//Infine, si stampa la lista in ordine.

void report(entities_pointer firstEntity) {
    reports_pointer reportHead = NULL;

    entities_pointer ptr = firstEntity;
    while(ptr != NULL) {
        relations_pointer rel = ptr->relations;
        while(rel != NULL) {
            if(reportHead == NULL) {
                reportHead = (reports_pointer) malloc(sizeof(Reports));
                reportHead->destID = (destinations_pointer) malloc(sizeof(DestIDs));
                reportHead->destID->destName = (char *) malloc((strlen(ptr->name) + 1) * sizeof(char));
                strcpy(reportHead->destID->destName, ptr->name);
                reportHead->destID->next = NULL;
                reportHead->relID = (char *) malloc((strlen(rel->name) + 1) * sizeof(char));
                strcpy(reportHead->relID, rel->name);
                int num = 0;
                origins_pointer originsTemp = rel->origins;
                while(originsTemp != NULL) {
                    num++;
                    originsTemp = originsTemp->next;
                }
                reportHead->num = num;
                reportHead->next = NULL;
            }
            else {
                reports_pointer report = reportHead;
                reports_pointer reportPrec = reportHead;
                while(report != NULL && strcmp(rel->name, report->relID) != 0) {
                    reportPrec = report;
                    report = report->next;
                }
                //Same as above but report is not empty, so if rel is already present, we have to check num to decide
                //if we have to add destID (numOld = num), substitute destID (numOld < num) or do nothing (numOld > num).

                if(report == NULL) {            //relID is not present, so we add a new node
                    report = (reports_pointer) malloc(sizeof(Reports));
                    report->destID = (destinations_pointer) malloc(sizeof(DestIDs));
                    report->destID->destName = (char *) malloc((strlen(ptr->name) + 1) * sizeof(char));
                    strcpy(report->destID->destName, ptr->name);
                    report->destID->next = NULL;
                    report->relID = (char *) malloc((strlen(rel->name) + 1) * sizeof(char));
                    strcpy(report->relID, rel->name);
                    int num = 0;
                    origins_pointer originsTemp = rel->origins;
                    while(originsTemp != NULL) {
                        num++;
                        originsTemp = originsTemp->next;
                    }
                    report->num = num;
                    report->next = NULL;
                    reportPrec->next = report;
                }
                else {                          //relID is already present, so we check num
                    int num = 0;
                    origins_pointer originsTemp = rel->origins;
                    while(originsTemp != NULL) {
                        num++;
                        originsTemp = originsTemp->next;
                    }
                    if(num == report->num) {    //if num is the same, we add the new destID
                        destinations_pointer destTemp = report->destID;
                        destinations_pointer destTempPrec = report->destID;
                        while(destTemp != NULL) {
                            destTempPrec = destTemp;
                            destTemp = destTemp->next;
                        }
                        destTemp = (destinations_pointer) malloc(sizeof(DestIDs));
                        destTemp->destName = (char *) malloc((strlen(ptr->name) + 1) * sizeof(char));
                        strcpy(destTemp->destName, ptr->name);
                        destTemp->next = NULL;
                        destTempPrec->next = destTemp;
                    }
                    else if(num > report->num) {    //if num is higher than the previous, we substitute the destID
                        report->destID->destName = NULL;
                        report->destID->next = NULL;
                        report->destID->destName = (char *) malloc((strlen(ptr->name) + 1) * sizeof(char));
                        strcpy(report->destID->destName, ptr->name);
                        report->num = num;
                    }
                }
            }
            rel = rel->next;
        }
        ptr = ptr->next;
    }

    //After the report list is created, we have to order it (relations and destIDs inside).

    if(reportHead == NULL) {
        printf("none\n");
        return;
    }
    else {
        reports_pointer reportTempD = reportHead;
        while (reportTempD != NULL) {

            destinations_pointer destTempPrec = reportTempD->destID;
            destinations_pointer destTemp;

            //If there is more than one destID in the relation, we have to check the order
            while (destTempPrec != NULL) {
                destTemp = destTempPrec->next;
                while (destTemp != NULL) {
                    if (strcmp(destTemp->destName, destTempPrec->destName) < 0) {
                        char *temporary = (char *) malloc(strlen(destTemp->destName));
                        strcpy(temporary, destTemp->destName);
                        //free(destTemp->destName);
                        destTemp->destName = (char *) malloc(strlen(destTempPrec->destName));
                        strcpy(destTemp->destName, destTempPrec->destName);
                        //free(destTempPrec->destName);
                        destTempPrec->destName = (char *) malloc(strlen(temporary));
                        strcpy(destTempPrec->destName, temporary);
                    }
                    destTemp = destTemp->next;
                }
                destTempPrec = destTempPrec->next;
            }
            reportTempD = reportTempD->next;
        }

        reports_pointer reportTempPrec = reportHead;
        reports_pointer reportTemp = reportHead->next;

        while (reportTemp != NULL) {
            if (strcmp(reportTemp->relID, reportTempPrec->relID) < 0) {
                char *temporary = (char *) malloc(strlen(reportTemp->relID));
                strcpy(temporary, reportTemp->relID);
                //free(reportTemp->relID);
                reportTemp->relID = (char *) malloc(strlen(reportTempPrec->relID));
                strcpy(reportTemp->relID, reportTempPrec->relID);
                //free(reportTempPrec->relID);
                reportTempPrec->relID = (char *) malloc(strlen(temporary));
                strcpy(reportTempPrec->relID, temporary);
                destinations_pointer temporaryDestPointer = reportTemp->destID;
                reportTemp->destID = reportTempPrec->destID;
                reportTempPrec->destID = temporaryDestPointer;
                int temporaryNum = reportTemp->num;
                reportTemp->num = reportTempPrec->num;
                reportTempPrec->num = temporaryNum;
            }
            reportTemp = reportTemp->next;
            reportTempPrec = reportTempPrec->next;
        }

        //Now that the report list is ordered, we have to print it.

        reports_pointer reportPrint = reportHead;
        char *output = NULL;
        char *space = " ";
        char *dot = ";";

        int countOutput = 0;

        while (reportPrint != NULL) {
            char *relName = (char *) malloc(strlen(reportPrint->relID) + strlen(space) + 1);
            strcpy(relName, reportPrint->relID);
            strcat(relName, space);


            char *destinations = NULL;
            int countDest = 0;
            while (reportPrint->destID != NULL) {
                char *destID = (char *) malloc(strlen(reportPrint->destID->destName) + 1);
                strcpy(destID, reportPrint->destID->destName);
                if (countDest == 0) {
                    destinations = (char *) malloc(strlen(destID) + 1);
                    strcpy(destinations, destID);
                } else {
                    destinations = realloc(destinations, strlen(destinations) + strlen(space) + strlen(destID) + 1);
                    strcat(destinations, space);
                    strcat(destinations, destID);
                }

                countDest++;
                reportPrint->destID = reportPrint->destID->next;
            }

            char num[2];
            sprintf(num, "%d", reportPrint->num);

            if (countOutput == 0) {
                output = (char *) malloc(strlen(relName) + strlen(destinations) + strlen(space) + strlen(num) + strlen(dot) + 1);
                strcpy(output, relName);
                strcat(output, destinations);
                strcat(output, space);
                strcat(output, num);
                strcat(output, dot);
            } else {
                output = realloc(output, strlen(output) + strlen(space) + strlen(relName) + strlen(destinations) +
                                         strlen(space) + strlen(num) + strlen(dot) + 1);
                strcat(output, space);
                strcat(output, relName);
                strcat(output, destinations);
                strcat(output, space);
                strcat(output, num);
                strcat(output, dot);
            }

            countOutput++;
            reportPrint = reportPrint->next;
        }
        printf("%s\n", output);
        return;
    }
}