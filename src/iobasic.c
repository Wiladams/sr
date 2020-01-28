#include <stdio.h>

void main()
{
    char name[100];
    
    puts("Name: ");
    gets(name);

    printf("Hello, %s\n", name);
}