/*
 * message_hider.c
 *
 *  Created on: Nov 24, 2018
 *      Author: user
 */
#include <stdio.h>
#include "encrypt.h"

int main(){
	char msg[80];
	while(fgets(msg, 80, stdin)){
		encrypt(msg);
		printf("%s", msg);
	}
}
