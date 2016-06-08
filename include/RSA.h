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
#include <string>
namespace EitiNotifications
{

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

			//unsigned partner_e;
			//unsigned partner_n;

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
			static int crypt(const char * given, char * &out, unsigned power, unsigned modul, int len);

		public:
			RSA();
			// preffered: e = 981967781U, d = 2245171601U, n = 4294894073U
			static void setNewKey(unsigned ee, unsigned dd, unsigned nn);
			static void setNewKey(); // for generating some random key
			// may be some padding; returns size of encrypted data
			static int encrypt(std::string msg, char *& out, unsigned ee=e, unsigned nn = n);
			// set length to 0 to automatically determine size of input data (do not use this feature!!!)
			static std::string decrypt(const char * BinaryInput, int length, unsigned dd=d, unsigned nn=n);

			static unsigned encrypt(unsigned given, unsigned ee=e, unsigned nn=n);
			static unsigned decrypt(unsigned given, unsigned ee=d, unsigned nn=n);

			static void getPublicKey(unsigned & ee, unsigned & nn);
	};

}
#endif // RSA_H
