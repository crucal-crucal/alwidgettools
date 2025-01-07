#pragma once

#include "alwidgettoolsdef.hpp"

class CALEvent;
class CUVLineEdit;

class CUVLineEditPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVLineEditPrivate)
	Q_DECLARE_PUBLIC(CUVLineEdit)
	Q_PROPERTY(qreal expandMarkWidth READ getExpandMarkWidth WRITE setExpandMarkWidth NOTIFY sigExpandMarkWidthChanged)

public:
	explicit CUVLineEditPrivate(CUVLineEdit* q, QObject* parent = nullptr);
	~CUVLineEditPrivate() override;

	void setExpandMarkWidth(qreal expandMarkWidth);
	[[nodiscard]] qreal getExpandMarkWidth() const;

	Q_INVOKABLE void invokableWMWindowClickedEvent(const QVariantMap& data);
	Q_SLOT void slotThemeChanged(const ALThemeType::ThemeMode& mode);

Q_SIGNALS:
	Q_SIGNAL void sigExpandMarkWidthChanged();

protected:
	CUVLineEdit* const q_ptr{ nullptr };

private:
	int borderRadius{};
	bool isClearButtonEnable{};
	qreal expandMarkWidth{};
	ALThemeType::ThemeMode themeMode{};
	CALEvent* focusEvent{ nullptr };
	qreal textSpacing{ 0.5 };
};
