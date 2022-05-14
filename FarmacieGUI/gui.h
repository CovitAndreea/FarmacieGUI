#pragma once
#include <QtWidgets/QApplication>
#include <QHBoxLayout>
#include <QListWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QDialog>
#include <QColor>
#include <QPainter>
#include <sstream>
#include "MyModel.h"
#include "service.h"

class Observer {
public:
	virtual void update() = 0;
};

class RetetaGUI : public Observer {
	QWidget *retetaWindow = new QWidget();
	QVBoxLayout *retetaLayout = new QVBoxLayout();
	QPushButton * randomReteta = new QPushButton("Generare aleatoare reteta");
	QPushButton * golireReteta = new QPushButton("Golire reteta");
	QTableView *retetaList = new QTableView();
	MyTableModel *retetaTableModel = new MyTableModel();
	Service& serv;
	std::vector<Observer*>& observers;



public:
	RetetaGUI(Service& serv, std::vector<Observer*>& observers) : serv{ serv }, observers{ observers } {
		Connections();
		retetaWindow->setLayout(retetaLayout);
		retetaLayout->addWidget(retetaList);
		retetaLayout->addWidget(randomReteta);
		retetaLayout->addWidget(golireReteta);
		retetaList->setModel(retetaTableModel);
		update();
		retetaWindow->show();
	};
	void Connections() {
		QObject::connect(randomReteta, &QPushButton::clicked, [=]() {
			serv.Random(serv.GetAll());
			for (auto& o : observers)
				o->update();
		});
		QObject::connect(golireReteta, &QPushButton::clicked, [=]() {
			serv.DeleteReteta();
			for (auto& o : observers)
				o->update();
		});
	}
	void update() override {
		/*
			retetaList->clear();
			retetaList->setHorizontalHeaderLabels(QStringList() << "Denumire" << "Producator" << "Substanta activa" << "Pret");
			retetaList->setRowCount(serv.GetAllReteta().size());
			int r = 0; char numar[10];
			for (auto& m : serv.GetAllReteta()) {
				retetaList->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(m.GetDenumire())));
				retetaList->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(m.GetProducator())));
				retetaList->setItem(r, 2, new QTableWidgetItem(QString::fromStdString(m.GetSubstanta())));
				retetaList->setItem(r, 3, new QTableWidgetItem(QString::fromStdString(itoa(m.GetPret(), numar, 10))));
				r++;
			}
		}
		*/
		retetaTableModel->setMedicamente(serv.GetAllReteta());
	}
};

class RetetaGUIRO : public Observer, public QWidget {
	QVBoxLayout *retetaLayout = new QVBoxLayout();
	Service& serv;
	std::vector<Observer*>& observers;
public:
	RetetaGUIRO(Service& serv, std::vector<Observer*>& observers) : serv{ serv }, observers{ observers } {
		this->setLayout(retetaLayout);
		this->setFixedHeight(512);
		this->setFixedWidth(512);
		update();
		this->show();
	}
	void paintEvent(QPaintEvent* ev) override {
		QPainter p{ this };
		int n = serv.GetAllReteta().size();
		for (int i = 0; i < n; i++) {
			std::random_device rdm;
			std::mt19937 eng{ rdm() };
			std::uniform_int_distribution<int> random(0, 1);
			int r = random(eng);
			int x = rand() % 512, y = rand() % 512;
			if (r == 0) p.drawLine(rand() % 512, rand() % 512, rand() % 512 - x, rand() % 512 - y);
			if (r == 1) p.drawRect(rand() % 512, rand() % 512, rand() % 512 - x, rand() % 512 - y);
		}
	}
	void update() override {
		this->repaint();
	}
};

