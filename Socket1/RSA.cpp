#include "RSA.h"
#include "Util.h"
#include "Socket.h"
#include <random>

#include <cstdlib>
#include <iostream>

unsigned RSA::e; // składowa klucza publicznego
unsigned RSA::d; // składowa klucza prywatnego
unsigned RSA::n;
bool RSA::keyGenerated = false;

unsigned RSA::encrypt(unsigned given)
{
	return ModExp(given,e,n);
}

unsigned RSA::decrypt(unsigned given)
{
	return ModExp(given,d,n);
}

void RSA::generate()
{
	while(1){
		unsigned p, q = 1;
		unsigned long long mul;
		unsigned phi;
		bool ready = false;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<unsigned> pq(pq_lower_bound,pq_upper_bound);
		std::uniform_int_distribution<unsigned> e_key(e_lower_bound,e_upper_bound);
		do{ // generate pseudorandom p and q
			p = pq(gen);
			//if(!p&1) ++p;
			//if(p%2==0) ++p;
			p|=1;
			
			while(!is_prime(p)) p+=2;
			
			mul = p*q;
			// check modulo= p*q
			if(ready && (mul > n_upper_bound || mul < n_lower_bound ))
			{
				ready = false;
				q = 1;
				continue;
			}
			if(ready) break;
			ready = true;
			q = p;
		}while(1); // generate pseudorandom p and q

		n = p*q;
		
		phi = (p-1)*(q-1);
		// find prime number in field phi - e (e is odd)
		e = e_key(gen)%(phi);
		if(e%2 == 0) ++e;
		while(!gcd_1(e,phi)) e=(e+2)%(phi);
		// e was found
		// find inversion of e in field phi - d
		d = getInverse(e, phi);
		// some output and testing;
		std::cout<<"p = " << p << ", \tq = " << q << ", \tn = " << n << std::endl;
		std::cout<<"e = " << e << ", d = " << d <<", phi = " << phi << std::endl;
		// since this place p q phi are used as temporary variables
		// start testing the key
		p=201;
		q=ModExp(p,e,n);
		phi=ModExp(q,d,n);
	
		if(p!=phi)
		{
			std::cout<<"Wrong!" << std::endl;
			continue;
		}
		
		// test rsa key
		for(unsigned i=1; i<=10; i++)
		{
			p+=p*i;
			q=ModExp(p,e,n);
			phi=ModExp(q,d,n);
	
			if(p!=phi) break;
		}
		if(p!=phi) continue;
		else break;
		break;
	}

	keyGenerated = true;
}

void RSA::getPublicKey(unsigned char * ee, unsigned char * nn)
{
	Util::uint32ToHEX(e, ee);
	Util::uint32ToHEX(n, nn);
}

bool RSA::isKeyGenerated()
{
	return keyGenerated;
}

unsigned RSA::getInverse(unsigned b, unsigned m)
{
	long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;
	long long a = (long long)b;
    if (m == 1)
      return 0;
 
    while (a > 1)
    {
        // q is quotient
        q = a / m;
 
        t = m;
 
        // m is remainder now, process same as
        // Euclid's algo
        m = a % m, a = t;
 
        t = x0;
 
        x0 = x1 - q * x0;
 
        x1 = t;
    }
 
    // Make x1 positive
    if (x1 < 0)
       x1 += m0;
 
    return (unsigned int)x1;
}

bool RSA::gcd_1(unsigned a, unsigned b)
{
	unsigned temp;

	while(b != 0)
	{
		temp = b;
		b = a % b;
		a = temp;
	};
	return (a == 1);
}

/*
 * Miller Rabin pseudorandom test as described on page 8 at:
 * http://cacr.uwaterloo.ca/hac/about/chap4.pdf
 */
bool RSA::is_prime(unsigned n)
{
	int t = 2;
	unsigned s = 0;
	unsigned n_1 = n-1;
	unsigned r = n_1;
	unsigned y, a;
	// check https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test#Deterministic_variants
	// if n < 1,373,653, it is enough to test a = 2 and 3
	unsigned a_array[] = {2, 3};
	// 1. Write n-1 = 2^s*r such that r is odd
	while(!r&1) // do when r is even
	{
		++s;
		r>>=1;
	}
	// 2. For i from 1 to t do the following
	for(int i = 0; i<t; ++i)
	{
		// Choose a random integer a, 2<=a<=n-2
		// select a from a_array
		a = a_array[i];
		// Compute  y = a^r mod n
		y = ModExp(a,r,n);
		if(y!=1 && y!=n_1)
		{
			for(unsigned j = 1; j<s && y!=n_1; ++j)
			{
				//y = y^2 mod n;
				y = ModExp(y,2,n);
				if(y==1) return false;
			}
			if(y!=n_1) return false;
		}
	}
	// 3. Miller-Rabin probabilistic primality test passed
	return true;
}

/* This function calculates (a^b)%c */
unsigned RSA::ModExp(unsigned a,unsigned b,unsigned c){
    unsigned long long x=1,y=a; // long long is taken to avoid overflow of intermediate results 
	while(b > 0){
        if(b%2 == 1){
            x=(x*y)%c;
        }
        y = (y*y)%c; // squaring the base
        b /= 2;
    }
    return x%c;
}