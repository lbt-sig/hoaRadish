#ifndef HTABWIDGET_H
#define HTABWIDGET_H

#include <QTabWidget>
#include <QSet>
#include <QMenu>
class HTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    HTabWidget(QWidget *parent = nullptr);
    void setTabCanMove(int index,bool canMove);
    void setTabsClosable(bool closeable);
    bool tabsClosable() const;
    int addTab(QWidget *page, const QString &label);
    int addTab(QWidget *page, const QIcon &icon, const QString &label);
    int insertTab(int index, QWidget *page, const QString &label);
    int insertTab(int index, QWidget *page, const QIcon &icon, const QString &label);
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
signals:

public slots:
private:
    bool m_tabClosable;
    bool m_isDrag;
    int m_dragIndex;
    int m_xOffset;
    int m_offset;
    bool m_tabBarPress;
    QSet<QWidget*> m_unDragWidgets;
    QSet<QWidget*> m_DragWidgets;
    QPoint m_mousePressPoint;
    QMenu* m_menu;
    QAction* m_closeLeftTabAct;
    QAction* m_closeRightTabAct;
    QAction* m_closeOtherTabAct;
    QAction* m_fixedTabAct;
    QAction* m_unFixedTabAct;
    void startDrag(int index);
    void createContextMenu();
    void setTabCloseBtn(int index,QWidget* widget);
signals:
private slots:
    void tabBarCustomContextMenuRequested(const QPoint &pos);
    void onCloseLeftTabAct();
    void onCloseRightTabAct();
    void onCloseOtherTabAct();
    void onFixedTabAct();
    void onUnFixedTabAct();
    void onCurrentChanged(int index);



};
#endif // HTABWIDGET_H
