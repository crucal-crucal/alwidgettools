#pragma once

#include "alwidgettoolsdef.hpp"

class QPropertyAnimation;

namespace AL {
class CALBaseDialog;
class CALMaskWidget;

class CALBaseDialogPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALBaseDialogPrivate)
	Q_DECLARE_PUBLIC(CALBaseDialog)

public:
	explicit CALBaseDialogPrivate(CALBaseDialog* q, QObject* parent = nullptr);
	~CALBaseDialogPrivate() override;

	[[nodiscard]] bool hasPolicy(const ALDialogPolicy::ShowPolicy& policy) const;

protected:
	CALBaseDialog* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	qint64 currentWinID{ 0 };
	ALDialogPolicy::ShowPolicys showPolicys{ ALDialogPolicy::Normal };
	CALMaskWidget* maskWidget{ nullptr };

	/// animation
	QPropertyAnimation* fadeInAnimation{ nullptr };  // 淡入动画
	QPropertyAnimation* fadeOutAnimation{ nullptr }; // 淡出动画

	void _doCloseAnimation();
};

} // namespace AL
