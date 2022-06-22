#ifndef _LOCALIZATION_RVIZ_PANEL_HPP_
#define _LOCALIZATION_RVIZ_PANEL_HPP_

#include <QLabel>
#include <QVBoxLayout>
#include <QtWidgets>
#include <rclcpp/rclcpp.hpp>
#include <rviz_common/display_group.hpp>
#include <rviz_common/panel.hpp>

#include <tier4_debug_msgs/msg/float32_stamped.hpp>

namespace localization_rviz_panel
{

class LocalizationRvizPanel : public rviz_common::Panel
{
  Q_OBJECT

public:
  explicit LocalizationRvizPanel(QWidget * parent = 0);
  virtual ~LocalizationRvizPanel();

  void onInitialize() override;

private
  Q_SLOT : void set_rate(double value);
  void send_rate();
  void play_toggle();

private:
  void callbackNVTL(const tier4_debug_msgs::msg::Float32Stamped::SharedPtr msg);
  void callbackTP(const tier4_debug_msgs::msg::Float32Stamped::SharedPtr msg);
  void callbackScore(const tier4_debug_msgs::msg::Float32Stamped::SharedPtr msg);

  rclcpp::Node::SharedPtr node_;
  rclcpp::Subscription<tier4_debug_msgs::msg::Float32Stamped>::SharedPtr nvtl_subscriber_;
  rclcpp::Subscription<tier4_debug_msgs::msg::Float32Stamped>::SharedPtr tp_subscriber_;
  rclcpp::Subscription<tier4_debug_msgs::msg::Float32Stamped>::SharedPtr
    range_image_score_subscriber_;

  QLabel * nvtl_label_;
  QLabel * tp_label_;
  QLabel * score_label_;
};

}  // namespace localization_rviz_panel

#endif
