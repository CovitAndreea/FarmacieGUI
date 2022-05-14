#pragma once

#include "service.h"

class Console {
	Service& serv;

public:
	/*
		Functie de creare UI
		serv - service medicamente
		postconditii: obiect de tip console
	*/
	Console(Service& serv) noexcept: serv { serv } {};
	Console(const Console& ui) = delete;

	/*
		Functie de tiparire lista de medicamente
		medicamente - lista de medicamente
	*/
	void print(const std::vector <Medicament>& medicamente) const;

	// Functie de pornire UI
	void start();
};
