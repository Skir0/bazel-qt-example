#include "Controller.h"
#include <iostream>
#include <QtWidgets>

Controller::Controller() {

    // enable mouse tracking for the main window
    setMouseTracking(true);

    QWidget *central_widget = new QWidget;
    setCentralWidget(central_widget);

    mode_box_ = new QComboBox(central_widget);
    mode_box_->addItems({"light", "polygons"});

    paint_widget_ = new PaintWidget(central_widget);

    // Install event filter on both the paint widget and the main window
    // paint_widget_->installEventFilter(this);

    QVBoxLayout *mode_layout = new QVBoxLayout();
    QVBoxLayout *painter_layout = new QVBoxLayout();

    mode_layout->addWidget(mode_box_);
    painter_layout->addWidget(paint_widget_);

    QVBoxLayout *main_layout = new QVBoxLayout();
    main_layout->addLayout(mode_layout);
    main_layout->addLayout(painter_layout);

    central_widget->setLayout(main_layout);
    CreateActions();
}

QPoint Controller::GetLightSource() {
    return light_source_;
}

void Controller::SetLightSource(QPoint lightSource) {
    light_source_ = lightSource;
}

const std::vector<Polygon> & Controller::GetPolygons() {
    return polygons_;
}

void Controller::AddPolygon(const Polygon &polygon) {
    polygons_.push_back(polygon);
}

void Controller::AddVertexToLastPolygon(const QPoint &new_vertex) {
    Polygon& last_polygon = polygons_[polygons_.size() - 1];
    last_polygon.AddVertex(new_vertex);
}
// TODO
void Controller::UpdateLastPolygon(const QPoint &new_vertex) {
    // auto& last_polygon = current_polygon_.;
    // last_polygon = new_vertex;
}

std::vector<Ray> Controller::CastRays() {
    std::vector<Ray> rays;
    for (auto polygon: polygons_) {
        std::vector<QPoint> vertices = polygon.GetVertices();
        for (QPoint& vertex: vertices) {
            // count angle
            double angle = qAtan2(vertex.y() - light_source_.y(),
                     vertex.x() - light_source_.x());
            std::cout << angle << std::endl;

            // create rays
            Ray ray1 = Ray(light_source_, vertex, angle);
            Ray ray2 = ray1.Rotate(angle + 0.0001);
            Ray ray3 = ray1.Rotate(angle - 0.0001);

            // add rays
            rays.push_back(ray1);
            rays.push_back(ray2);
            rays.push_back(ray3);
        }
    }
    return rays;
}

void Controller::IntersectRays(std::vector<Ray> *rays) {
    for (auto polygon: polygons_) {
        for (auto& ray: *rays) {
            auto intersect_result = polygon.IntersectRay(ray);

            if (intersect_result) {
                std::cout << "dis1: " << Polygon::GetDistance(ray.GetBegin(), intersect_result.value()) << std::endl;
                std::cout << "dist2 " << Polygon::GetDistance(ray.GetBegin(), ray.GetEnd()) << std::endl;
                if (Polygon::GetDistance(ray.GetBegin(), intersect_result.value()) <
                    Polygon::GetDistance(ray.GetBegin(), ray.GetEnd())) {
                        ray.SetEnd(intersect_result.value());
                }
            }

        }
    }
}

void Controller::RemoveAdjacentRays(std::vector<Ray> *rays) {
}

Polygon Controller::CreateLightArea() {
    std::vector<Ray> rays = CastRays();
    paint_widget_->PaintRays(rays);
    IntersectRays(&rays);
    RemoveAdjacentRays(&rays);
    std::sort(rays.begin(), rays.end(), [](const Ray &ray1, const Ray &ray2) {
        return ray1.GetAngle() < ray2.GetAngle();
    });
    std::vector<QPoint> vertices;
    for (auto ray: rays) {
        vertices.push_back(ray.GetEnd());
    }
    return Polygon(vertices);
}



void Controller::ModeChanged() {
    std::cout << "mode changed" << std::endl;
    if (mode_box_->currentIndex() == LIGHT) {
        // create a small red circle cursor
        QPixmap pixmap(16, 16);
        pixmap.fill(Qt::transparent);

        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::red);
        painter.setBrush(Qt::red);
        painter.drawEllipse(8, 8, 8, 8);

        // Set the cursor with hotspot at center
        QCursor customCursor(pixmap, 8, 8);
        setCursor(customCursor);

        mode_ = LIGHT;
    } else {
        paint_widget_->ClearRaysBuffer();
        paint_widget_->update();
        // Restore default cursor
        setCursor(Qt::ArrowCursor);
        mode_ = POLYGONS;
    }
}

void Controller::MouseMoveEvent(QPoint pos) {
    if (mode_ == LIGHT) {
        light_source_ = pos;
        std::vector<Ray> rays = CastRays();
        // IntersectRays(&rays);
        // paint_widget_->PaintRays(rays);
        Polygon polygon = CreateLightArea();
        paint_widget_->PaintPolygon(polygon);
    }
}

void Controller::MouseLeftClicked(QPoint pos) {
    if (mode_ == POLYGONS) {
        std::cout << pos.x() << " " << pos.y() << std::endl;
        if (is_right_clicked) {
            std::cout << "start" << std::endl;
            Polygon polygon;
            AddPolygon(polygon);
            is_right_clicked = false;
        }
        AddVertexToLastPolygon(pos);
        paint_widget_->PaintPolygon(polygons_[polygons_.size() - 1]);

    }
}

void Controller::MouseRightClicked(QPoint pos) {
    if (mode_ == POLYGONS) {
        is_right_clicked = true;
        std::cout << "light " << light_source_.x() << " " << light_source_.y() << std::endl;
    }
}

Controller::~Controller() {
}

void Controller::CreateActions() {
    connect(mode_box_, &QComboBox::currentIndexChanged,
            this, &Controller::ModeChanged);
    connect(paint_widget_, &PaintWidget::mouseMoved, this, &Controller::MouseMoveEvent);
    connect(paint_widget_, &PaintWidget::leftClicked, this, &Controller::MouseLeftClicked);
    connect(paint_widget_, &PaintWidget::rightClicked, this, &Controller::MouseRightClicked);
}