class RetetaGUIGrid : public Observer, public QWidget {
	QVBoxLayout *retetaLayout = new QVBoxLayout();
	Service& serv;
	std::vector<Observer*>& observers;
public:
	RetetaGUIGrid(Service& serv, std::vector<Observer*>& observers) : serv{ serv }, observers{ observers } {
		this->setLayout(retetaLayout);
		this->setFixedHeight(512);
		this->setFixedWidth(512);
		update();
		this->show();
	}
	void formaRandom(QPainter & p, int i, int j)
	{
		std::random_device rdm;
		std::mt19937 eng{ rdm() };
		std::uniform_int_distribution<int> random(0, 2);
		int forma = random(eng);

		if (forma == 0) {
			p.drawRect(j * 150, i * 150, 150, 150);
		}
		if (forma == 1) {
			p.drawLine(j * 150, i * 150, (j + 1) * 150, (i + 1) * 150);
		}
		if (forma == 2) {
			p.drawEllipse(j * 150, i * 150, 150, 150);
		}
	}
	void paintEvent(QPaintEvent* ev) override {
		QPainter p{ this };
		int n = serv.GetAllReteta().size();
		if (n != 0) {
			setFixedHeight(((n + (n % 2)) / 2) * 150);
		}
		for (int i = 0, cnt = 0; cnt < n; i++) {
			for (int j = 0; j < 2 && cnt < n; j++) {
				formaRandom(p, i, j);
				cnt++;
			}
		}
	}
	void update() override {
		this->repaint();
	}
};

class GUI : public QWidget {
	Service& serv;
	QListWidget *list = new QListWidget;

	QPushButton *refresh = new QPushButton("Refresh");
	QPushButton *sortByDenumire = new QPushButton("Sort dupa denumire");
	QPushButton *sortByProducator = new QPushButton("Sort dupa producator");
	QPushButton *sortBySubstanta = new QPushButton("Sort dupa substanta activa");
	
	QLineEdit *denBox = new QLineEdit();
	QLineEdit *prodBox = new QLineEdit();
	QLineEdit *substBox = new QLineEdit();
	QLineEdit *pretBox = new QLineEdit();

	QPushButton *add = new QPushButton("Add");
	QPushButton *upd = new QPushButton("Update");
	QPushButton *del = new QPushButton("Delete");
	QPushButton *find = new QPushButton("Find");

	QLineEdit *findBox = new QLineEdit();

	QPushButton *filterByPrice = new QPushButton("Filter pret");
	QPushButton *filterBySubstanta = new QPushButton("Filter substanta");

	QLineEdit *filterPriceBox = new QLineEdit();
	QLineEdit *filterSubstantaBox = new QLineEdit();

	QPushButton *undo = new QPushButton("Undo");
	QPushButton *redo = new QPushButton("Redo");

	QDialog *addWindow = new QDialog();
	QVBoxLayout *addWindowLayout = new QVBoxLayout();
	QFormLayout *addLayout = new QFormLayout;

	QLineEdit *denBoxAdd = new QLineEdit();
	QLineEdit *prodBoxAdd = new QLineEdit();
	QLineEdit *substBoxAdd = new QLineEdit();
	QLineEdit *pretBoxAdd = new QLineEdit();
	QPushButton *addDialogue = new QPushButton("Add");

	QDialog *updWindow = new QDialog();
	QVBoxLayout *updWindowLayout = new QVBoxLayout();
	QFormLayout *updLayout = new QFormLayout;

	QLineEdit *denBoxUpd = new QLineEdit();
	QLineEdit *prodBoxUpd = new QLineEdit();
	QLineEdit *substBoxUpd = new QLineEdit();
	QLineEdit *pretBoxUpd = new QLineEdit();
	QPushButton *updDialogue = new QPushButton("Update");

	RetetaGUI *reteta;
	std::vector<Observer*> observers;
	void addObserver(Observer* obs) {
		observers.push_back(obs);
	}
	void notifyObservers() {
		for (auto& o : observers)
			o->update();
	}

	QPushButton *addReteta = new QPushButton("Adaugare medicament la reteta");
	QPushButton *genReteta = new QPushButton("Generare aleatoare reteta");
	QPushButton *emptyReteta = new QPushButton("Golire reteta");
	QPushButton *showReteta = new QPushButton("Afisare reteta CRUD");
	QPushButton *showRetetaReadOnly = new QPushButton("Afisare reteta Read-Only");
	QPushButton *showRetetaGrid = new QPushButton("Afisare reteta grid");
	QPushButton *CSVReteta = new QPushButton("Export reteta in CSV");

public:
	GUI(Service& serv) : serv{ serv } {
		Layout();
		Connections();
		Start();
	}

