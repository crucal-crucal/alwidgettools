#include "albasedialog.hpp"

#include <QAbstractNativeEventFilter>
#include <QApplication>
#include <QOperatingSystemVersion>
#include <QPainter>
#include <QPropertyAnimation>
#include <QDebug>

#include "albasedialog_p.hpp"
#include "almaskwidget.hpp"
#include "althememanager.hpp"

#ifdef Q_OS_WIN
#include <dwmapi.h>
#include <windowsx.h>
#endif

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALBaseDialogPrivate
 * Internal class for CALBaseDialog
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALBaseDialogPrivate::CALBaseDialogPrivate(CALBaseDialog* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALBaseDialogPrivate::~CALBaseDialogPrivate() = default;

bool CALBaseDialogPrivate::hasPolicy(const ALDialogPolicy::ShowPolicy& policy) const {
	return showPolicys.testFlag(policy);
}

void CALBaseDialogPrivate::_doCloseAnimation() {
	Q_Q(CALBaseDialog);

	if (hasPolicy(ALDialogPolicy::ShowMask)) {
		maskWidget->doMaskAnimation(0);
	}

	if (hasPolicy(ALDialogPolicy::EnableAnimation)) {
		fadeOutAnimation->start();
	} else {
		q->QDialog::close();
	}
}

/**
 * @brief \class CALBaseDialog
 * @param parent pointer to the parent class
 */
CALBaseDialog::CALBaseDialog(QWidget* parent): QDialog(parent), d_ptr(new CALBaseDialogPrivate(this, this)) {
	Q_D(CALBaseDialog);

	QWidget* targetParent = parent ? parent : QApplication::activeWindow();
	d->maskWidget = new CALMaskWidget(targetParent);
	d->maskWidget->setFixedSize(targetParent->size());
	d->maskWidget->setVisible(false);
	d->maskWidget->move(0, 0);

	resize(400, height());

	d->fadeInAnimation = new QPropertyAnimation(this, "windowOpacity");
	d->fadeInAnimation->setEasingCurve(QEasingCurve::InOutQuad);
	d->fadeInAnimation->setDuration(200);
	d->fadeInAnimation->setStartValue(0.0);
	d->fadeInAnimation->setEndValue(1.0);

	d->fadeOutAnimation = new QPropertyAnimation(this, "windowOpacity");
	d->fadeOutAnimation->setEasingCurve(QEasingCurve::InOutQuad);
	d->fadeOutAnimation->setDuration(200);
	d->fadeOutAnimation->setStartValue(1.0);
	d->fadeOutAnimation->setEndValue(0.0);
	connect(d->fadeOutAnimation, &QPropertyAnimation::finished, this, &QDialog::close);

	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { d->themeMode = mode; });
}

CALBaseDialog::~CALBaseDialog() {
	delete d_func()->fadeInAnimation;
	delete d_func()->fadeOutAnimation;
	d_func()->maskWidget->deleteLater();
}

void CALBaseDialog::setShowPolicys(const ALDialogPolicy::ShowPolicys& policys) {
	d_func()->showPolicys = policys;
}

void CALBaseDialog::setShowPolicy(const ALDialogPolicy::ShowPolicy& policy, const bool enabled) {
	if (enabled) {
		d_func()->showPolicys |= policy;
	} else {
		d_func()->showPolicys &= ~policy;
	}
}

ALDialogPolicy::ShowPolicys CALBaseDialog::showPolicys() const {
	return d_func()->showPolicys;
}

bool CALBaseDialog::hasPolicy(const ALDialogPolicy::ShowPolicy& policy) const {
	return d_func()->hasPolicy(policy);
}

void CALBaseDialog::close() {
	d_func()->_doCloseAnimation();
}

void CALBaseDialog::paintEvent(QPaintEvent* event) {
	Q_D(CALBaseDialog);

	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(ALThemeColor(d->themeMode, ALThemeType::DialogBase));
	painter.drawRect(rect());
	painter.setBrush(ALThemeColor(d->themeMode, ALThemeType::DialogLayoutArea));
	painter.drawRoundedRect(QRectF(0, height() - 60, width(), 60), 8, 8);
	painter.restore();
}

void CALBaseDialog::showEvent(QShowEvent* event) {
	Q_D(CALBaseDialog);

	if (hasPolicy(ALDialogPolicy::ShowMask)) {
		d->maskWidget->setVisible(true);
		d->maskWidget->raise();
		d->maskWidget->setFixedSize(parentWidget()->size());
		d->maskWidget->doMaskAnimation(90);
	}

	if (hasPolicy(ALDialogPolicy::EnableAnimation)) {
		this->setWindowOpacity(0.0);
		d->fadeInAnimation->start();
	}
#ifdef Q_OS_WIN
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
	HWND hwnd = (HWND)d->_currentWinID;
	setShadow(hwnd);
	DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
	bool hasCaption = (style & WS_CAPTION) == WS_CAPTION;
	if (!hasCaption)
	{
		::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_CAPTION);
	}
#endif
#endif

	QDialog::showEvent(event);
}

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool CALBaseDialog::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#else
bool CALBaseDialog::nativeEvent(const QByteArray& eventType, void* message, long* result)
#endif
{
	Q_D(CALBaseDialog);

	// 验证消息类型和指针有效性
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
				wp->flags |= SWP_NOCOPYBITS; // 防止复制位图（即窗口重绘时产生闪烁）

				if (const LRESULT defProcResult = ::DefWindowProcW(hwnd, uMsg, wParam, lParam);
					defProcResult <= LONG_MAX && defProcResult >= LONG_MIN) {
					*result = static_cast<long>(defProcResult);
				} else {
					return false;
				}
				return true;
			}
			return false;
		}
		case WM_NCACTIVATE: {
			*result = true; // 启用非客户区域的激活状态
			return true;
		}
		case WM_NCCALCSIZE: { // 计算客户区大小，决定窗口的边框或阴影
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
			if (wParam == FALSE) {
				return false;
			}

			// 根据窗口状态调整内容边距
			if (::IsZoomed(hwnd)) {
				setContentsMargins(8, 8, 8, 8);
			} else {
				setContentsMargins(0, 0, 0, 0);
			}
			*result = 0;
			return true;
#else
			if (wParam == FALSE) {
				return false;
			}

			RECT* clientRect = &reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam)->rgrc[0];
			if (!::IsZoomed(hwnd)) {
				clientRect->top -= 1;
				clientRect->bottom -= 1;
			}
			*result = WVR_REDRAW;
			return true;
#endif
		}
		case WM_NCHITTEST: { // 拖动改变大小
			if (d->hasPolicy(ALDialogPolicy::EnableResize)) {
				POINT nativeLocalPos{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
				::ScreenToClient(hwnd, &nativeLocalPos);
				RECT clientRect{ 0, 0, 0, 0 };
				::GetClientRect(hwnd, &clientRect);
				const auto clientWidth = clientRect.right - clientRect.left;
				const auto clientHeight = clientRect.bottom - clientRect.top;
				const bool left = nativeLocalPos.x < 8;
				const bool right = nativeLocalPos.x > clientWidth - 8;
				const bool top = nativeLocalPos.y < 8;
				const bool bottom = nativeLocalPos.y > clientHeight - 8;
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
			}
			// 默认情况下是客户区
			*result = HTCLIENT;
			return true;
		}
		default: break;
	}
	return QDialog::nativeEvent(eventType, message, result);
}
#endif
}
