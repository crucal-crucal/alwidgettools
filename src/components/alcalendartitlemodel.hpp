#pragma once

#include <QAbstractListModel>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALCalendarTitleModel final : public QAbstractListModel {
	Q_OBJECT

public:
	explicit CALCalendarTitleModel(QObject* parent = nullptr);
	~CALCalendarTitleModel() override;

protected:
	[[nodiscard]] int rowCount(const QModelIndex& parent) const override;
	[[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;
};

} // namespace AL
