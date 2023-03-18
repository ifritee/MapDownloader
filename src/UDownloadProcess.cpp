#include "UDownloadProcess.h"

#include <QProcess>

UDownloadProcess::UDownloadProcess(const QString &procName, const QStringList &args, QObject *parent)
  : QObject{parent}
  , m_processName(procName)
  , m_args(args)
  , m_process(new QProcess(this))
{
  connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [this](int , QProcess::ExitStatus ) {
    emit this->finished();
    m_process->close();
  });
}

UDownloadProcess::~UDownloadProcess()
{
  delete m_process;
}

void UDownloadProcess::process()
{
  m_process->start(m_processName, m_args);
  m_process->waitForFinished(1000);
}

void UDownloadProcess::stop()
{

}
