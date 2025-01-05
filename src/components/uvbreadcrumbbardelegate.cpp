#include "uvbreadcrumbbardelegate.hpp"

#include <QPainter>

#include "uvthememanager.hpp"

/**
 * @brief \class CUVBreadcrumbBarDelegate
 * @param parent pointer to the parent class
 */
CUVBreadcrumbBarDelegate::CUVBreadcrumbBarDelegate(QObject* parent): QStyledItemDelegate(parent) {
	m_themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const UVThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CUVBreadcrumbBarDelegate::~CUVBreadcrumbBarDelegate() = default;

void CUVBreadcrumbBarDelegate::setPressIndex(const QModelIndex& index) {
	m_pressIndex = index;
}

QModelIndex CUVBreadcrumbBarDelegate::getPressIndex() const {
	return m_pressIndex;
}

void CUVBreadcrumbBarDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	painter->save();
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	painter->setPen(UVThemeColor(m_themeMode, UVThemeType::BasicText));
	QRect itemRect = option.rect;
	const QString breadcrumbDisplayData = index.data(Qt::DisplayRole).toString();
	const QString breadcrumbUserData = index.data(Qt::UserRole).toString();
	if (breadcrumbUserData != "LastBreadcrumb") {
		if (m_pressIndex == index) {
			painter->setPen(UVThemeColor(m_themeMode, UVThemeType::BasicTextPress));
		} else {
			if (!option.state.testFlag(QStyle::State_MouseOver)) {
				painter->setPen(UVThemeColor(m_themeMode, UVThemeType::BasicTextNoFocus));
			}
		}
	}

	if (breadcrumbDisplayData != ">") {
		painter->drawText(itemRect, Qt::AlignVCenter | Qt::AlignLeft, breadcrumbDisplayData);
	} else {
		QFont iconFont("CUVAwesome");
		iconFont.setPixelSize(painter->font().pixelSize() * 0.785); // NOLINT
		painter->setFont(iconFont);
		itemRect.setX(itemRect.x() - itemRect.width() * 0.36); // NOLINT
		painter->drawText(itemRect, Qt::AlignCenter, QChar(static_cast<unsigned short>(UVIcon::CUVAweSomeIcon::AngleRight)));
	}
	painter->restore();
}
