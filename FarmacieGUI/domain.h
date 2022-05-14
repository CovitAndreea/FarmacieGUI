#pragma once

#include <assert.h>
#include <string>

class Medicament {
	std::string denumire;
	std::string producator;
	std::string substanta;
	int pret;

public:
	/*
		Functie de creare medicament
		den - denumirea medicamentului
		prod - producatorul medicamentului
		subst - substanta activa
		pr - pretul medicamentului
		postconditii: obiect de tip medicament
	*/
	Medicament(const std::string den, 
			   const std::string prod,
			   const std::string subst,
			   int pr) :
		denumire{ den },
		producator{ prod },
		substanta{ subst },
		pret{ pr }
	{};
	
	//Functie de returnare denumire medicament
	std::string GetDenumire() const;

	//Functie de returnare producator medicament
	std::string GetProducator() const;

	//Functie de returnare substanta activa medicament
	std::string GetSubstanta() const;

	//Functie de returnare pret medicament
	int GetPret() const noexcept;
};

//Functie de testare domain
void TestDomain();
