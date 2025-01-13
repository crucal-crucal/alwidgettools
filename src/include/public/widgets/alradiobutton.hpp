#pragma once

#include <QRadioButton>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALRadioButtonPrivate;

class CALWIDGETTOOLS_EXPORT CALRadioButton : public QRadioButton {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALRadioButton)

public:
	explicit CALRadioButton(QWidget* parent = nullptr);
	explicit CALRadioButton(const QString& text, QWidget* parent = nullptr);
	~CALRadioButton() override;

protected:
	const QScopedPointer<CALRadioButtonPrivate> d_ptr{ nullptr };
};

} // namespace AL
