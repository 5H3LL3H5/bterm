/******************************************************************************
 *   Description:   See class declaration in header file
 *
 ******************************************************************************/

/******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include <QDebug>
#include <QtQml>
#include <QJSValue>

#include <iostream>

#include "bt_project.h"
#include "bt_event_data_raw.h"
#include "bt_event_data_msg.h"
#include "bt_event_sys.h"

#include "my_util.h"

#include "bt_bytearr_read.h"
#include "bt_bytearr_read_write.h"
#include "bt_codec_factory.h"
#include "bt_codec_transparent.h"
#include "bt_jshost.h"


using namespace BTCore;
using namespace std;

/*******************************************************************************
 * CONSTRUCTOR, DESTRUCTOR
 ******************************************************************************/

Project::Project(
        QString title
        ) :
    title(title),
    p_codec(),
    all_codecs({}),
    p_io_dev(),

    //-- JSHost will be actually created later, in `init()`, since we can't
    //   call `getSharedPtr()` from constructor
    p_jshost(),
    handlers(),
    codec_specific_handlers(),
    baudrate(9600),
    dirty(true)
{
    addKnownCodec(
            std::make_shared<CodecTransparent>(
                BTCore::CodecIdx::TRANSPARENT
                )
            );
    setCurrentCodecIdx(BTCore::CodecIdx::TRANSPARENT);

    //-- if title is empty, set it to "New project"
    if (this->title.isEmpty()){
        this->title = "New project";
    }

}

Project::~Project()
{
    //-- close IO device
    if (p_io_dev != nullptr){
        if (this->p_io_dev->isOpened()){
            p_io_dev->close();
        }
    }
}


/*******************************************************************************
 * STATIC METHODS
 ******************************************************************************/

/* private      */

/* protected    */

/* public       */

std::shared_ptr<Project> Project::create(
        QString title
        )
{
    //-- NOTE: we can't use std::make_shared, since Project constructor
    //   is private
    std::shared_ptr<Project> p_proj {new Project(title)};
    p_proj->init();

    return p_proj;
}


/*******************************************************************************
 * METHODS
 ******************************************************************************/

/* private      */

void Project::init()
{
    p_jshost = JSHost::create( getSharedPtr() );

    connect(
            p_jshost.get(), &JSHost::scriptMessage,
            this, &Project::scriptMessage
           );
}

ReqHandler::Result Project::runHandler(
        std::shared_ptr<ReqHandler> p_handler,
        QJSValue input_msg_jsval
        )
{
    //-- try to handle the request with current handler
    ReqHandler::Result res = p_handler->handle(
            input_msg_jsval
            );

    //-- take action depending on handling result
    switch (res){
        case ReqHandler::Result::UNKNOWN:
            qDebug("unknown result: should never be here");
            break;

        case ReqHandler::Result::OK_NOT_HANDLED:
            //-- this handler did not handle the request.
            //   Ok, we will proceed to the next handler (if any)
            break;

        case ReqHandler::Result::OK_HANDLED:
            //-- request is handled
            break;

        case ReqHandler::Result::ERROR:
            //-- some error has happened during executing the handler.
            //   (this is error in JavaScript handler code)
            qDebug() << "error";
            break;
    }

    return res;
}



/* protected    */

void Project::writeEncoded(
        const std::vector<uint8_t> &data,
        QString descr
        )
{
    if (data.size() > 0){
        DataMsg msg_tx = p_codec->encodeMessage(data);
        auto p_data_raw_tx = msg_tx.getRawData();
        p_io_dev->write(*p_data_raw_tx);

        //-- emit an event about outgoing (Tx) message
        auto p_event = std::make_shared<EventDataMsg>(
                msg_tx,
                EventDataMsg::Direction::TX,
                descr
                );
        emit event(p_event);
    } else {
        //-- nothing to send
    }
}

void Project::writePlain(
        const std::vector<uint8_t> &data,
        QString descr
        )
{
    if (data.size() > 0){
        DataMsg msg_tx;

        msg_tx.addData(DataPart::DataType::USER, data);

        auto p_data_raw_tx = msg_tx.getRawData();
        p_io_dev->write(*p_data_raw_tx);

        //-- emit an event about outgoing (Tx) message
        //
        //   TODO: probably, create new kind of event, like,
        //   EventDataRawTx? Then, we won't need to create dummy
        //   DataMsg above
        auto p_event = std::make_shared<EventDataMsg>(
                msg_tx,
                EventDataMsg::Direction::TX,
                descr
                );
        emit event(p_event);
    } else {
        //-- nothing to send
    }
}



/* public       */

std::shared_ptr<Project> Project::getSharedPtr()
{
    return shared_from_this();
}

