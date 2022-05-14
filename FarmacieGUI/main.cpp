#include <QtWidgets/QApplication>
#include "domain.h"
#include "repository.h"
#include "validator.h"
#include "service.h"
#include "console.h"
#include "gui.h"

void TestAll() {
	TestDomain();
	TestRepository();
	TestValidator();
	TestService();
}

int main(int argc, char* argv[]) {
	//TestAll();

	QApplication a(argc, argv);
	Repository repo;
	Service serv { repo };
	Console ui { serv };
	serv.ImportFile();
	serv.ExportFile();

	//ui.start();
	GUI gui(serv);
	gui.show();

	return a.exec();
}
