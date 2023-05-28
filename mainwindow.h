#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGroupBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <vector>
#include <QWheelEvent>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent =0) : QGraphicsView(parent) {
        setDragMode(RubberBandDrag);
        setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    }

public slots:
    void zoomIn() { scaleBy(1.1); }
    void zoomOut() { scaleBy(1.0 / 1.1); }

protected:
    void wheelEvent(QWheelEvent *event) {
        scaleBy(std::pow(4.0 / 3.0, (-event->angleDelta().y() / 240.0)));
    }

private:
    void scaleBy(double factor) { scale(factor, factor); }
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void DrawSolved();

public slots:
    void NewFile();
    void OpenFile();
    void SaveFile();
    void SaveAsFile();
    void AboutHelp();
    void AboutQtHelp();
    void Solve();
    void Draw();
    void Random();
    void ExportScene();
private:
    QGroupBox* createInputBox();
    void save_file(const QString& filename);
    void export_scene(const QString& filename);
    void save_string_to_polygon();
    void save_polygon_to_string();
    QPolygonF* convert_polygon(std::vector<std::pair<int, int>> points);

private:
    Ui::MainWindow *ui;

    QString _file_save_name;
    QString _file_export_name;
    QSpinBox* _solve_v1_x;
    QSpinBox* _solve_v1_y;
    QSpinBox* _solve_v2_x;
    QSpinBox* _solve_v2_y;
    QLineEdit* _solve_points_edit;
    QGraphicsScene* _scene;
    GraphicsView* _gview;
    std::vector<std::pair<int, int>> _solve_polygon;
};
#endif // MAINWINDOW_H
