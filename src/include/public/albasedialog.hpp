#pragma once

#include <QDialog>

#include "alwidgettoolsdef.hpp"

class CUVBaseDialogPrivate;

class CALWIDGETTOOLS_EXPORT CUVBaseDialog : public QDialog {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVBaseDialog)

public:
	explicit CUVBaseDialog(QWidget* parent = nullptr);
	~CUVBaseDialog() override;

	/**
	 * @brief 设置弹窗显示策略
	 * @param policys \see \enum UVDialogPolicy::ShowPolicys
	 */
	void setShowPolicys(const ALDialogPolicy::ShowPolicys& policys);

	/**
	 * @brief 设置弹窗显示策略
	 * @param policy \see \enum UVDialogPolicy::ShowPolicy
	 * @param enabled 是否开启
	 */
	void setShowPolicy(const ALDialogPolicy::ShowPolicy& policy, bool enabled);

	/**
	 * @brief 当前弹窗显示策略
	 * @return \see \enum UVDialogPolicy::ShowPolicys
	 */
	[[nodiscard]] ALDialogPolicy::ShowPolicys showPolicys() const;

	/**
	 * @brief 是否有指定策略
	 * @param policy \see \enum UVDialogPolicy::ShowPolicy
	 * @return 是否含有
	 */
	[[nodiscard]] bool hasPolicy(const ALDialogPolicy::ShowPolicy& policy) const;

	void close();

protected:
	const QScopedPointer<CUVBaseDialogPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
	void showEvent(QShowEvent* event) override;
#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
#else
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif
#endif
};
