#include "service.h"

void Service::AddMedicament(const Medicament& m) {
	val.ValidateMedicament(m);
	val.ValidateAdaugare(m, GetAll());
	repo.AddMedicament(m);
	undo.push_back(std::make_unique<UndoAdd>(repo, m));
	redo.clear();
}

const std::vector <Medicament>& Service::GetAll() const noexcept {
	return repo.GetAll();
}

int Service::UpdateMedicament(const std::string den, const Medicament & m) {
	for (auto& el : repo.GetAll())
		if (el.GetDenumire() == den) {
			undo.push_back(std::make_unique<UndoUpdate>(repo, m.GetDenumire(), el));
			redo.clear();
			break;
		}
	return repo.UpdateMedicament(den, m);
}

int Service::DeleteMedicament(const std::string den) {
	for (auto& el : repo.GetAll())
		if (el.GetDenumire() == den) {
			undo.push_back(std::make_unique<UndoDelete>(repo, el));
			break; }
	redo.clear();
	return repo.DeleteMedicament(den);
}

int Service::FindByDenumire(const std::string den) {
	int i = 0;
	for (auto& el : repo.GetAll()) {
		if (el.GetDenumire() == den)
			return i;
		i++;
	}
	return -1;
}

std::vector <Medicament> Service::Filter(std::function <bool(const Medicament&)> f) const {
	std::vector <Medicament> result;
	std::copy_if(repo.GetAll().begin(), repo.GetAll().end(), std::back_inserter(result), f);
	return result;
}

std::vector <Medicament> Service::FilterByPrice(int pr) const {
	return Filter([pr](const Medicament& m) noexcept {
		return m.GetPret() <= pr;
	});
}

std::vector <Medicament> Service::FilterBySubstanta(const std::string subst) {
	return Filter([subst](const Medicament& m) noexcept {
		return m.GetSubstanta().find(subst) != std::string::npos || subst.find(m.GetSubstanta()) != std::string::npos;
	});
}

std::vector <Medicament> Service::Sort(functie f) const {
	std::vector <Medicament> result{ repo.GetAll() };
	if (*f != NULL)
		std::sort(result.begin(), result.end(), f);
	return result;
}

std::vector <Medicament> Service::SortByDenumire() const {
	return Sort([](const Medicament& m1, const Medicament& m2) {
		return m1.GetDenumire() < m2.GetDenumire();
	});
}

std::vector <Medicament> Service::SortByProducator() const {
	return Sort([](const Medicament& m1, const Medicament& m2) {
		return m1.GetProducator() < m2.GetProducator();
	});
}

std::vector<Medicament> Service::SortBySubstantaPret() const {
	return Sort([](const Medicament& m1, const Medicament& m2) {
		if (m1.GetSubstanta() == m2.GetSubstanta())
			return m1.GetPret() < m2.GetPret();
		return m1.GetSubstanta() < m2.GetSubstanta();
	});
}

const std::vector<Medicament>& Service::AddReteta(const std::string& den) {
	const int i = FindByDenumire(den);
	ret.AddMedicament(repo.GetAll()[i]);
	return ret.GetAll();
}

int Service::CitesteNumar() {
	std::ifstream in("random.in");
	int n;
	in >> n;
	return n;
}

const std::vector<Medicament>& Service::Random(std::vector<Medicament> list) {
	const int n = CitesteNumar();
	std::shuffle(list.begin(), list.end(), std::default_random_engine(std::random_device{}()));
	while (GetAllReteta().size() < n && list.size() > 0) {
		this->ret.AddMedicament(list.back());
		list.pop_back();
	}
	return ret.GetAll();
}

const std::vector<Medicament>& Service::GetAllReteta() noexcept {
	return ret.GetAll();
}

const std::vector<Medicament>& Service::DeleteReteta() noexcept {
	ret.DeleteReteta();
	return ret.GetAll();
}

void Service::ExportCSV() {
	std::ofstream out("export.csv");
	for (auto& el : GetAllReteta()) {
		out << el.GetDenumire() << "," << el.GetPret() << "," << el.GetProducator() << "," << el.GetSubstanta() << '\n';
	}
	out.close();
}

void Service::ImportFile() {
	std::ifstream in("medicamente.txt");
	repo.GetAll().clear();
	char line[256]{ '\0' }, *next_token = NULL;
	while (in.getline(line, 256)) {
		char* pcs = strtok_s(line, ",", &next_token);
		std::string den = pcs;
		pcs = strtok_s(NULL, ",", &next_token);
		const int pr = atoi(pcs);
		pcs = strtok_s(NULL, ",", &next_token);
		std::string prod = pcs;
		pcs = strtok_s(NULL, "\n", &next_token);
		std::string subst = pcs;
		repo.AddMedicament(Medicament(den, prod, subst, pr));
	}
}

void Service::ExportFile() {
	remove("medicamente.txt");
	std::ofstream out("medicamente.txt");
	for (auto& el : repo.GetAll()) {
		out << el.GetDenumire() << ",";
		out << el.GetPret() << ",";
		out << el.GetProducator() << ",";
		out << el.GetSubstanta() << "\n";
	}
}

void Service::Undo() {
	if (undo.empty()) {
		std::vector <std::string> messages;
		messages.push_back("Nu mai exista operatii!");
		throw ValidateException(messages); }
	undo.back()->DoUndo();
	redo.push_back(std::move(undo.back()));
	undo.pop_back();
}

void Service::Redo() {
	if (redo.size() == 0) return;
	redo.back()->DoRedo();
	undo.push_back(std::move(redo.back()));
	redo.pop_back();
}

