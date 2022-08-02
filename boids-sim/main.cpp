#include "BoidsSim.h"

int main()
{
	srand(static_cast<unsigned>(time(NULL)));

	BoidsSim boidsSim;

	while (boidsSim.getWindow().isOpen())
	{
		boidsSim.update();
		boidsSim.render();
	}

	return 0;
}