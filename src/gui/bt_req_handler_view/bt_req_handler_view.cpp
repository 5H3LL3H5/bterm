/******************************************************************************
 *   Description:   See class declaration in header file
 *
 ******************************************************************************/

/******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include <QDebug>

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QDockWidget>
#include <QMessageBox>

#include "bt_req_handler_view.h"
#include "mainwindow.h"


#include "bt_project.h"
#include "bt_reqhandler.h"


using namespace std;
using namespace BTCore;
using namespace BTGui;

/*******************************************************************************
 * CONSTRUCTOR, DESTRUCTOR
 ******************************************************************************/

ReqHandlerView::ReqHandlerView(
        MainWindow &mainwindow,
        std::shared_ptr<Project> p_project,
        std::shared_ptr<ReqHandler> p_handler
        ) :
    mainwindow(mainwindow),
    p_handler(p_handler),
    p_dock(),
    p_list_item_widget(nullptr),
    wp_project(std::weak_ptr<Project>(p_project))
{


    //-- setup edit dockwidget
    {
        QWidget *p_edit_widg = createEditWidget();
        p_dock = make_shared<QDockWidget>(getEditDockWidgetTitle());
        //TODO: probably use some unique key instead of human-readable name
        p_dock->setObjectName("handler: " + p_handler->getTitle());
        p_dock->setWidget(p_edit_widg);

        connect(
                p_handler.get(), &ReqHandler::titleChanged,
                this, &ReqHandlerView::onReqHandlerTitleChanged
               );
    }
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

QWidget *ReqHandlerView::createEditWidget()
{
    QWidget *p_widg = new QWidget();

    QBoxLayout *p_vert_lay = new QBoxLayout(QBoxLayout::TopToBottom);

    {
        QLineEdit *p_title = new QLineEdit(p_handler->getTitle());
        p_vert_lay->addWidget(p_title);

        p_script_edit = new QPlainTextEdit(p_handler->getScript());
        p_vert_lay->addWidget(p_script_edit);

        connect(
                p_title, &QLineEdit::textChanged,
                this, &ReqHandlerView::onTitleChangedByUser
               );

        connect(
                p_script_edit, &QPlainTextEdit::textChanged,
                this, &ReqHandlerView::onScriptChangedByUser
               );

    }

    p_widg->setLayout(p_vert_lay);

    return p_widg;
}

QWidget *ReqHandlerView::createListItemWidget()
{
    QWidget *p_widg = new QWidget();

    QBoxLayout *p_vert_lay = new QBoxLayout(QBoxLayout::LeftToRight);

    {
        p_list_item_label_name = new QLabel(getListItemWidgetTitle());
        p_vert_lay->addWidget(p_list_item_label_name);
    }

    //-- edit button
    {
        QPushButton *p_edit_button = new QPushButton("Edit");
        p_edit_button->setMinimumWidth(5);
        p_vert_lay->addWidget(p_edit_button);

        connect(
                p_edit_button, &QPushButton::clicked,
                this, &ReqHandlerView::onEditButtonPressed
               );
    }

    //-- up button
    {
        QPushButton *p_up_button = new QPushButton("Up");
        p_up_button->setMinimumWidth(5);
        p_vert_lay->addWidget(p_up_button);

        connect(
                p_up_button, &QPushButton::clicked,
                this, &ReqHandlerView::onUpButtonPressed
               );
    }

    //-- down button
    {
        QPushButton *p_down_button = new QPushButton("Down");
        p_down_button->setMinimumWidth(5);
        p_vert_lay->addWidget(p_down_button);

        connect(
                p_down_button, &QPushButton::clicked,
                this, &ReqHandlerView::onDownButtonPressed
               );
    }

    //-- remove button
    {
        QPushButton *p_remove_button = new QPushButton("x");
        p_remove_button->setMinimumWidth(5);
        p_vert_lay->addWidget(p_remove_button);

        connect(
                p_remove_button, &QPushButton::clicked,
                this, &ReqHandlerView::onRemoveButtonPressed
               );
    }

    //-- widget destroy
    {
        connect(
                p_widg, &QObject::destroyed,
                this, &ReqHandlerView::onListItemWidgetDestroyed
               );
    }

    p_widg->setLayout(p_vert_lay);

    return p_widg;
}

#if 0
void ReqHandlerView::applyReqName()
{
    p_dock->setWindowTitle("Handler " + p_handler->getTitle());
}
#endif

QString ReqHandlerView::getEditDockWidgetTitle() const
{
    return "Handler #"
        + QString::number(p_handler->getHandlerIndex() + 1) + ": "
        + p_handler->getTitle();
}

QString ReqHandlerView::getListItemWidgetTitle() const
{
    return "#"
        + QString::number(p_handler->getHandlerIndex() + 1) + ": "
        + p_handler->getTitle();
}


/* protected    */

/* public       */

QWidget *ReqHandlerView::getListItemWidget()
{
    if (p_list_item_widget == nullptr){
        p_list_item_widget = createListItemWidget();
    }

    return p_list_item_widget;
}

QDockWidget *ReqHandlerView::getEditDockWidget() const
{
    return p_dock.get();
}



/*******************************************************************************
 * SLOTS
 ******************************************************************************/

/* private      */

void ReqHandlerView::onEditButtonPressed()
{
    if (p_dock->isHidden()){
        p_dock->show();
    } else {
        p_dock->hide();
    }

#if 0
    //qDebug("pressed");
    QWidget *p_edit_widg = createEditWidget();
    mainwindow.addHandlerEditWidget(
            p_handler,
            p_edit_widg
            );
#endif
}

void ReqHandlerView::onRemoveButtonPressed()
{
    QMessageBox::StandardButton reply =
        QMessageBox::question(
                nullptr,
                "Remove handler?",
                "Remove request handler \"" + p_handler->getTitle() + "\"?",
                (QMessageBox::Yes | QMessageBox::No)
                );

    if (reply == QMessageBox::Yes){
        if (auto p_project = wp_project.lock()){
            p_project->removeHandler(p_handler->getHandlerIndex());
        }
    }
}

void ReqHandlerView::onUpButtonPressed()
{
    if (auto p_project = wp_project.lock()){
        p_project->moveHandlerUp(p_handler->getHandlerIndex());
    }
}

void ReqHandlerView::onDownButtonPressed()
{
    if (auto p_project = wp_project.lock()){
        p_project->moveHandlerDown(p_handler->getHandlerIndex());
    }
}

void ReqHandlerView::onTitleChangedByUser(const QString &text)
{
    p_handler->setTitle(text);
}

void ReqHandlerView::onScriptChangedByUser()
{
    p_handler->setScript(p_script_edit->toPlainText());
}

void ReqHandlerView::onReqHandlerTitleChanged(const QString &text)
{
    std::ignore = text;

    p_dock->setWindowTitle(getEditDockWidgetTitle());
    p_list_item_label_name->setText(getListItemWidgetTitle());
}

void ReqHandlerView::onListItemWidgetDestroyed()
{
    p_list_item_widget = nullptr;
}



/* protected    */

/* public       */

