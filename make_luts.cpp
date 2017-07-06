#include <stdio.h>
#include <inttypes.h>

typedef uint64_t LUTbits;
typedef short wire;

typedef bool LUTbitCalc(int i);

LUTbits calc_LUTbits(const char *s, LUTbitCalc fn) {
    LUTbits res = 0;
    for (int i=0; i<64; ++i){
        LUTbits bit = fn(i);
        res |= bit << i;
    }
    printf("#define %s 0x%" PRIx64 "\n", s, res);
    return res;
}

int FFZ(uint64_t n){
    int cnt = 0;
    while (n&1){
        ++cnt;
        n>>=1;
    }
    return cnt;
}

#define DONTCARE 0

void emitLUT(wire res, const char *func, wire in0, wire in1, wire in2, wire in3, wire in4, wire in5){
    printf("  {%d, %s, %d, %d, %d, %d, %d, %d},\n", res, func, in0, in1, in2, in3, in4, in5);
}

wire nextWire;

struct ffz_layer_result {
    wire allOnes;
    wire bitNum[10];
};

ffz_layer_result make_ffz_layers(int depth, int first_input){
    printf("// make_ffz_layers %d %d\n", depth, first_input);
    ffz_layer_result res;
    wire in[4];
    ffz_layer_result child[4];
    int numForwardBits = depth*2;
    for (int i=0; i<4; ++i){
        if (depth == 0){
            in[i] = first_input + i;
        } else {
            child[i] = make_ffz_layers(depth-1, first_input + i * (1<<numForwardBits));
            in[i] = child[i].allOnes;
        }
    }
    wire allOnes = nextWire++;
    wire zeroPos1 = nextWire++;
    wire zeroPos0 = nextWire++;
    res.allOnes = allOnes;
    res.bitNum[numForwardBits]   = zeroPos0;
    res.bitNum[numForwardBits+1] = zeroPos1;
    emitLUT(allOnes, "AllSet4", in[0], in[1], in[2], in[3], DONTCARE, DONTCARE);
    emitLUT(zeroPos0, "FFZ4_0", in[0], in[1], in[2], in[3], DONTCARE, DONTCARE);
    emitLUT(zeroPos1, "FFZ4_1", in[0], in[1], in[2], in[3], DONTCARE, DONTCARE);
    for (int i=numForwardBits-1; i>=0; --i){
        wire muxout = nextWire++;
        res.bitNum[i] = muxout;
        emitLUT(muxout, "Mux4to1", zeroPos0, zeroPos1,
                child[0].bitNum[i], child[1].bitNum[i], child[2].bitNum[i], child[3].bitNum[i]);
    }
    return res;
}

int main(){
    // All 4 of the LSBs set?
    calc_LUTbits("AllSet4", [](int i){return (i & 0xf) == 0xf;});
    
    // The bottom two bits of the position of the first zero
    calc_LUTbits("FFZ4_0", [](int i)->bool{return FFZ(i) & 1<<0;});
    calc_LUTbits("FFZ4_1", [](int i)->bool{return FFZ(i) & 1<<1;});
    
    // 4:1 mux. Bottom two bits select from the next four.
    calc_LUTbits("Mux4to1", [](int i)->bool{return i & 1<<(2 + (i&3));});

    int depth = 5;
    int firstLUT = 1<<(2*depth); // first are inputs
    nextWire = firstLUT;
    auto res = make_ffz_layers(depth-1, 0);
    printf("#define NUM_WIRES %d\n", nextWire);
    printf("#define NUM_INPUTS %d\n", firstLUT);
    printf("#define NUM_LUTS %d\n", nextWire-firstLUT);
    printf("#define RES_NO_ZEROS %d\n", res.allOnes);
    for (int i=2*depth-1; i>=0; --i){
        printf("#define RES_POS_%d %d\n", i, res.bitNum[i]);
    }
    
    return 0;
}
