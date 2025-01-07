#include "uvcustomwidget.hpp"

#include <QPainter>
#include <QVBoxLayout>

#include "alapplication.hpp"
#include "althememanager.hpp"

Q_TAKEOVER_NATIVEEVENT_CPP(CUVCustomWidget, m_appBar)

/**
 * @brief \class CUVCustomWidget
 * @param parent pointer to the parent class
 */
CUVCustomWidget::CUVCustomWidget(QWidget* parent): QDialog(parent) {
	resize(500, 500);
	setObjectName("CUVCustomWidget");

	m_appBar = new CALAppBar(this);
	m_appBar->setWindowButtonFlags(ALAppBarType::MinimizeButtonHint | ALAppBarType::MaximizeButtonHint | ALAppBarType::CloseButtonHint);

	m_mainVLayout = new QVBoxLayout(this);
	m_mainVLayout->setContentsMargins(0, 0, 0, 0);

	m_themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) {
		m_themeMode = mode;
		update();
	});

	m_isEnableMica = uvApp->getIsEnableMica();
	connect(uvApp, &CUVApplication::sigIsEnableMicaChanged, this, [=]() {
		m_isEnableMica = uvApp->getIsEnableMica();
		update();
	});
	uvApp->syncMica(this);
	setAttribute(Qt::WA_DeleteOnClose);
}

CUVCustomWidget::~CUVCustomWidget() = default;

void CUVCustomWidget::setCenterWidget(QWidget* widget) {
	if (!widget) {
		return;
	}

	m_centerWidget = widget;
	m_mainVLayout->addWidget(widget);
	widget->setVisible(true);
}

void CUVCustomWidget::paintEvent(QPaintEvent* event) {
	if (!m_isEnableMica) {
		QPainter painter(this);
		painter.save();
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setBrush(UVThemeColor(m_themeMode, ALThemeType::WindowBase));
		painter.drawRect(rect());
		painter.restore();
	}
}
