
#include <base/std_base.h>
#include <base/inc/object/layer/pipeline_memory_object.h>

uxdevice::pipeline_memory_object_t() {}

/**
 * @fn  pipeline_memory_object_t(std::any, std::size_t, const
 * accepted_interfaces_storage_t*, const hash_function_t=={})
 * @brief constructor for objects.
 *
 * @param _o
 * @param _bits
 * @param _accepted_interfaces
 * @param _hash_function
 */
uxdevice::pipeline_memory_object_t(
    std::any _o, std::size_t _bits,
    accepted_interfaces_storage_t *_accepted_interfaces,
    const hash_function_t _hash_function)
    : object(_o), visitor_target_bits(_bits),
      accept_interfaces(_accepted_interfaces), hash_function(_hash_function) {}

/// @brief copy assignment operator
uxdevice::pipeline_memory_object_t &
operator=(const pipeline_memory_object_t &other) {
  return *this;
}

/// @brief copy assignment operator
uxdevice::pipeline_memory_object_t &
uxdevice::pipeline_memory_object_t::operator=(
    const pipeline_memory_object_t &other) {
  return *this;
}

/// @brief move assignment
uxdevice::pipeline_memory_object_t &
uxdevice::pipeline_memory_object_t::operator=(
    pipeline_memory_object_t &&other) noexcept {
  return *this;
}

/// @brief move constructor
uxdevice::pipeline_memory_object_t::pipeline_memory_object_t(
    pipeline_memory_object_t &&other) noexcept {}

/// @brief copy constructor
uxdevice::pipeline_memory_object_t::pipeline_memory_object_t(
    const pipeline_memory_object_t &other) {}

/** @brief [] operator reduces syntax*/
uxdevice::visitor_interface_t *
pipeline_memory_object_t::operator[](std::type_index ti) noexcept {
  visitor_interface_t *ret = {};
  if (accept_interfaces) {
    auto n = accept_interfaces->find(ti);
    if (n != accept_interfaces->end())
      ret = n->second;
  }
  return ret;
}
