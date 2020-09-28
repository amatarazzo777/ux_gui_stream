/*
 * This file is part of the PLATFORM_OBJ distribution
 * {https://github.com/amatarazzo777/platform_obj). Copyright (c) 2020 Anthony
 * Matarazzo.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

/**
\author Anthony Matarazzo
\file uxdisplayunitbase.hpp
\date 5/12/20
\version 1.0
\details The file implements all base classes and template class factories
that inform and operate within the rendering system. THe following templates
are for use by the displayunits.hpp.

painter_brush_emitter_t
marker_emitter_t
storage_emitter_t
class_storage_emitter_t
storage_drawing_function_t
class_storage_drawing_function_t

*/
/**
\internal
\typedef indirect_index_display_unit_t as a base member, the key data member
uses this type. All classes can provide an indirect numerical or string key.
The [] operator exposes searching for objects.
 */
namespace uxdevice {
typedef std::variant<std::monostate, std::string, std::size_t>
    indirect_index_storage_t;
}

/**
\internal
\template specializes the std::hash<uxdevice::indirect_index_display_unit_t>
 * std structure for () operator hashing.
 */
template <> struct std::hash<uxdevice::indirect_index_storage_t> {
  std::size_t
  operator()(uxdevice::indirect_index_storage_t const &o) const noexcept {
    size_t value = o.index();
    uxdevice::hash_combine(value, std::type_index(typeid(o)).hash_code());
    if (auto pval = std::get_if<std::string>(&o))
      uxdevice::hash_combine(value, *pval);
    else if (auto pval = std::get_if<std::size_t>(&o))
      uxdevice::hash_combine(value, *pval);
    return value;
  }
};

/// \typedef cairo_function_t
/// @brief holds a call to a cairo function with parameters bound.
/// The cairo context - cr is provided later.
namespace uxdevice {
typedef std::function<void(cairo_t *cr)> cairo_function_t;
using cairo_function_t = cairo_function_t;

} // namespace uxdevice
template <> struct std::hash<uxdevice::cairo_function_t> {
  std::size_t operator()(uxdevice::cairo_function_t const &o) const noexcept {
    return reinterpret_cast<std::size_t>(std::addressof(o));
  }
};

/**
\internal
\class display_unit_t
\brief

\brief base class for all display units. defaulted
is the is_output function. Drawing object should override
this and return true. This enables the checking of the surface
for errors after invocation.

*/
namespace uxdevice {
class display_unit_t : virtual public hash_members_t,
                       virtual public system_error_t {
public:
  display_unit_t() {}

  /// @brief copy assignment operator
  display_unit_t &operator=(const display_unit_t &other) {
    is_processed = other.is_processed;
    viewport_inked = other.viewport_inked;
    return *this;
  }
  /// @brief move assignment
  display_unit_t &operator=(display_unit_t &&other) noexcept {
    is_processed = std::move(other.is_processed);
    viewport_inked = std::move(other.viewport_inked);
    return *this;
  }

  /// @brief move constructor
  display_unit_t(display_unit_t &&other) noexcept
      : is_processed(std::move(other.is_processed)),
        viewport_inked(std::move(other.viewport_inked)) {}

  /// @brief copy constructor
  display_unit_t(const display_unit_t &other)
      : is_processed(other.is_processed), viewport_inked(other.viewport_inked) {
  }

  virtual ~display_unit_t() {}

  virtual bool is_output(void) { return false; }
  void changed(void) { bchanged = true; }
  bool has_changed(void) { return is_different_hash(); }

  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(display_unit_t)), is_processed,
                 viewport_inked, bchanged);
    return __value;
  }

  bool is_processed = false;
  bool viewport_inked = false;
  bool bchanged = false;
};
} // namespace uxdevice

/**
\internal

\class typed_index_t

\tparam T the type of interface reference to return from the index public member
function.

\brief A class which is inherited which manages the index key
    data.The operators,
    copy,
    move constructors are invoked by the class deriving : display_unit_t.
*/

namespace uxdevice {
class key_storage_t {
public:
  key_storage_t() {}
  virtual ~key_storage_t() {}
  key_storage_t(const indirect_index_storage_t &_key) : key(_key) {}
  /// @brief copy assignment operator
  key_storage_t &operator=(const key_storage_t &other) {
    key = other.key;
    return *this;
  }
  /// @brief move assignment
  key_storage_t &operator=(key_storage_t &&other) noexcept {
    key = std::move(other.key);
    return *this;
  }
  /// @brief move constructor
  key_storage_t(key_storage_t &&other) noexcept { key = std::move(other.key); }

  /// @brief copy constructor
  key_storage_t(const key_storage_t &other) { *this = other; }

