/* 
 * File:   mapping_main.c
 * Author: bhage
 *
 * Created on January 26, 2017, 11:11 AM
 */

#include "p24Hxxxx.h"
#include "stdint.h"	

uint16_t scale(uint16_t X, uint16_t p_min, uint16_t p_max) {
    uint16_t range = p_max-p_min;
    uint32_t mult = (uint32_t)X * (uint32_t)range;
    uint32_t baseVal = mult/4096;
    //uint16_t baseVal = (uint32_t)percentage * range;
    return baseVal+ p_min;
}


int main(void) {
    uint16_t test1,test2,test3,test4;
    
    uint16_t p_max = 783;
    uint16_t p_min = 600;
    
    
    test1 = scale(0, p_min, p_max);
    test2 = scale(800, p_min, p_max);
    test3 = scale(2400, p_min, p_max);
    test4 = scale(4096, p_min, p_max);
    
    while (1){
        
    }
}


