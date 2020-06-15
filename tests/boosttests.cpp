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

BOOST_AUTO_TEST_CASE( Q2 ) {
	std::cout << std::endl;
	std::cout << "Q2 " << std::endl;
	unsigned int aliceNo = generateRandom() ;				//Alice pige un nombre aleatoire

	//sleep pour quelques secondes, le temps de laisser le generateur faire une cle differente.
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	unsigned int bobNo = generateRandom() ;					//Bob pige un nombre aleatoire 

	std::cout << "  AliceNo: " << aliceNo << std::endl;
	std::cout << "	BobNo: " << bobNo << std::endl;

	//sleep pour quelques secondes, le temps de laisser le generateur faire une cle differente.
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	//Bob envoie son numero a Charlie
	//Alice envoie son numero a Charlie 

	//Question 2.3
	/*
		La cle pkc est produite par l'algorithme d'exponentiation modulaire.
		Cet algorithme prend en entree les cles qui sont le resultat de l'exponentiation modulaire de la 
		"cle secrete partagee" ainsi que du numero que chacun a envoye. Malgre que l'algorithme n'aie pas de 
		deuxieme sortie, la cle secrete est tout de meme necessaire.

		la creation de la cle publique est en temps O(), 
		l'exponentiation etant en temps O(log2 n) puisque la boucle execute une fois pour chaque bit utilise et qu'il
		n'y a que 3 appels de fonctions, la creation de la cle publique est en temps O(3( log2 n)).

		Le contenu entre cert→A et cert→B est different :
			cert→A : 
			    - cSecretKey
				- charleshB
			cert→B : 
				- cSecretKey
				- charleshA

	*/
	unsigned int cSecretKey = generateRandom();
	unsigned int charleshA = modularExponent(cSecretKey, aliceNo, 63);
	unsigned int charleshB = modularExponent(cSecretKey, bobNo, 63);
	unsigned int cPublicKey = modularExponent(charleshA, charleshB, 63);

	std::cout << "	secretKey: " << cSecretKey << std::endl;
	std::cout << "	charleshA: " << charleshA << std::endl;
	std::cout << "	charleshB: " << charleshB << std::endl;
	std::cout << "	publicKey: " << cPublicKey << std::endl;

	//Charles envoie pour cert→A a Alice
	//Charles envoie pour cert→B a Bob

	// ----- Alice essaye d'envoyer un message a Bob 
	//Alice genere une cle pour l'encription
	unsigned int hA = modularExponent(cSecretKey, aliceNo, 63);
	std::cout << "	hA: " << hA << std::endl;
	//Alice genere la cle 
	unsigned int kA = modularExponent(charleshB, aliceNo, 63);			//Alice output la cle kA
	std::cout << "	kA: " << kA << std::endl;

	//Alice envoie le message encrypte

	//Bob genere la cle pour generer la cre qui decrypte le message
	unsigned int hB = modularExponent(cPublicKey, bobNo, 63);
	std::cout << "	hB: " << hB << std::endl;
	//Bob genere la cle
	unsigned int kB = modularExponent(charleshA, bobNo, 63);			//Alice output la cle kA
	std::cout << "	kB: " << kB << std::endl;

	//Puisque hB == hA, Bob est capable de decrypter le message.
	std::cout << std::endl;
	std::cout << "	kA: " << kA << " kB:" << kB << std::endl;
	

	//Question 2.1
	/*
		Secret : Eve ne peut pas calculer d'information sur le protocole puisqu'elle n'a pas acces a des cles.
		Eve ne voit que l'information encryptee voyager.

		Robuste && Integre : On s'assure ici qu'on obtient effectivement les meme cles a la sortie du protocole.
	*/
	BOOST_CHECK(kA == kB);

	/*
		Authentifie : Alice ayant acces a hB et Bob ayant acces a hA, tous deux peuvent verifier 
		que l'exponation modulaire de leur deux cles reunis forment la cle publique emise par Charles.
	*/
	//Alice verifie la correspondance avec la cle publique. 
	unsigned int verificationKey = modularExponent(hA, charleshB, 63);
	std::cout << "	publicKey: " << cPublicKey << " verificationKey:" << verificationKey << std::endl;

	//Question 2.2
	/* 
		Pour envoyer un message a Bob avec cette cle, Alice traitera son message avec la cle kA 
		en le passant dans un algorithme d'encription qui modifiera chaque bit du message selon 
		l'operation XOR en comparant la cle kA a la position appropriee. 
	*/
}