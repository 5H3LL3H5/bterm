/*******************************************************************************
 *   Description:   See class declaration below
 *
 ******************************************************************************/

#ifndef _BT_CODEC_EMPTY_VIEW_H
#define _BT_CODEC_EMPTY_VIEW_H

/*******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include <QObject>
#include <memory>

#include "bt_codec_view.h"




/*******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

namespace BTGui {
    class CodecEmptyView;
}

/**
 * A subclass of `#BTGui::CodecView` that represents GUI object for
 * any subclass of `#BTCore::Codec` that doesn't contain any
 * settings.
 */
class BTGui::CodecEmptyView : public BTGui::CodecView
{
    /****************************************************************************
     * TYPES
     ***************************************************************************/

    /****************************************************************************
     * CONSTRUCTOR, DESTRUCTOR
     ***************************************************************************/
public:

    /**
     * @param codec_name
     *      Human-readable name of the codec for which this GUI
     *      view is created. It doesn't affect anything but the
     *      message to the user.
     */
    explicit CodecEmptyView(QString codec_name);


    /****************************************************************************
     * PRIVATE DATA
     ***************************************************************************/
private:

    QString codec_name;


    /****************************************************************************
     * STATIC METHODS
     ***************************************************************************/

    /****************************************************************************
     * METHODS
     ***************************************************************************/
public:

    /**
     * NOTE: each time this function is called, the same widget is returned,
     * and the caller is responsible to delete it eventually.
     *
     * Widget will be actually created at the first call, so if this method
     * never called, then widget is not created at all.
     */
    virtual QWidget *getCodecSettWidget() override;


    /****************************************************************************
     * SIGNALS, SLOTS
     ***************************************************************************/

};


#endif // _BT_CODEC_EMPTY_VIEW_H