void Project::setIODev(std::shared_ptr<IODev> p_io_dev)
{
    //-- if we already have some IODev set, then unsubscribe from its events
    if (this->p_io_dev != nullptr){
        if (this->p_io_dev->isOpened()){
            this->p_io_dev->close();
        }

        disconnect(
                this->p_io_dev.get(), &IODev::readyRead,
                this, &Project::onIODevReadyRead
               );
        disconnect(
                this->p_io_dev.get(), &IODev::baudRateChanged,
                this, &Project::onIODevBaudRateChanged
               );

    }

    this->p_io_dev = p_io_dev;

    //-- get existing received data and discard it
    this->p_io_dev->read();

    connect(
            this->p_io_dev.get(), &IODev::readyRead,
            this, &Project::onIODevReadyRead
           );

    //-- when baudrate of IODev changes, we want to keep our copy up-to-date
    connect(
            this->p_io_dev.get(), &IODev::baudRateChanged,
            this, &Project::onIODevBaudRateChanged
           );

    //-- check if we baudrate of IODev differs from the needed one
    if (p_io_dev->getBaudRate() != this->baudrate){
        //-- it differs; we need to change it.

        if (!this->p_io_dev->isOpened()){
            //-- ok, port is not opened, so, set baudrate and open it.
            this->p_io_dev->setBaudRate(this->baudrate);
            this->p_io_dev->open();
        } else {
            //-- port is already opened, so we can't alter its baudrate.
            //   For now, just echo warning. In the future, we may to close it,
            //   set baudrate, and open it again.  The difficulty is that this
            //   should be done asynchronously (when device is actually closed,
            //   the signal `openStatusChanged()` is emitted)
            auto p_event = std::make_shared<EventSys>(
                    MsgLevel::WARNING,
                    QString("IO device is opened when setting it to project. ")
                    + QString("Please change baudrate manually to ")
                    + QString::number(this->baudrate)
                    );
            emit event(p_event);
        }
    } else if (!this->p_io_dev->isOpened()){
        this->p_io_dev->open();
    }
}

std::shared_ptr<IODev> Project::getIODev() const
{
    return p_io_dev;
}

void Project::setCurrentCodecIdx(CodecIdx codec_idx)
{
    //-- if we already have some Codec set, then unsubscribe from its events
    if (p_codec != nullptr){
        disconnect(
                p_codec.get(), &Codec::messageDecoded,
                this, &Project::onMessageDecoded
                );
        disconnect(
                p_codec.get(), &Codec::settingsChanged,
                this, &Project::markDirty
               );
    }

    CodecFactory codec_factory {};

    if (all_codecs[static_cast<size_t>(codec_idx)] == nullptr){
        all_codecs[static_cast<size_t>(codec_idx)] =
            codec_factory.createCodec(codec_idx);
    }

    p_codec = all_codecs[static_cast<size_t>(codec_idx)];
    codec_specific_handlers = p_codec->getStdHandlers();

    //-- set JSHost to each codec-specific handler
    for (auto p_handler : codec_specific_handlers){
        p_handler->setJSHost(p_jshost);
    }

    connect(
            p_codec.get(), &Codec::messageDecoded,
            this, &Project::onMessageDecoded
           );
    connect(
            p_codec.get(), &Codec::settingsChanged,
            this, &Project::markDirty
           );

    emit currentCodecIdxChanged(p_codec);
    setDirty(true);
}

void Project::addKnownCodec(std::shared_ptr<Codec> p_new_codec)
{
    CodecIdx new_codec_idx = p_new_codec->getCodecIdx();
    all_codecs[static_cast<size_t>(new_codec_idx)] = p_new_codec;

    //-- refresh current p_codec, if we've just updated codec with the number
    //   of current codec
    if (p_codec != nullptr && p_codec->getCodecIdx() == new_codec_idx){
        p_codec = all_codecs[static_cast<size_t>(new_codec_idx)];
    }
}

void Project::setIODevBaudRate(int32_t baudrate)
{
    if (this->baudrate != baudrate){
        this->baudrate = baudrate;
        setDirty(true);
    }

    if (p_io_dev != nullptr){
        p_io_dev->setBaudRate(baudrate);

        //NOTE: baudrate will be updated again very soon,
        //      in the slot `onIODevBaudRateChanged()`.
        //      But NOTE that it doesn't happen if p_io_dev
        //      is nullptr, of course.
    }
}

int32_t Project::getIODevBaudRate()
{
    return this->baudrate;
}


