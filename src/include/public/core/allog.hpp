#pragma once

#include "alwidgettoolsdef.hpp"
#include "alsingleton.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALLogPrivate;

class CALWIDGETTOOLS_EXPORT CALLog : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALLog)
	Q_DECLARE_PRIVATE(CALLog)
	Q_PROPERTY(QString logSavePath READ getLogSavePath WRITE setLogSavePath NOTIFY sigLogSavePathChanged)
	Q_PROPERTY(QString logFileName READ getLogFileName WRITE setLogFileName NOTIFY sigLogFileNameChanged)
	Q_PROPERTY(bool isLogFileNameWithTime READ getIsLogFileNameWithTime WRITE setIsLogFileNameWithTime NOTIFY sigIsLogFileNameWithTimeChanged)

public:
	static CALLog* instance();

	void setMessageLogEnable(bool isEnable);

	void setLogSavePath(const QString& path);
	[[nodiscard]] QString getLogSavePath() const;

	void setLogFileName(const QString& name);
	[[nodiscard]] QString getLogFileName() const;

	void setIsLogFileNameWithTime(bool isWithTime);
	[[nodiscard]] bool getIsLogFileNameWithTime() const;

Q_SIGNALS:
	void sigLogSavePathChanged();
	void sigLogFileNameChanged();
	void sigIsLogFileNameWithTimeChanged();
	void sigLogMessage(const QString& msg);

protected:
	const QScopedPointer<CALLogPrivate> d_ptr{ nullptr };

	friend class CALSingleton<CALLog>;

private:
	explicit CALLog(QObject* parent = nullptr);
	~CALLog() override;
};
}
