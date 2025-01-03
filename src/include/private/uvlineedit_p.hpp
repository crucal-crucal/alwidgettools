#pragma once

#include "uvwidgettoolsdef.hpp"

class CUVEvent;
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
	Q_SLOT void slotThemeChanged(const UVThemeType::ThemeMode& mode);

Q_SIGNALS:
	Q_SIGNAL void sigExpandMarkWidthChanged();

protected:
	CUVLineEdit* const q_ptr{ nullptr };

private:
	int borderRadius{};
	bool isClearButtonEnable{};
	qreal expandMarkWidth{};
	UVThemeType::ThemeMode themeMode{};
	CUVEvent* focusEvent{ nullptr };
	qreal textSpacing{ 0.5 };
};
