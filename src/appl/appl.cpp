/******************************************************************************
 *   Description:   TODO
 *
 ******************************************************************************/

/******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include <memory>



#include "appl.h"

#include "htcodec.h"
#include "htcodec_iso14230.h"

#include "htiodev.h"
#include "htiodev_dbg.h"

#include "htdatamsg.h"

//#include "htevent_data_raw.h"
//#include "htevent_data_msg.h"


/*******************************************************************************
 * CONSTRUCTOR, DESTRUCTOR
 ******************************************************************************/

Appl::Appl() :
   p_codec(nullptr),
   p_data_src(nullptr),
   p_htcore(nullptr),
   htevent_visitor_handle(*this),
   main_window()
{

   p_data_src = std::make_shared<HTIODevDbg>();
   p_codec = std::make_shared<HTCodec_ISO14230>(0x01, 0x02);

   p_htcore = std::unique_ptr<HTCore>{
      new HTCore{p_codec, p_data_src}
   };

   p_events_data_raw = std::unique_ptr<HTEventsAcc<HTEventDataRaw>>{
      new HTEventsAcc<HTEventDataRaw>(1000/*TODO: settings*/)
   };

   p_events_data_msg = std::unique_ptr<HTEventsAcc<HTEventDataMsg>>{
      new HTEventsAcc<HTEventDataMsg>(1000/*TODO: settings*/)
   };



   connect(
         p_htcore.get(), SIGNAL(eventDataRaw(const std::shared_ptr<HTEventDataRaw> &)),
         this, SLOT(onNewDataRaw(const std::shared_ptr<HTEventDataRaw> &))
         );

   connect(
         p_htcore.get(), SIGNAL(eventDataMsg(const std::shared_ptr<HTEventDataMsg> &)),
         this, SLOT(onNewDataMsg(const std::shared_ptr<HTEventDataMsg> &))
         );



   //-- connect separate visitor's events to mainwindow's slots
   connect(
         p_htcore.get(), SIGNAL(eventDataRaw(const std::shared_ptr<HTEventDataRaw> &)),
         &main_window, SLOT(onNewDataRaw(const std::shared_ptr<HTEventDataRaw> &))
         );

   connect(
         p_htcore.get(), SIGNAL(eventDataMsg(const std::shared_ptr<HTEventDataMsg> &)),
         &main_window, SLOT(onNewDataMsg(const std::shared_ptr<HTEventDataMsg> &))
         );

   this->main_window.show();

}

Appl::~Appl()
{
   disconnect(
         p_htcore.get(), SIGNAL(eventDataMsg(const std::shared_ptr<HTEventDataMsg> &)),
         this, SLOT(onNewDataMsg(const std::shared_ptr<HTEventDataMsg> &))
         );

   disconnect(
         p_htcore.get(), SIGNAL(eventDataRaw(const std::shared_ptr<HTEventDataRaw> &)),
         this, SLOT(onNewDataRaw(const std::shared_ptr<HTEventDataRaw> &))
         );
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

/*******************************************************************************
 * SLOTS
 ******************************************************************************/

/* private      */

#if 0
void Appl::onHTEvent(const std::shared_ptr<HTEvent> &p_event)
{
   p_event->accept(htevent_visitor_handle);
}
#endif

void Appl::onNewDataRaw(const std::shared_ptr<HTEventDataRaw> &p_event)
{
   p_events_data_raw->addEvent(p_event);
}

void Appl::onNewDataMsg(const std::shared_ptr<HTEventDataMsg> &p_event)
{
   p_events_data_msg->addEvent(p_event);
}





#if 0
void Appl::onNewDataRaw(const std::vector<uint8_t> &data)
{
   //TODO
}

void Appl::onNewDataMsg(const HTDataMsg &msg)
{
   //TODO
}
#endif

/* protected    */

/* public       */

