#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMenu* FileMenu = new QMenu;
    QAction* NewFileActioon = new QAction("Новый файл CTRL + N");
    QAction* OpenFileActioon = new QAction("Открыть файл CTRL + O");
    QAction*SaveFileActioon = new QAction("Сохранить как CTRL + S");
    FileMenu->addAction(NewFileActioon);
    FileMenu->addAction(OpenFileActioon);
    FileMenu->addAction(SaveFileActioon);
    FilePushButton = new QPushButton(this);
    FilePushButton->setText("Файл");
    FilePushButton->setGeometry(0, 0, 70, 20);
    FilePushButton->setMenu(FileMenu);

    LabelForPaint = new QLabel(ui->scrollArea);
    LabelForPaint->setAlignment(Qt::AlignTop);
    ui->scrollArea->setWidget(LabelForPaint);

    Color1PushButton = new QPushButton(this);
    Color1PushButton->setPalette(Qt::black);
    Color1PushButton->setGeometry(100, 0, 20, 20);

    Color2PushButton = new QPushButton(this);
    Color2PushButton->setPalette(Qt::white);
    Color2PushButton->setGeometry(130, 0, 20, 20);

    FillChechBox = new QCheckBox(this);
    FillChechBox->setGeometry(130, 25, 20, 20);

    WidthOfPenComboBox = new QComboBox(this);
    WidthOfPenComboBox->setGeometry(160, 0, 100, 20);
    QStringList sotrComboBoxList = {"1 px", "2 px", "3 px", "4 px", "5 px", "6 px", "7 px", "8 px", "9 px", "10 px"};
    WidthOfPenComboBox->addItems(sotrComboBoxList);


    connect(OpenFileActioon, &QAction::triggered, [=](){OpenFile();});
    connect(SaveFileActioon, &QAction::triggered, [=](){SaveFile();});
    connect(NewFileActioon, &QAction::triggered, [=](){newFile();});
    connect(Color1PushButton, SIGNAL(pressed()), this, SLOT(on_Color1PushButton_clicked()));
    connect(Color2PushButton, SIGNAL(pressed()), this, SLOT(on_Color2PushButton_clicked()));
    connect(WidthOfPenComboBox, &QComboBox::activated, [&](){WidthOfPen = (WidthOfPenComboBox->currentText().split(' ')[0]).toInt();});
    newFile();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newFile()
{
    if (HasChanges) {
        SaveDialog * dialog = new SaveDialog(this->x(), this->y());
        dialog->exec();
        if (dialog->ans == 1){
            SaveFile();
            loadFile("/home/stepan/Kursach/untitled/template.png");
        }
        else if (dialog->ans == -1) loadFile("/home/stepan/Kursach/untitled/template.png");

    }
    else{
        loadFile("/home/stepan/Kursach/untitled/template.png");
    }
    appendPixMap = VectorOfPixMap.back();
}

void MainWindow::loadFile(QString nameOfFile)
{
    VectorOfPixMap.clear();
    bool a = pix.load(nameOfFile);
    if (a){
        HasChanges = false;
        pix.scaled(1000, 1000, Qt::KeepAspectRatio);
        VectorOfPixMap.append(pix);
        LabelForPaint->setPixmap(VectorOfPixMap.back());

    }
    else QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл");
    if (LabelForPaint->width() >= VectorOfPixMap.back().width() && LabelForPaint->height() >= VectorOfPixMap.back().height()) AddWhiteBackground();

}

void MainWindow::OpenFile()
{
    if (HasChanges){
        SaveDialog * dialog = new SaveDialog(this->x(), this->y());
        dialog->exec();
        if (dialog->ans == 1){
            SaveFile();
            QString FileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "/home/stepan/Изображения",tr("Изображения (*.png *.bmp *.jpg);;Все файлы (*.*)"));
            if (FileName != "") loadFile(FileName);
        }
        else if (dialog->ans == -1){
            QString FileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "/home/stepan/Изображения",tr("Изображения (*.png *.bmp *.jpg);;Все файлы (*.*)"));
            if (FileName != "") loadFile(FileName);
        }
    }
    else{
        QString FileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "/home/stepan/Изображения",tr("Изображения (*.png *.bmp *.jpg);;Все файлы (*.*)"));
        if (FileName != "") loadFile(FileName);
    }
}

