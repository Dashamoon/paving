#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SolveTile.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <random>

using std::min;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QHBoxLayout* hlayout = new QHBoxLayout;
    hlayout->addWidget(createInputBox());

    _scene = new QGraphicsScene(this);
    _gview = new GraphicsView();
    _scene->setSceneRect(0, 0, 2000, 2000);
    _gview->setScene(_scene);
    hlayout->addWidget(_gview);

    ui->centralwidget->setLayout(hlayout);

    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(NewFile()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(OpenFile()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(SaveFile()));
    connect(ui->actionSave_as, SIGNAL(triggered()), this, SLOT(SaveAsFile()));
    connect(ui->actionabout, SIGNAL(triggered()), this, SLOT(AboutHelp()));
    connect(ui->actionabout_Qt, SIGNAL(triggered()), this, SLOT(AboutQtHelp()));
    connect(ui->actionExport, SIGNAL(triggered()),this, SLOT(ExportScene()));
    connect(this, SIGNAL(DrawSolved()), this, SLOT(Draw()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Random(){
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_int_distribution ds{50, 100};
    MainWindow::_solve_v1_y->setValue(ds(gen));
    MainWindow::_solve_v2_y->setValue((int)(_solve_v1_y->value() / 2));
    MainWindow::_solve_v1_x->setValue(ds(gen));
    MainWindow::_solve_v2_x->setValue(ds(gen));
    _solve_polygon.clear();
    _solve_polygon.push_back(std::pair(0, 0));
    _solve_polygon.push_back(std::pair(_solve_v1_x->value(), _solve_v1_y->value()));
    _solve_polygon.push_back(std::pair(_solve_v1_x->value() + _solve_v2_x->value(), _solve_v1_y->value() + _solve_v2_y->value()));
    _solve_polygon.push_back(std::pair(_solve_v2_x->value(), _solve_v2_y->value()));
    save_polygon_to_string();
}

QGroupBox* MainWindow::createInputBox()
{
    QGroupBox* group = new QGroupBox(tr("Input"));
    group->setMinimumSize(QSize(250, 0));
    group->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred));
    QVBoxLayout* vbox = new QVBoxLayout;

    QGroupBox* group_v1 = new QGroupBox(tr("Vector 1"));
    QGridLayout* grid_v1 = new QGridLayout;
    _solve_v1_x = new QSpinBox;
    _solve_v1_x->setMinimum(-1000000);
    _solve_v1_x->setMaximum(1000000);
    _solve_v1_y = new QSpinBox;
    _solve_v1_y->setMinimum(-1000000);
    _solve_v1_y->setMaximum(1000000);
    QLabel* label_x1 = new QLabel("X");
    label_x1->setFixedWidth(10);
    QLabel* label_y1 = new QLabel("Y");
    grid_v1->addWidget(label_x1, 0, 0);
    grid_v1->addWidget(_solve_v1_x, 0, 1);
    grid_v1->addWidget(label_y1, 1, 0);
    grid_v1->addWidget(_solve_v1_y, 1, 1);
    group_v1->setLayout(grid_v1);
    vbox->addWidget(group_v1);

    QGroupBox* group_v2 = new QGroupBox(tr("Vector 2"));
    QGridLayout* grid_v2 = new QGridLayout;
    _solve_v2_x = new QSpinBox;
    _solve_v2_x->setMinimum(-1000000);
    _solve_v2_x->setMaximum(1000000);
    _solve_v2_y = new QSpinBox;
    _solve_v2_y->setMinimum(-1000000);
    _solve_v2_y->setMaximum(1000000);
    QLabel* label_x2 = new QLabel("X");
    label_x2->setFixedWidth(10);
    QLabel* label_y2 = new QLabel("Y");
    grid_v2->addWidget(label_x2, 0, 0);
    grid_v2->addWidget(_solve_v2_x, 0, 1);
    grid_v2->addWidget(label_y2, 1, 0);
    grid_v2->addWidget(_solve_v2_y, 1, 1);
    group_v2->setLayout(grid_v2);
    vbox->addWidget(group_v2);

    QGroupBox* group_polygon = new QGroupBox;
    group_polygon->setTitle("Polygon points");
    _solve_points_edit = new QLineEdit;
    QVBoxLayout* vbox_polygon = new QVBoxLayout;
    vbox_polygon->addWidget(_solve_points_edit);
    group_polygon->setLayout(vbox_polygon);
    vbox->addWidget(group_polygon);

    vbox->addStretch();

    QPushButton* but = new QPushButton(tr("Генератор"));
    connect(but, SIGNAL(clicked(bool)), this, SLOT(Random()));
    vbox->addWidget(but);
    QPushButton* button = new QPushButton(tr("Пуск"));
    connect(button, SIGNAL(clicked(bool)), this, SLOT(Solve()));
    vbox->addWidget(button);

    group->setLayout(vbox);
    return group;
}

void MainWindow::save_string_to_polygon()
{
    QString line = _solve_points_edit->text();
    _solve_polygon.clear();
    QTextStream in(&line);
    while (!in.atEnd()){
        int x, y;
        in >> x >> y;
        _solve_polygon.push_back(std::pair(x, y));
    }
}

void MainWindow::save_polygon_to_string()
{
    QString line;
    QTextStream out(&line);
    for(size_t i = 0; i < _solve_polygon.size();  i++){
        out << _solve_polygon[i].first << " " << _solve_polygon[i].second;
        if(i < _solve_polygon.size() - 1){
            out<<" ";
        }
    }
    _solve_points_edit->setText(line);
}

void MainWindow::NewFile()
{
    QString strFilter;
    QString fileName = QFileDialog::getSaveFileName(this, tr("New File"), "", "*.dat", &strFilter);
    if (fileName.isEmpty())
       return;
    _file_save_name = fileName;
    auto name = this->windowTitle();
    auto pos = name.indexOf(" (");
    if (pos >= 0)
        name.remove(pos, name.length() - pos + 1);
    this->setWindowTitle(name + " (" + _file_save_name + ")");
    _solve_v1_x->setValue(0);
    _solve_v1_y->setValue(0);
    _solve_v2_x->setValue(0);
    _solve_v2_y->setValue(0);
    _solve_polygon.clear();
    save_polygon_to_string();
}

void MainWindow::OpenFile()
{
    QString strFilter;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "*.dat", &strFilter);
    if (fileName.isEmpty())
       return;
    _file_save_name = fileName;

    auto name = this->windowTitle();
    auto pos = name.indexOf(" (");
    if (pos >= 0)
        name.remove(pos, name.length() - pos + 1);
    this->setWindowTitle(name + " (" + _file_save_name + ")");

    QFile file(_file_save_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);
    if(!in.atEnd()){
        int x, y;
        in >> x >> y;
        _solve_v1_x->setValue(x);
        _solve_v1_y->setValue(y);
    }
    if(!in.atEnd()){
        int x, y;
        in >> x >> y;
        _solve_v2_x->setValue(x);
        _solve_v2_y->setValue(y);
    }
    _solve_polygon.clear();
    while (!in.atEnd()){
        int x, y;
        in >> x >> y;
        _solve_polygon.push_back(std::pair(x, y));
    }
    save_polygon_to_string();
    file.close();
}

