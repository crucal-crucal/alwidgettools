#pragma once

#include <QCheckBox>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALCheckBoxPrivate;

class CALWIDGETTOOLS_EXPORT CALCheckBox : public QCheckBox {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALCheckBox)

public:
	explicit CALCheckBox(QWidget* parent = nullptr);
	explicit CALCheckBox(const QString& text, QWidget* parent = nullptr);
	~CALCheckBox() override;

protected:
	const QScopedPointer<CALCheckBoxPrivate> d_ptr{ nullptr };
};
} // namespace AL