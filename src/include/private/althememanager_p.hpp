#pragma once

#include <QColor>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALThemeManager;

class CALThemeManagerPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALThemeManagerPrivate)
	Q_DECLARE_PUBLIC(CALThemeManager)

public:
	explicit CALThemeManagerPrivate(CALThemeManager* q, QObject* parent = nullptr);
	~CALThemeManagerPrivate() override;

protected:
	CALThemeManager* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	QColor lightThemeColorList[40];
	QColor darkThemeColorList[40];

	void initColorList();
};
}
