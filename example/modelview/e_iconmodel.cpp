#include "e_iconmodel.hpp"

#include "alwidgettoolsdef.hpp"

E_IconModel::E_IconModel(const QMetaEnum& metaEnum, QObject* parent): QAbstractListModel(parent), m_metaEnum(metaEnum) {
	if (metaEnum.name() == QMetaEnum::fromType<ALIcon::FluentIcon>().name()) {
		m_iconType = ALIcon::Fluent;
		m_iconNamePrefix = "ALIcon::FluentIcon::";
	} else if (metaEnum.name() == QMetaEnum::fromType<ALIcon::AweSomeIcon>().name()) {
		m_iconType = ALIcon::Awesome;
		m_iconNamePrefix = "ALIcon::AweSomeIcon::";
	}
	m_rowCount = m_metaEnum.keyCount() - 1;
	m_isSearchMode = false;
}

E_IconModel::~E_IconModel() = default;

int E_IconModel::rowCount(const QModelIndex& parent) const {
	return m_rowCount;
}

QVariant E_IconModel::data(const QModelIndex& index, const int role) const {
	if (role == Qt::UserRole) {
		if (!m_isSearchMode) {
			return index.row() < m_metaEnum.keyCount() - 1 ? QStringList{ m_metaEnum.key(index.row() + 1), QChar(m_metaEnum.value(index.row() + 1)) } : QStringList();
		} else {
			QStringList iconList{};
			if (index.row() >= m_searchKeyList.count()) {
				return {};
			}
			iconList.append(m_searchKeyList.at(index.row()));
			iconList.append(QChar(m_metaEnum.keysToValue(m_searchKeyList.at(index.row()).toLocal8Bit().constData())));
			return iconList;
		}
	}

	return {};
}

void E_IconModel::setSearchKeyList(const QStringList& list) {
	beginResetModel();
	m_searchKeyList = list;
	m_rowCount = m_isSearchMode ? getSearchKeyList().count() : m_metaEnum.keyCount() - 1;
	endResetModel();
}

QStringList E_IconModel::getSearchKeyList() const {
	return m_searchKeyList;
}

QString E_IconModel::getIconNameFromModelIndex(const QModelIndex& index) const {
	QString iconName{};
	if (m_isSearchMode) {
		if (index.row() < m_searchKeyList.count()) {
			iconName = m_searchKeyList.at(index.row());
		}
	} else {
		if (index.row() < m_metaEnum.keyCount() - 1) {
			iconName = m_metaEnum.key(index.row() + 1);
		}
	}
	return iconName;
}

void E_IconModel::setIsSearchMode(const bool isSearchMode) {
	m_isSearchMode = isSearchMode;
	Q_EMIT sigIsSearchModeChanged();
}

bool E_IconModel::getIsSearchMode() const {
	return m_isSearchMode;
}
