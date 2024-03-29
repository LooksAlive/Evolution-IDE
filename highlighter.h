#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QString>
#include <QVector>
#include <QRegExp>
#include <QTextCharFormat>
#include <QDomDocument>
#include <QHash>
#include <QStringList>

#include "EnvironmentSettings.h"

/*

Only temporary highliter taken from Qt examples,
intill set up clang to choose what to highlight

*/

class Highlighter : public QSyntaxHighlighter {
Q_OBJECT
public:
    explicit Highlighter(const QString &filename, QObject *parent = 0);

    void highlightBlock(const QString &text) override;
    bool setExtension(const QString &_filename);
private:
    QString xml_filename;
    QRegExp current_extension;
    struct HighlightFormat{
        QRegExp pattern;
        QTextCharFormat format;
    };
    struct HighlightLang{
        QVector <HighlightFormat> for_keywords;
        QRegExp comment_beg_exp;
        QRegExp comment_end_exp;
        QTextCharFormat single_comment;
        QTextCharFormat multi_comment;
        QTextCharFormat keyword_format;
        QTextCharFormat classname_format;
        QTextCharFormat function_format;
        QTextCharFormat quotation_format;
        QTextCharFormat preproc_format;
    };
    /*
     // recognizing file extension and chose language word xml file to do thing
     QRegExp file_extension;
     QVector <HighlightFormat> for_keywords;
     QRegExp comment_beg_exp;
     QRegExp comment_end_exp;
     QTextCharFormat single_comment;
     QTextCharFormat multi_comment;
     QTextCharFormat keyword_format;
     QTextCharFormat classname_format;
     QTextCharFormat function_format;
     QTextCharFormat quotation_format;
     QTextCharFormat preproc_format;

     */

    QHash<QRegExp, HighlightLang> langs;


};

#endif // HIGHLIGHTER_H
