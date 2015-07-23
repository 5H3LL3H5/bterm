/******************************************************************************
 *   Description:   TODO
 *
 ******************************************************************************/

/******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include <QDebug>

#include <iostream>

#include "htcore.h"
#include "htevent_data_raw.h"
#include "htevent_data_msg.h"


/*******************************************************************************
 * CONSTRUCTOR, DESTRUCTOR
 ******************************************************************************/

HTCore::HTCore(
      std::shared_ptr<HTCodec> p_codec,
      std::shared_ptr<HTIODev> p_io_dev
      ) :
   p_codec(p_codec),
   p_io_dev(p_io_dev)
{

   connect(
         p_io_dev.get(), &HTIODev::readyRead,
         this, &HTCore::onDataSrcReadyRead
         );

   connect(
         p_codec.get(), &HTCodec::messageDecoded,
         this, &HTCore::onMessageDecoded
         );

}

HTCore::~HTCore()
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

/*******************************************************************************
 * SLOTS
 ******************************************************************************/

/* private      */

void HTCore::onDataSrcReadyRead(int bytes_available)
{
   std::ignore = bytes_available;

   //-- get received data
   std::vector<uint8_t> data = p_io_dev->read();

   auto p_event = std::make_shared<HTEventDataRaw>(data);
   emit (eventDataRaw(p_event));

#if 0
   for (auto byte : data){
      qDebug(byte + " ");
   }
#endif

   //-- feed received data as a raw data to codec
   p_codec->addRawRxData( data );
}

void HTCore::onMessageDecoded(const HTDataMsg &msg)
{
   //qDebug(msg.toString().c_str());

   auto p_event = std::make_shared<HTEventDataMsg>(msg);
   emit (eventDataMsg(p_event));

   //TODO: handle response rules, generate response if necessary
}



/* protected    */

/* public       */


