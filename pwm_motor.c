#include <stdlib.h>
#include <stdio.h>

#include "pwm.h"



int main(int argc, char *argv[]) {

	printf("Starting Program Execution\n");
	pwmInit();

	setPWMDuty(1,800000);
	setPWMDuty(0,800000);

	while(1) {
		char buff[100];

		if (gets(buff) != NULL) {
			printf("Characters: %s\n", buff);
		}
	}

	printf("EXITING PROGRAM\n");
	return EXIT_SUCCESS;
}