	void ShowList(std::vector<Medicament> vector) {
		list->clear();
		int prMax = 0;
		for (auto& m : vector) {
			if (prMax < m.GetPret()) prMax = m.GetPret();
		}
		for (auto& m : vector) {
			QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(m.GetDenumire()) + " de la " + QString::fromStdString(m.GetProducator()), list);
			if (m.GetPret() < prMax / 3) item->setBackgroundColor(QColor{ Qt::green });
			else
				if (m.GetPret() < 2 * prMax / 3) item->setBackgroundColor(QColor{ Qt::yellow });
				else
					item->setBackgroundColor(QColor{ Qt::red });
		}
	}

	void Find(std::string den) {
		serv.ImportFile();
		char numar[10]; int poz = serv.FindByDenumire(den);
		if (poz != -1) {
			denBox->setText(QString::fromUtf8(serv.GetAll()[poz].GetDenumire().c_str()));
			prodBox->setText(QString::fromUtf8(serv.GetAll()[poz].GetProducator().c_str()));
			substBox->setText(QString::fromUtf8(serv.GetAll()[poz].GetSubstanta().c_str()));
			pretBox->setText(QString::fromUtf8(itoa(serv.GetAll()[poz].GetPret(), numar, 10)));
		}
	}

	void Delete(std::string den) {
		serv.ImportFile();
		serv.DeleteMedicament(den);
		serv.ExportFile();
	}

	void LoadDetails() {
		if (list->selectedItems().isEmpty()) {
			denBox->setText("");
			prodBox->setText("");
			substBox->setText("");
			pretBox->setText("");
		}
		else {
			QListWidgetItem *item = list->selectedItems().at(0);
			char s[101]; strcpy(s, item->text().toStdString().c_str());
			char* pch = strtok(s, " ");
			std::string str(pch);
			Find(str);
		}
	}

	QPushButton *btns[18];

	void Layout() {
		QHBoxLayout *appLayout = new QHBoxLayout();
		this->setLayout(appLayout);

		QVBoxLayout *listLayout = new QVBoxLayout();
		listLayout->addWidget(list);
		listLayout->addWidget(refresh);
		listLayout->addWidget(sortByDenumire);
		listLayout->addWidget(sortByProducator);
		listLayout->addWidget(sortBySubstanta);

		for (int i = 0; i < 18; ++i)
		{
			std::stringstream s;
			s << "asdf " << i;
			btns[i] = new QPushButton(QString::fromStdString(s.str()));
			listLayout->addWidget(btns[i]);
		
			QObject::connect(btns[i], &QPushButton::clicked, [this]() {
					QMessageBox::warning(nullptr, "Warning", "lololol");
				});
		}
		appLayout->addLayout(listLayout);

		QVBoxLayout *actions = new QVBoxLayout();
		appLayout->addLayout(actions);

		QFormLayout *detailLayout = new QFormLayout;
		actions->addLayout(detailLayout);
		QLabel *denLabel = new QLabel("Denumire");
		QLabel *prodLabel = new QLabel("Producator");
		QLabel *substLabel = new QLabel("Substanta activa");
		QLabel *pretLabel = new QLabel("Pret");
		detailLayout->addRow(denLabel, denBox);
		detailLayout->addRow(prodLabel, prodBox);
		detailLayout->addRow(substLabel, substBox);
		detailLayout->addRow(pretLabel, pretBox);

		actions->addWidget(add);
		actions->addWidget(upd);
		actions->addWidget(del);

		QHBoxLayout *findLayout = new QHBoxLayout();
		findLayout->addWidget(find);
		findLayout->addWidget(findBox);
		actions->addLayout(findLayout);

		QHBoxLayout *filterPriceLayout = new QHBoxLayout();
		filterPriceLayout->addWidget(filterByPrice);
		filterPriceLayout->addWidget(filterPriceBox);
		actions->addLayout(filterPriceLayout);
		
		QHBoxLayout *filterSubstantaLayout = new QHBoxLayout();
		filterSubstantaLayout->addWidget(filterBySubstanta);
		filterSubstantaLayout->addWidget(filterSubstantaBox);
		actions->addLayout(filterSubstantaLayout);

		QHBoxLayout *UndoRedoLayout = new QHBoxLayout();
		UndoRedoLayout->addWidget(undo);
		UndoRedoLayout->addWidget(redo);
		actions->addLayout(UndoRedoLayout);

		addWindow->setLayout(addWindowLayout);
		addWindowLayout->addLayout(addLayout);
		updWindow->setLayout(updWindowLayout);
		updWindowLayout->addLayout(updLayout);

		QLabel *denLabelAdd = new QLabel("Denumire");
		QLabel *prodLabelAdd = new QLabel("Producator");
		QLabel *substLabelAdd = new QLabel("Substanta activa");
		QLabel *pretLabelAdd = new QLabel("Pret");
		addLayout->addRow(denLabelAdd, denBoxAdd);
		addLayout->addRow(prodLabelAdd, prodBoxAdd);
		addLayout->addRow(substLabelAdd, substBoxAdd);
		addLayout->addRow(pretLabelAdd, pretBoxAdd);
		addWindowLayout->addWidget(addDialogue);

		QLabel *denLabelUpd = new QLabel("Denumire");
		QLabel *prodLabelUpd = new QLabel("Producator");
		QLabel *substLabelUpd = new QLabel("Substanta activa");
		QLabel *pretLabelUpd = new QLabel("Pret");
		updLayout->addRow(denLabelUpd, denBoxUpd);
		updLayout->addRow(prodLabelUpd, prodBoxUpd);
		updLayout->addRow(substLabelUpd, substBoxUpd);
		updLayout->addRow(pretLabelUpd, pretBoxUpd);
		updWindowLayout->addWidget(updDialogue);

		QVBoxLayout *cosLayout = new QVBoxLayout();
		appLayout->addLayout(cosLayout);
		cosLayout->addWidget(addReteta);
		cosLayout->addWidget(genReteta);
		cosLayout->addWidget(emptyReteta);
		cosLayout->addWidget(showReteta);
		cosLayout->addWidget(showRetetaReadOnly);
		cosLayout->addWidget(showRetetaGrid);
		cosLayout->addWidget(CSVReteta);
		cosLayout->addStretch();
	}

	void Connections() {
		QObject::connect(refresh, &QPushButton::clicked, [=]() { ShowList(serv.GetAll()); });
		QObject::connect(sortByDenumire, &QPushButton::clicked, [=]() { ShowList(serv.SortByDenumire()); });
		QObject::connect(sortByProducator, &QPushButton::clicked, [=]() { ShowList(serv.SortByProducator()); });
		QObject::connect(sortBySubstanta, &QPushButton::clicked, [=]() { ShowList(serv.SortBySubstantaPret()); });

		QObject::connect(list, &QListWidget::itemSelectionChanged, [this]() { 
			LoadDetails();
		});

		QObject::connect(add, &QPushButton::clicked, [=]() {
			serv.ImportFile();
			addWindow->setModal(true);
			addWindow->show();
		});
		QObject::connect(addDialogue, &QPushButton::clicked, [=]() {
			std::string den = denBoxAdd->text().toStdString();
			std::string prod = prodBoxAdd->text().toStdString();
			std::string subst = substBoxAdd->text().toStdString();
			int pr = atoi(pretBoxAdd->text().toStdString().c_str());

			try {
				serv.AddMedicament(Medicament(den, prod, subst, pr));
			}
			catch (ValidateException& msg) {
				QMessageBox::warning(nullptr, "Warning", QString::fromStdString(msg.GetMessages()[0]));
			}
			serv.ExportFile();
			addWindow->close();
			ShowList(serv.GetAll()); 
		});
		QObject::connect(upd, &QPushButton::clicked, [=]() {
			if (list->selectedItems().empty()) QMessageBox::warning(nullptr, "Warning", QString::fromStdString("Nu s-a selectat nimic!"));
			else {
				updWindow->setModal(true);
				updWindow->show();
			}
		});
		QObject::connect(updDialogue, &QPushButton::clicked, [=]() {
			std::string den = denBox->text().toStdString();
			std::string denNew = denBoxUpd->text().toStdString();
			std::string prodNew = prodBoxUpd->text().toStdString();
			std::string substNew = substBoxUpd->text().toStdString();
			int prNew = atoi(pretBoxUpd->text().toStdString().c_str());

			try {
				serv.UpdateMedicament(den, Medicament(denNew, prodNew, substNew, prNew));
			}
			catch (ValidateException& msg) {
				QMessageBox::warning(nullptr, "Warning", QString::fromStdString(msg.GetMessages()[0]));
			}

			serv.ExportFile();
			ShowList(serv.GetAll());
		});
		QObject::connect(del, &QPushButton::clicked, [=]() {
			if (list->selectedItems().empty()) QMessageBox::warning(nullptr, "Warning", QString::fromStdString("Nu s-a selectat nimic!"));
			else Delete(denBox->text().toStdString());
			ShowList(serv.GetAll());
		});
		QObject::connect(find, &QPushButton::clicked, [=]() {
			if (findBox->text().toStdString() == "") QMessageBox::warning(nullptr, "Warning", QString::fromStdString("Parametrul este vid!"));
			else {
				serv.ImportFile();
				char numar[10]; int poz = serv.FindByDenumire(findBox->text().toStdString());
				if (poz == -1) QMessageBox::warning(nullptr, "Warning", QString::fromStdString("Nu s-a gasit acest medicament!"));
				else {
					denBox->setText(QString::fromUtf8(serv.GetAll()[poz].GetDenumire().c_str()));
					prodBox->setText(QString::fromUtf8(serv.GetAll()[poz].GetProducator().c_str()));
					substBox->setText(QString::fromUtf8(serv.GetAll()[poz].GetSubstanta().c_str()));
					pretBox->setText(QString::fromUtf8(itoa(serv.GetAll()[poz].GetPret(), numar, 10)));
					ShowList(serv.GetAll());
				}
			}
		});

		QObject::connect(filterByPrice, &QPushButton::clicked, [=]() { 
			if (filterPriceBox->text().toStdString() == "") QMessageBox::warning(nullptr, "Warning", QString::fromStdString("Parametrul este vid!"));
			else ShowList(serv.FilterByPrice(atoi(filterPriceBox->text().toStdString().c_str())));
		});
		QObject::connect(filterBySubstanta, &QPushButton::clicked, [=]() { 
			if (filterSubstantaBox->text().toStdString() == "") QMessageBox::warning(nullptr, "Warning", QString::fromStdString("Parametrul este vid!"));
			else ShowList(serv.FilterBySubstanta(filterSubstantaBox->text().toStdString().c_str()));
		});
		QObject::connect(undo, &QPushButton::clicked, [=]() {
			try {
				serv.Undo();
			}
			catch (ValidateException& msg) {
				QMessageBox::warning(nullptr, "Warning", QString::fromStdString(msg.GetMessages()[0]));
			}
			serv.ExportFile();
			ShowList(serv.GetAll());
		});
		QObject::connect(redo, &QPushButton::clicked, [=]() {
			try {
				serv.Redo();
			}
			catch (ValidateException& msg) {
				QMessageBox::warning(nullptr, "Warning", QString::fromStdString(msg.GetMessages()[0]));
			}
			serv.ExportFile();
			ShowList(serv.GetAll());
		});
		QObject::connect(addReteta, &QPushButton::clicked, [=]() {
			if (list->selectedItems().empty()) QMessageBox::warning(nullptr, "Warning", QString::fromStdString("Nu s-a selectat nimic!"));
			else {
				serv.AddReteta(denBox->text().toStdString());
				notifyObservers();
			}
		});
		QObject::connect(genReteta, &QPushButton::clicked, [=]() {
			serv.Random(serv.GetAll());
			notifyObservers();
		});
		QObject::connect(emptyReteta, &QPushButton::clicked, [=]() {
			serv.DeleteReteta();
			notifyObservers();
		});
		QObject::connect(showReteta, &QPushButton::clicked, [=]() {
			addObserver(new RetetaGUI{ serv, observers });
		});
		QObject::connect(showRetetaReadOnly, &QPushButton::clicked, [=]() {
			addObserver(new RetetaGUIRO{ serv, observers });
		});
		QObject::connect(showRetetaGrid, &QPushButton::clicked, [=]() {
			addObserver(new RetetaGUIGrid{ serv, observers });
		});
		QObject::connect(CSVReteta, &QPushButton::clicked, [=]() {
			serv.ExportCSV();
		});
	}

	void Start() {
		ShowList(serv.GetAll());

		denBox->setReadOnly(true);
		prodBox->setReadOnly(true);
		substBox->setReadOnly(true);
		pretBox->setReadOnly(true);
	}
};
