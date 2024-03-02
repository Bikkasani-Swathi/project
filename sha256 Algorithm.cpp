#include <bits/stdc++.h>

using namespace std;

uint32_t modular_addition(uint32_t a, uint32_t b) {
    return (a + b) % 0xFFFFFFFF;
}

uint32_t right_rotate(uint32_t a, unsigned int b)
{
    return (a >> b)|(a << (32 - b));
}

string convert_to_binary(string input){
    string binary = "";
    for (char c : input) {
        bitset<8> characters(c);
        binary += characters.to_string();
    }
    return binary;
}

string padding_input(string binary_input){
    uint64_t length = binary_input.size();
    string padded_input = binary_input;
    padded_input += '1';
    // append 'r' zeroes to string to make it multiple of 512 
    uint32_t r = (448 - (length + 1)%512 )%512;
    for (int i = 0; i < r; ++i) {
        padded_input += '0';
    }
    // append original length
    bitset<64> bits_size(length);
    padded_input += bits_size.to_string();
    return padded_input;
}

string sha256(string padded_input){
    
    //initial values of h  
    uint32_t h0=0x6a09e667;
    uint32_t h1=0xbb67ae85;
    uint32_t h2= 0x3c6ef372;
    uint32_t h3= 0xa54ff53a;
    uint32_t h4=0x510e527f;
    uint32_t h5=0x9b05688c;
    uint32_t h6=0x1f83d9ab;
    uint32_t h7=0x5be0cd19;
    
    //initial values of k 
    uint32_t k[] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    //working variables
    uint32_t a,b,c,d,e,f,g,h;
    string chunk;

    //main processing logic
    for (int i = 0; i < padded_input.size(); i += 512) {
        //get 512 sized chunks from input
        chunk=(padded_input.substr(i, 512));

        //message schedule array
        uint32_t w[64];
        int x=0;
        //first 16 values of message schedule are assigned 32 bit sized substring of the current chunk
        for(int i=0;i<16;i++){
            bitset<32> chars(chunk.substr(x, 32));
            w[i] = chars.to_ulong();
            x=x+32;
        }
        //remaining values of the message schedule array are assigned using defined calculations
        for(int i=16;i<64;i++){
            uint32_t s0 = (right_rotate(w[i-15],7)) ^ (right_rotate(w[i - 15], 18)) ^ (w[i - 15] >> 3);
            uint32_t s1 = (right_rotate(w[i - 2],17)) ^ (right_rotate(w[i - 2],19)) ^ (w[i - 2] >> 10);
            w[i] = modular_addition(w[i - 16], modular_addition(s0, modular_addition(w[i - 7], s1)));
        }

        //h values are used as initial values for working variables
        a=h0;
        b=h1;
        c=h2;
        d=h3;
        e=h4;
        f=h5;
        g=h6;
        h=h7;

        for (int i = 0; i < 64; ++i) {
            uint32_t S1 = (right_rotate(e, 6)) ^ (right_rotate(e,11)) ^ (right_rotate(e,25));
            uint32_t ch = (e & f) ^ ((~e) & g);
            uint32_t temp1 = modular_addition(h, modular_addition(S1, modular_addition(ch, modular_addition(k[i], w[i]))));
            uint32_t S0 = (right_rotate(a,2)) ^ (right_rotate(a,13)) ^ (right_rotate(a,22));
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = modular_addition(S0, maj);

            h = g;
            g = f;
            f = e;
            e = modular_addition(d, temp1);
            d = c;
            c = b;
            b = a;
            a = modular_addition(temp1, temp2);
        }

        //for next chunk compression, setting the input hash values equal to the addition of current hash values and
        // current chunk's compressed values
        h0 = modular_addition(h0, a);
        h1 = modular_addition(h1, b);
        h2 = modular_addition(h2, c);
        h3 = modular_addition(h3, d);
        h4 = modular_addition(h4, e);
        h5 = modular_addition(h5, f);
        h6 = modular_addition(h6, g);
        h7 = modular_addition(h7, h);
    }

    //message digest is created by attaching all the h values obtained at the end of all rounds
    stringstream output;
    output << hex << setfill('0') << setw(8) << h0;
    output << hex << setfill('0') << setw(8) << h1;
    output << hex << setfill('0') << setw(8) << h2;
    output << hex << setfill('0') << setw(8) << h3;
    output << hex << setfill('0') << setw(8) << h4;
    output << hex << setfill('0') << setw(8) << h5;
    output << hex << setfill('0') << setw(8) << h6;
    output << hex << setfill('0') << setw(8) << h7;
    return output.str();
}

int main() {
    //read input
    stringstream buffer;
    ifstream file("bible.txt");
    buffer<<file.rdbuf();
    string input=buffer.str();
    file.close();

    //binary format of input
    string binary_input=convert_to_binary(input);

    //padding input string 
    string padded_input=padding_input(binary_input);
   
    //hashing
    string digest=sha256(padded_input);
    cout<<"Hash value:\n"<<digest;
    return 0;
}

//Hash value for the input file: 8d202adf39c3e88510a591cbe1cbd96708e3237140060c0768f2577f9d81d3ad