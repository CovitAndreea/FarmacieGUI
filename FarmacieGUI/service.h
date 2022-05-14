#pragma once

#include "repository.h"
#include "validator.h"
#include <chrono>
#include <functional>
#include <fstream>
#include <memory>
#include <random>
#include <string>
using std::unique_ptr;

// Functie de comparare doua medicamente dupa un anumit criteriu
typedef bool(*functie) (const Medicament&, const Medicament&);

class Reteta {
	std::vector <Medicament> reteta;

public:
	// Constructor predefinit pentru reteta
	Reteta() = default;

	/*
		Functie de adaugare medicament pe reteta
		m - medicamentul de adaugat
	*/
	void AddMedicament(const Medicament& m) {
		reteta.push_back(m);
	}

	// Functie de GetAll reteta
	const std::vector<Medicament>& GetAll() const noexcept {
		return reteta;
	}

	// Functie de golire reteta
	void DeleteReteta() noexcept {
		reteta.clear();
	}
};

class Undo {
public:
	virtual void DoUndo() = 0;
	virtual void DoRedo() = 0;
	virtual ~Undo() = default;
};

class UndoAdd : public Undo {
	Medicament m;
	Repository& repo;
public:
	UndoAdd(Repository& repo, const Medicament& m) : repo{ repo }, m{ m } {}
	void DoUndo() override {
		repo.DeleteMedicament(m.GetDenumire());
	}
	void DoRedo() override {
		repo.AddMedicament(m);
	}
};

class UndoDelete : public Undo {
	Medicament m;
	Repository& repo;
public:
	UndoDelete(Repository& repo, const Medicament& m) noexcept : repo{ repo }, m{ m } {}
	void DoUndo() override {
		repo.AddMedicament(m);
	}
	void DoRedo() override {
		repo.DeleteMedicament(m.GetDenumire());
	}
};

class UndoUpdate : public Undo {
	Medicament m;
	std::string den;
	Repository& repo;
public:
	UndoUpdate(Repository& repo, std::string den, const Medicament& m) : repo{ repo }, den{ den }, m{ m } {};
	void DoUndo() override {
		repo.UpdateMedicament(den, m);
	}
	void DoRedo() override{
		;
	}
};

class Service {
	Repository& repo;
	Validator& val;
	Reteta ret;
	std::vector <unique_ptr<Undo>> undo;
	std::vector <unique_ptr<Undo>> redo;

	/*
		Functie de filtrare medicamente dupa un anumit camp
		f - campul dupa care se filtreaza
		Functia returneaza un vector de medicamente ce respecta conditia f
	*/
	std::vector <Medicament> Filter(std::function <bool(const Medicament&)> f) const;

	/*
		Functie de sortare generala medicamente dupa un anumit camp
		f - functia de comparare
		Functia returneaza un vector sortat de medicamente 
			dupa campul determinat de conditia f
	*/
	std::vector <Medicament> Sort(functie f) const;

public:
	/*
		Functie de creare service
		rep - repository medicamente
		postconditii: obiect de tip service
	*/
	Service(Repository& rep) noexcept : repo{ rep }, val{ val } {};
	Service(const Service& serv) = delete;

	/*
		Functie de adaugare medicament in lista
		m - medicament de adaugat
	*/
	void AddMedicament(const Medicament& m);

	//Functie de returnare toate medicamentele
	const std::vector <Medicament>& GetAll() const noexcept;

	/*
		Functie de actualizare medicament
		den - denumirea vechiului medicament
		m - noul medicament cu care se va inlocui
	*/
	int UpdateMedicament(const std::string den, const Medicament& m);

	/*
		Functie de stergere medicament din lista
		den - denumirea medicamentului de sters
		postconditii: element sters, indice pozitie returnat
	*/
	int DeleteMedicament(const std::string den);

	/*
		Functie de cautare medicament dupa denumire
		den - denumirea medicamentului de cautat
	*/
	int FindByDenumire(const std::string den);

	/*
		Functie de filtrare dupa pret mai mic de
		pr - pret de referinta
		Functia returneaza o lista ce reprezinta un subset de medicamente
			ce respecta conditia data
	*/
	std::vector <Medicament> FilterByPrice(int pr) const;

	/*
		Functie de filtrare dupa substanta activa
		subst - substanta de referinta
		Functia returneaza o lista ce reprezinta un subset de medicamente
			ce respecta conditia data
	*/
	std::vector <Medicament> FilterBySubstanta(const std::string subst);

	/*
		Functie de sortare medicamente dupa denumire
		Functia returneaza o lista ce reprezinta un subset de medicamente
			ce respecta conditia data
	*/
	std::vector <Medicament> SortByDenumire() const;

	/*
		Functie de sortare medicamente dupa producator
		Functia returneaza o lista ce reprezinta un subset de medicamente
			ce respecta conditia data
	*/
	std::vector <Medicament> SortByProducator() const;

	/*
		Functie de sortare medicamente dupa substanta activa si pret
		Functia returneaza o lista ce reprezinta un subset de medicamente
			ce respecta conditia data
	*/
	std::vector <Medicament> SortBySubstantaPret() const;

	/*
		Functie de adaugare medicament pe reteta
		den - denumirea medicamentului de adaugat
	*/
	const std::vector<Medicament>& AddReteta(const std::string& den);

	int CitesteNumar();

	/*
		Functie de populat aleator reteta
		n - cate elemente sa introduc
	*/
	const std::vector<Medicament>& Random(std::vector<Medicament> list);

	// Functie de returnat toate elementele din reteta
	const std::vector<Medicament>& GetAllReteta() noexcept;

	// Functie de sters reteta
	const std::vector<Medicament>& DeleteReteta() noexcept;

	// Functie de exportat in CVS
	void ExportCSV();

	// Functie de importat date din fisier
	void ImportFile();

	// Functie de exportat in fisier
	void ExportFile();

	// Functie Undo
	void Undo();

	void Redo();
};

// Functie de testare service
void TestService();
