/*******************************************************************************
 *   Description:   TODO
 *
 ******************************************************************************/

#ifndef _BT_EVENT_VISITOR_APPL_HANDLE_H
#define _BT_EVENT_VISITOR_APPL_HANDLE_H

/*******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include <QObject>

#include "bt_event_visitor.h"


class Appl;

namespace BTCore {
    class EventDataRaw;
    class EventDataMsg;
}

/*******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/


/**
 * Concrete visitor for `#BTCore::Event`, implements `applHandle()` method:
 * for `#BTCore::EventDataRaw` and `#BTCore::EventDataMsg` it adds the event
 * to the accumulator (see `#BTCore::EventsAcc`)
 *
 * (if you're unfamiliar with visitor pattern, read about it somewhere; for
 * example, on Wikipedia: https://en.wikipedia.org/wiki/Visitor_pattern )
 */
class EventVisitor_ApplHandle : public BTCore::EventVisitor
{
Q_OBJECT
    /****************************************************************************
     * TYPES
     ***************************************************************************/

    /****************************************************************************
     * CONSTRUCTOR, DESTRUCTOR
     ***************************************************************************/
public:
    EventVisitor_ApplHandle(Appl &appl);

    /****************************************************************************
     * PRIVATE DATA
     ***************************************************************************/
private:

    Appl &appl;

    /****************************************************************************
     * STATIC METHODS
     ***************************************************************************/

    /****************************************************************************
     * METHODS
     ***************************************************************************/
public:

    virtual void visit(BTCore::EventDataRaw &bt_event_data_raw) override;
    virtual void visit(BTCore::EventDataMsg &bt_event_data_msg) override;
    virtual void visit(BTCore::EventSys &bt_event_data_msg) override;


    /****************************************************************************
     * SIGNALS, SLOTS
     ***************************************************************************/

};


#endif // _BT_EVENT_VISITOR_APPL_HANDLE_H
