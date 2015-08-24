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
 * Class that provides `console.*` methods for scripts
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

    Q_INVOKABLE void log(QString text);




    /****************************************************************************
     * SIGNALS, SLOTS
     ***************************************************************************/
signals:

    void message(const QString &text, BTCore::MsgLevel = BTCore::MsgLevel::INFO);


};


#endif // _BT_SCRIPT_CONSOLE_H
