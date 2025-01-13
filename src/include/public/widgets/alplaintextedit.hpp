#pragma once

#include <QPlainTextEdit>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALPlainTextEditPrivate;

class CALWIDGETTOOLS_EXPORT CALPlainTextEdit : public QPlainTextEdit {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALPlainTextEdit)

public:
	explicit CALPlainTextEdit(QWidget* parent = nullptr);
	explicit CALPlainTextEdit(const QString& text, QWidget* parent = nullptr);
	~CALPlainTextEdit() override;

protected:
	const QScopedPointer<CALPlainTextEditPrivate> d_ptr{ nullptr };

	void focusInEvent(QFocusEvent* event) override;
	void focusOutEvent(QFocusEvent* event) override;
	void contextMenuEvent(QContextMenuEvent* event) override;
};
} // namespace AL