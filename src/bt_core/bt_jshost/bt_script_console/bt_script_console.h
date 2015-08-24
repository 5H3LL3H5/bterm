/*******************************************************************************
 *   Description:   See class declaration below
 *
 ******************************************************************************/

#ifndef _BT_SCRIPT_CONSOLE_H
#define _BT_SCRIPT_CONSOLE_H

/*******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include <QObject>

#include "bt_core.h"


/*******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

namespace BTCore {
    class ScriptConsole;
}


/**
 * Class that provides `console` methods for scripts:
 *
 * - `console.log()`
 */
class BTCore::ScriptConsole : public QObject
{
Q_OBJECT
    /****************************************************************************
     * TYPES
     ***************************************************************************/



    /****************************************************************************
     * CONSTRUCTOR, DESTRUCTOR
     ***************************************************************************/
public:
    explicit ScriptConsole();


    /****************************************************************************
     * PRIVATE DATA
     ***************************************************************************/
private:

    /****************************************************************************
     * STATIC METHODS
     ***************************************************************************/

    /****************************************************************************
     * METHODS
     ***************************************************************************/
public:

    /**
     * To be used from scripts: provides `console.log()` function
     */
    Q_INVOKABLE void log(QString text);




    /****************************************************************************
     * SIGNALS, SLOTS
     ***************************************************************************/
signals:

    /**
     * Emitted when user sends message to console
     */
    void message(
            const QString &text,
            BTCore::MsgLevel level = BTCore::MsgLevel::INFO
            );


};


#endif // _BT_SCRIPT_CONSOLE_H