/*******************************************************************************
 *   Description:   See class declaration below
 *
 ******************************************************************************/

#ifndef _BT_PROJECT_STORAGE_XML_H
#define _BT_PROJECT_STORAGE_XML_H

/*******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include <QObject>
#include <QDomElement>
#include <memory>




namespace BTCore {
    class Project;
    class Codec;
    class Codec_ISO14230;
    class ReqHandler;

    class CodecVisitor_SaveXML;
}

class QIODevice;
class QDomDocument;

/*******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

namespace BTCore {
    class ProjectStorageXML;
}


/**
 * Class that provides loading and saving `#BTCore::Project`
 * from and to an XML format.
 */
class BTCore::ProjectStorageXML : public QObject
{
Q_OBJECT

    friend CodecVisitor_SaveXML;

    /****************************************************************************
     * TYPES
     ***************************************************************************/

    /****************************************************************************
     * CONSTRUCTOR, DESTRUCTOR
     ***************************************************************************/
public:
    ProjectStorageXML(
            std::shared_ptr<QIODevice> p_device
            );

    /****************************************************************************
     * PRIVATE DATA
     ***************************************************************************/
private:

    //-- IO device to use when reading/saving the project
    std::shared_ptr<QIODevice> p_device;


    static const QString XML_TAG_NAME__PROJECT;
    static const QString XML_TAG_NAME__CODECS;
    static const QString XML_TAG_NAME__CODEC;
    static const QString XML_TAG_NAME__REQ_HANDLERS;
    static const QString XML_TAG_NAME__REQ_HANDLER;
    static const QString XML_TAG_NAME__RH_CODE;

    static const QString XML_ATTR_NAME__COMMON__TITLE;

    static const QString XML_ATTR_NAME__CODEC_ISO14230__LOCAL_ADDR;
    static const QString XML_ATTR_NAME__CODEC_ISO14230__REMOTE_ADDR;

    static const QString CODEC_NAME__ISO14230;

    /****************************************************************************
     * STATIC METHODS
     ***************************************************************************/

    /****************************************************************************
     * METHODS
     ***************************************************************************/
public:

    /**
     * Read project from QIODevice that was given to the constructor
     *
     * NOTE: in case of error, std::invalid_argument is thrown,
     * so you should be prepared to catch it.
     */
    std::shared_ptr<Project> readProject();

    /**
     * Save project to QIODevice that was given to the constructor
     */
    void saveProject(std::shared_ptr<Project> p_proj);



private:

    QDomElement getSingleChildElementByTagName(
            const QDomElement &elem_parent,
            QString tagName
            );

    std::shared_ptr<Codec> readCodecFromDomElement(
            const QDomElement &elem_codec
            );

    std::shared_ptr<Codec_ISO14230> readCodecIso14230FromDomElement(
            const QDomElement &elem_codec
            );

    std::shared_ptr<ReqHandler> readReqHandlerFromDomElement(
            const QDomElement &elem_rh
            );


    std::shared_ptr<QDomElement> saveCodecToDomElement(
            QDomDocument &doc, std::shared_ptr<Codec> p_codec
            );

    std::shared_ptr<QDomElement> saveReqHandlerToDomElement(
            QDomDocument &doc, std::shared_ptr<ReqHandler> p_handler
            );



    /****************************************************************************
     * SIGNALS, SLOTS
     ***************************************************************************/

};


#endif // _BT_PROJECT_STORAGE_XML_H