void Project::addHandler(std::shared_ptr<ReqHandler> p_handler)
{
    //-- set JavaScript engine and factory to the handler
    p_handler->setJSHost(p_jshost);

    //-- add handler
    handlers.push_back(p_handler);

    //-- subscribe for the handler events
    connect(
            p_handler.get(), &ReqHandler::titleChanged,
            this, &Project::markDirty
           );
    connect(
            p_handler.get(), &ReqHandler::scriptChanged,
            this, &Project::markDirty
           );

    size_t handler_index = handlers.size() - 1;

    p_handler->setHandlerIndex(handler_index);

    //-- notify listeners about new handler
    emit reqHandlerAdded(p_handler, handler_index);
    setDirty(true);
}

std::shared_ptr<ReqHandler> Project::getHandler(size_t index)
{
    std::shared_ptr<ReqHandler> p_ret {};

    if (index < handlers.size()){
        p_ret = handlers[index];
    }

    return p_ret;
}

void Project::removeHandler(size_t index)
{
    if (index < handlers.size()){
        std::shared_ptr<ReqHandler> p_handler = handlers[index];

        handlers.erase(handlers.begin() + index);

        size_t cur_index = index;
        for (
                auto pp_cur_handler = (handlers.begin() + index);
                pp_cur_handler < handlers.end();
                ++pp_cur_handler, ++cur_index
            )
        {
            auto p_cur_handler = *pp_cur_handler;
            p_cur_handler->setHandlerIndex(cur_index);
        }

        //-- notify listeners about removed handler
        emit reqHandlerRemoved(p_handler, index);
        setDirty(true);
    }
}

void Project::moveHandlerUp(size_t index)
{
    if (index < handlers.size() && index > 0){
        std::swap(handlers[index], handlers[index - 1]);

        handlers[index]->setHandlerIndex(index);
        handlers[index - 1]->setHandlerIndex(index - 1);

        //-- notify listeners about removed handler
        emit reqHandlersReordered();
        setDirty(true);
    }
}

void Project::moveHandlerDown(size_t index)
{
    if (index + 1 < handlers.size()){
        std::swap(handlers[index], handlers[index + 1]);

        handlers[index]->setHandlerIndex(index);
        handlers[index + 1]->setHandlerIndex(index + 1);

        //-- notify listeners about removed handler
        emit reqHandlersReordered();
        setDirty(true);
    }
}


size_t Project::getHandlersCnt() const
{
    return handlers.size();
}

std::shared_ptr<Codec> Project::getCodec() const
{
    return p_codec;
}

QString Project::getTitle() const
{
    return title;
}

void Project::setTitle(QString title)
{
    this->title = title;
    emit titleChanged(title);
    setDirty(true);
}

bool Project::isDirty()
{
    return dirty;
}

void Project::setDirty(bool dirty)
{
    this->dirty = dirty;

    emit dirtyStatusChanged(dirty);
}



/*******************************************************************************
 * SLOTS
 ******************************************************************************/

/* private      */

/**
 * Called when new raw data is available to read from IO device
 */
void Project::onIODevReadyRead(int bytes_available)
{
    std::ignore = bytes_available;

    //-- get received data
    std::vector<uint8_t> data = p_io_dev->read();

    //-- emit an event about received raw data
    auto p_event = std::make_shared<EventDataRaw>(data);
    emit event(p_event);

    //-- feed received data as a raw data to codec
    p_codec->addRawRxData( data );
}

void Project::onIODevBaudRateChanged(int32_t baudrate)
{
    if (this->baudrate != baudrate){
        this->baudrate = baudrate;
        setDirty(true);
    }
}

/**
 * Called when new Rx message is decoded
 */
void Project::onMessageDecoded(const DataMsg &msg)
{
    //qDebug(msg.toString().c_str());

    auto p_event = std::make_shared<EventDataMsg>(msg, EventDataMsg::Direction::RX);
    emit event(p_event);

    std::shared_ptr<std::vector<uint8_t>> p_req_data = msg.getUserData();

    QJSValue input_msg_jsval = p_jshost->getHandlerInputMsgObject(p_req_data);

    //QThread::msleep(60);

    //-- iterate through all the request handlers
    for (auto p_handler : handlers){

        ReqHandler::Result res = runHandler(p_handler, input_msg_jsval);

        if (res == ReqHandler::Result::OK_HANDLED){
            //-- if request was handled, then stop iterating handlers,
            //   and exit immediately
            goto handling_finished;
        }
    }

    for (auto p_handler : codec_specific_handlers){

        ReqHandler::Result res = runHandler(p_handler, input_msg_jsval);

        if (res == ReqHandler::Result::OK_HANDLED){
            //-- if request was handled, then stop iterating handlers,
            //   and exit immediately
            goto handling_finished;
        }
    }

handling_finished:
    return;
}

void Project::markDirty()
{
    setDirty(true);
}


/* protected    */

/* public       */


