#include "Controller.h"
#include <iostream>
#include <QtWidgets>


const double compare_value = 0.00001;


Controller::Controller() {
    // enable mouse tracking for the main window
    setMouseTracking(true);

    QWidget *central_widget = new QWidget();
    central_widget = new QWidget();
    setCentralWidget(central_widget);

    mode_box_ = new QComboBox(central_widget);
    mode_box_->addItems({"light", "polygons"});

    paint_widget_ = new PaintWidget(central_widget);

    // Install event filter on both the paint widget and the main window

    QVBoxLayout *mode_layout = new QVBoxLayout();
    QVBoxLayout *painter_layout = new QVBoxLayout();

    mode_layout->addWidget(mode_box_);
    painter_layout->addWidget(paint_widget_);

    QVBoxLayout *main_layout = new QVBoxLayout();
    main_layout->addLayout(mode_layout);
    main_layout->addLayout(painter_layout);

    central_widget->setLayout(main_layout);
    CreateActions();

    std::vector<QPointF> vertices;
    Polygon polygon({
        QPointF(0, 0), QPointF(qApp->screens()[0]->size().width(), 0),
        QPointF(qApp->screens()[0]->size().width(), qApp->screens()[0]->size().height()),
        QPointF(0, qApp->screens()[0]->size().height()),
        QPointF(0, 0)
    });
    AddPolygon(polygon);
}

QPointF Controller::GetLightSource() {
    return light_source_;
}

void Controller::SetLightSource(QPointF lightSource) {
    light_source_ = lightSource;
}

const std::vector<Polygon> &Controller::GetPolygons() {
    return polygons_;
}

void Controller::AddPolygon(const Polygon &polygon) {
    polygons_.push_back(polygon);
}

void Controller::AddVertexToLastPolygon(const QPointF &new_vertex) {
    Polygon &last_polygon = polygons_[polygons_.size() - 1];
    last_polygon.AddVertex(new_vertex);
}

// TODO
void Controller::UpdateLastPolygon(const QPointF &new_vertex) {
    // auto& last_polygon = current_polygon_.;
    // last_polygon = new_vertex;
}

std::vector<Ray> Controller::CastRays(QPointF light_source) {
    std::vector<Ray> rays;
    for (auto polygon: polygons_) {
        std::vector<QPointF> vertices = polygon.GetVertices();
        for (QPointF &vertex: vertices) {
            // count angle
            double angle = AngleBetweenPoints(vertex, light_source);

            // create rays
            Ray ray1 = Ray(light_source, vertex, angle);
            rays.push_back(ray1);
            // TODO fix logic with extra rays
            // rays.push_back(FindNeighbourRay(angle + compare_value));
            // rays.push_back(FindNeighbourRay(angle - compare_value));
        }
    }
    return rays;
}

void Controller::IntersectRays(std::vector<Ray> *rays, QPointF light_source) {
    for (auto polygon: polygons_) {
        for (auto &ray: *rays) {
            auto intersection_result = polygon.IntersectRay(ray);
            if (intersection_result != std::nullopt) {
                if (Polygon::GetDistance(ray.GetBegin(), intersection_result.value()) <
                    Polygon::GetDistance(ray.GetBegin(), ray.GetEnd())) {
                    double angle1 = AngleBetweenPoints(intersection_result.value(), light_source);
                    double angle2 = AngleBetweenPoints(ray.GetEnd(), light_source);

                    if (CompareAngles(angle1, angle2)) {
                        ray.SetEnd(intersection_result.value());
                    }
                }
            }
        }
    }
}

void Controller::RemoveAdjacentRays(std::vector<Ray> *rays) {
}

Polygon Controller::CreateLightArea(QPointF light_source) {
    std::vector<Ray> rays = CastRays(light_source);
    IntersectRays(&rays, light_source);
    // RemoveAdjacentRays(&rays);

    std::sort(rays.begin(), rays.end(), [](const Ray &ray1, const Ray &ray2) {
        return ray1.GetAngle() < ray2.GetAngle();
    });

    std::vector<QPointF> vertices;
    for (auto ray: rays) {
        vertices.push_back(ray.GetEnd());
    }
    vertices.push_back(vertices[0]);
    return Polygon(vertices);
}


void Controller::ModeChanged() {

    if (mode_box_->currentIndex() == LIGHT) {

        if (!is_right_clicked) {
            AddVertexToLastPolygon(polygons_[polygons_.size() - 1].GetVertices()[0]);
            is_right_clicked = true;
        }
        // create a small red circle cursor
        QPixmap pixmap(30, 30);
        pixmap.fill(Qt::transparent);

        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::red);
        painter.setBrush(Qt::red);
        painter.drawEllipse(15, 15, 5, 5);

        double radius = 10.0;
        for (int i = 0; i < 10; ++i) {
            // Вычисляем позицию дополнительного источника
            double angle = 2 * M_PI * i / 10;
            painter.drawEllipse(15 + radius * cos(angle), 15 + radius * sin(angle), 5, 5);
        }

        // Set the cursor with hotspot at center
        QCursor customCursor(pixmap, 15, 15);
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
    if (mode_ == LIGHT) {
        light_source_ = pos;

        // Очищаем предыдущие лучи
        paint_widget_->ClearRaysBuffer();

        // Рисуем основной источник света
        Polygon main_polygon = CreateLightArea(light_source_);
        paint_widget_->PaintRays(main_polygon, QColor(255, 255, 255));

        // Рисуем 10 дополнительных источников вокруг основного
        const int additional_lights = 10;
        const double radius = 7.0; // радиус круга, по которому расположены дополнительные источники
        double color_id = 235;
        for (int i = 0; i < additional_lights; ++i) {
            // Вычисляем позицию дополнительного источника
            double angle = 2 * M_PI * i / additional_lights;
            QPointF offset(radius * cos(angle), radius * sin(angle));
            QPointF secondary_light = light_source_ + offset;

            // Создаем и рисуем область света для дополнительного источника
            Polygon secondary_polygon = CreateLightArea(secondary_light);
            paint_widget_->PaintRays(secondary_polygon, QColor(color_id, color_id, color_id));
            // paint_widget_->PaintSecondaryLightSource(secondary_light);
            color_id -= 20;
        }
    }
}

