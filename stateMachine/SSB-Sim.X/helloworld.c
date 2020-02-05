/* 
 * File:   helloworld.c
 * Author: gleece
 *
 * Created on February 4, 2020, 4:33 PM
 */

#include <stdio.h>
//#include <stdlib.h>
//#include <xc.h>
#include <BOARD.h>

/*
 * 
 */
int main(void)
{
    BOARD_Init();
    printf("HELLO WORLD!\n");
    while(1);
}

