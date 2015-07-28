/*******************************************************************************
 *   Description:   See class declaration below
 *
 ******************************************************************************/

#ifndef _HANDLER_VIEW_H
#define _HANDLER_VIEW_H

/*******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include <QObject>
#include <memory>



namespace HTCore {
   class ReqHandler;
}

/*******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

/**
 * TODO
 */
class HandlerView : public QObject
{
Q_OBJECT
   /****************************************************************************
    * TYPES
    ***************************************************************************/

   /****************************************************************************
    * CONSTRUCTOR, DESTRUCTOR
    ***************************************************************************/
public:
   HandlerView(
         std::shared_ptr<HTCore::ReqHandler> p_handler
         );


   /****************************************************************************
    * PRIVATE DATA
    ***************************************************************************/
private:

   std::shared_ptr<HTCore::ReqHandler> p_handler;


   /****************************************************************************
    * STATIC METHODS
    ***************************************************************************/

   /****************************************************************************
    * METHODS
    ***************************************************************************/
public:

   QWidget *createWidget() const;


   /****************************************************************************
    * SIGNALS, SLOTS
    ***************************************************************************/

};


#endif // _HANDLER_VIEW_H