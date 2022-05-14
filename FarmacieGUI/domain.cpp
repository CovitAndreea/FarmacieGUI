#include "domain.h"

std::string Medicament::GetDenumire() const {
	return denumire;
}

std::string Medicament::GetProducator() const {
	return producator;
}

std::string Medicament::GetSubstanta() const {
	return substanta;
}

int Medicament::GetPret() const noexcept {
	return pret;
}

void TestDomain() {
	Medicament m = Medicament("Aspirina", "Bayer", "X", 5);
	assert(m.GetDenumire() == "Aspirina");
	assert(m.GetPret() == 5);
	assert(m.GetProducator() == "Bayer");
	assert(m.GetSubstanta() == "X");
}
