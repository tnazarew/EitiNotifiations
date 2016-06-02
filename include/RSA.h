#ifndef RSA_H
#define RSA_H
/**********************************************************
 * Server usage:
 * 1. Create instance of RSA
 * 2. Get client public key via getPartnerPublicKey() method
 * 3. Encrypt message via encryptWithPPKey() mehod
 * 4. Send encrypted message
 * Client usage:
 * 1. Generate RSA key pair via generate() method
 * 2. Convert to string public key via pushPublicKey() method
 * 3. Send Public key to server
 * 4. Get encrypted message; decrypt it by decrypt() method
 ***********************************************************/

	class RSA
	{
			static const unsigned long long n_lower_bound = 3500000000ULL;
			static const unsigned long long n_upper_bound = 4294000000ULL;
			static const unsigned pq_lower_bound = 20000U;
			static const unsigned pq_upper_bound = 200000U;
			static const unsigned e_lower_bound = 10000U;
			static const unsigned e_upper_bound = 2000000000U;
			static unsigned e; // składowa klucza publicznego
			static unsigned d; // składowa klucza prywatnego
			static unsigned n; // key module

			unsigned partner_e;
			unsigned partner_n;
				
			// Sprawdza czy NWD dla liczb a i b jest równy 1
			static bool gcd_1(unsigned a, unsigned b);
			// obliczenie liczby odwrotnej dla liczby e w pierścieniu ℕϕ(n)
			// zwraca składowę klucza prywatnego
			// phi jest porządku n
			static unsigned getInverse(unsigned e,unsigned phi);
			// potęgowanie moduło
			static unsigned ModExp(unsigned base, unsigned exponent, unsigned mod);
			// Miller Rabin test
			static bool is_prime(unsigned prime);
			// szyfrowanie
			static void crypt(const char * given, char * &out, unsigned power, unsigned modul, int len);
		
		public:
			RSA();
			static void generate();
			// szyfrowanie kluczem publicznym (para (e,n))
			static void encrypt(const char * source, char * &out, int len=0);
			static unsigned encrypt(unsigned source);
			//szyfrowanie kluczem prywatnym (para (d,n))
			static void decrypt(const char * source, char * &out, int len=0);
			static unsigned decrypt(unsigned source);
			// szyfruj kluczem partnera
			void encryptWithPPKey(const char * source, char * &out, int len=0);
			unsigned encryptWithPPKey(unsigned source);
			static void pushPublicKey(char * out);
			void getPartnerPublicKey(char * in);
		

	};
#endif // RSA_H
