#ifndef AES_H
#define AES_H
class AES
{
	typedef unsigned char   byte;
		
	public:
		static const unsigned  block_size = 16; // 128 bit
		void encrypt(const byte in[block_size], byte out[block_size]);
		void decrypt(const byte in[block_size], byte out[block_size]);
		bool expand_key(const byte * key); // initialize a key
		void keygen(void); // initialize with random key
		void convertKey(unsigned char buffer[32]); // convert key to hex format AESkey[16]->buffer[32]
		void reconvertKey(unsigned char from[32]); // convert key to uchar format from[32]->AESkey[16]
													// AESkey declared below
	protected:

		unsigned char AESkey[block_size];
		// initialized in expand_key func
		//unsigned    rounds;
		static const unsigned rounds = 10;

		// expanded round keys
		unsigned     encryption_round_key[64], decryption_round_key[64];
			
		static  unsigned s_box[256];     ///< direct swap
		static  unsigned r_box[256];     ///< inderect swap
		static  unsigned ft[4][256];     ///< expanded direct swap
		static  unsigned rt[4][256];     ///< expanded inderect swap
    
		// swap table format
		void inline init_tables();
    
		/** 
			*  Generating polynom (basis) of field GF(2**8).
			*  defined as:     phi(x) = x**8 + x**4 + x**3 + x**1 + 1;
			*/
		static const unsigned   basis = 1<<8 | 1<<4 | 1<<3 | 1<<1 | 1<<0;

/** 
    *  Galua generator (Linear Feedback Shift Register)
    *  non-zero members of field GF(2**8) in the given basis.
    *  make multiplication by x and modulo by phi(x).
    */
byte static inline lfsr2(byte a);/// a < previuos state of generator

    //  cyclic ROL
unsigned static inline rot_left_8(unsigned value);

//  parse 4 bytes to unsigned
unsigned static inline get(const byte array[4]);

// parse unsigned to 4 bytes
void static inline put(unsigned  value, byte array[4]);

/** 
	*  auxilarry function of direct table swapping
    *  one of the bytes of unsigned int used as index 
    */
unsigned static inline
    fb(
    unsigned byte,           ///< word contains a byte.
    unsigned position   ///< position of the byte in word.
    );

/** 
	*  auxilarry function of indirect table swapping
    *  one of the bytes of unsigned int used as index 
    */
unsigned static inline
    rb(
    unsigned byte,           ///< word contains a byte.
    unsigned position   ///< position of the byte in word.
    );

};//class   aes
#endif // AES_H