void TestService() {
	Repository lista;
	Service serv{ lista };
	serv.AddMedicament(Medicament("Aspirina", "Bayer", "X", 5));

	serv.Random(serv.GetAll());
	assert(serv.GetAllReteta().size() == 1);
	serv.ExportCSV();
	serv.DeleteReteta();
	assert(serv.GetAllReteta().size() == 0);

	serv.AddMedicament(Medicament("Nurofen", "Sant", "U", 10));
	serv.AddMedicament(Medicament("Diclofenac", "Ceva", "T", 20));
	assert(serv.GetAll().size() == 3);

	serv.AddReteta("Aspirina");
	assert(serv.GetAllReteta().size() == 1);
	serv.DeleteReteta();

	Medicament m{ serv.GetAll()[0] };
	assert(m.GetDenumire() == "Aspirina");
	assert(m.GetPret() == 5);
	assert(m.GetProducator() == "Bayer");
	assert(m.GetSubstanta() == "X");

	int i = serv.FindByDenumire("Aspirina");
	assert(i == 0);
	i = serv.FindByDenumire("Paracetamol");
	assert(i == -1);

	auto result = serv.FilterByPrice(10);
	assert(result.size() == 2);

	m = result[1];
	assert(m.GetDenumire() == "Nurofen");
	assert(m.GetPret() == 10);
	assert(m.GetProducator() == "Sant");
	assert(m.GetSubstanta() == "U");

	result = serv.FilterBySubstanta("X");
	assert(result.size() == 1);
	m = result[0];
	assert(m.GetDenumire() == "Aspirina");
	assert(m.GetPret() == 5);
	assert(m.GetProducator() == "Bayer");
	assert(m.GetSubstanta() == "X");

	Repository lista2;
	Service serv2{ lista2 };
	serv2.AddMedicament(Medicament("Aspirina", "Bayer", "X", 5));
	serv2.AddMedicament(Medicament("Nurofen", "Sant", "U", 10));
	serv2.AddMedicament(Medicament("Diclofenac", "Ceva", "X", 4));

	auto result2 = serv2.SortByDenumire();
	m = result2[1];
	assert(m.GetDenumire() == "Diclofenac");
	assert(m.GetPret() == 4);
	assert(m.GetProducator() == "Ceva");
	assert(m.GetSubstanta() == "X");

	result2 = serv2.SortByProducator();
	m = result2[1];
	assert(m.GetDenumire() == "Diclofenac");
	assert(m.GetPret() == 4);
	assert(m.GetProducator() == "Ceva");
	assert(m.GetSubstanta() == "X");

	result2 = serv2.SortBySubstantaPret();
	m = result2[1];
	assert(m.GetDenumire() == "Diclofenac");
	assert(m.GetPret() == 4);
	assert(m.GetProducator() == "Ceva");
	assert(m.GetSubstanta() == "X");

	Repository lista3;
	Service serv3{ lista3 };
	serv3.AddMedicament(Medicament("Aspirina", "Bayer", "X", 5));
	serv3.AddMedicament(Medicament("Nurofen", "Sant", "U", 10));
	serv3.AddMedicament(Medicament("Diclofenac", "Ceva", "X", 4));

	assert(serv.UpdateMedicament("Dicarbocalm", Medicament("Aspirina", "Bayer", "X", 5)) == -1);
	serv.UpdateMedicament("Aspirina", Medicament("Ceva", "Altceva", "A", 100));
	m = serv.GetAll()[0];
	assert(m.GetDenumire() == "Ceva");
	assert(m.GetPret() == 100);
	assert(m.GetProducator() == "Altceva");
	assert(m.GetSubstanta() == "A");

	assert(serv.DeleteMedicament("Dicarbocalm") == -1);
	serv.DeleteMedicament("Diclofenac");
	assert(serv.GetAll().size() == 2);

	int n = serv.CitesteNumar();
	n = 0;

	Repository lista7;
	Service serv7{ lista7 };
	try {
		serv7.Undo();
	}
	catch (ValidateException&) {};
	serv7.AddMedicament(Medicament("Aspirina", "Bayer", "X", 5));
	serv7.Undo();
	assert(serv7.GetAll().size() == 0);
	serv7.AddMedicament(Medicament("Aspirina", "Bayer", "X", 5));
	serv7.DeleteMedicament("Aspirina");
	serv7.Undo();
	assert(serv7.GetAll().size() == 1);
	serv7.UpdateMedicament("Aspirina", Medicament("Nurofen", "Sant", "U", 10));
	serv7.Undo();
	assert(serv7.GetAll().size() == 1);

	// INCEPE TESTUL SUPREM
	Repository lista14;
	Service serv14{ lista14 };
	serv14.AddMedicament(Medicament("Aspirina", "Bayer", "X", 5));
	serv14.AddMedicament(Medicament("Nurofen", "Sant", "U", 10));
	assert(serv14.GetAll().size() == 2);
	serv14.Undo(); serv14.Undo();
	assert(serv14.GetAll().size() == 0);
	serv14.Redo(); serv14.Redo();
	assert(serv14.GetAll().size() == 2);
	serv14.Undo();
	assert(serv14.GetAll().size() == 1);

	//============================================================

	serv14.Redo();
	assert(serv14.GetAll().size() == 2);
	serv14.Redo();
	assert(serv14.GetAll().size() == 2);
	serv14.Undo();
	assert(serv14.GetAll().size() == 1);
	serv14.AddMedicament(Medicament("Diclofenac", "Ceva", "X", 4));
	serv14.Redo();
	assert(serv14.GetAll().size() == 2);
	serv14.Undo(); serv14.Undo();
	assert(serv14.GetAll().size() == 0);
	serv14.Redo(); serv14.Redo();
	assert(serv14.GetAll().size() == 2);
}
