#ifndef RSA_H
#define RSA_H
// Szyfrowanie asymetryczne (algorytm RSA)
	class RSA
	{
	public:
			static const unsigned long long n_lower_bound = 3500000000ULL;
			static const unsigned long long n_upper_bound = 4294000000ULL;
			static const unsigned pq_lower_bound = 20000U;
			static const unsigned pq_upper_bound = 200000U;
			static const unsigned e_lower_bound = 10000U;
			static const unsigned e_upper_bound = 2000000000U;
			static unsigned e; // składowa klucza publicznego
			static unsigned d; // składowa klucza prywatnego
			static unsigned n; // key module
			static bool keyGenerated;

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
			// 
			void crypt(const char * given, char * &out, unsigned power, unsigned modul);
			//void crypt(unsigned sorce, char * &out, unsigned power, unsigned modul);
			//void crypt1(const char * source, char * out, unsigned power, unsigned modul, int len); // length in bytes
			//void crypt1(unsigned sorce, char * out, unsigned power, unsigned modul);
		
		public:
			RSA();
			static bool isKeyGenerated();
			// generowanie klucza publicznego i prywatnego
			static void generate();
			// szyfrowanie 
			static unsigned encrypt(unsigned source);
			static unsigned decrypt(char * source);
			static unsigned decrypt(unsigned source);
			// szyfruj kluczem partnera
			void encryptWithPPKey(const char * source, char * &out);
			//
			void decrypt(char * source, char * &out);
			// 
			void getPublicKey(char ee[8]);
			// 
			static void pushPublicKey(char * out);
		

	};
#endif // RSA_H
