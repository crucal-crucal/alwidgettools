#include "e_tableviewmodel.hpp"

#include <QIcon>
#include <QPixmap>

#define DATACOUNT 6
#define HEADERCOUNT 5

E_TableViewModel::E_TableViewModel(QObject* parent): QAbstractTableModel(parent) {
	for (int i = 0; i < HEADERCOUNT; ++i) {
		m_header.append(QString("header %1").arg(i));
	}

	for (int i = 0; i < DATACOUNT; ++i) {
		QStringList data{};
		for (int j = 0; j < HEADERCOUNT; ++j) {
			data.append(QString("data %1 %2").arg(i).arg(j));
		}
		m_dataList.append(data);
	}

	for (int i = 0; i < DATACOUNT; ++i) {
		QIcon icon;
		QPixmap pixmap = QPixmap(QString(":example/promotion/promotion%1.jpg").arg(i + 1)).scaled(38, 38, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		icon.addPixmap(pixmap, QIcon::Disabled, QIcon::Off);
		icon.addPixmap(pixmap, QIcon::Normal, QIcon::Off);
		icon.addPixmap(pixmap, QIcon::Selected, QIcon::Off);
		m_iconList.append(icon);
	}
}

E_TableViewModel::~E_TableViewModel() = default;

int E_TableViewModel::rowCount(const QModelIndex& parent) const {
	return 100;
}

int E_TableViewModel::columnCount(const QModelIndex& parent) const {
	return m_header.count();
}

QVariant E_TableViewModel::data(const QModelIndex& index, const int role) const {
	if (role == Qt::DisplayRole && index.column() != 0) {
		return m_dataList.at(index.row() % (DATACOUNT)).at(index.column() - 1);
	} else if (role == Qt::DecorationRole && index.column() == 0) {
		return m_iconList.at(index.row() % (DATACOUNT));
	} else if (role == Qt::DecorationPropertyRole || role == Qt::TextAlignmentRole) {
		return Qt::AlignCenter;
	}

	return {};
}

QVariant E_TableViewModel::headerData(const int section, const Qt::Orientation orientation, const int role) const {
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		return m_header.at(section);
	}

	return QAbstractTableModel::headerData(section, orientation, role);
}
