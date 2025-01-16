#pragma once

#include <QObject>

/**
 * @brief \namespace AL
 */
namespace AL {
class CALLog;

class CALLogPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALLogPrivate)
	Q_DECLARE_PUBLIC(CALLog)

public:
	explicit CALLogPrivate(CALLog* q, QObject* parent = nullptr);
	~CALLogPrivate() override;

protected:
	CALLog* const q_ptr{ nullptr };

private:
	QString logSavePath{};
	QString logFileName{};
	bool isLogFileNameWithTime{};

	static void messageLogHander(QtMsgType type, const QMessageLogContext& context, const QString& msg);
	void clearLogFile() const;
	static void writeLogToFile(const QString& logInfo, const CALLog* log);
};
}
