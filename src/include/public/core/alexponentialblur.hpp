#pragma once

#include <QImage>
#include <QObject>

#include "alwidgettoolsdef.hpp"
#include "alsingleton.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALExponentialBlurPrivate;

class CALWIDGETTOOLS_EXPORT CALExponentialBlur : public QObject {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALExponentialBlur)

public:
	static CALExponentialBlur* instance();

	/**
	 * @brief 对输入图像执行指数模糊操作
	 * @param image 输入图像
	 * @param blurRadius 模糊半径, 指定模糊的强度
	 * @return 模糊处理后的图像
	 */
	static QPixmap doExponentialBlur(const QImage& image, const quint64& blurRadius);

protected:
	const QScopedPointer<CALExponentialBlurPrivate> d_ptr{ nullptr };

	friend class CALSingleton<CALExponentialBlur>;

private:
	explicit CALExponentialBlur(QObject* parent = nullptr);
	~CALExponentialBlur() override;
};
}
