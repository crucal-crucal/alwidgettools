#include "uvcustomwidget.hpp"

#include <QPainter>
#include <QVBoxLayout>

#include "uvapplication.hpp"
#include "uvtheme.hpp"

Q_TAKEOVER_NATIVEEVENT_CPP(CUVCustomWidget, m_appBar)

CUVCustomWidget::CUVCustomWidget(QWidget* parent): QDialog(parent) {
	resize(500, 500);
	setObjectName("CUVCustomWidget");

	m_appBar = new CUVAppBar(this);
	m_appBar->setWindowButtonFlags(UVAppBarType::MinimizeButtonHint | UVAppBarType::MaximizeButtonHint | UVAppBarType::CloseButtonHint);

	m_mainVLayout = new QVBoxLayout(this);
	m_mainVLayout->setContentsMargins(0, 0, 0, 0);

	m_themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const UVThemeType::ThemeMode& mode) {
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
		painter.setBrush(UVThemeColor(m_themeMode, UVThemeType::WindowBase));
		painter.drawRect(rect());
		painter.restore();
	}
}
