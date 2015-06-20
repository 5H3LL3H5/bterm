/*****************************************************************************************
 *   Description:   TODO
 *
 *****************************************************************************************/

#ifndef _HTCODEC_H
#define _HTCODEC_H

/******************************************************************************************
 * INCLUDED FILES
 *****************************************************************************************/

#include <cstdint>

#include <QObject>
#include "htdatamsg.h"

#include <vector>

using namespace std;

/******************************************************************************************
 * CLASS DECLARATION
 *****************************************************************************************/

class HTCodec : public QObject
{
   Q_OBJECT

   /******************************************************************************************
    * TYPES
    *****************************************************************************************/

   /******************************************************************************************
    * CONSTRUCTOR, DESTRUCTOR
    *****************************************************************************************/
public:

   /******************************************************************************************
    * PRIVATE DATA
    *****************************************************************************************/
private:

   /******************************************************************************************
    * STATIC METHODS
    *****************************************************************************************/

   /******************************************************************************************
    * METHODS
    *****************************************************************************************/
public:
   virtual void                  addRawRxData   (const vector<uint8_t> &data) = 0;
   virtual void                  clearRawRxData () = 0;
   virtual HTDataMsg             encodeMessage  (const vector<uint8_t> &data) const = 0;


   /******************************************************************************************
    * SIGNALS, SLOTS
    *****************************************************************************************/
signals:
   void messageDecoded(const HTDataMsg &msg);
   //void curMessageChanged(const DTMsg &msg);
   //void invalidDataDetected(const QByteArray &data);

};


#endif // _HTCODEC_H
