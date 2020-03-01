#include <stdio.h>

struct AFunction
{
    void operator()(void)
    {
        printf("FUNCTOR\n");
    }
};

void main()
{
    AFunction func;

    func();
}