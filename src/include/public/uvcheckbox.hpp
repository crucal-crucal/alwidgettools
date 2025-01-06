#pragma once

#include <QCheckBox>

#include "uvwidgettoolsdef.hpp"

class CUVCheckBoxPrivate;

class CUVWIDGETTOOLS_EXPORT CUVCheckBox : public QCheckBox {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVCheckBox)

public:
	explicit CUVCheckBox(QWidget* parent = nullptr);
	explicit CUVCheckBox(const QString& text, QWidget* parent = nullptr);
	~CUVCheckBox() override;

protected:
	const QScopedPointer<CUVCheckBoxPrivate> d_ptr{ nullptr };
};