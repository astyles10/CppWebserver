#include <iostream>
#include <thread>
#include <functional>

class widget {
 public:
  widget() = default;
  ~widget() = default;
  void update_data(int inData) {
    std::printf("Called update_data for object %p with data %d\n", this,
                inData);
    data = inData;
  }

  int id;
  int data;
};

void update_widget_data(int inId, widget& inWidget) {
  inWidget.data = 69420;
  inWidget.id = inId;
}

widget oops_widget(int id) {
  widget aWidget;
  // Can't compile without using std::ref for widget reference parameter
  std::thread aThread(update_widget_data, id, std::ref(aWidget));
  aThread.join();
  return aWidget;
}

int main() {
  widget aWidget = oops_widget(100);
  std::printf("Widget id = %d, data = %d\n", aWidget.id, aWidget.data);

  // std::thread constructor and std::bind use same mechanism to call a
  // member function with parameters
  const auto& aBoundFunction = std::bind(&widget::update_data, &aWidget, std::placeholders::_1);
  aBoundFunction(420);
  std::thread aThread(&widget::update_data, &aWidget, 737373);
  aThread.join();
  std::printf("Widget ptr = %p, data = %d\n", &aWidget, aWidget.data);
  return 0;
}