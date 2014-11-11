#include "gtapi.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QUrl>
#include <QUrlQuery>


GTApi::GTApi(QObject *parent) :
    QObject(parent), net(new QNetworkAccessManager(this))
{}

GTApi::~GTApi() {}

void GTApi::translate(const QString &text, const QString &dstLang, const QString &srcLang, const QString &posLang)
{
    /*Google Translate URL*/
    QUrl translateUrl = QString("https://translate.google.com/translate_a/single");

    /* Query String*/
    QUrlQuery query;
    query.addQueryItem("client", "t"); // client=j - responce is in json
    query.addQueryItem("sl", srcLang);
    query.addQueryItem("tl", dstLang);
    query.addQueryItem("hl", posLang); // language for naming the "parts of speech".
    query.addQueryItem("ie", "UTF-8");
    query.addQueryItem("oe", "UTF-8");
    query.addQueryItem("dt", "bd");query.addQueryItem("dt", "ex");query.addQueryItem("dt", "ld");query.addQueryItem("dt", "md");
    query.addQueryItem("dt", "qc");query.addQueryItem("dt", "rw");query.addQueryItem("dt", "rm");query.addQueryItem("dt", "ss");
    query.addQueryItem("dt", "t"); query.addQueryItem("dt", "at");query.addQueryItem("dt", "sw");
    //query.addQueryItem("prev", "btn"); query.addQueryItem("rom", "1"); query.addQueryItem("srcrom", "1"); query.addQueryItem("ssel", "3"); query.addQueryItem("tsel", "0");
    //dt=bd&dt=ex&dt=ld&dt=md&dt=qc&dt=rw&dt=rm&dt=ss&dt=t&dt=at&dt=sw
    query.addQueryItem("q", text);
    translateUrl.setQuery(query);

    QNetworkRequest request(translateUrl);
    request.setRawHeader("user-agent", "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/35.0.1916.114 Safari/537.36");
    request.setRawHeader("accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    request.setRawHeader("cache-control", "no-cache");
    request.setRawHeader("pragma", "no-cache");

    QNetworkReply *reply = net->get(request);
    connect(reply, SIGNAL(finished()), SLOT(onReplyFinished()));
}

void GTApi::onReplyFinished() {
    QNetworkReply *reply = reinterpret_cast<QNetworkReply*>(sender());

    if (reply->error()) {
        //gtReply.setNetworkErrorMessage(reply->errorString());
        qWarning() << Q_FUNC_INFO << "Network Error: " << reply->errorString();
    }

    QByteArray rawReply = reply->readAll();
    GTReplyObject gtObject;
    if ( !rawReply.isEmpty() ){
        qDebug() << "[Parser] Starting";
        gtObject = GTReplyObject::fromRawString(rawReply);
        qDebug() << "[Parser] No errors: " << (QString(rawReply) == gtObject.toRawString());
    } else {
        qDebug() << Q_FUNC_INFO << "Empty Reply";
    }
    GTApiTranslation gtApiTr(gtObject);
    //QUrlQuery query (reply->request().url());
    //gtReply.query_srclang = query.queryItemValue("sl");
    //gtReply.query_dstlang = query.queryItemValue("tl");
    //gtReply.query_hllang  = query.queryItemValue("hl");

    emit translationReady(gtApiTr);
    reply->deleteLater();
}
