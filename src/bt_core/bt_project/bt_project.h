/*******************************************************************************
 *   Description:   See class declaration below
 *
 ******************************************************************************/

#ifndef _BT_PROJECT_H
#define _BT_PROJECT_H

/*******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include <memory>

#include <QObject>

#include "bt_iodev.h"
#include "bt_codec.h"
#include "bt_codec_factory.h"
#include "bt_reqhandler.h"
#include "bt_core.h"



namespace BTCore {
    class Event;
    class DataMsg;
    class ScriptFactory;
    class ScriptIO;
}

/*******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

namespace BTCore {
    class Project;
}


/**
 * Class that represents opened project. The project contains
 * name of the codec that is being used (see `#BTCore::Codec`),
 * request handlers, and other things.
 */
class BTCore::Project :
    public QObject,
    public std::enable_shared_from_this<BTCore::Project>
{
Q_OBJECT

    //-- `ScriptIO` only calls protected methods that write to IO:
    //   `writeEncoded()` and others
    friend ScriptIO;


    /****************************************************************************
     * TYPES
     ***************************************************************************/

    /****************************************************************************
     * CONSTRUCTOR, DESTRUCTOR
     ***************************************************************************/
private:
    /**
     * NOTE: this constructor is private, since we have to use additional
     * initialization right after construction, which can't be done right in
     * the constructor (because it involves shared_ptr to this Project). Use
     * `#BTCore::Project::create()` static method instead.
     *
     * Constructs the project with given title.
     */
    explicit Project(
            QString title = ""
            );

public:
    virtual ~Project();

    /****************************************************************************
     * PRIVATE DATA
     ***************************************************************************/
private:

    //-- project's title
    QString title;

    //-- codec that was given to the project's constructor
    std::shared_ptr<Codec> p_codec;

    //-- all possible codecs
    std::shared_ptr<Codec> all_codecs[
        static_cast<size_t>(CodecIdx::_COUNT)
        ];

    //-- IO device being used for communication with outside world
    //   (set by `#BTCore::Project::setIODev()`)
    std::shared_ptr<IODev> p_io_dev;

    //-- JavaScript host BTCore environment
    std::shared_ptr<JSHost> p_jshost;

    //-- sequence of handlers, they get executed sequentially
    //   when new message arrives
    std::vector<std::shared_ptr<ReqHandler>> handlers;

    //-- sequence of codec-specific handlers, they get executed sequentially
    //   after user-defined handlers
    std::vector<std::shared_ptr<ReqHandler>> codec_specific_handlers;

    //-- baudrate that is needed for this project. Will be set to IODev.
    int32_t baudrate;

    //-- true if the project is not saved in its current state;
    //   false otherwise
    bool dirty;

    /****************************************************************************
     * STATIC METHODS
     ***************************************************************************/
public:

    /**
     * Creates the project with given title.
     */
    static std::shared_ptr<Project> create(
            QString title = ""
            );




    /****************************************************************************
     * METHODS
     ***************************************************************************/
public:

    /**
     * Returns shared pointer to this instance of `Project`
     */
    std::shared_ptr<Project> getSharedPtr();

    /**
     * Set IO device, so that we can communicate with outside world
     */
    void setIODev(std::shared_ptr<IODev> p_io_dev);

    /**
     * Return IO device
     */
    std::shared_ptr<IODev> getIODev() const;

    /**
     * Set index of current codec. If the codec being selected was
     * already used during this program run, then existing instance
     * will be used. Otherwise, new instance will be created.
     *
     * You may want to check `setCurrentCodecIdx()` for some details.
     */
    void setCurrentCodecIdx(CodecIdx codec_idx);

    /**
     * The project has an array of "known" codecs, i.e. an array of
     * created instances of codecs.
     *
     * By using this function, we set particular instance of codec
     * for its index (determined by `#BTCore::Codec::getCodecIdx()`),
     * and when `setCurrentCodecIdx()` will be called next time,
     * exactly this instance will be used.
     *
     * If codec with the same index already exists as a "known" one,
     * then after calling this method old instance is removed.
     *
     * This method is primarily needed to make the project behave more
     * friendly in the following situation:
     *
     * - User has set up its codec as he/she wants
     * - User selected different codec (i.e. with different index, see
     *   `#BTCore::CodecIdx`), probably accidentally
     * - User switches back to previously used codec
     *
     * Without the collection of "known" codecs, after the sequence above,
     * settings of the first codec are lost, which is frustrating.
     *
     * But with known codecs, settings will be kept.
     *
     * This known codecs collection is kept only in runtime, it isn't
     * stored to the XML file.
     */
    void addKnownCodec(std::shared_ptr<Codec> p_codec);

    /**
     * Set needed baudrate. It will be propagated to IODev.:
     *
     */
    void setIODevBaudRate(int32_t baudrate);

    /**
     * Get current baudrate
     */
    int32_t getIODevBaudRate();

    /**
     * Add new handler to the end of the sequence of handlers.
     */
    void addHandler(std::shared_ptr<ReqHandler> p_handler);

    /**
     * Get handler at specified index. If index is too large,
     * empty shared_ptr is returned.
     */
    std::shared_ptr<ReqHandler> getHandler(size_t index);

    /**
     * Remove handler at specified index. After handler is removed, the signal
     * `reqHandlerRemoved()` will be emitted.
     */
    void removeHandler(size_t index);

    /**
     * Get handlers count
     */
    size_t getHandlersCnt() const;

    /**
     * Move hander with specified index "up", i.e. swap it with the previous
     * handler.
     */
    void moveHandlerUp(size_t index);

    /**
     * Move hander with specified index "down", i.e. swap it with the next
     * handler.
     */
    void moveHandlerDown(size_t index);

    /**
     * Get codec being currently used by the project
     */
    std::shared_ptr<Codec> getCodec() const;

    /**
     * Get project's title
     */
    QString getTitle() const;

    /**
     * Set project's title
     */
    void setTitle(QString title);

    /**
     * Returns whether the project is dirty
     */
    bool isDirty();

    /**
     * Set whether the project is dirty
     */
    void setDirty(bool dirty);



protected:

    /**
     * Encode given data with current codec, and write it to IO device.
     *
     * Used by `#BTCore::ScriptIO`.
     */
    void writeEncoded(
            const std::vector<uint8_t> &data,
            QString descr
            );

    /**
     * Write given data literally to IO device.
     *
     * Used by `#BTCore::ScriptIO`.
     */
    void writePlain(
            const std::vector<uint8_t> &data,
            QString descr
            );


private:



    /**
     * Should be called right after construction, this is done automatically
     * by `create()` static method
     */
    void init();

    ReqHandler::Result runHandler(
            std::shared_ptr<ReqHandler> p_handler,
            QJSValue input_msg_jsval
            );


    /****************************************************************************
     * SIGNALS, SLOTS
     ***************************************************************************/
signals:

    /**
     * Emitted when some event has happened (say, `EventDataRaw` or
     * `EventDataMsg`)
     */
    void event(std::shared_ptr<Event> p_event);

    /**
     * Emitted when new handler was just added to the sequence of handlers.
     *
     * @param p_handler
     *      handler that was just added
     * @param index
     *      index of newly added handler in the sequence of handlers
     */
    void reqHandlerAdded(std::shared_ptr<ReqHandler> p_handler, size_t index);

    /**
     * Emitted when handler was just removed from the sequence of handlers.
     *
     * @param p_handler
     *      handler that was just removed
     * @param index
     *      index of removed handler. When this signal is emitted, this index
     *      is no more occupied by this handler.
     */
    void reqHandlerRemoved(std::shared_ptr<ReqHandler> p_handler, size_t index);

    /**
     * Emitted when handlers are reordered
     */
    void reqHandlersReordered();

    /**
     * Emitted when human-readable title has changed by calling
     * `setTitle()`.
     */
    void titleChanged(const QString &title);

    /**
     * Emitted when current codec is changed by calling `setCurrentCodecIdx()`
     */
    void currentCodecIdxChanged(std::shared_ptr<Codec> p_codec);

    /**
     * Emitted whenever dirty/saved status is changed
     */
    void dirtyStatusChanged(bool dirty);

    /**
     * Emitted when script sends a message to the console, using, for example,
     * `console.log()` call.
     *
     * This signal is just forwarded from `#BTCore::JSHost`.
     */
    void scriptMessage(const QString &text, BTCore::MsgLevel level);



private slots:

    void onIODevReadyRead(int bytes_available);
    void onIODevBaudRateChanged(int32_t baudrate);
    void onMessageDecoded(const DataMsg &msg);

    void markDirty();


};


#endif // _BT_PROJECT_H