void MainWindow::SaveFile()
{
    if (!_file_save_name.isEmpty())
        save_file(_file_save_name);
    else {
        SaveAsFile();
    }
}

void MainWindow::SaveAsFile()
{
    QString strFilter;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", "*.dat", &strFilter);
    if (fileName.isEmpty())
       return;
    _file_save_name = fileName;

    auto name = this->windowTitle();
    auto pos = name.indexOf(" (");
    if (pos >= 0)
        name.remove(pos, name.length() - pos + 1);
    this->setWindowTitle(name + " (" + _file_save_name + ")");

    save_file(_file_save_name);
}

void MainWindow::save_file(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << _solve_v1_x->value() << " " << _solve_v1_y->value() << "\n";
    out << _solve_v2_x->value() << " " << _solve_v2_y->value() << "\n";

    save_string_to_polygon();
    for(size_t i = 0; i < _solve_polygon.size(); i++){
        out << _solve_polygon[i].first << " " << _solve_polygon[i].second;
        if (i < _solve_polygon.size() - 1){
            out << "\n";
        }
    }
    file.close();
}

void MainWindow::ExportScene(){
    QString strFilter;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", "*.jpeg", &strFilter);
    if (fileName.isEmpty())
       return;
    _file_export_name = fileName;
    export_scene(_file_export_name);
}

