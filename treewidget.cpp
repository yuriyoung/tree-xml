#include "treewidget.h"
#include "constance.h"

#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

#include <QDebug>

TreeWidget::TreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setAlternatingRowColors(true);

    m_reader = new XmlReader(this);
    m_writer = new XmlWriter(this);
    m_isSaved = false;
}

TreeWidget::~TreeWidget()
{
}

void TreeWidget::contextMenuEvent(QContextMenuEvent */*event*/)
{
    QMenu *popMenu = new QMenu(this);
    QAction *addCategoryAct = popMenu->addAction(tr("Add &Category"));
    QAction *removeCategoryAct = popMenu->addAction(tr("&Remove Category"));
    popMenu->addSeparator();
    QAction* addFileAct = popMenu->addAction(tr("Add &File"));
    QAction* removeFileAct = popMenu->addAction(tr("R&emove File"));

    int _x = mapFromGlobal(QCursor::pos()).x();
    int _y = mapFromGlobal(QCursor::pos()).y() - this->header()->height();// 如果有表头,需要减去表头高度

    QTreeWidgetItem *item = this->itemAt( QPoint(_x, _y) );

    if( !item )
    {
        this->clearSelection();
        this->setCurrentItem(0);
        this->selectionModel()->setCurrentIndex(QModelIndex(), QItemSelectionModel::Clear);
        removeCategoryAct->setEnabled(false);
        removeFileAct->setEnabled(false);
    }

    connect(addCategoryAct, SIGNAL( triggered() ), this, SLOT( addCategory() ));
    connect(removeCategoryAct, SIGNAL( triggered() ), this, SLOT( deleteCategory() ));
    connect(addFileAct, SIGNAL( triggered() ), this, SLOT( addFile() ));
    connect(removeFileAct, SIGNAL( triggered() ), this, SLOT( deleteFile() ));

    popMenu->exec(cursor().pos());
}

bool TreeWidget::loadFile(const QString &xmlFile)
{
    if (xmlFile.isEmpty())
        return false;

    QFile file(xmlFile);
    if ( !file.open(QFile::ReadOnly | QFile::Text ))
    {
        QMessageBox::warning(this, tr("Simple XML"),
                             tr("Cannot load file %1:\n%2.")
                             .arg(xmlFile)
                             .arg(file.errorString()));
        return false;
    }

    if (!m_reader->read(&file))
    {
        QMessageBox::warning( this, tr("Simple XML"),
                            tr("Parse error in file %1:\n\n%2")
                            .arg(xmlFile).arg( m_reader->errorString() ) );
        return false;
    }

    file.close();

    return true;
}

bool TreeWidget::saveFile(const QString &xmlFile)
{
    if (xmlFile.isEmpty())
        return false;

    QFile file(xmlFile);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Simple XML"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(xmlFile)
                             .arg( file.errorString() ));
        return false;
    }

    if ( !m_writer->write(&file) )
    {
        QMessageBox::warning(this, tr("Simple XML"),
                            tr("Save error in file %1:\n\n%2")
                            .arg(xmlFile).arg(file.errorString()));
        return false;
    }

    return true;
}

void TreeWidget::addCategory()
{
    QTreeWidgetItem *selItem = this->currentItem();
    QTreeWidgetItem *newItem = NULL;
    if( !selItem )
    {
        newItem = new QTreeWidgetItem(this);
        newItem->setFlags(newItem->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable );
    }
    else
    {
        QString tagName = selItem->data(0, Qt::UserRole).toString();
        if ( tagName == XML_TAG_CATEGORY )
        {
            newItem = new QTreeWidgetItem(selItem);
            newItem->setFlags(selItem->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable );
        }
        else
        {
            newItem = new QTreeWidgetItem(selItem->parent());
            newItem->setFlags(selItem->parent()->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable );
        }
    }

    newItem->setText( 0, tr("[New CateGory]") );
    newItem->setData( 0, Qt::UserRole, XML_TAG_CATEGORY );
}

void TreeWidget::deleteCategory()
{
    QTreeWidgetItem *selItem = this->currentItem();
    if( !selItem ) return;
    QString tagName = selItem->data(0, Qt::UserRole).toString();
    if ( tagName == XML_TAG_CATEGORY )
    {
        delete selItem;
    }
    else
    {
        delete selItem;
    }
}

void TreeWidget::addFile()
{
    QTreeWidgetItem *selItem = this->currentItem();
    QTreeWidgetItem *newItem = NULL;
    if( !selItem )
    {
        newItem = new QTreeWidgetItem(0);
        newItem->setFlags(newItem->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable );
        newItem->setFlags(newItem->flags() & ~Qt::ItemIsDropEnabled );
    }
    else
    {
        QString tagName = selItem->data(0, Qt::UserRole).toString();
        if ( tagName == XML_TAG_CATEGORY )
        {
            newItem = new QTreeWidgetItem(selItem);
            newItem->setFlags(selItem->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable );
        }
        else
        {
            newItem = new QTreeWidgetItem(selItem->parent());
            newItem->setFlags(selItem->parent()->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable );
        }
        newItem->setFlags(newItem->flags() & ~Qt::ItemIsDropEnabled );
    }

    newItem->setText( 0, tr("[New File]") );
    newItem->setData( 0, Qt::UserRole, XML_TAG_FILE );
}

void TreeWidget::deleteFile()
{
    QTreeWidgetItem *selItem = this->currentItem();
    if( !selItem ) return;
    QString tagName = selItem->data(0, Qt::UserRole).toString();
    if ( tagName == XML_TAG_FILE )
    {
        delete selItem;
    }
}


