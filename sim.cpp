#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef uint64_t LUTbits;
typedef unsigned char LUTinputs;
typedef short wire;

struct LUT {
    wire out;
    LUTbits bits;
    wire inputs[6];
};

LUT luts[] = {
    #include "luts.h"
};

bool wires[NUM_WIRES];

void make_random_inputs(){
    int num_leading_ones = random() % NUM_INPUTS;
    for (int i=0; i<num_leading_ones; ++i) wires[i] = true;
    wires[num_leading_ones] = false;
    for (int i=num_leading_ones+1; i<num_leading_ones; ++i) wires[i] = random()&1;    
}

int FFZ(){
    for (int i=0; i<NUM_INPUTS; ++i){
        if (!wires[i]) return i;
    }
    return -1;
}

void run_sim(){
    for (int i = 0; i<NUM_LUTS; ++i){
        LUTinputs input = 0;
        for (int bit=0; bit<6; ++bit){
            bool v = wires[luts[i].inputs[bit]];
            input |= v<<bit;
        }
        bool res = (luts[i].bits>>input)&1;
        wires[luts[i].out] = res;
    }
}

#define BIT(n) (wires[RES_POS_##n]<<n)

int numTests, numFailures;

void test(){
    ++numTests;
    run_sim();
    int zero_pos = BIT(9) | BIT(8) | BIT(7) | BIT(6) | BIT(5) | BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0);
    if (wires[RES_NO_ZEROS]) zero_pos = -1;
    if (zero_pos != FFZ()){
        ++numFailures;
        printf("All ones = %d, position = %d\n", wires[RES_NO_ZEROS], zero_pos);
    }
}

int main(){
    for (int i=0; i<1000000; ++i){
        make_random_inputs();
        test();
    }
    printf("Tests:%d Failures:%d\n", numTests, numFailures);
    return 0;
}
