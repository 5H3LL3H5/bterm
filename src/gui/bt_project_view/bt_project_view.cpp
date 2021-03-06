/******************************************************************************
 *   Description:   See class declaration in header file
 *
 ******************************************************************************/

/******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include <QDockWidget>
#include <QComboBox>
#include <QPushButton>

#include "bt_project_view.h"
#include "bt_project.h"
#include "bt_codec_factory.h"
#include "mainwindow.h"


using namespace BTGui;
using namespace BTCore;

/*******************************************************************************
 * CONSTRUCTOR, DESTRUCTOR
 ******************************************************************************/

ProjectView::ProjectView(
        std::shared_ptr<Project> p_project,
        MainWindow &mainwindow
        ) :
    p_project_view_ui(new Ui::BTProjectView),
    wp_project(std::weak_ptr<Project>(p_project)),
    p_project_sett_widg(),
    mainwindow(mainwindow)
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

QWidget *ProjectView::createProjectSettWidget()
{
    QWidget *p_widg = new QWidget();

    if (auto p_project = wp_project.lock()){
        p_project_view_ui->setupUi(p_widg);

        //-- populate codec drop-down select with available codecs
        {
            CodecFactory codec_factory;
            size_t codecs_cnt = codec_factory.getCodecsCnt();

            for (size_t i = 0; i < codecs_cnt; i++){
                p_project_view_ui->codec_select->addItem(
                        codec_factory.getCodecTitle(static_cast<CodecIdx>(i)),
                        QVariant(i)
                        );
            }
        }

        //-- set current project title
        p_project_view_ui->proj_title_edit->setText(
                p_project->getTitle()
                );

        //-- set current codec
        p_project_view_ui->codec_select->setCurrentIndex(
                static_cast<int>(p_project->getCodec()->getCodecIdx())
                );


        //-- subscribe on user changes:
        //-- project title
        connect(
                p_project_view_ui->proj_title_edit, &QLineEdit::textChanged,
                this, &ProjectView::onTitleChangedByUser
               );

        //-- codec
        connect(
                p_project_view_ui->codec_select,
                static_cast<void (QComboBox::*)(int)>(
                    &QComboBox::currentIndexChanged
                    ),
                this, &ProjectView::onCodecSelectionChangedByUser
               );



        //-- codec "Settings" button
        connect(p_project_view_ui->codec_sett_btn, &QPushButton::clicked,
                this, &ProjectView::toggleCodecSettWindow
               );


        //-- listen for widget destroy event
        connect(
                p_widg, &QObject::destroyed,
                this, &ProjectView::onWidgetDestroyed
               );
    }

    return p_widg;
}


/* protected    */

/* public       */

QWidget *ProjectView::getProjectSettWidget()
{
    if (p_project_sett_widg == nullptr){
        p_project_sett_widg = createProjectSettWidget();
    }

    return p_project_sett_widg;
}


/*******************************************************************************
 * SLOTS
 ******************************************************************************/

/* private      */

void ProjectView::onTitleChangedByUser(const QString &text)
{
    if (auto p_project = wp_project.lock()){
        p_project->setTitle(text);
    }
}

void ProjectView::onCodecSelectionChangedByUser(int index)
{
    if (auto p_project = wp_project.lock()){
        int codec_idx_int =
            p_project_view_ui->codec_select->itemData(index).toInt();
        p_project->setCurrentCodecIdx(static_cast<CodecIdx>(codec_idx_int));
    }
}

void ProjectView::onWidgetDestroyed()
{
    p_project_sett_widg = nullptr;
}

void ProjectView::toggleCodecSettWindow()
{
    mainwindow.toggleCodecSettWindow();
}

/* protected    */

/* public       */


