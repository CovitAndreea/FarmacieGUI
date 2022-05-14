#include "repository.h"

void Repository::AddMedicament(const Medicament& m) {
	repo.push_back(m);
}

std::vector <Medicament>& Repository::GetAll() noexcept {
	return repo;
}

int Repository::UpdateMedicament(const std::string den, const Medicament& m) {
	const int i { DeleteMedicament(den) };
	if (i != -1)
		std::vector <Medicament> ::iterator it { repo.insert(repo.begin() + i, m) };
	return i;
}

int Repository::DeleteMedicament(const std::string den) {
	int i = 0;
	for (auto& el : repo) {
		if (el.GetDenumire() == den) {
			std::vector <Medicament> ::iterator it{ repo.erase(repo.begin() + i) };
			return i;
		}
		i++;
	}
	return -1;
}

void TestRepository() {
	Repository lista;
	lista.AddMedicament(Medicament("Aspirina", "Bayer", "X", 5));
	lista.AddMedicament(Medicament("Paracetamol", "Saniuta", "Y", 10));
	assert(lista.GetAll().size() == 2);

	Medicament m{ lista.GetAll()[0] };
	assert(m.GetDenumire() == "Aspirina");
	assert(m.GetPret() == 5);
	assert(m.GetProducator() == "Bayer");
	assert(m.GetSubstanta() == "X");

	lista.DeleteMedicament("Aspirina");
	assert(lista.GetAll().size() == 1);
	m = lista.GetAll()[0];
	assert(m.GetDenumire() == "Paracetamol");
	assert(m.GetPret() == 10);
	assert(m.GetProducator() == "Saniuta");
	assert(m.GetSubstanta() == "Y");

	lista.UpdateMedicament("Paracetamol", Medicament("Dicarbocalm", "Trestii", "Z", 7));
	m = lista.GetAll()[0];
	assert(m.GetDenumire() == "Dicarbocalm");
	assert(m.GetPret() == 7);
	assert(m.GetProducator() == "Trestii");
	assert(m.GetSubstanta() == "Z");

	assert(lista.DeleteMedicament("Aspirina") == -1);
}
