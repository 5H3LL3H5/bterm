/*******************************************************************************
 *   Description:   See class declaration below
 *
 ******************************************************************************/

#ifndef _APPL_SETT_DIALOG_H
#define _APPL_SETT_DIALOG_H

/*******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include <QObject>
#include <memory>

#include "ui_appl_sett_dialog.h"



namespace Ui {
    class ApplSettDialog;
}

namespace BTCore {
    class Project;
}

/*******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/

/**
 * TODO
 */
class ApplSettDialog : public QDialog
{
   /****************************************************************************
    * TYPES
    ***************************************************************************/

   /****************************************************************************
    * CONSTRUCTOR, DESTRUCTOR
    ***************************************************************************/
public:

    explicit ApplSettDialog(
            QWidget *p_parent = nullptr,
            Qt::WindowFlags flags = 0
            );



   /****************************************************************************
    * PRIVATE DATA
    ***************************************************************************/
private:

    Ui::ApplSettDialog *ui;


   /****************************************************************************
    * STATIC METHODS
    ***************************************************************************/

   /****************************************************************************
    * METHODS
    ***************************************************************************/

   /****************************************************************************
    * SIGNALS, SLOTS
    ***************************************************************************/

};


#endif // _APPL_SETT_DIALOG_H