  indirect_index_storage_t key = {};
};

template <typename T>
class typed_index_t : public key_storage_t, virtual public hash_members_t {
public:
  /// @brief default constructor
  typed_index_t() : key_storage_t{} {}

  /// @brief copy assignment operator
  typed_index_t &operator=(const typed_index_t &other) {
    key_storage_t::operator=(other);
    return *this;
  }
  /// @brief move assignment
  typed_index_t &operator=(typed_index_t &&other) noexcept {
    key_storage_t::operator=(other);
    return *this;
  }
  /// @brief move constructor
  typed_index_t(typed_index_t &&other) noexcept : key_storage_t(other) {}

  /// @brief copy constructor
  typed_index_t(const typed_index_t &other) : key_storage_t(other) {}

  T &index(const std::string &_k) {
    key_storage_t::operator=(indirect_index_storage_t(_k));
    return *static_cast<T *>(this);
  }
  T &index(const std::size_t &_k) {
    key_storage_t::operator=(indirect_index_storage_t(_k));
    return *static_cast<T *>(this);
  }
  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(display_visual_t)), key);
    return __value;
  }
  virtual ~typed_index_t() {}
};
} // namespace uxdevice



/**
\internal
\class painter_brush_emitter_t
\tparam T used to note and enable the typed index interface which allows
continuation syntax when objects are created and an indirect index is applied.
Such as: text_font_t{"arial 20px"}.index("idxfont")
Here the .index property returns a strong type of the original.


\tparam Args... provides the ability to catch specific functions and their
prototypes. emit. Attributes may also be listed that apply to specific
functionality of the system such as labeling items as textual rendering
related. This is useful when applying attributes that comprise a rendering
function.


\brief creates a painter brush object that is also a display unit.
class inherits publicly display_unit_t and painter_brush_t. The
painter_brush_t constructor interface is also inherited which allows
multiple types of brushes to be defined in several formats,

*/

namespace uxdevice {
template <typename T, typename... Args>
class painter_brush_emitter_t : public display_unit_t,
                                public painter_brush_t,
                                virtual public hash_members_t,
                                public typed_index_t<T>,
                                std::enable_shared_from_this<T>,
                                public Args... {

public:
  using painter_brush_t::painter_brush_t;
  painter_brush_emitter_t(){}

  // copy constructor
  painter_brush_emitter_t(const painter_brush_emitter_t &other)
      : display_unit_t(other),
        painter_brush_t(other), typed_index_t<T>::typed_index_t(other) {}
  // move constructor
  painter_brush_emitter_t(painter_brush_emitter_t &&other) noexcept
      : display_unit_t(other),
        painter_brush_t(other), typed_index_t<T>::typed_index_t(other) {}

  // copy assignment operator
  painter_brush_emitter_t &operator=(const painter_brush_emitter_t &other) {
    display_unit_t::operator=(other);
    typed_index_t<T>::operator=(other);
    hash_members_t::operator=(other);
    painter_brush_t::operator=(other);
    return *this;
  }
  // move assignment operator
  painter_brush_emitter_t &operator=(painter_brush_emitter_t &&other) noexcept {
    display_unit_t::operator=(other);
    typed_index_t<T>::operator=(other);
    hash_members_t::operator=(other);
    painter_brush_t::operator=(other);
    return *this;
  }

  virtual ~painter_brush_emitter_t() {}

  void emit(display_context_t *context) { painter_brush_t::emit(context->cr); }
  void emit(cairo_t *cr) { painter_brush_t::emit(cr); }
  void emit(cairo_t *cr, coordinate_t *a) { painter_brush_t::emit(cr, a); }

  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, std::type_index(typeid(T)),
                 painter_brush_t::hash_code(), display_unit_t::hash_code(),
                 lineWidth, radius, x, y);

    return __value;
  }

  double lineWidth = 1;
  unsigned short radius = 3;
  double x = 1, y = 1;
};
} // namespace uxdevice

/**
\internal
\class marker_emitter_t

\tparam T - the name the display unit should assume.

\tparam Args... - lists applicable invoke and emit functions.
Depending on the particular display unit type, context, pango or cairo
emit functions may be included from the abstract base functions. When
these are listed, the interface callback must be defined.

\brief declares a class that marks a unit as an emitter but does not store a
value. This is useful for switch and state logic. When the item is present, the
emit method is called. class inherits publicly display_unit_t

*/
namespace uxdevice {
template <typename T, typename... Args>
class marker_emitter_t : public display_unit_t,
                         virtual public hash_members_t,
                         public typed_index_t<T>,
                         public Args... {
public:
  marker_emitter_t() {}

  marker_emitter_t &operator=(marker_emitter_t &&other) noexcept {
    display_unit_t::operator=(other);
    return *this;
  }

  marker_emitter_t &operator=(const marker_emitter_t &other) {
    display_unit_t::operator=(other);
    return *this;
  }
  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, typed_index_t<T>::hash_code(),
                 std::type_index(typeid(T)).hash_code(),
                 display_unit_t::hash_code());

