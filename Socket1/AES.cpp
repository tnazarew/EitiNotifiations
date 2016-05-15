#include "AES.h"
#include "RSA.h"
#include "Util.h"
#include <random>

#include <cstdlib>
#include <iostream>

unsigned   AES::s_box[256], AES::r_box[256];
unsigned   AES::ft[4][256], AES::rt[4][256];

// generate key schedule (128 bit)
bool AES::expand_key( const byte * key)
{
    unsigned i;

    //   s_box contain non-zero value
    if( ! s_box[0] ) init_tables();

    //  copying a key

    unsigned   * ek = encryption_round_key;
    for( i = 0; i < 128 / (8*sizeof(unsigned)); i++ )
        ek[i] = get( &key[i * 4] );

    //  creating expanded key for encryption

    unsigned round_constant = 1;
    for( i = 0; i < 10; i++, ek += 4 )
    {
        ek[4] = ek[0]
                ^ s_box[ ek[3] >>  0 & 0xFF ] << 24
                ^ s_box[ ek[3] >> 24        ] << 16
                ^ s_box[ ek[3] >> 16 & 0xFF ] <<  8
                ^ s_box[ ek[3] >>  8 & 0xFF ] <<  0
                ^ round_constant;
        ek[5] = ek[1] ^ ek[4];
        ek[6] = ek[2] ^ ek[5];
        ek[7] = ek[3] ^ ek[6];
        round_constant = lfsr2((byte)round_constant);
    }

    // creating expanded key for decryption

    unsigned   * dk = decryption_round_key;
    dk[0] = ek[0];  dk[1] = ek[1];  dk[2] = ek[2];  dk[3] = ek[3];

    for( i = 4 * (rounds-1); i; --i, dk++ )
    {
        ek += i & 0x3 ? 1 : -7;
        dk[4] = rt[0][ s_box[ ek[4-1] >>  0 & 0xFF ] ]
              ^ rt[1][ s_box[ ek[4-1] >>  8 & 0xFF ] ]
              ^ rt[2][ s_box[ ek[4-1] >> 16 & 0xFF ] ]
              ^ rt[3][ s_box[ ek[4-1] >> 24        ] ];
    }
    dk[4] = ek[4-8];    dk[5] = ek[5-8];
    dk[6] = ek[6-8];    dk[7] = ek[7-8];

    return true;
}

// generate random key and initialize aes
void AES::keygen(void)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<unsigned char> key(7,127); //key(32,255); with key > 127 java encryption is in trouble
	//while((AESkey[0] = key(gen))>0xEE);
	for(int i = 0; i< 16; ++i)
		AESkey[i] = key(gen);
	expand_key(AESkey);
}

// convert key to hex
void AES::convertKey(unsigned char * to)
{
	unsigned key[4];
	unsigned char hhh[2*block_size+1];
	//unsigned char newKey[block_size];
	hhh[2*block_size] = '\0';
	/*std::cout << "AES key : " << std::endl;
	for(char i = 0; i< block_size; ++i)
	{
		std::cout << (int)AESkey[i] << " ";
	}
	std::cout <<std::endl << "----------------" << std::endl;
	for(char i=0; i<4; ++i)
	{
		key[i] = get(AESkey+i*4);
		Util::uint32ToHEX(key[i], hhh+i*8);
	}

	std::cout << "AES key to hex: " << hhh << std::endl;*/
	for(char i=0; i<4; ++i)
	{
		key[i] = get(AESkey+i*4);
		key[i] = RSA::encrypt(key[i]);
		Util::uint32ToHEX(key[i], hhh+i*8);
	}
	/*std::cout << "Encrypted AES key to hex: " << hhh << std::endl;
	for(char i=0; i<4; ++i)
	{
		Util::hexToUint32(hhh+i*8,key[i]);
		key[i] = RSA::decrypt(key[i]);
		put(key[i], newKey+i*4);
	}
	for(char i=0; i<4; ++i)
	{
		key[i] = get(newKey+i*4);
		Util::uint32ToHEX(key[i], hhh+i*8);
	}
	std::cout << "Decrypted AES key to hex: " << hhh << std::endl;
	std::cout << std::endl << "AESkeys: " << std::endl;
	for(char i = 0; i < block_size; ++i)
	{
		if(newKey[i] != AESkey[i]) std::cout << (int)newKey[i] << "-:-" << (int)AESkey[i] << std::endl;
	}*/
}

