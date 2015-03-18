#include "mainwindow.h"
#include "treewidget.h"
#include "ui_mainwindow.h"
#include "constance.h"

#include <QMenu>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    m_fileMenu = menuBar()->addMenu(tr("&File"));
//    m_fileMenu->addAction(tr("&Open..."), this, SLOT(open_file()),
//                        QKeySequence::Open);
//    m_fileMenu->addAction(tr("&Save"), this, SLOT(save_file()),
//                        QKeySequence::Save);
//    m_fileMenu->addAction(tr("E&xit"), this, SLOT(close()),
//                        QKeySequence::Quit);

    QStringList labels;
    labels << tr("Title") << tr("tooltips");
    m_tree = new TreeWidget(this);
    m_tree->header()->setResizeMode(QHeaderView::Stretch);
    m_tree->setHeaderLabels(labels);
//    m_tree->hideColumn(1);
    this->setCentralWidget(m_tree);
    this->setWindowTitle(tr("Simple XML"));

    m_xmlFilename =QDir::currentPath() + QDir::separator() + XmlFilename;
    m_tree->loadFile(m_xmlFilename);
}

MainWindow::~MainWindow()
{
    m_tree->saveFile(m_xmlFilename);
    delete ui;
}

