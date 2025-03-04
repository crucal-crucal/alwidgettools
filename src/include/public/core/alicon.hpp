#pragma once

#include <QDebug>
#include <QIcon>
#include <QMetaEnum>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief 管理 ALIcon 图标抽象类
 */
class CALWIDGETTOOLS_EXPORT CALIconType {
public:
	virtual ~CALIconType() = default;

	/**
	 * @brief 获取图标名称
	 * @return QString 图标名称
	 */
	[[nodiscard]] virtual QString name() const = 0;

	/**
	 * @brief 获取图标值
	 * @return int 图标值
	 */
	[[nodiscard]] virtual int value() const = 0;

	/**
	 * @brief 获取图标字体名称
	 * @return QString 图标字体名称
	 */
	[[nodiscard]] virtual QString familyName() const = 0;

	/**
	 * @brief 获取图标类型
	 * @return \see \enum ALIcon::IconType
	 */
	[[nodiscard]] virtual ALIcon::IconType iconType() const = 0;

	/**
	 * @brief 是否为空图标
	 * @return 是否为空
	 */
	[[nodiscard]] virtual bool isNull() const = 0;
};

class CALAwesomeIconTypePrivate;

/**
 * @brief Awesome Icon 类
 */
class CALWIDGETTOOLS_EXPORT CALAwesomeIconType final : public CALIconType {
public:
	explicit CALAwesomeIconType(const ALIcon::AweSomeIcon& icon);
	~CALAwesomeIconType() override;
	[[nodiscard]] QString name() const override;
	[[nodiscard]] int value() const override;
	[[nodiscard]] QString familyName() const override;
	[[nodiscard]] ALIcon::IconType iconType() const override;
	[[nodiscard]] bool isNull() const override;

protected:
	std::unique_ptr<CALAwesomeIconTypePrivate> d_ptr{ nullptr };

	friend class CALAwesomeIconTypePrivate;
};

class CALFluentIconTypePrivate;

/**
 * @brief Fluent Icon 类
 */
class CALWIDGETTOOLS_EXPORT CALFluentIconType final : public CALIconType {
public:
	explicit CALFluentIconType(const ALIcon::FluentIcon& icon);
	~CALFluentIconType() override;
	[[nodiscard]] QString name() const override;
	[[nodiscard]] int value() const override;
	[[nodiscard]] QString familyName() const override;
	[[nodiscard]] ALIcon::IconType iconType() const override;
	[[nodiscard]] bool isNull() const override;

protected:
	std::unique_ptr<CALFluentIconTypePrivate> d_ptr{ nullptr };

	friend class CALFluentIconTypePrivate;
};

/**
 * @brief ALIcon 工厂类，可以通过此类获取图标
 */
class CALWIDGETTOOLS_EXPORT CALIconFactory {
public:
	static std::unique_ptr<CALIconType> createIconType(const ALIcon::AweSomeIcon& awesomeicon) {
		return std::make_unique<CALAwesomeIconType>(awesomeicon);
	}

	static std::unique_ptr<CALIconType> createIconType(const ALIcon::FluentIcon& fluenticon) {
		return std::make_unique<CALFluentIconType>(fluenticon);
	}

	static std::unique_ptr<CALIconType> createIconType(const QMetaEnum& metaEnum, const QString& iconName) {
		if (metaEnum.name() == QMetaEnum::fromType<ALIcon::AweSomeIcon>().name()) {
			if (int value = metaEnum.keysToValue(iconName.toUtf8().constData()); value != -1) {
				return std::make_unique<CALAwesomeIconType>(static_cast<ALIcon::AweSomeIcon>(value));
			}
		} else if (metaEnum.name() == QMetaEnum::fromType<ALIcon::FluentIcon>().name()) {
			if (int value = metaEnum.keysToValue(iconName.toUtf8().constData()); value != -1) {
				return std::make_unique<CALFluentIconType>(static_cast<ALIcon::FluentIcon>(value));
			}
		}

		qWarning() << "Failed to create icon type for" << iconName;
		return nullptr;
	}
};