bool MainWindow::SaveFile()
{
    const QString initialPath = QDir::currentPath() + "/untitled.png";
    const QString FileName =
        QFileDialog::getSaveFileName(this, tr("Сохранить как"), initialPath,
                                     tr("Изображения PNG (*.png);;Изображения BMP (*.bmp);;")+
                                         tr("Изображения JPG (*.jpg);;Все файлы (*.*)"));

    if (!FileName.isEmpty()) {
        QFileInfo fi(FileName);
        bool b = VectorOfPixMap.back().save(FileName, fi.completeSuffix().toStdString().c_str());
        if (b) HasChanges = false;
        else QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось сохранить файл"));
        return b;
    }
    return false;
}

void MainWindow::AddWhiteBackground()
{
    if(WorkArea == "Paint"){
        QPixmap scaledPixmap(LabelForPaint->width(), LabelForPaint->height());
        scaledPixmap.fill(Qt::white);
        QPainter paint(&scaledPixmap);
        paint.drawPixmap(0, 0, VectorOfPixMap.back());
        VectorOfPixMap.back() = scaledPixmap;
        LabelForPaint->setPixmap(VectorOfPixMap.back());
    }
}

void MainWindow::CropBackground()
{
    HasChanges = true;
    QPixmap scaledPixmap = VectorOfPixMap.back().copy(QRect(0,0, ui->scrollArea->width(), ui->scrollArea->height()));
    VectorOfPixMap.append(scaledPixmap);
    LabelForPaint->setPixmap(VectorOfPixMap.back());
}

void MainWindow::pen()
{
    HasChanges = true;
    painter.begin(&appendPixMap);
    painter.setPen(QPen(ColorOfPen, WidthOfPen, Qt::SolidLine));
    painter.drawLine(StartLinePoint, EndPoint);
    painter.end();
    LabelForPaint->setPixmap(appendPixMap);
}

void MainWindow::eraser()
{
    HasChanges = true;
    painter.begin(&appendPixMap);
    painter.setPen(QPen(Qt::white, WidthOfPen, Qt::SolidLine));
    painter.drawLine(StartLinePoint, EndPoint);
    painter.end();
    LabelForPaint->setPixmap(appendPixMap);
}

void MainWindow::drawEllipse()
{
    appendPixMap = VectorOfPixMap.back();
    HasChanges = true;
    painter.begin(&appendPixMap);
    painter.setPen(QPen(ColorOfPen, WidthOfPen, Qt::SolidLine));
    if (FillChechBox->isChecked()) painter.setBrush(ColorOfFill);
    painter.drawEllipse(StartPoint + (EndPoint - StartPoint) / 2, (EndPoint - StartPoint).x() / 2, (EndPoint - StartPoint).y() / 2);
    painter.end();
    VectorOfPixMap.append(appendPixMap);
    LabelForPaint->setPixmap(VectorOfPixMap.back());
    repaint();
}

void MainWindow::drawRectangle()
{
    appendPixMap = VectorOfPixMap.back();
    HasChanges = true;
    painter.begin(&appendPixMap);
    painter.setPen(QPen(ColorOfPen, WidthOfPen, Qt::SolidLine));
    if (FillChechBox->isChecked()) painter.setBrush(ColorOfFill);
    painter.drawRect(StartPoint.x(), StartPoint.y(), (EndPoint - StartPoint).x(), (EndPoint - StartPoint).y());
    painter.end();
    VectorOfPixMap.append(appendPixMap);
    LabelForPaint->setPixmap(VectorOfPixMap.back());
    repaint();
}

void MainWindow::DrawLine()
{
    appendPixMap = VectorOfPixMap.back();
    HasChanges = true;
    painter.begin(&appendPixMap);
    painter.setPen(QPen(ColorOfPen, WidthOfPen, Qt::SolidLine));
    painter.drawLine(StartPoint, EndPoint);
    painter.end();
    VectorOfPixMap.append(appendPixMap);
    LabelForPaint->setPixmap(VectorOfPixMap.back());
    repaint();
}

