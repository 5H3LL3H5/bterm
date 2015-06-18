/*******************************************************************************
 *   Description:   TODO
 *
 ******************************************************************************/

#ifndef _HTDATAMSG_H
#define _HTDATAMSG_H

/*******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include "htdatapart.h"

#include <vector>
#include <string>

using namespace std;


/*******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

class HTDataMsg
{
   /****************************************************************************
    * TYPES
    ***************************************************************************/

   /****************************************************************************
    * CONSTRUCTOR, DESTRUCTOR
    ***************************************************************************/
public:
   explicit HTDataMsg();


   /****************************************************************************
    * PRIVATE DATA
    ***************************************************************************/
private:

   vector<HTDataPart> data_parts;


   /****************************************************************************
    * STATIC METHODS
    ***************************************************************************/

   /****************************************************************************
    * METHODS
    ***************************************************************************/

public:
   void addData(HTDataPart::Type type, const vector<unsigned char> &data);
   void addData(HTDataPart::Type type, unsigned char byte);

   void clear();

   std::string toString() const;

   /****************************************************************************
    * SIGNALS, SLOTS
    ***************************************************************************/


   /*******************************************************************************
    * OPERATORS
    ******************************************************************************/

};


#endif // _HTDATAMSG_H
