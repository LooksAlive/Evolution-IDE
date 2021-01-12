#include <QFile>
#include <QVBoxLayout>
#include "QHexView/document/buffer/qmemorybuffer.h"
#include "hexview.h"

HexView::HexView(QWidget *parent)
    : QWidget(parent)
{
    auto layout = new QVBoxLayout();
    layout->addWidget(m_HexView = new QHexView(this));
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

HexView::~HexView(){
    delete m_HexView;
}

QString HexView::filePath() const
{
    return m_FilePath;
}

void HexView::open(const QString &path)
{
    auto document = QHexDocument::fromFile<QMemoryBuffer>(path);
    m_HexView->setDocument(document);
    m_FilePath = path;
}

bool HexView::save()
{
    QFile file(m_FilePath);
    if (file.open(QFile::WriteOnly)) {
        m_HexView->document()->saveTo(&file);
        file.close();
        return true;
    }
    return false;
}