void MainWindow::DrawTriangle()
{
    appendPixMap = VectorOfPixMap.back();
    HasChanges = true;
    QPoint point3 = QPoint(StartPoint.x() - (EndPoint.x() - StartPoint.x()), EndPoint.y());
    painter.begin(&appendPixMap);
    QPolygon polygon;
    polygon << StartPoint << EndPoint << point3;
    painter.setPen(QPen(ColorOfPen, WidthOfPen, Qt::SolidLine));
    if (FillChechBox->isChecked()) painter.setBrush(ColorOfFill);
    painter.drawPolygon(polygon);
    painter.end();
    VectorOfPixMap.append(appendPixMap);
    LabelForPaint->setPixmap(VectorOfPixMap.back());
    repaint();
}

void MainWindow::endPolygon()
{
    appendPixMap = VectorOfPixMap.back();
    HasChanges = true;
    painter.begin(&appendPixMap);
    painter.setPen(QPen(ColorOfPen, WidthOfPen, Qt::SolidLine));
    painter.drawLine(StartPoint, StartPolygonPoint);
    painter.end();
    VectorOfPixMap.append(appendPixMap);
    LabelForPaint->setPixmap(VectorOfPixMap.back());
    HasPolygon = false;
    StartPolygonPoint = QPoint(-1, -1);
    repaint();
}

void MainWindow::MakeShapeCorrect()
{
    if (ToolNumber == 2 || ToolNumber == 4){
        int deltaX = EndPoint.x() - StartPoint.x();
        int deltaY = EndPoint.y() - StartPoint.y();
        if (deltaX == 0 || deltaY == 0) return;
        if (abs(deltaX) < abs(deltaY)) deltaY = abs(deltaX) * deltaY/abs(deltaY);
        else deltaX = abs(deltaY) * deltaX/abs(deltaX);
        EndPoint = QPoint(StartPoint.x() + deltaX, StartPoint.y() + deltaY);
    }
    else if (ToolNumber == 5){
        int deltaX = EndPoint.x() - StartPoint.x();
        int deltaY = EndPoint.y() - StartPoint.y();
        if (abs(deltaX) > abs(deltaY)) EndPoint = QPoint(EndPoint.x(), StartPoint.y());
        else EndPoint = QPoint(StartPoint.x(), EndPoint.y());
    }
    else if (ToolNumber == 6){
        int y = StartPoint.y() + (EndPoint.y() - StartPoint.y()) / abs(EndPoint.y() - StartPoint.y()) * abs(EndPoint.x() - StartPoint.x()) * sqrt(3);
        EndPoint = QPoint(EndPoint.x(), y);
    }
}

void MainWindow::MoveFill()
{
    QPoint deltaPoint;
    HasChanges = true;
    if (FirstFillPoint.x() > EndFillPoint.x() && FirstFillPoint.y() > EndFillPoint.y()){
        deltaPoint = FirstFillPoint;
        FirstFillPoint = EndFillPoint;
        EndFillPoint = deltaPoint;
    }
    else if (FirstFillPoint.x() < EndFillPoint.x() && FirstFillPoint.y() > EndFillPoint.y()){
        deltaPoint = FirstFillPoint;
        FirstFillPoint = QPoint(FirstFillPoint.x(), EndFillPoint.y());
        EndFillPoint = QPoint(EndFillPoint.x(), deltaPoint.y());
    }
    else if (FirstFillPoint.x() > EndFillPoint.x() && FirstFillPoint.y() < EndFillPoint.y()){
        deltaPoint = FirstFillPoint;
        FirstFillPoint = QPoint(EndFillPoint.x(), FirstFillPoint.y());
        EndFillPoint = QPoint(deltaPoint.x(), EndFillPoint.y());
    }

    appendPixMap = VectorOfPixMap.back();
    deltaPoint = StartPoint - FirstFillPoint;
    QPixmap scaledPixmap, pixmap;
    scaledPixmap = VectorOfPixMap.back().copy(QRect(FirstFillPoint, EndFillPoint));
    pixmap = VectorOfPixMap.back().copy(QRect(FirstFillPoint, EndFillPoint));
    pixmap.fill(Qt::white);
    painter.begin(&appendPixMap);
    painter.drawPixmap(FirstFillPoint, pixmap);
    painter.drawPixmap(EndPoint - deltaPoint, scaledPixmap);
    painter.end();
    VectorOfPixMap.append(appendPixMap);
    LabelForPaint->setPixmap(VectorOfPixMap.back());
    repaint();

}

