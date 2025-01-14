#pragma once

#include <QIcon>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALWIDGETTOOLS_EXPORT CALIcon {
public:
	/**
	 * @brief 从 AwesomeIcon 创建一个 QIcon
	 * @param awesomeIcon \enum ALIcon::AweSomeIcon
	 * @return QIcon 生成的图标
	 */
	static QIcon getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon);

	/**
	 * @brief 从 AwesomeIcon 创建一个指定颜色的 QIcon
	 * @param awesomeIcon \enum ALIcon::AweSomeIcon
	 * @param iconColor 图标颜色
	 * @return QIcon 生成的图标
	 */
	static QIcon getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, const QColor& iconColor);

	/**
	 * @brief 从 AwesomeIcon 创建一个指定大小的 QIcon
	 * @param awesomeIcon \enum ALIcon::AweSomeIcon
	 * @param pixelSize 图标像素大小
	 * @return QIcon 生成的图标
	 */
	static QIcon getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, int pixelSize);

	/**
	 * @brief 从 AwesomeIcon 创建一个指定大小和颜色的 QIcon
	 * @param awesomeIcon \enum ALIcon::AweSomeIcon
	 * @param pixelSize 图标像素大小
	 * @param iconColor 图标颜色
	 * @return QIcon 生成的图标
	 */
	static QIcon getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, int pixelSize, const QColor& iconColor);

	/**
	 * @brief 从 AwesomeIcon 创建一个指定大小和固定宽高的 QIcon
	 * @param awesomeIcon \enum ALIcon::AweSomeIcon
	 * @param pixelSize 图标像素大小
	 * @param fixedWidth 图标宽度
	 * @param fixedHeight 图标高度
	 * @return QIcon 生成的图标
	 */
	static QIcon getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, int pixelSize, int fixedWidth, int fixedHeight);

	/**
	 * @brief 从 AwesomeIcon 创建一个指定大小、固定宽高和颜色的 QIcon
	 * @param awesomeIcon \enum ALIcon::AweSomeIcon
	 * @param pixelSize 图标像素大小
	 * @param fixedWidth 图标宽度
	 * @param fixedHeight 图标高度
	 * @param iconColor 图标颜色
	 * @return QIcon 生成的图标
	 */
	static QIcon getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, int pixelSize, int fixedWidth, int fixedHeight, const QColor& iconColor);

private:
	explicit CALIcon();
	~CALIcon();
};
}
