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
    current_polygon_.AddVertex(new_vertex);
}

void Controller::UpdateLastPolygon(const QPoint &new_vertex) {
    auto& last_polygon = polygons_.back().GetVertices().back();
    last_polygon = new_vertex;
}


void Controller::IntersectRays(std::vector<Ray> *rays) {
    for (auto polygon: polygons_) {
        for (auto& ray: *rays) {
            auto intersect_result = polygon.IntersectRay(ray);
            if (Polygon::GetDistance(ray.GetBegin(), intersect_result.value()) <
                Polygon::GetDistance(ray.GetBegin(), ray.GetEnd())) {
                ray.SetEnd(intersect_result.value());
            }
        }
    }
}

void Controller::RemoveAdjacentRays(std::vector<Ray> *rays) {

}

// Polygon Controller::CreateLightArea() {
//
// }

std::vector<Ray> Controller::CastRays() {
    std::vector<Ray> rays;
    for (auto polygon: polygons_) {
        std::vector<QPoint> vertices = polygon.GetVertices();
        for (QPoint& vertex: vertices) {
            // count angle
            double angle = qAtan2(vertex.y() - light_source_.y(),
                     vertex.x() - light_source_.x());

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
        // Restore default cursor
        setCursor(Qt::ArrowCursor);
        mode_ = POLYGONS;
    }
}

void Controller::MouseMoveEvent(QPoint pos) {
    if (mode_ == LIGHT) {
        light_source_ = pos;
        paint_widget_->update();
    }
}

void Controller::MouseLeftClicked(QPoint pos) {
    if (mode_ == POLYGONS) {
        std::cout << pos.x() << " " << pos.y() << std::endl;
        if (current_polygon_.IsEmpty()) {
            first_vertex_ = pos;
        }
        AddVertexToLastPolygon(pos);
        paint_widget_->SetVertex(pos, false);
        paint_widget_->update();
    }
}

void Controller::MouseRightClicked(QPoint pos) {
    if (mode_ == POLYGONS) {

        paint_widget_->SetVertex(first_vertex_, true);
        paint_widget_->update();

        // add polygon ...
        AddPolygon(current_polygon_);
        current_polygon_.Clear();
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
