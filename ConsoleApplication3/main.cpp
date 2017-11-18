#include "lcms.h"
#include <stdio.h>
#include <stdlib.h>


int main() {
	
	
	INITHandles((SQLCHAR*)"DSN=projectdb;USER=root;PASSWORD=chammakCHALO9@");
	display_complaints();
	getchar();

		return 0;
}