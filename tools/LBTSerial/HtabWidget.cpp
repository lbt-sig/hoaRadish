#include "HtabWidget.h"
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QTabBar>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QMoveEvent>
#include <QStackedWidget>
#include <QLabel>
#include <QGuiApplication>
#include <QPushButton>

HTabWidget::HTabWidget(QWidget *parent) : QTabWidget(parent)
{
    setAcceptDrops(true);
    setMouseTracking(true);
    QTabBar* tabBar=this->tabBar();
    tabBar->installEventFilter(this);
    tabBar->setMouseTracking(true);
    tabBar->setAcceptDrops(true);
    tabBar->setMovable(true);
    tabBar->setContextMenuPolicy(Qt::CustomContextMenu);
    setStyleSheet("QTabBar::tab{height:28px;}");
    connect(tabBar,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(tabBarCustomContextMenuRequested(QPoint)));
//    connect(this,SIGNAL(currentChanged(int)),this,SLOT(onCurrentChanged(int)));
    m_tabBarPress=false;
    m_menu=nullptr;
}

void HTabWidget::setTabCanMove(int index, bool canMove)
{
    QWidget* widget=this->widget(index);
    QTabBar* tabBar=this->tabBar();
    if(widget)
    {
        if(canMove)
        {

            QWidget* widget1=tabBar->tabButton(index,QTabBar::RightSide);
            if(m_tabClosable)
            {
                QPushButton* btn=new QPushButton();
                btn->setStyleSheet("QPushButton{border-image: url(:/img/close_normal.png);}"
                                   "QPushButton:hover{border-image: url(:/img/close_hover.png);}");
                btn->setFixedSize(12,11);
                tabBar->setTabButton(index,QTabBar::RightSide,btn);
                connect(btn,SIGNAL(clicked()),widget,SLOT(deleteLater()));
            }
            else
            {
                tabBar->setTabButton(index,QTabBar::RightSide,nullptr);
            }

            if(widget1)
            {
                widget1->setParent(nullptr);
                widget1->deleteLater();
                widget1=nullptr;
            }
            if(index!=m_unDragWidgets.count()-1)
            {
                tabBar->moveTab(index,m_unDragWidgets.count()-1);

            }
            m_unDragWidgets.remove(widget);
        }
        else
        {
            QLabel* label=new QLabel();
            label->setPixmap(QPixmap("://lock.png").scaled(10,12));
            label->setScaledContents(true);
            tabBar->setTabButton(index,QTabBar::RightSide,label);
            if(index!=m_unDragWidgets.count())
            {
                tabBar->moveTab(index,m_unDragWidgets.count());
            }

            m_unDragWidgets.insert(widget);
        }
    }
}

void HTabWidget::setTabsClosable(bool closeable)
{
    m_tabClosable=closeable;
    QSet<QWidget*>::iterator iter;
    QTabBar* tabBar=this->tabBar();
    for(iter=m_unDragWidgets.begin();iter!=m_unDragWidgets.end();iter++)
    {
        int index=this->indexOf(*iter);
        QLabel* label=new QLabel();
        label->setPixmap(QPixmap("://locker.png").scaled(10,12));
        label->setScaledContents(true);
        tabBar->setTabButton(index,QTabBar::RightSide,label);
    }
    for(int i=m_unDragWidgets.count();i<count();i++)
    {
        setTabCloseBtn(i,this->widget(i));
    }
}

bool HTabWidget::tabsClosable() const
{
    return m_tabClosable;
}

int HTabWidget::addTab(QWidget *page, const QString &label)
{
    int index=QTabWidget::addTab(page,label);
    if(m_tabClosable)
    {
        setTabCloseBtn(index,page);
    }
    return index;
}

int HTabWidget::addTab(QWidget *page, const QIcon &icon, const QString &label)
{
    int index=QTabWidget::addTab(page,icon,label);
    if(m_tabClosable)
    {
        setTabCloseBtn(index,page);
    }
    return index;
}