void Controller::MouseLeftClicked(QPointF pos) {
    if (mode_ == POLYGONS) {
        if (is_right_clicked) {
            Polygon polygon;
            AddPolygon(polygon);
            is_right_clicked = false;
        }
        AddVertexToLastPolygon(pos);
        paint_widget_->PaintPolygon(polygons_[polygons_.size() - 1]);
    }
}

void Controller::MouseRightClicked(QPointF pos) {
    if (mode_ == POLYGONS) {
        AddVertexToLastPolygon(polygons_[polygons_.size() - 1].GetVertices()[0]);
        is_right_clicked = true;
    }
}

double Controller::AngleBetweenPoints(const QPointF &p1, const QPointF &p2) {
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();

    return qAtan2(dy, dx);
}

bool Controller::CompareAngles(double angle1, double angle2) {
    double diff = qAbs(angle1 - angle2);
    diff = qMin(diff, 6.28 - diff);
    return diff <= compare_value;
}

Ray Controller::FindNeighbourRay(double angle_radians) {
    QPointF direction(qCos(angle_radians), qSin(angle_radians));

    // Get the boundaries of the drawing area
    const double left = 0;
    const double right = width();
    const double top = 0;
    const double bottom = height();

    // Variables to store potential intersection points and their distances
    QPointF closest_intersection;
    double min_distance = std::numeric_limits<double>::max();
    bool found_intersection = false;

    // Calculate intersection parameters for each boundary
    // Right boundary (x = right)
    if (direction.x() > compare_value) {
        // Moving right
        double t = (right - light_source_.x()) / direction.x();
        if (t > 0) {
            double y = light_source_.y() + t * direction.y();
            if (y >= top && y <= bottom) {
                QPointF intersection(right, y);
                double dist = Polygon::GetDistance(light_source_, intersection);
                if (dist < min_distance) {
                    min_distance = dist;
                    closest_intersection = intersection;
                    found_intersection = true;
                }
            }
        }
    }
    // Left boundary (x = left)
    else if (direction.x() < -compare_value) {
        // Moving left
        double t = (left - light_source_.x()) / direction.x();
        if (t > 0) {
            double y = light_source_.y() + t * direction.y();
            if (y >= top && y <= bottom) {
                QPointF intersection(left, y);
                double dist = Polygon::GetDistance(light_source_, intersection);
                if (dist < min_distance) {
                    min_distance = dist;
                    closest_intersection = intersection;
                    found_intersection = true;
                }
            }
        }
    }

    // Top boundary (y = top)
    if (direction.y() < -compare_value) {
        // Moving up
        double t = (top - light_source_.y()) / direction.y();
        if (t > 0) {
            double x = light_source_.x() + t * direction.x();
            if (x >= left && x <= right) {
                QPointF intersection(x, top);
                double dist = Polygon::GetDistance(light_source_, intersection);
                if (dist < min_distance) {
                    min_distance = dist;
                    closest_intersection = intersection;
                    found_intersection = true;
                }
            }
        }
    }
    // Bottom boundary (y = bottom)
    else if (direction.y() > compare_value) {
        // Moving down
        double t = (bottom - light_source_.y()) / direction.y();
        if (t > 0) {
            double x = light_source_.x() + t * direction.x();
            if (x >= left && x <= right) {
                QPointF intersection(x, bottom);
                double dist = Polygon::GetDistance(light_source_, intersection);
                if (dist < min_distance) {
                    min_distance = dist;
                    closest_intersection = intersection;
                    found_intersection = true;
                }
            }
        }
    }

    if (!found_intersection) {
        // If no intersections found (shouldn't happen for valid angles)
        // Return a ray with a large length
        return Ray(light_source_,
                   light_source_ + direction * qMax(width(), height()) * 2,
                   angle_radians);
    }

    return Ray(light_source_, closest_intersection, angle_radians);
}


Controller::~Controller() {
}

void Controller::CreateActions() {
    connect(mode_box_, &QComboBox::currentIndexChanged, this, &Controller::ModeChanged);
    connect(paint_widget_, &PaintWidget::mouseMoved, this, &Controller::MouseMoveEvent);
    connect(paint_widget_, &PaintWidget::leftClicked, this, &Controller::MouseLeftClicked);
    connect(paint_widget_, &PaintWidget::rightClicked, this, &Controller::MouseRightClicked);
}
