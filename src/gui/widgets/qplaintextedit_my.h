/*****************************************************************************************
 *   Description:
 *
 *   FIXME: this class is actually a dirty hack: we shouldn't inherit
 *   QPlainTextEdit. I hope I'll have some time in the future to fix that
 *   (at least, we may use composition instead)
 *
 *****************************************************************************************/

#ifndef _QPLAINTEXTEDIT_MY_H
#define _QPLAINTEXTEDIT_MY_H

/******************************************************************************************
 * INCLUDED FILES
 *****************************************************************************************/

#include <QPlainTextEdit>

/******************************************************************************************
 * CLASS DECLARATION
 *****************************************************************************************/

class QPlainTextEdit_My : public QPlainTextEdit
{
   /******************************************************************************************
    * TYPES
    *****************************************************************************************/

   /******************************************************************************************
    * CONSTRUCTOR, DESTRUCTOR
    *****************************************************************************************/
public:
   explicit QPlainTextEdit_My(QWidget *parent);
   explicit QPlainTextEdit_My(const QString &text, QWidget *parent);

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
   void appendPlainTextNoNL(const QString &text, bool check_nl = false);
   void appendHtmlNoNL     (const QString &html, bool check_br = false);
   void insertNL           ();

   /******************************************************************************************
    * SIGNALS, SLOTS
    *****************************************************************************************/

};


#endif // _QPLAINTEXTEDIT_MY_H
