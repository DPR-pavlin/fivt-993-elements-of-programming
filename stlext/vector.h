#ifndef _STLEXT_VECTOR_H_
#define _STLEXT_VECTOR_H_

#include <iostream>
#include <cassert>

#include <boost/make_shared.hpp>
#include <boost/smart_ptr.hpp>

namespace stlext {

using boost::shared_ptr;
using boost::static_pointer_cast;
using boost::make_shared;

static const size_t ARITY = 2,
                    LOG_ARITY = 1,
                    ARITY_MASK = (1 << LOG_ARITY) - 1;

template<class T>
class vector {
 private:
  struct node_base {};

  struct internal_node : public node_base {
    shared_ptr<const node_base> childs[ARITY];
  };

  struct leaf_node : public node_base {
    shared_ptr<const T> elements[ARITY];
  };

 public:
  class element_reference {
   public:
    operator T() const {
      return (*static_cast<const vector*>(owner_))[index_];
    }

    element_reference& operator= (const T& new_value) {
      owner_->insert(index_, new_value);
      return *this;
    }

   private:
    vector* owner_;
    size_t index_;

    element_reference(vector* owner, size_t index)
        : owner_(owner), index_(index) {}

    friend class vector;
  };

  vector() : size_(0), height_(0), capacity_(0) {}

  void push_back(const T& element) {
    if (++size_ > capacity_) {
      increase_height();
    }

    insert(size_ - 1, element);
  }

  const T& operator[] (size_t index) const {
    assert(index < size_);

    return get_element(index);
  }

  element_reference operator[] (size_t index) {
    assert(index < size_);

    return element_reference(this, index);
  }

  size_t size() const {
    return size_;
  }

 private:
  shared_ptr<const node_base> root_;
  size_t size_, height_, capacity_;

  friend class element_reference;

  void increase_height() {
    if (0 == height_) {
      root_ = make_shared<const leaf_node>();
    } else {
      shared_ptr<internal_node> new_root =
          make_shared<internal_node>();
      new_root->childs[0] = root_;
      root_ = new_root;
    }

    ++height_;

    if (0 != capacity_) {
      capacity_ *= ARITY;
    } else {
      capacity_ = ARITY;
    }
  }

  void trace_tree(const shared_ptr<const node_base>& node,
                  size_t height) const {
    for (int i = 0; i < height_ - height; ++i) {
      std::cerr << "  ";
    }

    if (!node) {
      std::cerr << "null" << std::endl;
      return;
    }

    std::cerr << node << "-> ";

    if (1 == height) {
      const shared_ptr<const leaf_node>& leaf =
          static_pointer_cast<const leaf_node>(node);
      for (size_t i = 0; i < ARITY; ++i) {
        std::cerr << leaf->elements[i];
        if (leaf->elements[i]) {
          std::cerr << "(" << *leaf->elements[i] << ")";
        }

        std::cerr << " ";
      }
    }

    std::cerr << std::endl;

    if (1 != height) {
      const shared_ptr<const internal_node>& internal =
          static_pointer_cast<const internal_node>(node);
      for (size_t i = 0; i < ARITY; ++i) {
        trace_tree(internal->childs[i], height - 1);
      }
    }
  }

  size_t child_index(size_t index, size_t height) const {
//    std::cerr << "[i: " << index << ", h: " << height
//              << ", c: " << ((index >> (height - 1)) & ARITY_MASK)
//              << "]" << std::endl;
    return (index >> (height - 1)) & ARITY_MASK;
  }

  template<class NodeType>
  shared_ptr<NodeType> make_copy(const shared_ptr<const node_base>& node) {
    if (node) {
      return make_shared<NodeType>(
          *static_pointer_cast<const NodeType>(node));
    } else {
      return make_shared<NodeType>();
    }
  }

  void insert(size_t index, const T& new_element) {
    root_ = insert_recur(root_, index, height_,
        make_shared<const T>(new_element));
  }

  const T& get_element(size_t index) const {
    return get_element_recur(root_, index, height_);
  }

  shared_ptr<const node_base> insert_recur(
        const shared_ptr<const node_base>& node,
        size_t index, size_t height,
        const shared_ptr<const T>& new_element) {
//    std::cerr << "insert(..., " << index << ", "
//              << height << ", ...)" << std::endl;
    if (1 == height) {
      shared_ptr<leaf_node> new_leaf = make_copy<leaf_node>(node);

      new_leaf->elements[child_index(index, height)] = new_element;
      return new_leaf;
    } else {
      shared_ptr<internal_node> new_internal_node =
          make_copy<internal_node>(node);

      shared_ptr<const node_base>& child =
          new_internal_node->childs[child_index(index, height)];
      child = insert_recur(child, index, height - 1, new_element);
      return new_internal_node;
    }
  }

  const T& get_element_recur(const shared_ptr<const node_base>& node,
                             size_t index, size_t height) const {
//    std::cerr << "get_element(..., " << index << ", "
//              << height << ")" << std::endl;
    if (1 == height) {
      const shared_ptr<const leaf_node>& leaf =
          static_pointer_cast<const leaf_node>(node);
      return *(leaf->elements[child_index(index, height)]);
    } else {
      const shared_ptr<const internal_node>& current_node =
          static_pointer_cast<const internal_node>(node);
      return get_element_recur(current_node->childs[child_index(index, height)],
                               index, height - 1);
    }
  }
};

}  // namespace stlext

#endif  // _STLEXT_VECTOR_H_
