#pragma once

#include <QImage>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALImageCard;

class CALImageCardPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALImageCardPrivate)
	Q_DECLARE_PUBLIC(CALImageCard)

public:
	explicit CALImageCardPrivate(CALImageCard* q, QObject* parent = nullptr);
	~CALImageCardPrivate() override;

protected:
	CALImageCard* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	bool isPreserveAspectCrop{};
	qreal borderRadius{};
	qreal maximumAspectRatio{};
	QImage cardImage{};
};
} // namespace AL