/*
 * encrypt.c
 *
 *  Created on: Nov 24, 2018
 *      Author: user
 */

void encrypt(char *message){
	char c;
	while(*message){
		*message = *message ^ 31;
		message++;
	}
}