void MainWindow::ReturnOldPixMap()
{
    if (VectorOfPixMap.size() > 1){
        VectorOfPixMap.pop_back();
        LabelForPaint->setPixmap(VectorOfPixMap.back());
    }
}

void MainWindow::FillingArea(QPoint point)
{

    QImage image = appendPixMap.toImage();
    const unsigned char *data = image.bits();
    int offset = (point.x() + point.y() * image.width()) * 4;
    QRgb colorOfPixel = qRgba(data[offset], data[offset + 1], data[offset + 2], data[offset + 3]);
    QColor color = QColor::fromRgba(colorOfPixel);
    if (color == ColorOfPen || color != colorOfStartFilling){
        return;
    }
    painter.begin(&appendPixMap);
    painter.setPen(QPen(ColorOfPen, 1, Qt::SolidLine));
    painter.drawPoint(point);
    painter.end();
    image = appendPixMap.toImage();
    if (point.x() > 0){
        int ind1 = (point.x() - 1 + point.y() * image.width()) * 4;
        QRgb colorOfPixel1 = qRgba(data[ind1], data[ind1 + 1], data[ind1 + 2], data[ind1 + 3]);
        QColor color1 = QColor::fromRgba(colorOfPixel1);
        if (!(color1 == ColorOfPen || color1 != colorOfStartFilling)) FillingArea(QPoint(point.x() - 1, point.y()));

    }
    if (point.x() < image.width()){
        int ind1 = (point.x() + 1 + point.y() * image.width()) * 4;
        QRgb colorOfPixel1 = qRgba(data[ind1], data[ind1 + 1], data[ind1 + 2], data[ind1 + 3]);
        QColor color1 = QColor::fromRgba(colorOfPixel1);
        if (!(color1 == ColorOfPen || color1 != colorOfStartFilling)) FillingArea(QPoint(point.x() + 1, point.y()));

    }
    if (point.y() > 0){
        int ind1 = (point.x() + (point.y() - 1) * image.width()) * 4;
        QRgb colorOfPixel1 = qRgba(data[ind1], data[ind1 + 1], data[ind1 + 2], data[ind1 + 3]);
        QColor color1 = QColor::fromRgba(colorOfPixel1);
        if (!(color1 == ColorOfPen || color1 != colorOfStartFilling)) FillingArea(QPoint(point.x(), point.y() - 1));

    }
    if (point.y() < image.height()){
        int ind1 = (point.x() + (point.y() + 1) * image.width()) * 4;
        QRgb colorOfPixel1 = qRgba(data[ind1], data[ind1 + 1], data[ind1 + 2], data[ind1 + 3]);
        QColor color1 = QColor::fromRgba(colorOfPixel1);
        if (!(color1 == ColorOfPen || color1 != colorOfStartFilling)) FillingArea(QPoint(point.x(), point.y() + 1));
    }

}

void MainWindow::Copy()
{
    QPoint deltaPoint;
    if (FirstFillPoint.x() > EndFillPoint.x() && FirstFillPoint.y() > EndFillPoint.y()){
        deltaPoint = FirstFillPoint;
        FirstFillPoint = EndFillPoint;
        EndFillPoint = deltaPoint;
    }
    else if (FirstFillPoint.x() < EndFillPoint.x() && FirstFillPoint.y() > EndFillPoint.y()){
        deltaPoint = FirstFillPoint;
        FirstFillPoint = QPoint(FirstFillPoint.x(), EndFillPoint.y());
        EndFillPoint = QPoint(EndFillPoint.x(), deltaPoint.y());
    }
    else if (FirstFillPoint.x() > EndFillPoint.x() && FirstFillPoint.y() < EndFillPoint.y()){
        deltaPoint = FirstFillPoint;
        FirstFillPoint = QPoint(EndFillPoint.x(), FirstFillPoint.y());
        EndFillPoint = QPoint(deltaPoint.x(), EndFillPoint.y());
    }

    appendPixMap = VectorOfPixMap.back();
    deltaPoint = StartPoint - FirstFillPoint;
    QPixmap scaledPixmap, pixmap;
    CopyPixMap = VectorOfPixMap.back().copy(QRect(FirstFillPoint, EndFillPoint));
    LabelForPaint->setPixmap(VectorOfPixMap.back());
    Repaint = false;
    repaint();
}

