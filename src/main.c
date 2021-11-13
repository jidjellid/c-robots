#include <stdio.h>
#include <stdlib.h>
#include "view.c"
#include "config.c"

/**
 * 	Enter of the game, and check number of args
 */
int main(int argc, const char *argv[])
{
	if (argc != (NUMBER_ROBOTS + 1))
	{
		printf("Need to provide %i arguments, not %i !\n", NUMBER_ROBOTS, argc - 1);
		return EXIT_FAILURE;
	}
	srand(time(NULL));
	launch(argc, argv);
	return EXIT_SUCCESS;
}
