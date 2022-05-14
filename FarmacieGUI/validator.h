#pragma once
#include <cctype>
#include <iostream>
#include <vector>
#include "repository.h"

class ValidateException {
	std::vector<std::string> messages;

public:
	/*
	Functie de initializare validator
	errors - eroors de afisat
	*/
	ValidateException(const std::vector<std::string>& errors) :
		messages{ errors } {};

	std::vector<std::string> GetMessages() {
		return messages;
	}

	friend std::ostream& operator<<(std::ostream& out, const ValidateException& err);
};

/*
Functie de overload al operatorului <<
out - streamul de scriere
err - eroarea ce se afiseaza
*/
std::ostream& operator<<(std::ostream& out, const ValidateException& err);

class Validator {
public:
	/*
	Functie de validare medicament
	m - medicamentul de validat
	*/
	void ValidateMedicament(const Medicament& m);

	/*
	Functie de validare adaugare medicament in repository
	m - medicamentul de validat
	list - repository de referinta
	*/
	void ValidateAdaugare(const Medicament& m, const std::vector <Medicament> list);

	/*
	Functie de validare modifica & sterge medicament din repository
	m - medicamentul de validat
	list - repository de referinta
	*/
	void ValidateModificareStergere(std::string den, std::vector <Medicament> list);

	// Functie de test afisare eroare
	void TestAfisare();
};

// Functie de testare validator
void TestValidator();
