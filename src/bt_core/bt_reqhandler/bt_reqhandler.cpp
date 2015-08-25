/******************************************************************************
 *   Description:   See class declaration in header file
 *
 ******************************************************************************/

/******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include <QtQml>
#include <QJSValue>
#include <QVariant>
#include <QDebug>

#include "bt_bytearr_read.h"
#include "bt_bytearr_read_write.h"
#include "bt_script_factory.h"
#include "my_util.h"
#include "bt_reqhandler.h"
#include "bt_jshost.h"


using namespace BTCore;

/****************************************************************************
 * CONSTANTS
 ***************************************************************************/

const size_t ReqHandler::INDEX_UNKNOWN = (size_t)-1;



/*******************************************************************************
 * CONSTRUCTOR, DESTRUCTOR
 ******************************************************************************/

ReqHandler::ReqHandler(
        QString title,
        std::shared_ptr<JSHost> p_jshost,
        QString script_func_code
        ) :
    title(title),
    p_jshost(p_jshost),
    script_func_code(script_func_code),
    last_error(Error::UNKNOWN),
    p_response(),
    last_exception_details(),
    handler_index(INDEX_UNKNOWN)
{

}

ReqHandler::ReqHandler(
        QString title,
        QString script_func_code
        ) :
    ReqHandler(
            title,
            std::shared_ptr<JSHost>(),
            script_func_code
            )
{

}

ReqHandler::ReqHandler() :
    ReqHandler(
            "Untitled handler",
            std::shared_ptr<JSHost>(),
            "(function(inputMsg){ \n"
            "     var handled = false;\n"
            "     var outputArr;\n"

            "     if (false){\n"
            "        outputArr = factory.createByteArr();\n"
            "        outputArr.putU08(1, 0x04);\n"
            "        handled = true;\n"
            "     };\n"

            "     return {\n"
            "        handled: handled,\n"
            "        response: outputArr\n"
            "     };\n"
            " })\n"
            )
{

}



/*******************************************************************************
 * STATIC METHODS
 ******************************************************************************/

/* private      */

/* protected    */

/* public       */


/*******************************************************************************
 * METHODS
 ******************************************************************************/

/* private      */

/* protected    */

/* public       */

void ReqHandler::setJSHost(std::shared_ptr<JSHost> p_jshost)
{
    this->p_jshost = p_jshost;
}

QString ReqHandler::getTitle() const
{
    return title;
}

void ReqHandler::setTitle(const QString &title)
{
    this->title = title;
    emit titleChanged(this->title);
}

QString ReqHandler::getScript() const
{
    return script_func_code;
}

void ReqHandler::setScript(QString script_func_code)
{
    this->script_func_code = script_func_code;
    emit scriptChanged(this->script_func_code);
}

ReqHandler::Result ReqHandler::handle(
        QJSValue input_msg_jsval,
        QJSValue script_ctx_jsval
        )
{
    //-- before handling, set global properties

    ReqHandler::Result ret = Result::UNKNOWN;

    //-- before running the handler, set response to an empty pointer.
    p_response = std::shared_ptr<ByteArrReadWrite>();
    last_error = Error::UNKNOWN;
    last_exception_details = QVariantMap();

    QJSValue result;



    //-- try to evaluate JavaScript code
    QJSValue func = p_jshost->evaluate(
            script_func_code,
            title
            );

    if (func.isError()){
        //-- evaluation is failed: some error in JavaScript code.

        ret = Result::ERROR;
        last_error = Error::EXCEPTION;
        last_exception_details = MyUtil::qjsErrorToVariant(func);
        qDebug() << "exception 1: " << last_exception_details;

    } else if (!func.isCallable()){
        //-- evaluation has succeed, but the script has evaluated to
        //   non-function value.

        ret = Result::ERROR;
        last_error = Error::SCRIPT_IS_NOT_FUNCTION;

    } else {

        //-- ok, we have function value. Try to call it,
        //   passing the scripts context as `this`.
        QJSValue returned = func.callWithInstance(
                script_ctx_jsval,
                QJSValueList() << input_msg_jsval
                );

        if (returned.isError()){
            //-- returned value is an error: some exception has happened
            //   during handler invocation

            ret = Result::ERROR;
            last_error = Error::EXCEPTION;
            last_exception_details = MyUtil::qjsErrorToVariant(returned);
            qDebug() << "exception 2: " << last_exception_details;
        } else {
            //-- ok, handler has executed successfully. Let's see the result:
            //   whether the request was handled or not.

            bool handled = returned.property("handled").toBool();
            if (handled){
                //-- yes, request is handled
                ret = Result::OK_HANDLED;

                if (returned.property("response").isQObject()){
                    //-- we have "response" property, which, if defined, should
                    //   contain response to send. So, remember it: the client
                    //   may want to get it by calling `getResponse()`.

                    p_response = std::shared_ptr<ByteArrReadWrite>(
                            dynamic_cast<ByteArrReadWrite *>(
                                returned.property("response").toQObject()
                                )
                            );
                    //-- don't forget to set ownership of this object to
                    //   C++ code, so that JavaScript engine won't
                    //   garbage-collect it.
                    QQmlEngine::setObjectOwnership(
                            p_response.get(),
                            QQmlEngine::CppOwnership
                            );

                    //-- p_response now contains the byte array that was
                    //   written by the script
                } else {
                    //-- although the request is handled, we have no response.
                    //   Ok, response is already set to an empty pointer, so,
                    //   leave it as it is.
                }

            } else {
                //-- request was not handled. That's ok.
                ret = Result::OK_NOT_HANDLED;
            }

        }

    }

    return ret;
}

ReqHandler::Error ReqHandler::getLastError()
{
    return last_error;
}

QVariantMap ReqHandler::getLastExceptionDetails()
{
    return last_exception_details;
}

std::shared_ptr<const std::vector<uint8_t>> ReqHandler::getResponse()
{
    if (p_response != nullptr){
        return p_response->getData();
    } else {
        return std::make_shared<const std::vector<uint8_t>>();
    }
}

void ReqHandler::setHandlerIndex(size_t index)
{
    handler_index = index;
}

size_t ReqHandler::getHandlerIndex()
{
    return handler_index;
}

/*******************************************************************************
 * SLOTS
 ******************************************************************************/

/* private      */

/* protected    */

/* public       */