class CALWIDGETTOOLS_EXPORT CALIcon {
public:
	/**
	 * @brief 从 AwesomeIcon 创建一个 QIcon
	 * @param awesomeIcon \enum ALIcon::AweSomeIcon
	 * @return QIcon 生成的图标
	 */
	static QIcon getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon);

	/**
	 * @brief 从 FluentIcon 创建一个 QIcon
	 * @param fluentIcon \enum ALIcon::FluentIcon
	 * @return QIcon 生成的图标
	 */
	static QIcon getQIconFromFluentIcon(const ALIcon::FluentIcon& fluentIcon);

	/**
	 * @brief 从 AwesomeIcon 创建一个指定颜色的 QIcon
	 * @param awesomeIcon \enum ALIcon::AweSomeIcon
	 * @param iconColor 图标颜色
	 * @return QIcon 生成的图标
	 */
	static QIcon getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, const QColor& iconColor);

	/**
	 * @brief 从 FluentIcon 创建一个指定颜色的 QIcon
	 * @param fluentIcon \enum ALIcon::FluentIcon
	 * @param iconColor 图标颜色
	 * @return QIcon 生成的图标
	 */
	static QIcon getQIconFromFluentIcon(const ALIcon::FluentIcon& fluentIcon, const QColor& iconColor);

	/**
	 * @brief 从 AwesomeIcon 创建一个指定大小的 QIcon
	 * @param awesomeIcon \enum ALIcon::AweSomeIcon
	 * @param pixelSize 图标像素大小
	 * @return QIcon 生成的图标
	 */
	static QIcon getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, int pixelSize);

	/**
	 * @brief 从 FluentIcon 创建一个指定大小的 QIcon
	 * @param fluentIcon \enum ALIcon::FluentIcon
	 * @param pixelSize 图标像素大小
	 * @return QIcon 生成的图标
	 */
	static QIcon getQIconFromFluentIcon(const ALIcon::FluentIcon& fluentIcon, int pixelSize);

	/**
	 * @brief 从 AwesomeIcon 创建一个指定大小和颜色的 QIcon
	 * @param awesomeIcon \enum ALIcon::AweSomeIcon
	 * @param pixelSize 图标像素大小
	 * @param iconColor 图标颜色
	 * @return QIcon 生成的图标
	 */
	static QIcon getQIconFromAwesomeIcon(const ALIcon::AweSomeIcon& awesomeIcon, int pixelSize, const QColor& iconColor);

	/**
	 * @brief 从 FluentIcon 创建一个指定大小和颜色的 QIcon
	 * @param fluentIcon \enum ALIcon::FluentIcon
	 * @param pixelSize 图标像素大小
	 * @param iconColor 图标颜色
	 * @return QIcon 生成的图标
	 */
	static QIcon getQIconFromFluentIcon(const ALIcon::FluentIcon& fluentIcon, int pixelSize, const QColor& iconColor);

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
	 * @brief 从 FluentIcon 创建一个指定大小和固定宽高的 QIcon
	 * @param fluentIcon \enum ALIcon::FluentIcon
	 * @param pixelSize 图标像素大小
	 * @param fixedWidth 图标宽度
	 * @param fixedHeight 图标高度
	 * @return QIcon 生成的图标
	 */
	static QIcon getQIconFromFluentIcon(const ALIcon::FluentIcon& fluentIcon, int pixelSize, int fixedWidth, int fixedHeight);

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

	/**
	 * @brief 从 FluentIcon 创建一个指定大小、固定宽高和颜色的 QIcon
	 * @param fluentIcon \enum ALIcon::FluentIcon
	 * @param pixelSize 图标像素大小
	 * @param fixedWidth 图标宽度
	 * @param fixedHeight 图标高度
	 * @param iconColor 图标颜色
	 * @return QIcon 生成的图标
	 */
	static QIcon getQIconFromFluentIcon(const ALIcon::FluentIcon& fluentIcon, int pixelSize, int fixedWidth, int fixedHeight, const QColor& iconColor);

private:
	explicit CALIcon();
	~CALIcon();
};
}
