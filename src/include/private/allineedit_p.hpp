#pragma once

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALEvent;
class CALLineEdit;

class CALLineEditPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALLineEditPrivate)
	Q_DECLARE_PUBLIC(CALLineEdit)
	Q_PROPERTY(qreal expandMarkWidth READ getExpandMarkWidth WRITE setExpandMarkWidth NOTIFY sigExpandMarkWidthChanged)

public:
	explicit CALLineEditPrivate(CALLineEdit* q, QObject* parent = nullptr);
	~CALLineEditPrivate() override;

	void setExpandMarkWidth(qreal expandMarkWidth);
	[[nodiscard]] qreal getExpandMarkWidth() const;

	Q_INVOKABLE void invokableWMWindowClickedEvent(const QVariantMap& data);
	Q_SLOT void slotThemeChanged(const ALThemeType::ThemeMode& mode);

Q_SIGNALS:
	Q_SIGNAL void sigExpandMarkWidthChanged();

protected:
	CALLineEdit* const q_ptr{ nullptr };

private:
	int borderRadius{};
	bool isClearButtonEnable{};
	qreal expandMarkWidth{};
	ALThemeType::ThemeMode themeMode{};
	CALEvent* focusEvent{ nullptr };
	qreal textSpacing{ 0.5 };
};
} // namespace AL