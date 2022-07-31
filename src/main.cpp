#include "../headers/filtering.h"
    
int main()
{
    Filtering game; 

    if (game.Construct(1200, 1200, 1, 1)) game.Start();
    return 0;
}
