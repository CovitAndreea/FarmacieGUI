#include "console.h"

void Console::print(const std::vector <Medicament>& medicamente) const {
	for (auto& el : medicamente) {
		std::cout << "Denumire: " << el.GetDenumire() << '\n';
		std::cout << "Pret: " << el.GetPret() << '\n';
		std::cout << "Producator: " << el.GetProducator() << '\n';
		std::cout << "Substanta activa: " << el.GetSubstanta() << '\n';
		std::cout << '\n';
	}
}

void Console::start() {
	std::cout << "Aplicatie gestiune farmacie\n";
	std::cout << "1. Adaugare medicament\n";
	std::cout << "2. Stergere medicament\n";
	std::cout << "3. Modificare medicament\n";
	std::cout << "4. Cautare medicament\n";
	std::cout << "5. Afisare medicamente\n";
	std::cout << "6. Filtrare medicamente\n";
	std::cout << "7. Sortare medicamente\n";
	std::cout << "8. Adaugare medicament in reteta\n";
	std::cout << "9. Generare aleatoare reteta\n";
	std::cout << "10. Golire reteta\n";
	std::cout << "11. Afisare reteta\n";
	std::cout << "12. Export reteta in CSV\n";
	std::cout << "13. Undo\n";
	std::cout << "14. Redo\n";
	std::cout << "0. Exit\n";
	std::cout << '\n';
	int cmd = 0;

	do {
		std::cout << "Dati comanda: "; std::cin >> cmd; std::cout << '\n';

		try {
			serv.ImportFile();

			if (cmd == 1) {
				std::string den, prod, subst; int pr;
				std::cout << "Dati denumirea: "; std::cin >> den;
				std::cout << "Dati producatorul: "; std::cin >> prod;
				std::cout << "Dati substanta activa: "; std::cin >> subst;
				std::cout << "Dati pretul: "; std::cin >> pr;
				std::cout << '\n';
				serv.AddMedicament(Medicament(den, prod, subst, pr));
			}

			if (cmd == 2) {
				std::string den;
				std::cout << "Dati denumirea: "; std::cin >> den;
				if (serv.DeleteMedicament(den) == -1)
					std::cout << "Nu exista acest medicament!\n";
				std::cout << '\n';
			}

			if (cmd == 3) {
				std::string denOld, denNew, prodNew, substNew; int prNew;
				std::cout << "Dati denumirea veche: "; std::cin >> denOld;
				std::cout << "Dati denumirea noua: "; std::cin >> denNew;
				std::cout << "Dati producatorul nou: "; std::cin >> prodNew;
				std::cout << "Dati substanta activa noua: "; std::cin >> substNew;
				std::cout << "Dati pretul nou: "; std::cin >> prNew;
				std::cout << '\n';
				if (serv.UpdateMedicament(denOld, Medicament(denNew, prodNew, substNew, prNew)) == -1)
					std::cout << "Nu exista acest medicament!\n\n";
			}

			if (cmd == 4) {
				std::string den;
				std::cout << "Dati denumirea: "; std::cin >> den;
				const int i = serv.FindByDenumire(den);
				if (i != -1) {
					std::cout << "Pret: " << serv.GetAll()[i].GetPret() << '\n';
					std::cout << "Producator: " << serv.GetAll()[i].GetProducator() << '\n';
					std::cout << "Substanta activa: " << serv.GetAll()[i].GetSubstanta() << '\n';
				}
				else std::cout << "Nu exista acest medicament!\n";
				std::cout << '\n';
			}

			if (cmd == 5) print(serv.GetAll());

			if (cmd == 6) {
				std::cout << "1. Dupa pret\n";
				std::cout << "2. Dupa substanta activa\n";
				std::cout << '\n';
				std::cout << "Dati comanda: "; std::cin >> cmd; std::cout << '\n';

				if (cmd == 1) {
					int pr = 0;
					std::cout << "Dati pretul: "; std::cin >> pr; std::cout << '\n';
					print(serv.FilterByPrice(pr));
				}

				if (cmd == 2) {
					std::string subst;
					std::cout << "Dati substanta activa: "; std::cin >> subst; std::cout << '\n';
					print(serv.FilterBySubstanta(subst));
				}
			}

			if (cmd == 7) {
				std::cout << "1. Dupa denumire\n";
				std::cout << "2. Dupa producator\n";
				std::cout << "3. Dupa substanta activa + pret\n";
				std::cout << '\n';
				std::cout << "Dati comanda: "; std::cin >> cmd; std::cout << '\n';

				if (cmd == 1) print(serv.SortByDenumire());
				if (cmd == 2) print(serv.SortByProducator());
				if (cmd == 3) print(serv.SortBySubstantaPret());
			}

			if (cmd == 8) {
				std::string den;
				std::cout << "Dati denumirea: "; std::cin >> den;
				std::cout << '\n';
				serv.AddReteta(den);
				std::cout << "Exista " << serv.GetAllReteta().size() << " medicamente in reteta. \n\n";
			}

			if (cmd == 9) {
				serv.Random(serv.GetAll());
				std::cout << "Exista " << serv.GetAllReteta().size() << " medicamente in reteta. \n\n";
			}

			if (cmd == 10) {
				serv.DeleteReteta();
				std::cout << "Exista " << serv.GetAllReteta().size() << " medicamente in reteta. \n\n";
			}

			if (cmd == 11) {
				std::cout << "Exista " << serv.GetAllReteta().size() << " medicamente in reteta: \n";
				print(serv.GetAllReteta());
			}

			if (cmd == 12) {
				serv.ExportCSV();
			}

			if (cmd == 13) {
				serv.Undo();
			}

			if (cmd == 14) {
				serv.Redo();
			}

			serv.ExportFile();
		}
		catch (ValidateException& message) {
			std::cout << message << '\n';
		}
		
	} while (cmd);
}
