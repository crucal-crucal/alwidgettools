#pragma once

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALEvent;
class CALPlainTextEditStyle;
class CALPlainTextEdit;

class CALPlainTextEditPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALPlainTextEditPrivate)
	Q_DECLARE_PUBLIC(CALPlainTextEdit)

public:
	explicit CALPlainTextEditPrivate(CALPlainTextEdit* q, QObject* parent = nullptr);
	~CALPlainTextEditPrivate() override;

	Q_INVOKABLE void invokableWMWindowClickedEvent(const QVariantMap& data);
	Q_SLOT void slotThemeChanged(const ALThemeType::ThemeMode& mode);

protected:
	CALPlainTextEdit* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	CALPlainTextEditStyle* style{ nullptr };
	CALEvent* focusEvent{ nullptr };
};
}
