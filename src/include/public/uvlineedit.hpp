#pragma once

#include <QLineEdit>

#include "uvwidgettoolsdef.hpp"

class CUVLineEditPrivate;

class CUVWIDGETTOOLS_EXPORT CUVLineEdit : public QLineEdit {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVLineEdit)
	Q_PROPERTY(int borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(bool isClearButtonEnable READ isClearButtonEnable WRITE setIsClearButtonEnable NOTIFY sigIsClearButtonEnableChanged)

public:
	explicit CUVLineEdit(QWidget* parent = nullptr);
	~CUVLineEdit() override;

	void setBorderRadius(int borderRadius);
	[[nodiscard]] int getBorderRadius() const;

	void setIsClearButtonEnable(bool isEnable);
	[[nodiscard]] bool isClearButtonEnable() const;

Q_SIGNALS:
	Q_SIGNAL void sigBorderRadiusChanged();
	Q_SIGNAL void sigIsClearButtonEnableChanged();
	Q_SIGNAL void sigFocusIn(const QString& text);
	Q_SIGNAL void sigFocusOut(const QString& text);
	Q_SIGNAL void sigWmFocusOut(const QString& text);

protected:
	const QScopedPointer<CUVLineEditPrivate> d_ptr{ nullptr };

	void focusInEvent(QFocusEvent* event) override;
	void focusOutEvent(QFocusEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
	void contextMenuEvent(QContextMenuEvent* event) override;
};
