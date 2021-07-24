# Prova Finale Algoritmi e Strutture Dati 2018/2019

Final project of the "Algoritmi e Principi dell'Informatica" course 2018/2019.\
Developed by Diego Savoia.

## Description
The complete description of the project can be found in the [official PDF file](https://github.com/savoiadiego/ProgettoAPI2019/blob/master/Project%20Description.pdf) (in Italian).

The goal is to implement a mechanism to monitor relationships between entities, which can change over time. Like in a social network, new users can register, delete their account, become "friends" of other users, break the "friends" relationship and so on. Relationships between entities are not necessarily simmetric: for example, Alice can be "friend" of Bruno, but Bruno could not be "friend" of Alice.

More abstractly, the mechanisms monitors the following actions:
* A new entity starts to be monitored
* A monitored entity stops to be monitored
* A new relationship is created between two monitored entities
* An existing relationship between two monitored entities is removed

Each entity has an identifiying name (like "Alice", "Bruno" and so on).\
There could exists different relationships types, each one identified by a name (like "is_friend_of", "follows" and so on).\
Each relationship has a direction and is not necessarily simmetric (for example, if Alice is "friend_of" Bruno, the direction of the relationship is from Alice towards Bruno, so Bruno is the "receiver" of the relationship).\
The mechanism must be optimized to handle a large number of entities and relationships instances, but generally only a few relationships types.

### Methods
The methods that can be handle by the mechanism are the following:
* addent <id_ent> - It adds a new entity (identified by "id_ent") to the list of monitored entities. If the entity is being already monitored, it does nothing.
* delent <id_ent> - It removes the entity identified by "id_ent" from the list of monitored entities. It also deletes all the relationships that involves "id_ent" (both as origin and as destination).
* addrel <id_orig> <id_dest> <id_rel> - It adds a new relationship (identified by "id_rel) between the entities "id_orig" and "id_dest", in which "id_dest" is the receiver of the relationship. If that specific relationship already exists, or if at least one of the two entities is not monitored, it does nothing. The monitoring of the relationship type "id_rel" starts when the first "addrel" message involving "id_rel" is received.
* delrel <id_orig> <id_dest> <id_rel> - It removes the relationship identified by "id_rel" between the entities "id_orig" and "id_dest", in which "id_dest" is the receiver of the relationship. If that specific relationship between those specific two entities does not exists, it does nothing.
* report - It prints the list of all the monitored relationships. For each one of them, it prints also the entities with the largest number of inbound relationships, and the number itself. In case of ties, it prints all the involved entities in increasing order of identifier. If no relationship of that type is present, it outputs "none". 
* end - It stops the commands sequence.

### Implementation
The mechanism is implemented using the standard C language, relying only on the _libc_ library.\
It reads in standard input a sequence of commands and, when requested, it prints in standard output the result (as explained above).