int HTabWidget::insertTab(int index, QWidget *page, const QString &label)
{
    int newIndex=QTabWidget::insertTab(index,page,label);
    if(m_tabClosable)
    {
        setTabCloseBtn(newIndex,page);
    }
    return newIndex;
}

int HTabWidget::insertTab(int index, QWidget *page, const QIcon &icon, const QString &label)
{
    int newIndex=QTabWidget::insertTab(index,page,icon,label);
    if(m_tabClosable)
    {
        setTabCloseBtn(newIndex,page);
    }
    return newIndex;
}


void HTabWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
    event->acceptProposedAction();

}

void HTabWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void HTabWidget::dragMoveEvent(QDragMoveEvent *event)
{
    event->setDropAction(Qt::MoveAction);
}

bool HTabWidget::eventFilter(QObject *watched, QEvent *event)
{
    QTabBar* tabBar=this->tabBar();
    if(watched==tabBar)
    {
        if(event->type()==QEvent::MouseButtonPress)
        {
            m_tabBarPress=true;
            QMouseEvent* mouseEvent=static_cast<QMouseEvent*>(event);
            if(mouseEvent)
            {
                int index=tabBar->tabAt(mouseEvent->pos());
                if(index>-1)
                {
                    if(index<m_unDragWidgets.count())
                    {
                        m_isDrag=false;

                    }
                    else
                    {
                        QRect rect=tabBar->tabRect(index);
                        m_xOffset=rect.left()-mouseEvent->pos().x();
                        m_mousePressPoint=mouseEvent->pos();
                        m_dragIndex=index;
                        m_isDrag=true;
                    }
                }



            }
        }
        else if(event->type()==QEvent::MouseMove)
        {
            QMouseEvent* mouseEvent=static_cast<QMouseEvent*>(event);
            if(mouseEvent)
            {

                if(!m_tabBarPress)
                {
                    int index=tabBar->tabAt(mouseEvent->pos());
                    for(int i=m_unDragWidgets.count();i<count();i++)
                    {
                        QWidget* widget=tabBar->tabButton(i,QTabBar::RightSide);
                        if(widget)
                        {
                            if(i!=currentIndex())
                            {
                                widget->setVisible(i==index);
                            }

                        }
                    }
                }

                if(m_isDrag)
                {
                    QPoint point=mouseEvent->pos();
                    if(m_dragIndex==m_unDragWidgets.count()&&mouseEvent->x()<m_mousePressPoint.x())
                    {
                        return true;
                    }
                    if(qAbs(m_mousePressPoint.y()-point.y())>tabBar->height())
                    {
                        startDrag(m_dragIndex);
                    }
                }
                else
                {

                    return true;
                }
                int index=tabBar->tabAt(QPoint(mouseEvent->pos().x()+m_xOffset,mouseEvent->pos().y()));
                if(m_unDragWidgets.count()>index)
                {
                    return true;
                }
            }

        }
        else if(event->type()==QEvent::MouseButtonRelease)
        {
            m_tabBarPress=false;
            QMouseEvent* mouseEvent=static_cast<QMouseEvent*>(event);
            if(mouseEvent)
            {
                int index=tabBar->tabAt(mouseEvent->pos());
                if(index!=-1)
                {
                    QWidget* widget1=this->widget(index);
                    QSet<QWidget*>::iterator iter=m_unDragWidgets.find(widget1);
                    if(iter!=m_unDragWidgets.end())
                    {
                        return true;
                    }
                }
            }
        }
        else if(event->type()==QEvent::HoverLeave)
        {
            QHoverEvent* hEvent=static_cast<QHoverEvent*>(event);
            if(hEvent)
            {
                int index=tabBar->tabAt(hEvent->oldPos());
                if(index>=m_unDragWidgets.count()&&index!=currentIndex())
                {
                    QWidget* widget=tabBar->tabButton(index,QTabBar::RightSide);
                    if(widget){
                        widget->hide();
                    }
                }
            }
        }



    }
    QWidget* dragOutWidget=qobject_cast<QWidget*>(watched);

    if(dragOutWidget&&dragOutWidget!=tabBar&&dragOutWidget->parentWidget()==nullptr)
    {
        if(event->type()==QEvent::Move)
        {
            QPoint pos=dragOutWidget->pos();
            QRect tabRect(tabBar->mapToGlobal(QPoint(0,0)),QSize(this->width(),tabBar->height()));
            if (tabRect.contains(pos))
            {
                int index = tabBar->tabAt(tabBar->mapFromGlobal(pos));
                int newIndex;
                if(index>=0)
                {
                    QWidget* widget=this->widget(index);
                    if (m_unDragWidgets.find(widget) != m_unDragWidgets.end())
                    {
                        return true;
                    }
//                    // 获取当前窗口大小
//                    int m_width = 0;
//                    int m_height = 0;
//                    if(!m_unDragWidgets.empty()){
//                        m_width = (*m_unDragWidgets.begin())->width();
//                        m_height = (*m_unDragWidgets.begin())->height();
//                    }
//                    dragOutWidget->resize(m_width, m_height);
                    newIndex = this->insertTab(index, dragOutWidget, dragOutWidget->windowIcon(), dragOutWidget->windowTitle());

                }
                else
                {

                    newIndex = this->addTab(dragOutWidget, dragOutWidget->windowIcon(), dragOutWidget->windowTitle());
                }

                dragOutWidget->setAttribute(Qt::WA_DeleteOnClose, false);
                setCurrentIndex(newIndex);

            }

        }
    }

    return QTabWidget::eventFilter(watched, event);
}

