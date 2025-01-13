#include "alcalendartitlemodel.hpp"

namespace AL {
/**
 * @brief \class CALCalendarTitleModel
 * @param parent pointer to the parent class
 */
CALCalendarTitleModel::CALCalendarTitleModel(QObject* parent): QAbstractListModel(parent) {
}

CALCalendarTitleModel::~CALCalendarTitleModel() = default;

int CALCalendarTitleModel::rowCount(const QModelIndex& parent) const {
	return 7;
}

QVariant CALCalendarTitleModel::data(const QModelIndex& index, const int role) const {
	if (role == Qt::UserRole) {
		switch (index.row()) {
			case 0: {
				return tr("Su");
			}
			case 1: {
				return tr("Mo");
			}
			case 2: {
				return tr("Tu");
			}
			case 3: {
				return tr("We");
			}
			case 4: {
				return tr("Th");
			}
			case 5: {
				return tr("Fr");
			}
			case 6: {
				return tr("Sa");
			}
			default: {
				break;
			}
		}
	}

	return {};
}
} // namespace AL