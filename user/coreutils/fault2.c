#include "signal.h"
#include "unistd.h"

int main()
{
    kill(getppid(), SIGUSR1);
}
