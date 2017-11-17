/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_POLYMORPHIC_WRAPPER_HPP
#define IROHA_POLYMORPHIC_WRAPPER_HPP

#include <memory>

namespace shared_model {
  namespace detail {

    /**
     * Wrapper for polymorphic types for correct working with constructors
     * @tparam T - type of wrapped object
     */
    template <class T>
    class PolymorphicWrapper final {
      template <typename>
      friend class PolymorphicWrapper;

     public:
      /// Type of wrapped object
      using WrappedType = T;

      /**
       * Value constructor
       * @param value - pointer for wrapping
       */
      template <typename Y>
      explicit PolymorphicWrapper(const Y *value)
          : ptr_(std::shared_ptr<Y>(value)) {}

      template <typename... Args>
      explicit PolymorphicWrapper(Args &&... args)
          : ptr_(std::make_shared<T>(std::forward<Args>(args)...)) {}

      template <typename Y>
      PolymorphicWrapper(const PolymorphicWrapper<Y> &rhs)
          : ptr_(std::shared_ptr<T>(rhs.ptr_->copy())) {}

      template <typename Y>
      PolymorphicWrapper(PolymorphicWrapper<Y> &&rhs) noexcept
          : ptr_(rhs.ptr_) {
        rhs.ptr_ = nullptr;
      }
      /**
       * Copy constructor that performs deep copy
       * @param rhs - another wrapped value
       */
      PolymorphicWrapper(const PolymorphicWrapper &rhs)
          : ptr_(std::shared_ptr<T>(rhs.ptr_->copy())) {}

      /**
       * Move constructor
       * @param rhs - wrapped temporary value
       */
      PolymorphicWrapper(PolymorphicWrapper &&rhs) noexcept : ptr_(nullptr) {
        std::swap(this->ptr_, rhs.ptr_);
      }

      /**
       * Copy operator=
       * @param rhs - another wrapped value
       * @return *this
       */
      PolymorphicWrapper &operator=(const PolymorphicWrapper &rhs) {
        ptr_ = std::shared_ptr<T>(rhs.ptr_->copy());
        return *this;
      }

      /**
       * Move operator=
       * @param rhs - another temporary wrapped value
       * @return *this
       */
      PolymorphicWrapper &operator=(PolymorphicWrapper &&rhs) noexcept {
        std::swap(this->ptr_, rhs.ptr_);
        return *this;
      }

      /**
       * Checks equality of objects inside
       * @param rhs - other wrapped value
       * @return true, if wrapped objects are same
       */
      bool operator==(const PolymorphicWrapper &rhs) const {
        return *ptr_ == *rhs.ptr_;
      }

      /**
       * Mutable wrapped object pointer
       * @return pointer for wrapped object
       */
      WrappedType *operator->() { return ptr_.get(); }

      /**
       * Immutable wrapped object pointer
       * @return pointer for wrapped object
       */
      const WrappedType *operator->() const { return ptr_.get(); }

     private:
      /// pointer with wrapped value
      std::shared_ptr<WrappedType> ptr_;
    };

  }  // namespace detail
}  // namespace shared_model

#endif  // IROHA_POLYMORPHIC_WRAPPER_HPP
