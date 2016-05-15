#ifndef RSA_H
#define RSA_H
// Szyfrowanie asymetryczne (algorytm RSA)
	class RSA
	{
		friend class AES;
			static const unsigned long long n_lower_bound = 4000000000ULL;
			static const unsigned long long n_upper_bound = 4294967295ULL;
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
			bool getPPKey();
		
		public:
			static bool isKeyGenerated();
			// generowanie klucza publicznego i prywatnego
			static void generate();
			// szyfrowanie 
			static unsigned encrypt(unsigned source);
			// deszyfrowanie
			static unsigned decrypt(unsigned source);
			// szyfruj kluczem partnera
			unsigned encryptWithPPKey(unsigned source);
			// zwraca klucz publiczny (tzn. (e,n))
			static void getPublicKey(unsigned char ee[8], unsigned char nn[8]);
		

	};
#endif // RSA_H