    return __value;
  }

  marker_emitter_t(marker_emitter_t &&other) noexcept : display_unit_t(other) {}
  marker_emitter_t(const marker_emitter_t &other) : display_unit_t(other) {}

  virtual ~marker_emitter_t() {}
};
} // namespace uxdevice

/**
\internal

\class storage_emitter_t

\tparam T - the name the display unit should assume.

\tparam TS - the storage class or trivial type.

\tparam Args... - lists applicable invoke and emit functions.
Depending on the particular display unit type, context, pango or cairo
emit functions may be included from the abstract base functions. When
these are listed, the interface callback must be defined.

\brief provides the flexibility to store associated trivial data such as enum,
double, etc. The template names this storage area within the object 'value'.
This can be accessed and controlled from the emit function. A default
constructor is established which zero initializes the value. As well,
a constructor accepting the initial value of the given storage  type.

*/
namespace uxdevice {
template <typename T, typename TS, typename... Args>
class storage_emitter_t : virtual public hash_members_t,
                          public display_unit_t,
                          public typed_index_t<T>,
                          std::enable_shared_from_this<T>,
                          public Args... {
public:
  storage_emitter_t() : value(TS{}) {}
  storage_emitter_t(const TS &o) : value(o) {}

  // copy constructor
  storage_emitter_t(const storage_emitter_t &other)
      : hash_members_t(other), display_unit_t(other), typed_index_t<T>(other),
        value(other.value) {}

  // move constructor
  storage_emitter_t(storage_emitter_t &&other) noexcept
      : hash_members_t(other), display_unit_t(other), typed_index_t<T>(other),
        value(other.value) {}

  // copy assignment operator
  storage_emitter_t &operator=(const storage_emitter_t &other) {
    value = other.value;
    display_unit_t::operator=(other);
    typed_index_t<T>::operator=(other);
    hash_members_t::operator=(other);
    return *this;
  }

  // move assignment operator
  storage_emitter_t &operator=(storage_emitter_t &&other) noexcept {
    value = other.other;
    display_unit_t::operator=(other);
    typed_index_t<T>::operator=(other);
    hash_members_t::operator=(other);
    return *this;
  }

  virtual ~storage_emitter_t() {}
  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, typed_index_t<T>::hash_code(),
                 std::type_index(typeid(T)).hash_code(),
                 display_unit_t::hash_code(), value);

    return __value;
  }
  TS value;
};
} // namespace uxdevice

/**
\internal
\class class_storage_emitter_t

\tparam T - the name the display unit should assume.

\tparam TC - the storage class type. The storage type is publicly
inherited and all constructors are also inherited. The class must include
the hash_members_t interface. That is, implement the hash_code()
 member function.

\tparam Args... - inherits the data class interface as public.
  There constructors are inherited as well as the public methods of the class.

\brief provides the flexibility to create classes that store associated data.


*/
namespace uxdevice {
template <typename T, typename TC, typename... Args>
class class_storage_emitter_t : public TC,
                                public typed_index_t<T>,
                                public display_unit_t,
                                virtual public hash_members_t,
                                std::enable_shared_from_this<T>,
                                public Args... {
public:
  using TC::TC;
  class_storage_emitter_t() {}

  // copy constructor
  class_storage_emitter_t(const class_storage_emitter_t &other)
      : hash_members_t(other), TC(other), typed_index_t<T>(other),
        display_unit_t(other) {}

  // move constructor
  class_storage_emitter_t(class_storage_emitter_t &&other) noexcept
      : hash_members_t(other), TC(other), typed_index_t<T>(other),
        display_unit_t(other) {}

  // copy assignment operator
  class_storage_emitter_t &operator=(const class_storage_emitter_t &other) {
    display_unit_t::operator=(other);
    hash_members_t::operator=(other);
    typed_index_t<T>::operator=(other);
    TC::operator=(other);
    return *this;
  }

  // move assignment operator
  class_storage_emitter_t &operator=(class_storage_emitter_t &&other) noexcept {
    display_unit_t::operator=(other);
    hash_members_t::operator=(other);
    typed_index_t<T>::operator=(other);
    TC::opterator = (other);
    return *this;
  }

  virtual ~class_storage_emitter_t() {}

  std::size_t hash_code(void) const noexcept {
    std::size_t __value = {};
    hash_combine(__value, typed_index_t<T>::hash_code(),
                 std::type_index(typeid(T)).hash_code(),
                 display_unit_t::hash_code(), TC::hash_code());

    return __value;
  }
};
} // namespace uxdevice

