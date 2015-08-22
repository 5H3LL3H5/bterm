/*******************************************************************************
 *   Description:   See class declaration below
 *
 ******************************************************************************/

#ifndef _HTREQHANDLER_H
#define _HTREQHANDLER_H

/*******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include <QString>
#include <QJSValue>
#include <QVariantMap>

#include <memory>


class QJSEngine;

namespace HTCore {
    class ScriptFactory;
}

/*******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

namespace HTCore {
    class ReqHandler;
    class ByteArrReadWrite;
}

/**
 * Response generator
 * TODO
 */
class HTCore::ReqHandler : public QObject
{
Q_OBJECT
    /****************************************************************************
     * TYPES
     ***************************************************************************/
public:

    enum class Result {
        UNKNOWN,
        OK_NOT_HANDLED,
        OK_HANDLED,
        ERROR,
    };

    enum class Error {
        UNKNOWN,
        SCRIPT_IS_NOT_FUNCTION,
        EXCEPTION,
    };

    /****************************************************************************
     * CONSTRUCTOR, DESTRUCTOR
     ***************************************************************************/
public:
    ReqHandler(
            QString title,
            std::shared_ptr<QJSEngine> p_engine,
            std::shared_ptr<ScriptFactory> p_script_factory,
            QString script_func_code
            );

    ReqHandler(
            QString title,
            QString script_func_code = ""
            );


    /****************************************************************************
     * PRIVATE DATA
     ***************************************************************************/
private:

    QString title;
    std::shared_ptr<QJSEngine> p_engine;
    std::shared_ptr<ScriptFactory> p_script_factory;
    QString script_func_code;
    Error last_error;
    std::shared_ptr<ByteArrReadWrite> p_response;
    QVariantMap last_exception_details;


    /****************************************************************************
     * STATIC METHODS
     ***************************************************************************/

    /****************************************************************************
     * METHODS
     ***************************************************************************/
public:

    void setQJSEngine(std::shared_ptr<QJSEngine> p_engine);
    void setScriptFactory(std::shared_ptr<ScriptFactory> p_script_factory);
    QString getTitle() const;
    void setTitle(const QString &title);
    QString getScript() const;
    void setScript(QString script_func_code);

    /**
     * TODO
     */
    Result handle(
            QJSValue input_msg_jsval,
            QJSValue script_ctx_jsval
            );

    Error getLastError();

    QVariantMap getLastExceptionDetails();

    /**
     * If latest `#HTCore::ReqHandler::handle()` call returned
     * `#HTCore::ReqHandler::Result::OK_HANDLED`, then this method returns
     * response that was generated by the handler script.
     */
    std::shared_ptr<const std::vector<uint8_t>> getResponse();


    /****************************************************************************
     * SIGNALS, SLOTS
     ***************************************************************************/

signals:

    void nameChanged(const QString &title);

};


#endif // _HTREQHANDLER_H
