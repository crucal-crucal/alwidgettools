#include "albreadcrumbbardelegate.hpp"

#include <QPainter>

#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALBreadcrumbBarDelegate
 * @param parent pointer to the parent class
 */
CALBreadcrumbBarDelegate::CALBreadcrumbBarDelegate(QObject* parent): QStyledItemDelegate(parent) {
	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [this](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALBreadcrumbBarDelegate::~CALBreadcrumbBarDelegate() = default;

void CALBreadcrumbBarDelegate::setPressIndex(const QModelIndex& index) {
	m_pressIndex = index;
}

QModelIndex CALBreadcrumbBarDelegate::getPressIndex() const {
	return m_pressIndex;
}

void CALBreadcrumbBarDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	painter->save();
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	painter->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicText));
	QRect itemRect = option.rect;
	const QString breadcrumbDisplayData = index.data(Qt::DisplayRole).toString();
	if (const QString breadcrumbUserData = index.data(Qt::UserRole).toString(); breadcrumbUserData != "LastBreadcrumb") {
		if (m_pressIndex == index) {
			painter->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicTextPress));
		} else {
			if (!option.state.testFlag(QStyle::State_MouseOver)) {
				painter->setPen(ALThemeColor(m_themeMode, ALThemeType::BasicTextNoFocus));
			}
		}
	}

	if (breadcrumbDisplayData != ">") {
		painter->drawText(itemRect, Qt::AlignVCenter | Qt::AlignLeft, breadcrumbDisplayData);
	} else {
		QFont iconFont(ALIcon::getEnumTypeFontName(ALIcon::Awesome));
		iconFont.setPixelSize(painter->font().pixelSize() * 0.785); // NOLINT
		painter->setFont(iconFont);
		itemRect.setX(itemRect.x() - itemRect.width() * 0.36); // NOLINT
		painter->drawText(itemRect, Qt::AlignCenter, QChar(static_cast<unsigned short>(ALIcon::AweSomeIcon::AngleRight)));
	}
	painter->restore();
}
}
