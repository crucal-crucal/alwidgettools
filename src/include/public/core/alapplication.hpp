#pragma once

#include "alwidgettoolsdef.hpp"
#include "alsingleton.hpp"

#define alApp AL::CALApplication::instance()

/**
 * @brief \namespace AL
 */
namespace AL {
class CALApplicationPrivate;

class CALWIDGETTOOLS_EXPORT CALApplication : public QObject {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALApplication)
	Q_PROPERTY(bool isEnableMica READ getIsEnableMica WRITE setIsEnableMica NOTIFY sigIsEnableMicaChanged)
	Q_PROPERTY(QString micaImagePath READ getMicaImagePath WRITE setMicaImagePath NOTIFY sigMicaImagePathChanged)

public:
	static CALApplication* instance();

	/**
	 * @brief 初始化 AL 界面类所依赖的资源
	 */
	void init();

	/**
	 * @brief 同步 Mica 效果到指定窗口
	 * @param widget 窗口
	 * @param isSync 是否启用同步
	 */
	void syncMica(QWidget* widget, bool isSync = true);

	/**
	 * @brief 检测光标是否在指定 item 范围内
	 * @param item 需要检测的控件
	 * @return 是否在范围内
	 */
	static bool containsCursorToItem(const QWidget* item);

	/**
	 * @brief 是否启用 Mica 效果
	 * @param enable 是否启用
	 */
	void setIsEnableMica(bool enable);
	[[nodiscard]] bool getIsEnableMica() const;

	/**
	 * @brief 设置 Mica 效果背景图片
	 * @param path 需要做 Mica 效果的背景图片
	 */
	void setMicaImagePath(const QString& path);
	[[nodiscard]] QString getMicaImagePath() const;

Q_SIGNALS:
	Q_SIGNAL void sigIsEnableMicaChanged();
	Q_SIGNAL void sigMicaImagePathChanged();

protected:
	const QScopedPointer<CALApplicationPrivate> d_ptr{ nullptr };

	friend class CALSingleton<CALApplication>;

private:
	explicit CALApplication(QObject* parent = nullptr);
	~CALApplication() override;
};
}