void MainWindow::insert()
{
    HasChanges = true;
    QPoint deltaPoint = StartPoint - QPoint(0 ,0);
    appendPixMap = VectorOfPixMap.back();
    painter.begin(&appendPixMap);
    painter.drawPixmap(EndPoint - deltaPoint, CopyPixMap);
    painter.end();
    VectorOfPixMap.append(appendPixMap);
    LabelForPaint->setPixmap(VectorOfPixMap.back());
    repaint();
    Operation = "Move";
}

void MainWindow::DrawPolygonLine()
{
    HasChanges = true;
    if (StartPolygonPoint == QPoint(-1, -1)){
        StartPolygonPoint = StartPoint;
        HasPolygon = true;
        return;
    }
    appendPixMap = VectorOfPixMap.back();
    painter.begin(&appendPixMap);
    painter.setPen(QPen(ColorOfPen, WidthOfPen, Qt::SolidLine));
    painter.drawLine(StartLinePoint, StartPoint);
    painter.end();
    VectorOfPixMap.append(appendPixMap);
    LabelForPaint->setPixmap(VectorOfPixMap.back());
}



void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if ((event->modifiers() &Qt::ControlModifier) && event->key() == 83) SaveFile();
    else if ((event->modifiers() &Qt::ControlModifier) && event->key() == 78) newFile();
    else if ((event->modifiers() &Qt::ControlModifier) && event->key() == 79) OpenFile();
    else if ((event->modifiers() &Qt::ControlModifier) && event->key() == 90) ReturnOldPixMap();
    if (HasFill){
        if ((event->modifiers() &Qt::ControlModifier) && event->key() == 67){
            Copy();
            HasFill = false;
            EndMove = false;
        }
    }
    if ((event->modifiers() &Qt::ControlModifier) && event->key() == 86){
        if (!CopyPixMap.isNull()){
            ToolNumber = 10;
            Operation = "Insert";
            tmpPix = VectorOfPixMap.back();
            painter.begin(&tmpPix);
            painter.drawPixmap(QPoint(0, 0), CopyPixMap);
            painter.end();
            LabelForPaint->setPixmap(tmpPix);
        }
    }

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    if (HasChanges){
        SaveDialog * dialog = new SaveDialog(this->x(), this->y());
        dialog->exec();
        if (dialog->ans == 1){
            bool save = SaveFile();
            if (save) event->accept();
        }
        else if (dialog->ans == -1){
            event->accept();
        }
        else{
            event->Close;
        }
    }
    else{
        event->accept();
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (isSrart){
        isSrart = false;
        return;
    }
    if (event->oldSize().width() < event->size().width() || event->oldSize().height()< event->size().height()) AddWhiteBackground();
}

void MainWindow::on_Color1PushButton_clicked()
{

    QColor color = QColorDialog::getColor();
    if (color.isValid()){
        ColorOfPen = color;
        Color1PushButton->setPalette(color);
    }
}