void AES::reconvertKey(unsigned char * from)
{
	unsigned key[4];
	unsigned char hhh[2*block_size];
	//hhh[2*block_size] = '\0';
	 
	for(char i=0; i<4; ++i)
	{
		Util::hexToUint32(hhh+i*8,key[i]);
		key[i] = RSA::decrypt(key[i]);
		put(key[i], AESkey+i*4);
	}
}

void AES::encrypt( const byte in[block_size], byte out[block_size] )
{
    //  round constants pointer
    unsigned   * rk = encryption_round_key;

    //  get block and add round key
    unsigned     a0 = get( &in[ 0] ) ^ rk[0];
    unsigned     a1 = get( &in[ 4] ) ^ rk[1];
    unsigned     a2 = get( &in[ 8] ) ^ rk[2];
    unsigned     a3 = get( &in[12] ) ^ rk[3];
    unsigned     b0, b1, b2, b3;

    //  cpryptography rounds
    for( int i = rounds; ; rk += 8 )   
    {
        b0 = fb(a0, 0) ^ fb(a1, 1) ^ fb(a2, 2) ^ fb(a3, 3) ^ rk[4];
        b1 = fb(a1, 0) ^ fb(a2, 1) ^ fb(a3, 2) ^ fb(a0, 3) ^ rk[5];
        b2 = fb(a2, 0) ^ fb(a3, 1) ^ fb(a0, 2) ^ fb(a1, 3) ^ rk[6];
        b3 = fb(a3, 0) ^ fb(a0, 1) ^ fb(a1, 2) ^ fb(a2, 3) ^ rk[7];
        
        //  some hack with MSVC
        #if defined(_MSC_VER) && ! defined(__cplusplus_cli)
            __asm nop
        #endif

        if( ! (i -= 2) ) break;

        a0 = fb(b0, 0) ^ fb(b1, 1) ^ fb(b2, 2) ^ fb(b3, 3) ^ rk[8];
        a1 = fb(b1, 0) ^ fb(b2, 1) ^ fb(b3, 2) ^ fb(b0, 3) ^ rk[9];
        a2 = fb(b2, 0) ^ fb(b3, 1) ^ fb(b0, 2) ^ fb(b1, 3) ^ rk[10];
        a3 = fb(b3, 0) ^ fb(b0, 1) ^ fb(b1, 2) ^ fb(b2, 3) ^ rk[11];
    }
      
    //  last round
    put(
        rk[8]  ^ s_box[ b3 >> 24        ] << 24
               ^ s_box[ b2 >> 16 & 0xFF ] << 16
               ^ s_box[ b1 >>  8 & 0xFF ] <<  8
               ^ s_box[ b0 >>  0 & 0xFF ] <<  0
        , &out[ 0]
        );
    put( 
        rk[9]  ^ s_box[ b0 >> 24        ] << 24
               ^ s_box[ b3 >> 16 & 0xFF ] << 16 
               ^ s_box[ b2 >>  8 & 0xFF ] <<  8 
               ^ s_box[ b1 >>  0 & 0xFF ] <<  0
        , &out[ 4]
        );   
    put( 
        rk[10] ^ s_box[ b1 >> 24        ] << 24
               ^ s_box[ b0 >> 16 & 0xFF ] << 16
               ^ s_box[ b3 >>  8 & 0xFF ] <<  8
               ^ s_box[ b2 >>  0 & 0xFF ] <<  0
        , &out[ 8]
        );  
    put( 
        rk[11] ^ s_box[ b2 >> 24        ] << 24
               ^ s_box[ b1 >> 16 & 0xFF ] << 16
               ^ s_box[ b0 >>  8 & 0xFF ] <<  8
               ^ s_box[ b3 >>  0 & 0xFF ] <<  0
        , &out[12]
        );
}

