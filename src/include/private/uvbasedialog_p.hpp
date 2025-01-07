#pragma once

#include "alwidgettoolsdef.hpp"

class CUVBaseDialog;
class CUVMaskWidget;
class QPropertyAnimation;

class CUVBaseDialogPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVBaseDialogPrivate)
	Q_DECLARE_PUBLIC(CUVBaseDialog)

public:
	explicit CUVBaseDialogPrivate(CUVBaseDialog* q, QObject* parent = nullptr);
	~CUVBaseDialogPrivate() override;

	[[nodiscard]] bool hasPolicy(const ALDialogPolicy::ShowPolicy& policy) const;

protected:
	CUVBaseDialog* const q_ptr{ nullptr };

private:
	ALThemeType::ThemeMode themeMode{};
	qint64 currentWinID{ 0 };
	ALDialogPolicy::ShowPolicys showPolicys{ ALDialogPolicy::Normal };
	CUVMaskWidget* maskWidget{ nullptr };

	/// animation
	QPropertyAnimation* fadeInAnimation{ nullptr }; // 淡入动画
	QPropertyAnimation* fadeOutAnimation{ nullptr }; // 淡出动画

	void _doCloseAnimation();
};
