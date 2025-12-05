#ifndef KUIPER_INCLUDE_BASE_BUFFER_H_
#define KUIPER_INCLUDE_BASE_BUFFER_H_
#include <memory>
#include "base/alloc.h"
namespace base {

// std::enable_shared_from_this<T> 是 C++ 中一个非常常用也非常重要的工具类，用于让一个对象在成员函数内部安全地获得自己的 std::shared_ptr，
//（因为从裸指针创建 shared_ptr，比如 std::shared_ptr<T>(raw_ptr)，一定会创建新的控制块，也就是引用计数为 1，如果两次从同一个 raw_ptr 创建 share_ptr，这两个计数都为 1 的 share_ptr 在生命周期结束时都会触发一次对 raw_ptr 的释放操作，即二次释放）
class Buffer : public NoCopyable, std::enable_shared_from_this<Buffer> {
 private:
  size_t byte_size_ = 0;
  void* ptr_ = nullptr;
  bool use_external_ = false;  // 是否拥有这块数据的所有权，即是否需要在 ptr 没有被使用了的时候释放该内存
  DeviceType device_type_ = DeviceType::kDeviceUnknown;
  std::shared_ptr<DeviceAllocator> allocator_;

 public:
  explicit Buffer() = default;

  // 这里写了默认参数，所以后面三个参数是可选的；默认参数只能写在函数声明中（也就是头文件）
  explicit Buffer(size_t byte_size, std::shared_ptr<DeviceAllocator> allocator = nullptr,
                  void* ptr = nullptr, bool use_external = false);

  virtual ~Buffer();  // 析构函数

  bool allocate();

  void copy_from(const Buffer& buffer) const;

  void copy_from(const Buffer* buffer) const;

  void* ptr();

  const void* ptr() const;

  size_t byte_size() const;

  std::shared_ptr<DeviceAllocator> allocator() const;

  DeviceType device_type() const;

  void set_device_type(DeviceType device_type);

  std::shared_ptr<Buffer> get_shared_from_this();

  bool is_external() const;
};
}  // namespace base

#endif