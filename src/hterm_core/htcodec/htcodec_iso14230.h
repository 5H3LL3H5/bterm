/*******************************************************************************
 *   Description:   TODO
 *
 ******************************************************************************/

#ifndef _HTCODEC_ISO14230_H
#define _HTCODEC_ISO14230_H

/*******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include <QObject>

#include "htdatamsg.h"
#include "htcodec.h"


/*******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

class HTCodec_ISO14230 : public HTCodec
{
   Q_OBJECT

   /****************************************************************************
    * TYPES
    ***************************************************************************/

   /****************************************************************************
    * CONSTRUCTOR, DESTRUCTOR
    ***************************************************************************/
public:
   explicit HTCodec_ISO14230();

   /****************************************************************************
    * PRIVATE DATA
    ***************************************************************************/

private:
   HTDataMsg cur_rx_msg;
   int ragel_cs;
   int rx_user_data_len;
   int rx_user_data_got_len;
   unsigned char rx_checksum;


   /****************************************************************************
    * STATIC METHODS
    ***************************************************************************/

   /****************************************************************************
    * METHODS
    ***************************************************************************/
public:
   void                  addRawRxData   (const vector<unsigned char> &data);
   void                  clearRawRxData ();
   vector<unsigned char> encodeMessage  (const vector<unsigned char> &data);

private:

   /****************************************************************************
    * SIGNALS, SLOTS
    ***************************************************************************/
signals:
   void messageDecoded(const HTDataMsg &msg);

};


#endif // _HTCODEC_ISO14230_H
