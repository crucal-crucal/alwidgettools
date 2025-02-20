#include "alcustomwidget.hpp"

#include <QPainter>
#include <QVBoxLayout>

#include "alapplication.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALCustomWidget
 * @param parent pointer to the parent class
 */
CALCustomWidget::CALCustomWidget(QWidget* parent): QDialog(parent) {
	resize(500, 500);
	setObjectName("CALCustomWidget");

	m_appBar = new CALAppBar(this);
	m_appBar->setWindowButtonFlags(ALAppBarType::MinimizeButtonHint | ALAppBarType::MaximizeButtonHint | ALAppBarType::CloseButtonHint);

	m_mainVLayout = new QVBoxLayout(this);
	m_mainVLayout->setContentsMargins(0, 0, 0, 0);

	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [this](const ALThemeType::ThemeMode& mode) {
		m_themeMode = mode;
		update();
	});

	m_isEnableMica = alApp->getIsEnableMica();
	connect(alApp, &CALApplication::sigIsEnableMicaChanged, this, [this]() {
		m_isEnableMica = alApp->getIsEnableMica();
		update();
	});
	alApp->syncMica(this);
	setAttribute(Qt::WA_DeleteOnClose);
}

AL_TAKEOVER_NATIVEEVENT_CPP(CALCustomWidget, m_appBar)

CALCustomWidget::~CALCustomWidget() = default;

void CALCustomWidget::setCenterWidget(QWidget* widget) {
	if (!widget) {
		return;
	}

	m_centerWidget = widget;
	m_mainVLayout->addWidget(widget);
	widget->setVisible(true);
}

void CALCustomWidget::paintEvent(QPaintEvent* event) {
	if (!m_isEnableMica) {
		QPainter painter(this);
		painter.save();
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setBrush(ALThemeColor(m_themeMode, ALThemeType::WindowBase));
		painter.drawRect(rect());
		painter.restore();
	}
}
}