void MainWindow::on_Color2PushButton_clicked()
{
    QColor color = QColorDialog::getColor();
    if (color.isValid()){
        ColorOfFill = color;
        Color2PushButton->setPalette(color);
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    if (Repaint == true){
        if (ToolNumber == 1){
            pen();
        }
        else if (ToolNumber == 2){
            tmpPix = VectorOfPixMap.back();
            painter.begin(&tmpPix);
            painter.setPen(QPen(ColorOfPen, WidthOfPen, Qt::SolidLine));
            if (FillChechBox->isChecked()) painter.setBrush(ColorOfFill);
            painter.drawEllipse(StartPoint + (EndPoint - StartPoint) / 2, (EndPoint - StartPoint).x() / 2, (EndPoint - StartPoint).y() / 2);
            painter.end();
            LabelForPaint->setPixmap(tmpPix);
        }
        else if (ToolNumber == 3){
            eraser();
        }
        else if (ToolNumber == 4){
            tmpPix = VectorOfPixMap.back();
            painter.begin(&tmpPix);
            painter.setPen(QPen(ColorOfPen, WidthOfPen, Qt::SolidLine));
            if (FillChechBox->isChecked()) painter.setBrush(ColorOfFill);
            painter.drawRect(StartPoint.x(), StartPoint.y(), (EndPoint - StartPoint).x(), (EndPoint - StartPoint).y());
            painter.end();
            LabelForPaint->setPixmap(tmpPix);
        }
        else if (ToolNumber == 5)
        {
            tmpPix = VectorOfPixMap.back();
            painter.begin(&tmpPix);
            painter.setPen(QPen(ColorOfPen, WidthOfPen, Qt::SolidLine));
            painter.drawLine(StartPoint, EndPoint);
            painter.end();
            LabelForPaint->setPixmap(tmpPix);
        }
        else if (ToolNumber == 6){
            QPoint point3 = QPoint(StartPoint.x() - (EndPoint.x() - StartPoint.x()), EndPoint.y());
            QPolygon polygon;
            polygon << StartPoint << EndPoint << point3;
            tmpPix = VectorOfPixMap.back();
            painter.begin(&tmpPix);
            painter.setPen(QPen(ColorOfPen, WidthOfPen, Qt::SolidLine));
            if (FillChechBox->isChecked()) painter.setBrush(ColorOfFill);
            painter.drawPolygon(polygon);
            painter.end();
            LabelForPaint->setPixmap(tmpPix);
        }
        else if (ToolNumber == 8){
            if (!HasFill){
                tmpPix = VectorOfPixMap.back();
                painter.begin(&tmpPix);
                painter.setPen(QPen(Qt::black, 1, Qt::DashLine));
                painter.drawRect(FirstFillPoint.x(), FirstFillPoint.y(), (EndFillPoint - FirstFillPoint).x(), (EndFillPoint - FirstFillPoint).y());
                painter.end();
                LabelForPaint->setPixmap(tmpPix);
            }
            else{
                    QPoint deltaPoint;
                    if (FirstFillPoint.x() > EndFillPoint.x() && FirstFillPoint.y() > EndFillPoint.y()){
                        deltaPoint = FirstFillPoint;
                        FirstFillPoint = EndFillPoint;
                        EndFillPoint = deltaPoint;
                    }
                    else if (FirstFillPoint.x() < EndFillPoint.x() && FirstFillPoint.y() > EndFillPoint.y()){
                        deltaPoint = FirstFillPoint;
                        FirstFillPoint = QPoint(FirstFillPoint.x(), EndFillPoint.y());
                        EndFillPoint = QPoint(EndFillPoint.x(), deltaPoint.y());
                    }
                    else if (FirstFillPoint.x() > EndFillPoint.x() && FirstFillPoint.y() < EndFillPoint.y()){
                        deltaPoint = FirstFillPoint;
                        FirstFillPoint = QPoint(EndFillPoint.x(), FirstFillPoint.y());
                        EndFillPoint = QPoint(deltaPoint.x(), EndFillPoint.y());
                    }
                    tmpPix = VectorOfPixMap.back();
                    deltaPoint = StartPoint - FirstFillPoint;
                    QPixmap scaledPixmap, pixmap;
                    scaledPixmap = VectorOfPixMap.back().copy(QRect(FirstFillPoint, EndFillPoint));
                    pixmap = VectorOfPixMap.back().copy(QRect(FirstFillPoint, EndFillPoint));
                    pixmap.fill(Qt::white);
                    painter.begin(&tmpPix);
                    painter.drawPixmap(FirstFillPoint, pixmap);
                    painter.drawPixmap(EndPoint - deltaPoint, scaledPixmap);
                    painter.end();
                    LabelForPaint->setPixmap(tmpPix);
            }
        }
        else if (ToolNumber == 10){
            QPoint deltaPoint;
            tmpPix = VectorOfPixMap.back();
            deltaPoint = StartPoint - QPoint(0 ,0);
            QPixmap scaledPixmap, pixmap;
            scaledPixmap = CopyPixMap;
            painter.begin(&tmpPix);
            painter.drawPixmap(EndPoint - deltaPoint, scaledPixmap);
            painter.end();
            LabelForPaint->setPixmap(tmpPix);
        }
        Repaint = false;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    if (!(ev->buttons() & Qt::LeftButton)) return;
    StartLinePoint = EndPoint;
    EndPoint = ev->pos() - ui->scrollArea->pos() + QPoint(ui->scrollArea->horizontalScrollBar()->value(), ui->scrollArea->verticalScrollBar()->value());
    if (QGuiApplication::keyboardModifiers() & Qt::ShiftModifier) MakeShapeCorrect();
    Repaint = true;
    repaint();
    if (ToolNumber == 8 && !HasFill){
        EndFillPoint = ev->pos() - ui->scrollArea->pos() + QPoint(ui->scrollArea->horizontalScrollBar()->value(), ui->scrollArea->verticalScrollBar()->value());
    }

}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() != Qt::LeftButton) return;
    StartLinePoint = StartPoint;
    StartPoint = ev->pos() - ui->scrollArea->pos() + QPoint(ui->scrollArea->horizontalScrollBar()->value(), ui->scrollArea->verticalScrollBar()->value());
    EndPoint = ev->pos() - ui->scrollArea->pos() + QPoint(ui->scrollArea->horizontalScrollBar()->value(), ui->scrollArea->verticalScrollBar()->value());
    if (ToolNumber == 8l){
        if (!HasFill){
            FirstFillPoint = ev->pos() - ui->scrollArea->pos() + QPoint(ui->scrollArea->horizontalScrollBar()->value(), ui->scrollArea->verticalScrollBar()->value());
            EndFillPoint = ev->pos() - ui->scrollArea->pos() + QPoint(ui->scrollArea->horizontalScrollBar()->value(), ui->scrollArea->verticalScrollBar()->value());
        }
    }
    if (ToolNumber == 7) DrawPolygonLine();
    else{
        HasPolygon = false;
        StartPolygonPoint = QPoint(-1, -1);
    }
    if (ToolNumber == 1 || ToolNumber == 3) appendPixMap = VectorOfPixMap.back();

    if (ToolNumber == 9){
        appendPixMap = VectorOfPixMap.back();
        QImage image = VectorOfPixMap.back().toImage();
        const unsigned char *data = image.bits();
        int offset = (StartPoint.x() + StartPoint.y() * image.width()) * 4;
        QRgb colorOfPixel = qRgba(data[offset], data[offset + 1], data[offset + 2], data[offset + 3]);
        colorOfStartFilling = QColor::fromRgba(colorOfPixel);
        FillingArea(StartPoint);
        VectorOfPixMap.append(appendPixMap);
        LabelForPaint->setPixmap(VectorOfPixMap.back());
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() != Qt::LeftButton) return;
    if (ToolNumber == 1){
        VectorOfPixMap.append(appendPixMap);
        appendPixMap = VectorOfPixMap.back();
    }
    else if (ToolNumber == 2) drawEllipse();
    else if (ToolNumber == 3){
        VectorOfPixMap.append(appendPixMap);
        appendPixMap = VectorOfPixMap.back();
    }
    else if (ToolNumber == 4) drawRectangle();
    else if (ToolNumber == 5) DrawLine();
    else if (ToolNumber == 6) DrawTriangle();
    else if (ToolNumber == 8){
        if (HasFill == true){
            EndMove = true;
        }
        else{
            HasFill = true;
        }
        if (EndMove){
            MoveFill();
            HasFill = false;
            EndMove = false;
        }
    }
    else if (ToolNumber == 10){
        insert();
        ToolNumber = ui->spinBox->value();
    }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) CropBackground();
    else if (event->button() == Qt::LeftButton && ToolNumber == 7 && HasPolygon) endPolygon();
}



void MainWindow::on_spinBox_valueChanged(int arg1)
{
    ToolNumber = ui->spinBox->value();
}