void HTabWidget::startDrag(int index)
{
    if (index >= 0)
    {
        QWidget* widget = this->widget(index);
        if (widget)
        {
            m_dragIndex = index;
            QPixmap pix = widget->grab();
            if (pix.isNull())
            {
                return;
            }

            QMimeData *mimeData = new QMimeData;
            QDrag *drag = new QDrag(this);
            drag->setMimeData(mimeData);
            drag->setPixmap(pix);
            m_offset = (QCursor::pos().x() - this->pos().x()) / 2;
            drag->setHotSpot(QPoint(m_offset, 0));
            drag->exec();
            QPoint point = QCursor::pos();
            QTabBar* tabBar = this->tabBar();
            QRect tabRect(tabBar->mapToGlobal(QPoint(0, 0)), QSize(this->width(), tabBar->height()));
            //鼠标拖动释放位置在tabBar内不移除标签
            if (tabRect.contains(point))
            {
                return;
            }
            widget->setWindowFlags(Qt::WindowCloseButtonHint|Qt::WindowMinimizeButtonHint);
            widget->setFixedSize(widget->width(), widget->height());

            widget->setWindowTitle(this->tabText(m_dragIndex));
            widget->setWindowIcon(this->tabIcon(m_dragIndex));
            widget->setAttribute(Qt::WA_DeleteOnClose);
            removeTab(m_dragIndex);
            widget->setParent(nullptr);
            widget->move(point.x() - this->m_offset, point.y() - (widget->height() - widget->geometry().height()));
            widget->show();
            widget->setMouseTracking(true);
            widget->installEventFilter(this);
        }
    }
}



void HTabWidget::createContextMenu()
{
    m_menu=new QMenu(this);

    m_closeOtherTabAct=new QAction(tr("关闭其他标签"));
    m_closeLeftTabAct=new QAction(tr("关闭左侧标签"));
    m_closeRightTabAct=new QAction(tr("关闭右侧标签"));
    m_fixedTabAct=new QAction(tr("固定标签"));
    m_unFixedTabAct=new QAction(tr("取消固定标签"));
    m_menu->addAction(m_closeOtherTabAct);
    m_menu->addAction(m_closeLeftTabAct);
    m_menu->addAction(m_closeRightTabAct);
    m_menu->addAction(m_fixedTabAct);
    m_menu->addAction(m_unFixedTabAct);
    connect(m_closeOtherTabAct,SIGNAL(triggered()),this,SLOT(onCloseOtherTabAct()));
    connect(m_closeLeftTabAct,SIGNAL(triggered()),this,SLOT(onCloseLeftTabAct()));
    connect(m_closeRightTabAct,SIGNAL(triggered()),this,SLOT(onCloseRightTabAct()));
    connect(m_fixedTabAct,SIGNAL(triggered()),this,SLOT(onFixedTabAct()));
    connect(m_unFixedTabAct,SIGNAL(triggered()),this,SLOT(onUnFixedTabAct()));
}

