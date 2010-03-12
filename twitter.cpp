#include "twitter.h"
#include <QTextDocument>
namespace Twitter{
    QString html2plainText(const QString& html){
        static QTextDocument *s_doc;
        if(!s_doc) s_doc = new QTextDocument(NULL);
        s_doc->setHtml(html);
        return s_doc->toPlainText();
    }
}
