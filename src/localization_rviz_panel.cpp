#include "localization_rviz_panel.hpp"

#include <QSizePolicy>
#include <rviz_common/display_context.hpp>

namespace localization_rviz_panel
{

LocalizationRvizPanel::LocalizationRvizPanel(QWidget * parent) : rviz_common::Panel(parent)
{
  nvtl_label_ = new QLabel;
  tp_label_ = new QLabel;
  score_label_ = new QLabel;

  std::string style;
  style += "color: #303030; ";
  style += "background-color: #00FFFF; ";
  style += "font-family: arial; ";
  style += "font-size: 24px; ";
  nvtl_label_->setStyleSheet(QString::fromStdString("QLabel {" + style + "}"));
  tp_label_->setStyleSheet(QString::fromStdString("QLabel {" + style + "}"));
  score_label_->setStyleSheet(QString::fromStdString("QLabel {" + style + "}"));
  nvtl_label_->setAlignment(Qt::AlignCenter);
  tp_label_->setAlignment(Qt::AlignCenter);
  score_label_->setAlignment(Qt::AlignCenter);

  std::string title_style;
  title_style += "font-size: 12px; ";
  title_style += "color: #FFFFFF;";
  title_style += "background-color: #303030;";
  QLabel * nvtl_title_label = new QLabel;
  nvtl_title_label->setText(QString::fromStdString("NVTL"));
  nvtl_title_label->setStyleSheet(QString::fromStdString("QLabel {" + title_style + "}"));
  nvtl_title_label->setAlignment(Qt::AlignCenter);
  QLabel * tp_title_label = new QLabel;
  tp_title_label->setText(QString::fromStdString("TP"));
  tp_title_label->setStyleSheet(QString::fromStdString("QLabel {" + title_style + "}"));
  tp_title_label->setAlignment(Qt::AlignCenter);
  QLabel * score_title_label = new QLabel;
  score_title_label->setText(QString::fromStdString("RangeImageScore"));
  score_title_label->setStyleSheet(QString::fromStdString("QLabel {" + title_style + "}"));
  score_title_label->setAlignment(Qt::AlignCenter);

  QVBoxLayout * box_layout_nvtl = new QVBoxLayout(parent);
  box_layout_nvtl->addWidget(nvtl_title_label);
  box_layout_nvtl->addWidget(nvtl_label_);
  box_layout_nvtl->setSpacing(0);
  QVBoxLayout * box_layout_tp = new QVBoxLayout(parent);
  box_layout_tp->addWidget(tp_title_label);
  box_layout_tp->addWidget(tp_label_);
  box_layout_tp->setSpacing(0);
  QVBoxLayout * box_layout_score = new QVBoxLayout(parent);
  box_layout_score->addWidget(score_title_label);
  box_layout_score->addWidget(score_label_);
  box_layout_score->setSpacing(0);

  QGridLayout * grid = new QGridLayout;
  grid->addLayout(box_layout_nvtl, 1, 0);
  grid->addLayout(box_layout_tp, 1, 1);
  grid->addLayout(box_layout_score, 1, 2);
  QGridLayout * layout = new QGridLayout();
  QWidget * widget = new QWidget();
  widget->setLayout(grid);
  layout->addWidget(widget, 0, 0);
  setLayout(layout);
}

LocalizationRvizPanel::~LocalizationRvizPanel() {}

void LocalizationRvizPanel::onInitialize()
{
  node_ = this->getDisplayContext()->getRosNodeAbstraction().lock()->get_raw_node();

  nvtl_subscriber_ = node_->create_subscription<tier4_debug_msgs::msg::Float32Stamped>(
    "/localization/pose_estimator/nearest_voxel_transformation_likelihood", 5,
    std::bind(&LocalizationRvizPanel::callbackNVTL, this, std::placeholders::_1));
  tp_subscriber_ = node_->create_subscription<tier4_debug_msgs::msg::Float32Stamped>(
    "/localization/pose_estimator/transform_probability", 5,
    std::bind(&LocalizationRvizPanel::callbackTP, this, std::placeholders::_1));
  range_image_score_subscriber_ = node_->create_subscription<tier4_debug_msgs::msg::Float32Stamped>(
    "score", 5, std::bind(&LocalizationRvizPanel::callbackScore, this, std::placeholders::_1));
}

void LocalizationRvizPanel::callbackNVTL(const tier4_debug_msgs::msg::Float32Stamped::SharedPtr msg)
{
  nvtl_label_->setText(QString::number(msg->data, 'f', 3));
  std::string style;
  style += "font-family: arial; ";
  style += "font-size: 24px; ";
  if (msg->data < 2.3) {
    style += "background-color: #FF3333; ";
  } else {
    style += "background-color: #00FFFF; ";
  }
  nvtl_label_->setStyleSheet(QString::fromStdString("QLabel {" + style + "}"));
}

void LocalizationRvizPanel::callbackTP(const tier4_debug_msgs::msg::Float32Stamped::SharedPtr msg)
{
  tp_label_->setText(QString::number(msg->data, 'f', 3));
  std::string style;
  style += "font-family: arial; ";
  style += "font-size: 24px; ";
  if (msg->data < 3.0) {
    style += "background-color: #FF3333; ";
  } else {
    style += "background-color: #00FFFF; ";
  }
  tp_label_->setStyleSheet(QString::fromStdString("QLabel {" + style + "}"));
}

void LocalizationRvizPanel::callbackScore(
  const tier4_debug_msgs::msg::Float32Stamped::SharedPtr msg)
{
  score_label_->setText(QString::number(msg->data, 'f', 1));
}

}  // namespace localization_rviz_panel

#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(localization_rviz_panel::LocalizationRvizPanel, rviz_common::Panel)
