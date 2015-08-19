/******************************************************************************
 *   Description:   TODO
 *
 ******************************************************************************/

/******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include <QDebug>

#include "htevent_visitor_handle.h"

#include "htevent_data_raw.h"
#include "htevent_data_msg.h"


using namespace HTCore;

/*******************************************************************************
 * CONSTRUCTOR, DESTRUCTOR
 ******************************************************************************/

EventVisitor_Handle::EventVisitor_Handle(Appl &appl) : 
    appl(appl)
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

void EventVisitor_Handle::visit(EventDataRaw &htevent_data_raw)
{
    const std::vector<uint8_t> data = htevent_data_raw.getData();
    emit(newDataRaw(data));
#if 0
    qDebug("handle data raw: ");
    for (auto byte : data){
        qDebug("0x%2.x", byte);
    }
#endif
}

void EventVisitor_Handle::visit(EventDataMsg &htevent_data_msg)
{
    //qDebug(("handle data msg: " + htevent_data_msg.getMsg().toString()).c_str());
    emit(
            newDataMsg(
                htevent_data_msg.getMsg()
                )
        );
}

void EventVisitor_Handle::visit(EventSys &htevent_sys)
{
    std::ignore = htevent_sys;
}



/*******************************************************************************
 * SLOTS
 ******************************************************************************/

/* private      */

/* protected    */

/* public       */


