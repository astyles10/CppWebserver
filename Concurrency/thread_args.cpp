#include <functional>
#include <iostream>
#include <thread>

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

void update_unique_widget(int inId, std::unique_ptr<widget> inWidget) {
  inWidget->id = inId;
  inWidget->data = 99999;
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
  const auto& aBoundFunction =
      std::bind(&widget::update_data, &aWidget, std::placeholders::_1);
  aBoundFunction(420);
  std::thread aThread(&widget::update_data, &aWidget, 737373);
  aThread.join();
  std::printf("Widget ptr = %p, data = %d\n", &aWidget, aWidget.data);

  // Like unique_ptr owning a dynamic object, threads own a resource in a
  // single thread of execution
  // Threads are move only (cannot be copied)
  std::unique_ptr<widget> aUniqueWidget(new widget);
  std::thread aAnotherThread(update_unique_widget, 101,
                             std::move(aUniqueWidget));
  aAnotherThread.join();
  if (aUniqueWidget.get() == nullptr) {
    std::cout << "aUniqueWidget passed ownership to thread!\n";
  }
  return 0;
}