void AES::decrypt( const byte in[block_size], byte out[block_size] )
{
    // pointer to round constants
    unsigned   * rk = decryption_round_key;

    //  get block and add round key
    unsigned     a0 = get( &in[ 0] ) ^ rk[0];
    unsigned     a1 = get( &in[ 4] ) ^ rk[1];
    unsigned     a2 = get( &in[ 8] ) ^ rk[2];
    unsigned     a3 = get( &in[12] ) ^ rk[3];
    unsigned     b0, b1, b2, b3;

    // decryption rounds
    for( int i = rounds; ; rk += 8 )
    {
        b0 = rb(a0, 0) ^ rb(a3, 1) ^ rb(a2, 2) ^ rb(a1, 3) ^ rk[4];
        b1 = rb(a1, 0) ^ rb(a0, 1) ^ rb(a3, 2) ^ rb(a2, 3) ^ rk[5];
        b2 = rb(a2, 0) ^ rb(a1, 1) ^ rb(a0, 2) ^ rb(a3, 3) ^ rk[6];
        b3 = rb(a3, 0) ^ rb(a2, 1) ^ rb(a1, 2) ^ rb(a0, 3) ^ rk[7];

        //  some hack with MSVC
        #if defined(_MSC_VER) && ! defined(__cplusplus_cli)
            __asm nop
        #endif

        if( ! (i -= 2) ) break;

        a0 = rb(b0, 0) ^ rb(b3, 1) ^ rb(b2, 2) ^ rb(b1, 3) ^ rk[8];
        a1 = rb(b1, 0) ^ rb(b0, 1) ^ rb(b3, 2) ^ rb(b2, 3) ^ rk[9];
        a2 = rb(b2, 0) ^ rb(b1, 1) ^ rb(b0, 2) ^ rb(b3, 3) ^ rk[10];
        a3 = rb(b3, 0) ^ rb(b2, 1) ^ rb(b1, 2) ^ rb(b0, 3) ^ rk[11];
    }
    
    // last round 
    put(
        rk[8]  ^ r_box[ b1 >> 24        ] << 24
               ^ r_box[ b2 >> 16 & 0xFF ] << 16
               ^ r_box[ b3 >>  8 & 0xFF ] <<  8
               ^ r_box[ b0 >>  0 & 0xFF ] <<  0
        , &out[ 0]
        );
    put(
        rk[9]  ^ r_box[ b2 >> 24        ] << 24
               ^ r_box[ b3 >> 16 & 0xFF ] << 16
               ^ r_box[ b0 >>  8 & 0xFF ] <<  8
               ^ r_box[ b1 >>  0 & 0xFF ] <<  0
        , &out[ 4]
        );
    put(
        rk[10] ^ r_box[ b3 >> 24        ] << 24
               ^ r_box[ b0 >> 16 & 0xFF ] << 16
               ^ r_box[ b1 >>  8 & 0xFF ] <<  8
               ^ r_box[ b2 >>  0 & 0xFF ] <<  0
        , &out[ 8]
        );
    put(
        rk[11] ^ r_box[ b0 >> 24        ] << 24
               ^ r_box[ b1 >> 16 & 0xFF ] << 16
               ^ r_box[ b2 >>  8 & 0xFF ] <<  8
               ^ r_box[ b3 >>  0 & 0xFF ] <<  0
        , &out[12]
        );
}

AES::byte inline AES::lfsr2( byte a )
{
    return a & 0x80 ? a<<1 ^ (byte)basis    // вычитание по модулю 2
                    : a<<1;                 // нет обратноё связи
}

