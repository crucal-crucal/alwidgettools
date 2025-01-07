#pragma once

#include <QRadioButton>

#include "alwidgettoolsdef.hpp"

class CUVRadioButtonPrivate;

class CALWIDGETTOOLS_EXPORT CUVRadioButton : public QRadioButton {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVRadioButton)

public:
	explicit CUVRadioButton(QWidget* parent = nullptr);
	explicit CUVRadioButton(const QString& text, QWidget* parent = nullptr);
	~CUVRadioButton() override;

protected:
	const QScopedPointer<CUVRadioButtonPrivate> d_ptr{ nullptr };
};