void MainWindow::export_scene(const QString& filename){
    QImage image(_gview->size()*2, QImage::Format_ARGB32);
    image.setDotsPerMeterX(image.dotsPerMeterX() * 2);
    image.setDotsPerMeterY(image.dotsPerMeterY() * 2);
        QPainter painter(&image);
        painter.setRenderHints(QPainter::Antialiasing| QPainter::TextAntialiasing);
        _scene->render(&painter);
    if(image.save(filename)){

    }else{
        QMessageBox::warning(this, tr("Error"), tr("Failed to export: %1").arg(filename));
    }

}

void MainWindow::AboutHelp()
{
    QMessageBox::about(this, "About", "My Progamm Ver. 1.0");
}

void MainWindow::AboutQtHelp()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::Solve()
{
    save_string_to_polygon();
    SolveTile paving;
    int x1 = _solve_v1_x->value();
    int y1 = _solve_v1_y->value();
    int x2 = _solve_v2_x->value();
    int y2 = _solve_v2_y->value();
    paving.setVectors(x1, y1, x2, y2);
    paving.setPolygon(_solve_polygon);
    if (!paving.check_hole() && !paving.check_intersection()) {
        emit DrawSolved();
    } else {
        QMessageBox::critical(this, "Error", "Not solved");
    }
}

QPolygonF* MainWindow::convert_polygon(std::vector<std::pair<int, int>> points)
{
    QList<QPoint> pts;
    for(size_t i = 0; i < points.size(); i++){
        pts.push_back(QPoint(points[i].first, points[i].second));
    }
    pts.push_back(QPoint(points[0].first, points[0].second));
    return new QPolygonF(pts);
}

void MainWindow::Draw()
{
    QRectF rect = _gview->sceneRect();
    _scene->clear();

    int x1 = _solve_v1_x->value();
    int y1 = _solve_v1_y->value();
    int x2 = _solve_v2_x->value();
    int y2 = _solve_v2_y->value();

    QBrush backBrush(Qt::white, Qt::BrushStyle::SolidPattern);
    _scene->setBackgroundBrush(backBrush);

    QPen pen(QColor("black"));
    QBrush brush(Qt::white, Qt::NoBrush);
    QPointF initPoint {rect.width() / 2.0, rect.height() / 2.0};
    QPointF cur = initPoint;

    QPolygonF polygon = *convert_polygon(_solve_polygon);
    polygon.translate(cur.x(), cur.y());;

    int n1_v2 = (int) min(((rect.width() - cur.y()) / x2), ((rect.height() - cur.y()) / y2));
    int n2_v2 = (int) min((cur.x()/x1), (cur.y()/y1));
    int n1_v1 = (int) min(((rect.width() - cur.y()) / x1), ((rect.height() - cur.y()) / y1));
    int n2_v1 = (int) min((cur.x()/x1), (cur.y()/y1));

    for (int k = 0; k < n1_v2; k++){
        for(int i = 0; i < n1_v1; i++){
            _scene->addPolygon(polygon.translated(QPointF(i*x1 + k*x2, i*y1 + k*y2)), pen, brush);
        }
        for(int j = 0; j < n2_v1; j++){
            _scene->addPolygon(polygon.translated(QPointF(0 - j*x1 + k*x2, 0 - j*y1 + k*y2)), pen, brush);
        }
        cur.setX(cur.x() + x2);
        cur.setY(cur.y() + y2);
        n1_v1 = (int) min(((rect.width() - cur.y()) / x1), ((rect.height() - cur.y()) / y1));
        n2_v1 = (int) min((cur.x() / x1), (cur.y() / y1));
    }
    cur = initPoint;
    for (int k = 0; k < n2_v2; k++){
        for(int i = 0; i < n1_v1; i++){
            _scene->addPolygon(polygon.translated(QPointF(i*x1 - k*x2, i*y1 - k*y2)), pen, brush);
        }
        for(int j = 0; j < n2_v1; j++){
            _scene->addPolygon(polygon.translated(QPointF(-j*x1 - k*x2, -j*y1 - k*y2)), pen, brush);
        }
        cur.setX(cur.x() - x2);
        cur.setY(cur.y() - y2);
        n1_v1 = (int) std::min(((rect.width() - cur.y()) / x1), ((rect.height() - cur.y()) / y1));
        n2_v1 = (int) min((cur.x() / x1), (cur.y() / y1));
    }
}
