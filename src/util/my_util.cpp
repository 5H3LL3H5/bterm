/******************************************************************************
 *   Description:   TODO
 *
 ******************************************************************************/

/******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include "my_util.h"




/*******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/

QString MyUtil::byteArrayToHex(const std::vector<uint8_t> &data)
{
   QString text;

   for (uint8_t byte : data){
      QString tmp;
      tmp.sprintf("%.2x ", byte);
      text += tmp;
   }

   return text;
}

