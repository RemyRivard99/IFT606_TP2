#include "crypto.h"
#include <ctime>
#include <string>
#include <chrono>
#include <thread>
#include <random>
#include <iostream>

//Genere un nombre entre 0 et 2^32
unsigned int generateRandom() {
	srand(time(NULL));
	unsigned int n = (unsigned int)rand();
	return n;
}

//Calcule la cle h, tel que : hx = 3^P (mod 2^G)
unsigned int modularExponent(unsigned int number, unsigned int power, unsigned int binaryModuloExponent) {
	unsigned int result = 1;

	unsigned long long modulo = 2;
	//computing modulo value 
	for (int i = 0; i < binaryModuloExponent - 1; i++) {
		modulo = modulo * 2; 
		//std::cout << " loop: " << modulo << std::endl;
	}

	//std::cout << "2e63: " << modulo << std::endl; 

	return expLoop(number, power, modulo, result);
}

/*
* On calcule l'exponentiation a l'aide des bits de puissance. L'implementation est donc en O(log n).
* On calcule le reste pour chaque bit "1", et on l'ajoute au résultat final.
* Il faut incrémenter la base de façon exponentielle pour représenter correctement les puissances a chaque recursion.
*/
unsigned int expLoop(unsigned int number, unsigned int power, unsigned long long modulo, unsigned int result) {
	if (power <= 0)
		return result;
	
	if (power & 1) {
		result = (result * number) % modulo;
	}
	int base = (number * number);
	power >>= 1;
	
	//std::cout << "valeur de loop : { base: " << number << " power: " << power << " result: " << result << " }" << std::endl;

	return expLoop(base, power, modulo, result);
}