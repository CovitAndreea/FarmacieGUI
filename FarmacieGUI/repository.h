#pragma once

#include "domain.h"
#include <algorithm>
#include <vector>

class Repository {
	std::vector <Medicament> repo;

public:
	Repository() = default;
	Repository(const Repository& rep) = delete;

	/*
		Functie de adaugare medicament in repository
		m - medicament de adaugat
	*/
	void AddMedicament(const Medicament& m);

	//Functie de returnare toate medicamentele
	std::vector <Medicament>& GetAll() noexcept;

	/*
		Functie de actualizare medicament
		den - denumirea vechiului medicament
		m - noul medicament cu care se va inlocui
	*/
	int UpdateMedicament(const std::string den, const Medicament& m);
	
	/*
		Functie de stergere medicament din repository
		den - denumirea medicamentului de sters
		postconditii: element sters, indice pozitie returnat
	*/
	int DeleteMedicament(const std::string den);
};

//Functie de testare repository
void TestRepository();
