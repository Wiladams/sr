#include <stdint.h>
#include <stdio.h>

typedef struct person_t {
    uint8_t age;
    char name[100];
} PERSON;

void printPerson(const PERSON *p)
{
    printf("Name:  %s Age: %d", p->name, p->age);
}

void main()
{
    PERSON p;
    p.age = 23;
    strcpy(p.name, "William");
    
    printPerson(&p);
}