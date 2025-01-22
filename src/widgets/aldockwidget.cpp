#include "aldockwidget.hpp"

#include <QEvent>
#include <QPainter>

#include "alapplication.hpp"
#include "aldockwidgettitlebar.hpp"
#include "aldockwidget_p.hpp"
#include "althememanager.hpp"
#include "alwinshadowhelper.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALDockWidgetPrivate
 * Internal class for CALDockWidget
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALDockWidgetPrivate::CALDockWidgetPrivate(CALDockWidget* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALDockWidgetPrivate::~CALDockWidgetPrivate() = default;

void CALDockWidgetPrivate::slotThemeModeChanged(const ALThemeType::ThemeMode& themeMode) {
	q_func()->update();
	this->themeMode = themeMode;
}

/**
 * @brief \class CALDockWidget
 * @param parent pointer to the parent class
 * @param flags window flags \enum Qt::WindowFlags
 */
CALDockWidget::CALDockWidget(QWidget* parent, const Qt::WindowFlags flags): QDockWidget(parent, flags), d_ptr(new CALDockWidgetPrivate(this, this)) {
	Q_D(CALDockWidget);

	setObjectName("CALDockWidget");
	d->margins = 8;
	d->currentWinID = 0;
	d->shadowBorderWidth = 6;
	d->titleBar = new CALDockWidgetTitleBar(this);
	setTitleBarWidget(d->titleBar);
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, d, &CALDockWidgetPrivate::slotThemeModeChanged);
	d->isEanbleMica = alApp->getIsEnableMica();
	connect(alApp, &CALApplication::sigIsEnableMicaChanged, this, [=]() {
		d->isEanbleMica = alApp->getIsEnableMica();
		update();
	});
	connect(this, &CALDockWidget::topLevelChanged, this, [=](const bool topLevel) {
		alApp->syncMica(this, topLevel);
		d_func()->titleBar->changeFloatButtonIcon(topLevel);
	});
}

CALDockWidget::CALDockWidget(const QString& title, QWidget* parent, const Qt::WindowFlags flags): CALDockWidget(parent, flags) {
	setWindowTitle(title);
}

CALDockWidget::~CALDockWidget() = default;

void CALDockWidget::paintEvent(QPaintEvent* event) {
	Q_D(CALDockWidget);

	if (isFloating()) {
		QPainter painter(this);
		painter.save();
		painter.setRenderHint(QPainter::Antialiasing);
		// bacground
#ifdef  Q_OS_WIN
		if (!d->isEanbleMica) {
			painter.setPen(Qt::NoPen);
			painter.setBrush(ALThemeColor(d->themeMode, ALThemeType::DialogBase));
			painter.drawRect(rect());
		}
#else
		painter.setPen(Qt::NoPen);
		painter.setBrush(d->isEanbleMica ? Qt::transparent : ALThemeColor(d->themeMode, ALThemeType::DialogBase));
		painter.drawRect(rect());
#endif
		painter.restore();
	}

	QDockWidget::paintEvent(event);
}

#ifdef Q_OS_WIN
bool CALDockWidget::event(QEvent* event) {
	Q_D(CALDockWidget);

	switch (event->type()) {
		case QEvent::Show:
		case QEvent::MouseButtonRelease: {
			auto hwnd = reinterpret_cast<HWND>(d->currentWinID);
			DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
			::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME);
			setShadow(hwnd);
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
			bool hasCaption = (style & WS_CAPTION) == WS_CAPTION;
			if (!hasCaption)
			{
				::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_CAPTION);
			}
#endif
			break;
		}
		default: {
			break;
		}
	}

	return QDockWidget::event(event);
}
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool CALDockWidget::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#else
bool CALDockWidget::nativeEvent(const QByteArray& eventType, void* message, long* result)
#endif
{
	Q_D(CALDockWidget);

	if ((eventType != "windows_generic_MSG") || !message) {
		return false;
	}
	const auto msg = static_cast<const MSG*>(message);
	HWND hwnd = msg->hwnd;
	if (!hwnd) {
		return false;
	}
	d->currentWinID = reinterpret_cast<qint64>(hwnd);
	const UINT uMsg = msg->message;
	const WPARAM wParam = msg->wParam;
	const LPARAM lParam = msg->lParam;
	switch (uMsg) {
		case WM_WINDOWPOSCHANGING: {
			if (const auto wp = reinterpret_cast<WINDOWPOS*>(lParam); wp != nullptr && (wp->flags & SWP_NOSIZE) == 0) {
				wp->flags |= SWP_NOCOPYBITS;
				*result = static_cast<long>(::DefWindowProcW(hwnd, uMsg, wParam, lParam));
				return true;
			}
			return false;
		}
		case WM_NCACTIVATE: {
			*result = TRUE;
			return true;
		}
		case WM_NCHITTEST: {
			POINT nativeLocalPos{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			::ScreenToClient(hwnd, &nativeLocalPos);
			RECT clientRect{ 0, 0, 0, 0 };
			::GetClientRect(hwnd, &clientRect);
			const auto clientWidth = clientRect.right - clientRect.left;
			const auto clientHeight = clientRect.bottom - clientRect.top;
			const bool left = nativeLocalPos.x < d->margins;
			const bool right = nativeLocalPos.x > clientWidth - d->margins;
			const bool top = nativeLocalPos.y < d->margins;
			const bool bottom = nativeLocalPos.y > clientHeight - d->margins;
			*result = 0;
			if (!window()->isFullScreen() && !window()->isMaximized()) {
				if (left && top) {
					*result = HTTOPLEFT;
				} else if (left && bottom) {
					*result = HTBOTTOMLEFT;
				} else if (right && top) {
					*result = HTTOPRIGHT;
				} else if (right && bottom) {
					*result = HTBOTTOMRIGHT;
				} else if (left) {
					*result = HTLEFT;
				} else if (right) {
					*result = HTRIGHT;
				} else if (top) {
					*result = HTTOP;
				} else if (bottom) {
					*result = HTBOTTOM;
				}
			}
			if (0 != *result) {
				return true;
			}
			*result = HTCLIENT;
			return true;
		}
		case WM_NCCALCSIZE: {
			if (wParam == FALSE) {
				return false;
			}
			*result = 0;
			return true;
		}
		default: {
			break;
		}
	}


	return QDockWidget::nativeEvent(eventType, message, result);
}
#endif
}
