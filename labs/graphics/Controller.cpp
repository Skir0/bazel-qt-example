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

QPointF Controller::GetLightSource() {
    return light_source_;
}

void Controller::SetLightSource(QPointF lightSource) {
    light_source_ = lightSource;
}

const std::vector<Polygon> & Controller::GetPolygons() {
    return polygons_;
}

void Controller::AddPolygon(const Polygon &polygon) {
    polygons_.push_back(polygon);
}

void Controller::AddVertexToLastPolygon(const QPointF &new_vertex) {
    Polygon& last_polygon = polygons_[polygons_.size() - 1];
    last_polygon.AddVertex(new_vertex);
}
// TODO
void Controller::UpdateLastPolygon(const QPointF &new_vertex) {
    // auto& last_polygon = current_polygon_.;
    // last_polygon = new_vertex;
}

std::vector<Ray> Controller::CastRays() {
    std::vector<Ray> rays;
    for (auto polygon: polygons_) {
        std::vector<QPointF> vertices = polygon.GetVertices();
        for (QPointF& vertex: vertices) {
            // count angle
            double angle = AngleBetweenPoints(light_source_, vertex);
            // std::cout << angle << std::endl;

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
            auto intersection_result = polygon.IntersectRay(ray);
            if (intersection_result != std::nullopt) {
                if (Polygon::GetDistance(ray.GetBegin(), intersection_result.value()) <
                    Polygon::GetDistance(ray.GetBegin(), ray.GetEnd())) {
                    double angle1 = AngleBetweenPoints(light_source_, intersection_result.value());
                    double angle2 = AngleBetweenPoints(light_source_, ray.GetEnd());

                    if (compareAngles(angle1, angle2)) {
                        std::cout << "comapare is good" << std::endl;
                        std::cout << "angles " <<  angle1 << " " << angle2 << " " << ray.GetAngle() << std::endl;
                        ray.SetEnd(intersection_result.value());
                    }
                }
            }
        }
    }
}

void Controller::RemoveAdjacentRays(std::vector<Ray> *rays) {
}

Polygon Controller::CreateLightArea() {
    std::vector<Ray> rays = CastRays();
    IntersectRays(&rays);
    // RemoveAdjacentRays(&rays);

    paint_widget_->PaintRays(rays);
    std::sort(rays.begin(), rays.end(), [](const Ray &ray1, const Ray &ray2) {
        return ray1.GetAngle() < ray2.GetAngle();
    });
    std::cout << "sorted, size: " << rays.size() << std::endl;
    for (auto ray: rays) {
        std::cout << ray.GetAngle() << " ";
    }
    std::cout << std::endl;
    std::vector<QPointF> vertices;
    for (auto ray: rays) {
        vertices.push_back(ray.GetEnd());
    }
    vertices.push_back(light_source_);
    return Polygon(vertices);
}



void Controller::ModeChanged() {
    std::cout << "mode changed" << std::endl;

    if (mode_box_->currentIndex() == LIGHT) {
        std::cout << "size of polygons: " << polygons_[0].GetVertices().size() << std::endl;
        for (auto v: polygons_[0].GetVertices()) {
            std::cout << v.x() << " " << v.y() << std::endl;
        }
        if (!is_right_clicked) {
            std::cout << "right click is pressed " << std::endl;
            AddVertexToLastPolygon(polygons_[polygons_.size() - 1].GetVertices()[0]);
            is_right_clicked = true;
        }
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

void Controller::MouseMoveEvent(QPointF pos) {
    // std::cout << "mouse moved " <<  pos.x() << " " << pos.y() << std::endl;
    // std::cout << "angle " <<  CountAngle(QPointF(0, 0)) << std::endl;

    if (mode_ == LIGHT) {
        light_source_ = pos;
        std::vector<Ray> rays = CastRays();
        IntersectRays(&rays);
        paint_widget_->PaintRays(rays);
        // Polygon polygon = CreateLightArea();
        // paint_widget_->PaintPolygon(polygon);
    }
}

void Controller::MouseLeftClicked(QPointF pos) {
    if (mode_ == POLYGONS) {
        std::cout << pos.x() << " " << pos.y() << std::endl;
        if (is_right_clicked) {
            std::cout << "start" << std::endl;
            Polygon polygon;
            AddPolygon(polygon);
            is_right_clicked = false;
        }
        AddVertexToLastPolygon(pos);
        std::cout << "add: " << pos.x() << " " << pos.y() << std::endl;
        paint_widget_->PaintPolygon(polygons_[polygons_.size() - 1]);

    }
}

void Controller::MouseRightClicked(QPointF pos) {
    if (mode_ == POLYGONS) {
        AddVertexToLastPolygon(polygons_[polygons_.size() - 1].GetVertices()[0]);
        is_right_clicked = true;
        std::cout << "light " << light_source_.x() << " " << light_source_.y() << std::endl;
    }
    if (mode_ == LIGHT) {
        paint_widget_->PaintRays(CastRays());

    }
}
double Controller::AngleBetweenPoints(const QPointF& p1, const QPointF& p2) {
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    return qRadiansToDegrees(qAtan2(dy, dx));
}

bool Controller::compareAngles(double angle1, double angle2) {
    double diff = qAbs(angle1 - angle2);
    diff = qMin(diff, 360.0 - diff);  // Handle wrap-around
    return diff <= 0.1;  // Use a more reasonable threshold (0.1 degrees)
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