void HTabWidget::setTabCloseBtn(int index, QWidget *widget)
{
    QPushButton* btn=new QPushButton();
    btn->setStyleSheet("QPushButton{border-image: url(:/img/close_normal.png);}"
                       "QPushButton:hover{border-image: url(:/img/close_hover.png);}");
    btn->setFixedSize(12,11);
    tabBar()->setTabButton(index,QTabBar::RightSide,btn);
    btn->setVisible(index==currentIndex());
    connect(btn,SIGNAL(clicked()),widget,SLOT(deleteLater()));
}

void HTabWidget::tabBarCustomContextMenuRequested(const QPoint &pos)
{
    QTabBar* tabBar=this->tabBar();
    QPoint globalPos=tabBar->mapToGlobal(pos);
    if(!m_menu)
    {
        createContextMenu();
    }
    int index=tabBar->tabAt(pos);
    setCurrentIndex(index);
    int ucount=m_unDragWidgets.count();
    int tcount=tabBar->count();
    if((index<ucount&&ucount+1<=tcount)||(index>=ucount&&ucount+2<=tcount))
    {
        m_closeOtherTabAct->setVisible(true);
    }
    else
    {
        m_closeOtherTabAct->setVisible(false);
    }
    if(ucount<index)
    {
        m_closeLeftTabAct->setVisible(true);
    }
    else
    {
        m_closeLeftTabAct->setVisible(false);
    }

    if(index>=ucount-1&&index<tcount-1)
    {
        m_closeRightTabAct->setVisible(true);
    }
    else
    {
        m_closeRightTabAct->setVisible(false);
    }
    QWidget* widget1=this->widget(index);
    if(m_unDragWidgets.find(widget1)!=m_unDragWidgets.end())
    {
        m_unFixedTabAct->setVisible(true);
        m_fixedTabAct->setVisible(false);
    }
    else
    {
        m_unFixedTabAct->setVisible(false);
        m_fixedTabAct->setVisible(true);
    }

    m_menu->exec(globalPos);
}

void HTabWidget::onCloseLeftTabAct()
{
    int index=this->currentIndex();
    for(int i=m_unDragWidgets.count();i<index;i++)
    {
        QWidget* widget=this->widget(i);
        widget->deleteLater();
    }
}

void HTabWidget::onCloseRightTabAct()
{
    QTabBar* tabBar=this->tabBar();
    int index=this->currentIndex();
    for(int i=index+1;i<tabBar->count();i++)
    {
        QWidget* widget=this->widget(i);
        widget->deleteLater();
    }
}

void HTabWidget::onCloseOtherTabAct()
{
    QTabBar* tabBar=this->tabBar();
    QWidget* currentWiget=this->widget(currentIndex());
    for(int i=m_unDragWidgets.count();i<tabBar->count();i++)
    {
        QWidget* widget=this->widget(i);
        if(currentWiget!=widget)
        {
            widget->deleteLater();
        }

    }
}

void HTabWidget::onFixedTabAct()
{
    setTabCanMove(currentIndex(),false);
}

void HTabWidget::onUnFixedTabAct()
{
    setTabCanMove(currentIndex(),true);
}

void HTabWidget::onCurrentChanged(int index)
{
    for(int i=m_unDragWidgets.count();i<count();i++)
    {
        QWidget* widget=tabBar()->tabButton(i,QTabBar::RightSide);
        if(widget)
        {
            widget->setVisible(i==index);
        }
    }
}