/// creates direct and reverse swap S-box tables
/// expand tables of swap
void AES::init_tables()
{
    //  tables of fast multiplication in field GF(2**8)  
    byte    pow[1<<8],  log[1<<8];

    //  fulling the tables.
    unsigned    i = 0;      //  initial power of omega;
    byte        a = 1;      //  omega**i in polynomial form;
    log[0] = 0;             //  this element is required for S-box
    do
    {
        pow[i] = a;         //  omega**i = x7**7 + .. + x0**0;
        log[a] = (byte)i;
        //  next element of table is selected 
        //  on the base of polynom phi(x+1)
        a ^= lfsr2(a);      //  a *= (x + 1)
    }
    while( (byte)++i );     //  2**8 iterations

    //  initialization of elements [0x63, 0]
    pow[ (1<<8) - 1 ] = 0;

    //  generating of inverse S-box'es
    i = 0;
    do
    {
        byte    a = pow[(1<<8)-1 - log[i]];

        //  multiplying by     x**4 + x**3 + x**2 + x + 1
        a ^= a << 1 ^ a << 2 ^ a << 3 ^ a << 4  // a<<1|a>>7 == a<<1^a>>7 !
          ^  a >> 4 ^ a >> 5 ^ a >> 6 ^ a >> 7  // swapping
        // and adding    x**6 + x**5 + x**1 + x**0
          ^                (1<<6 ^ 1<<5 ^ 1<<1 ^ 1<<0);   
        
        s_box[i] = a;
        r_box[a] = i;
    }
    while( (byte)++i );     //  2**8 iterations

    //  generating expanded tables of swapping
    for( i = 0; i < 256; i++ )
    {
        byte f = (byte)s_box[i]; 
        byte f2 = lfsr2(f);
        ft[0][i] = (f ^ f2) << 24 | f << 16 | f << 8 | f2;
        ft[1][i] = rot_left_8( ft[0][i] );
        ft[2][i] = rot_left_8( ft[1][i] );
        ft[3][i] = rot_left_8( ft[2][i] );

        byte r = (byte)r_box[i];
        //  multiplying:
        //  pow[(log[x] + log[y]) % 255]    ( where x and y != 0 )
        rt[0][i] = ! r ? r :
                 (unsigned) pow[(0x68 + log[r]) % 255] << 24 // log[0x0B] == 0x68
               ^ (unsigned) pow[(0xEE + log[r]) % 255] << 16 // log[0x0D] == 0xEE
               ^ (unsigned) pow[(0xC7 + log[r]) % 255] <<  8 // log[0x09] == 0xC7
               ^ (unsigned) pow[(0xDF + log[r]) % 255] <<  0;// log[0x0E] == 0xDF
        rt[1][i] = rot_left_8( rt[0][i] );
        rt[2][i] = rot_left_8( rt[1][i] );
        rt[3][i] = rot_left_8( rt[2][i] );
    }
}


unsigned inline AES::get( const byte array[4] )
{
#if (('1234' >> 24) == '1')     // little endian ?
    return  *(unsigned *)(array);
#else
    return  array[0] <<  0
          | array[1] <<  8
          | array[2] << 16
          | array[3] << 24;
#endif
}


void inline AES::put( unsigned value, byte array[4] )
{
#if (('1234' >> 24) == '1')     // little endian ?
    *(unsigned *)(array) = value;
#else
    array[0] = value >>  0 & 0xFF;
    array[1] = value >>  8 & 0xFF;
    array[2] = value >> 16 & 0xFF;
    array[3] = value >> 24       ;
#endif
}


unsigned inline AES::fb(unsigned byte, unsigned pos)
{
    return ft[pos][ byte >> pos*8 & 0xFF ];
}


unsigned inline AES::rb(unsigned byte, unsigned pos)
{
    return rt[pos][ byte >> pos*8 & 0xFF ];
}

unsigned inline AES::rot_left_8(unsigned value ) // make ROL
{
    return  value >> 24 | value << 8;
}