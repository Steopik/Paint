#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QPixelFormat>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>
#include <QFileDialog>
#include <QFile>
#include <QColorDialog>
#include <QColor>
#include <QKeyEvent>
#include <QMessageBox>
#include <QComboBox>
#include <QPainter>
#include <QScrollBar>
#include <QCheckBox>
#include <QVector>
#include <QPalette>


#include "savedialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool HasChanges = false;
    QPixmap pix, tmpPix;
    QPushButton *FilePushButton;
    QPushButton *Color1PushButton;
    QPushButton *Color2PushButton;
    QLabel *LabelForPaint;
    QComboBox* WidthOfPenComboBox;
    QCheckBox* FillChechBox;

    QFile file;
    QPainter painter;
    QColor ColorOfPen = Qt::black;
    QColor ColorOfFill = Qt::white;
    QColor colorOfStartFilling;
    QPoint StartPoint, StartLinePoint, EndPoint, StartPolygonPoint = QPoint(-1, -1);
    QPoint FirstFillPoint = QPoint(-1, -1), EndFillPoint = QPoint(-1, -1);
    QPixmap CopyPixMap;
    QVector <QPixmap> VectorOfPixMap;
    QVector <QPoint> FillQueueVector;

    QString Operation = "Move";

    bool Repaint = true;
    bool isSrart = true;
    bool HasPolygon = false;
    bool HasFill = false;
    bool EndMove = false;
    int WidthOfPen = 1;
    int ToolNumber = 6;
    QString WorkArea = "Paint";

    QPixmap appendPixMap;
    void newFile();
    void loadFile(QString nameOfFile);
    void OpenFile();
    bool SaveFile();

    void AddWhiteBackground();
    void CropBackground();

    void pen();
    void eraser();
    void drawEllipse();
    void drawRectangle();
    void DrawLine();
    void DrawTriangle();
    void DrawPolygonLine();
    void endPolygon();
    void MakeShapeCorrect();
    void MoveFill();
    void ReturnOldPixMap();
    void FillingArea(QPoint point);
    void Copy();
    void insert();




private slots:
    void on_Color1PushButton_clicked();
    void on_Color2PushButton_clicked();


    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent* event);
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
