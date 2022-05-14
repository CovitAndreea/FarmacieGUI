#include "validator.h"

void Validator::ValidateMedicament(const Medicament& m) {
	std::vector <std::string> messages;
	if (m.GetDenumire().size() == 0) messages.push_back("Denumirea nu poate fi vida!");
	if (m.GetPret() < 0) messages.push_back("Pretul nu poate fi negativ!");
	if (m.GetProducator().size() == 0) messages.push_back("Producatorul nu poate fi vid!");
	if (m.GetSubstanta().size() == 0) messages.push_back("Substanta activa nu poate fi vida!");
	if (messages.size() > 0)
		throw ValidateException(messages); }

void Validator::ValidateAdaugare(const Medicament& m, const std::vector <Medicament> list) {
	std::vector <std::string> messages;
	for (auto& el : list)
		if (el.GetDenumire() == m.GetDenumire() &&
			el.GetProducator() == m.GetProducator())
			messages.push_back("Acest medicament deja exista!");
	if (messages.size() > 0)
		throw ValidateException(messages); }

void Validator::ValidateModificareStergere(std::string den, std::vector <Medicament> list) {
	for (auto& el : list)
		if (el.GetDenumire() == den) return;
	std::vector <std::string> messages;
	messages.push_back("Acest medicament nu exista!");
	throw ValidateException(messages); }

void Validator::TestAfisare() {
	std::vector <std::string> messages;
	messages.push_back("");
	throw ValidateException(messages); }

std::ostream& operator<<(std::ostream& out, const ValidateException& err) {
	for (const auto& message : err.messages)
		out << message << "\n";
	return out;
}

void TestValidator() {
	Validator val;
	Medicament m = Medicament("Diprogenta", "Bayer", "Nuj", -1);
	try { val.ValidateMedicament(m); }
	catch (ValidateException&) {
		assert(true);
		std::cout << "";
	}
	Repository repo;
	m = Medicament("Aspirina", "Bayer", "X", 5); repo.AddMedicament(m);
	try { val.ValidateAdaugare(m, repo.GetAll()); }
	catch (ValidateException&) {
		assert(true);
	}
	try { val.ValidateModificareStergere("Smth", repo.GetAll()); }
	catch (ValidateException&) {
		assert(true);
	}
	try { val.TestAfisare(); }
	catch (ValidateException& message) {
		std::cout << message;
	}
}
