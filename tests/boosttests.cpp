#include "crypto.h"

#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <chrono>
#include <thread>
#include <string>

//Le generateur etant basé sur un intervalle de temps,
//on ne fait pas d'assertions mais on observe les valeurs différentes d'un test a un autre.
BOOST_AUTO_TEST_CASE( random_float_generator_test ) {
	std::cout << "random_float_generator_test" << std::endl;
	std::cout << "	valeur genere : " << generateRandom() << std::endl;
}

//P = 3 
//G = 63
//h = (3^G mod 2^P)
//h = (3^n mod 2^63)
BOOST_AUTO_TEST_CASE( computing_h_value_test ) {
	std::cout << "computing_h_value_test" << std::endl;

	unsigned int n = 3;
	unsigned int hValue = modularExponent(3, n, 63);
	std::cout << "	n = 3,     h value: " << hValue << " / should be: 27" <<  std::endl;
	BOOST_CHECK(hValue == 27);

	unsigned int n2 = 6;
	unsigned int hValue2 = modularExponent(3, n2, 63);
	std::cout << "	n=6,     h value: " << hValue2 << " / should be: 729" << std::endl;
	BOOST_CHECK(hValue2 == 729);

	unsigned int n3 = 20;
	unsigned int hValue3 = modularExponent(3, n3, 63);
	std::cout << "	n=20,     h value: " << hValue3 << " / should be: 3486784401" << std::endl;
	BOOST_CHECK(hValue3 == 3486784401);
}

BOOST_AUTO_TEST_CASE( Q1 ) {
	std::cout << "Q1" << std::endl;

	//1
	unsigned int aliceNo = generateRandom();				//Alice pige un nombre aleatoire
	unsigned int hA = modularExponent(3, aliceNo, 63);		//Alice calcule sa valeur de h
	std::cout << "	hA: " << hA << std::endl;				//"Envoie" hA a Bob

	//sleep pour quelques secondes, le temps de laisser le generateur faire une cle differente.
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	//2
	unsigned int bobNo = generateRandom();					//Bob pige un nombre aleatoire
	unsigned int hB = modularExponent(3, bobNo, 63);		//Bob calcule sa valeur de h
	std::cout << "	hB: " << hB << std::endl;				//"Envoie" hA a Alice

	//3 
	unsigned int kA = modularExponent(hB, aliceNo, 63);			//Alice output la cle kA

	//4
	unsigned int kB = modularExponent(hA, bobNo, 63);			//Bob output la cle kB

	//Question 1.1
	/*
		La relle difference se trouve dans le calcul de h
		Les cles sont effectivement les memes.
	*/
	std::cout << "	kA: " << kA << " kB:" << kB << std::endl;
	BOOST_CHECK(kA == kB);

	//Question 1.2 
	/*
		Si Eve intercepte une kA ainsi que kB, qui circulent en message clair, elle peut envoyer
		kA' ainsi que kB' avec sa propre cle. De cette facon, Alice et Bob croient communiquer ensemble alors
		qu'ils communiquent tout les deux avec Eve. Par contre, sans cle publique, elle ne peut rien faire.

		Pour s'assurer que la cle kA et la cle kB se rendent a bon port discretement, nous pourrions la passer
		dans un algorithme de RSA. 
	*/
}