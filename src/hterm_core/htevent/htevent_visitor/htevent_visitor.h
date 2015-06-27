/*******************************************************************************
 *   Description:   HTEvent Visitor
 *
 *                  See visitor pattern, for example, on Wikipedia:
 *                  https://en.wikipedia.org/wiki/Visitor_pattern
 *
 ******************************************************************************/

#ifndef _HTEVENT_VISITOR_H
#define _HTEVENT_VISITOR_H

/*******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include "htevent.h"




class HTEventDataRaw;
class HTEventDataMsg;

/*******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

class HTEventVisitor : public QObject
{
Q_OBJECT
   /****************************************************************************
    * TYPES
    ***************************************************************************/

   /****************************************************************************
    * CONSTRUCTOR, DESTRUCTOR
    ***************************************************************************/
public:

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

   virtual void accept(HTEventDataRaw &) = 0;
   virtual void accept(HTEventDataMsg &) = 0;




   /****************************************************************************
    * SIGNALS, SLOTS
    ***************************************************************************/

};


#endif // _HTEVENT_VISITOR_H
