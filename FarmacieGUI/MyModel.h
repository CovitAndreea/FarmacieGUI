#pragma once
#include <QAbstractTableModel>
#include "domain.h"

class MyTableModel : public QAbstractTableModel {
	std::vector<Medicament> medicamente;


public:
	MyTableModel() {}
	MyTableModel(const std::vector<Medicament>& medicamente) : medicamente{ medicamente } {};

	int rowCount(const QModelIndex& parent = QModelIndex()) const override {
		return medicamente.size();
	}
	int columnCount(const QModelIndex& parent = QModelIndex()) const override {
		return 4;
	}
	QVariant data(const QModelIndex& index = QModelIndex(), int role = Qt::DisplayRole) const override {
		if (role == Qt::DisplayRole) {
			Medicament m = medicamente[index.row()];
			if (index.column() == 0)
				return QString::fromStdString(m.GetDenumire());
			if (index.column() == 1)
				return QString::fromStdString(m.GetProducator());
			if (index.column() == 2)
				return QString::fromStdString(m.GetSubstanta());
			if (index.column() == 3) {
				char numar[10];
				return QString::fromStdString(itoa(m.GetPret(), numar, 10));
			}
		}
		return QVariant{};
	}
	void setMedicamente(const std::vector<Medicament>& medicamente) {
		this->medicamente = medicamente;
		QModelIndex topLeft = createIndex(0, 0);
		QModelIndex bottomRight = createIndex(rowCount(), columnCount());
		emit dataChanged(topLeft, bottomRight);
		emit layoutChanged();
	}
};
