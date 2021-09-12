#include "SeerStackFramesBrowserWidget.h"
#include "SeerUtl.h"
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItemIterator>
#include <QtWidgets/QApplication>
#include <QtCore/QDebug>

SeerStackFramesBrowserWidget::SeerStackFramesBrowserWidget (QWidget* parent) : QWidget(parent) {

    // Construct the UI.
    setupUi(this);

    _previousStackFrameText = "";

    // Setup the widgets
    stackTreeWidget->setSortingEnabled(false);
    stackTreeWidget->resizeColumnToContents(0); // level
    stackTreeWidget->resizeColumnToContents(1); // addr
    stackTreeWidget->resizeColumnToContents(2); // func
    stackTreeWidget->resizeColumnToContents(3); // file
    stackTreeWidget->resizeColumnToContents(4); // fullname
    stackTreeWidget->resizeColumnToContents(5); // line
    stackTreeWidget->resizeColumnToContents(6); // arch

    /*
    stackTreeWidget->setColumnHidden(1, true); // ??? Hide or have a config to hide/show columns.
    stackTreeWidget->setColumnHidden(6, true);
    */

    stackTreeWidget->clear();

    // Connect things.
    QObject::connect(stackTreeWidget,            &QTreeWidget::itemDoubleClicked,    this,  &SeerStackFramesBrowserWidget::handleItemDoubleClicked);
}

SeerStackFramesBrowserWidget::~SeerStackFramesBrowserWidget () {
}

void SeerStackFramesBrowserWidget::handleText (const QString& text) {

    // Don't do any work if the widget is hidden.
    if (isHidden()) {
        return;
    }

    QApplication::setOverrideCursor(Qt::BusyCursor);

    if (text.startsWith("^done,stack=[") && text.endsWith("]")) {

        // ^done,stack=[
        //     frame={level=\"0\",addr=\"0x0000000000400d72\",func=\"function1\",file=\"function1.cpp\",fullname=\"/home/erniep/Development/Peak/src/Seer/helloworld/function1.cpp\",line=\"7\",arch=\"i386:x86-64\"},
        //     frame={level=\"1\",addr=\"0x0000000000400cc3\",func=\"main\",file=\"helloworld.cpp\",fullname=\"/home/erniep/Development/Peak/src/Seer/helloworld/helloworld.cpp\",line=\"14\",arch=\"i386:x86-64\"}
        // ]

        //qDebug() << __PRETTY_FUNCTION__ << ":" << text;

        // If we are simply moving up and down the stack (via frame-select) then the text won't change.
        // If it is different, reconstruct the tree. Select the first frame.
        if (text != _previousStackFrameText) {

            _previousStackFrameText = text;

            stackTreeWidget->clear();

            QString stack_text = Seer::parseFirst(text, "stack=", '[', ']', false);

            if (stack_text != "") {

                // Parse through the frame list and set the current lines that are in the frame list.
                QStringList frame_list = Seer::parse(text, "frame=", '{', '}', false);

                for ( const auto& frame_text : frame_list  ) {

                    QString level_text    = Seer::parseFirst(frame_text, "level=",    '"', '"', false);
                    QString addr_text     = Seer::parseFirst(frame_text, "addr=",     '"', '"', false);
                    QString func_text     = Seer::parseFirst(frame_text, "func=",     '"', '"', false);
                    QString file_text     = Seer::parseFirst(frame_text, "file=",     '"', '"', false);
                    QString fullname_text = Seer::parseFirst(frame_text, "fullname=", '"', '"', false);
                    QString line_text     = Seer::parseFirst(frame_text, "line=",     '"', '"', false);
                    QString arch_text     = Seer::parseFirst(frame_text, "arch=",     '"', '"', false);

                    //qDebug() << __PRETTY_FUNCTION__ << ":" << file_text << fullname_text;

                    // Add the frame to the tree.
                    QTreeWidgetItem* item = new QTreeWidgetItem;
                    item->setText(0, level_text);
                    item->setText(1, addr_text);
                    item->setText(2, func_text);
                    item->setText(3, file_text);
                    item->setText(4, fullname_text);
                    item->setText(5, line_text);
                    item->setText(6, arch_text);

                    stackTreeWidget->addTopLevelItem(item);
                }
            }

            // Select the first frame level.
            // ??? Perhaps show the current frame level somehow.
            stackTreeWidget->clearSelection();

            QList<QTreeWidgetItem*> matches = stackTreeWidget->findItems("0", 0);
            if (matches.size() > 0) {
                stackTreeWidget->setCurrentItem(matches.first());
            }
        }

    }else if (text.startsWith("^error,msg=\"No registers.\"")) {
        stackTreeWidget->clear();

    }else{
        // Ignore others.
    }

    stackTreeWidget->resizeColumnToContents(0);
    stackTreeWidget->resizeColumnToContents(1);
    stackTreeWidget->resizeColumnToContents(2);
    stackTreeWidget->resizeColumnToContents(3);
    stackTreeWidget->resizeColumnToContents(4);
    stackTreeWidget->resizeColumnToContents(5);
    stackTreeWidget->resizeColumnToContents(6);

    QApplication::restoreOverrideCursor();
}

void SeerStackFramesBrowserWidget::handleItemDoubleClicked (QTreeWidgetItem* item, int column) {

    Q_UNUSED(column);

    int lineno = item->text(5).toInt();

    //qDebug() << __PRETTY_FUNCTION__ << ":" << "Emit selectedFile and selectedFrame";

    emit selectedFile(item->text(3), item->text(4), lineno);
    emit selectedFrame(item->text(0).toInt());
}

void SeerStackFramesBrowserWidget::handleStoppingPointReached () {

    // Don't do any work if the widget is hidden.
    if (isHidden()) {
        return;
    }

    refresh();
}

void SeerStackFramesBrowserWidget::refresh () {
    emit refreshStackFrames();
}

void SeerStackFramesBrowserWidget::showEvent (QShowEvent* event) {

    QWidget::showEvent(event);

    refresh();
}
