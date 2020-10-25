/*
 * This file is part of the ux_gui_stream distribution
 * (https://github.com/amatarazzo777/ux_gui_stream).
 * Copyright (c) 2020 Anthony Matarazzo.
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
@author Anthony Matarazzo
@file ux_os_window_manager_base.h
@date 10/15/20
@version 1.0
@brief
*/

namespace uxdevice {
template <typename T> class message_queue_t {
public:
  message_queue_t() {
    /// @brief start message monitor and queued block dispatch.
    std::thread message_queue_thread([=]() {
      bProcessing = true;
      message_loop();
    });

    message_queue_thread.detach();

    /// @brief start queue dispatcher. THis uses a condition variable while
    /// blocks are input. messages are processed here serially and filtered.
    std::thread event_queue_thread([=]() {
      bProcessing = true;
      event_queue_processor();
    });

    event_queue_thread.detach();
  }

  ~message_queue_t() { bProcessing = false; }

  /**
   * @internal
   * @fn typename T fn_poll_message(void)=0
   * @brief The fn_message_wait routine should invoke the message wait function
   * of the os. This function will not return until a message has been received.
   * It returns that message.
   *
   */
  virtual T fn_message_wait(void) = 0;

  /**
   * @internal
   * @fn typename T fn_poll_message(void)=0
   * @brief The poll_message routine should poll the os message queue and return
   * it. This function is template inherited from window_interface which is also
   * a friend class.
   *
   */
  virtual T fn_poll_message(void) = 0;

  /**
   * @internal
   * @fn void complete_message(void)=0
   * @brief The message provides a completion callback before the system
   * disposes of resources. On Unix these messages coming from the system
   * dispatch and poll routines are pointers and have to be freed. The windows
   * implementation may use another type such as struct. This function is
   * template inherited from window_interface which is also a friend class.
   *
   */
  virtual void fn_complete_message(T msg) = 0;

  /**
   * @internal
   * @fn fn_visit_dispatch
   * @brief the translation function which takes the typically size_t integer
   * type "a message constant id such as XCB_EXPOSE", and converts it into a
   * class type. The classes used within the visit dispatch may hold anything.
   * However they have to be in classes for the variant visit. std::variant type
   * erasure prevents the base xcb system pointer types from being unique within
   * the variant structure. A number of them, while being names appropriately,
   * derive their type using typedef alias of another pointer type. Because
   * essentially the data usage of the structure is the same for both.
   *
   * So this system implements the ptr_type_class_alias class to provide a type
   * for each pointer cast used for messages. This provides a list for the
   * variant visitor which is a change from the switch/case usually in message
   * handlers.
   *
   * This function is template inherited from window_interface which
   * is also a friend class.
   *
   */
  virtual void fn_visit_dispatch(T msg) = 0;

  /**
   * @internal
   * @fn message_loop
   * @brief the routine handles the message processing for the specific
   * operating system. The function is called from processEvents.
   * @details The entry point that processes messages from the operating
   * system application level services. Typically on Linux this is a coupling
   * of xcb and keysyms library for keyboard. Previous incarnations of
   * technology such as this typically used xserver. However, XCB is the newer
   * form. Primarily looking at the code of such programs as vlc, the routine
   * simply places pixels into the memory buffer. while on windows the direct
   * x library is used in combination with windows message queue processing.
   */
  void message_loop(void) {
    T msg;

    /** @brief wait for an event, gather it. as well, gather all messages that
     * may be waiting. Filter duplicate messages that may be received due to
     * continuous input. When all have been gathered, signal the queue processor
     * to translate these messages into uxdevice versions and dispatch them.
     * some priorities in queue processing go to window services to resize
     * surface first and output the new area background. */
    while (bProcessing && (msg = fn_message_wait())) {
      {
        std::lock_guard lock(event_queue_mutex);
        event_queue.emplace_back(msg);

        // qt5 does this, it queues all of the input messages at once.
        // this makes the processing of painting and reading input faster.
        while (bProcessing && (msg = fn_poll_message()))
          event_queue.emplace_back(msg);
      }

      event_queue_condition_variable.notify_one();
    }
  }

  /**
   * @fn void event_queue_processor(void)
   * @brief thread dispatches queued messages from from window manager. This
   * thread processes these filter message when a block is received. Perhaps
   * one, or more. At times the processing style saves and reflects screen
   * changes quicker due to the condition variable and queued approach.
   *
   */
  void event_queue_processor(void) {
    while (bProcessing) {
      bool bdone = {};

      // wait until messages are queued and the signal is received.
      std::unique_lock<std::mutex> lk(event_queue_condition_mutex);
      event_queue_condition_variable.wait(lk);
      lk.unlock();

      {
        std::lock_guard lock(event_queue_mutex);
        if (event_queue.empty())
          bdone = true;
      }

      T msg = {};

      while (!bdone) {
        // get message
        {
          std::lock_guard lock(event_queue_mutex);
          msg = event_queue.front();
          event_queue.pop_front();
        }

        // invoke the search
        fn_visit_dispatch(msg);

        // free is here - is odd, pointer should be managed at allocation level
        fn_complete_message(msg);

        // check for next iteration.
        {
          std::lock_guard lock(event_queue_mutex);
          if (event_queue.empty())
            bdone = true;
          else if (!bProcessing)
            bdone = true;
        }
      }
    }
  }

  std::atomic<bool> bProcessing = false;
  std::mutex event_queue_mutex = {};
  std::list<T> event_queue = {};
  std::mutex event_queue_condition_mutex = {};
  std::condition_variable event_queue_condition_variable = {};
};

} // namespace uxdevice
