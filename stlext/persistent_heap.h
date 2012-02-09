#ifndef PERSISTENT_HEAP_H_
#define PERSISTENT_HEAP_H_

#include <algorithm>
#include <functional>
#include <memory>
#include <iostream>

template<class T>
struct node {

  typedef std::shared_ptr<node<T> > node_ptr;

  ~node(){
    //std::cout << "delete node value =" << value_ << std::endl;
  }

  explicit node(T value)
    : value_(value) {
    //std::cout << "new node value =" << value_ << std::endl;
  }

  node(T value, node_ptr left, node_ptr right)
    : value_(value) {
    child_[0] = left;
    child_[1] = right;
    //std::cout << "new node value =" << value_ << std::endl;
  }

  explicit node(node_ptr base_node)
    : value_(base_node->value()) {
    child_[0] = base_node->child_[0];
    child_[1] = base_node->child_[1];
    //std::cout << "new node value =" << value_ << std::endl;
  }

  node(node_ptr base_node, T value)
    : value_(value){
    child_[0] = base_node->child_[0];
    child_[1] = base_node->child_[1];
    //std::cout << "new node value =" << value_ << std::endl;
  }

  T value() {
    return value_;
  }

  node_ptr child_[2];
  node_ptr parent_;

 private:

  T value_;


};

template<class T, class Comparator = std::less<T> >
class persistent_heap {

 public:

  typedef std::shared_ptr<node<T> > node_ptr;

  persistent_heap()
    : root_(NULL), size_(0), top_size_(0) {
  }

  persistent_heap(const persistent_heap<T, Comparator> &base_heap)
    : root_(base_heap.root_), size_(base_heap.size_),
      top_size_(base_heap.top_size_) {
  }

  void operator = (const persistent_heap<T, Comparator> &base_heap){
    root_ = base_heap.root_;
    size_ = base_heap.size_;
    top_size_ = base_heap.top_size_;
  }

  void push(T value) {
    if (root_ == NULL) {
      root_.reset(new node<T>(value));
      size_ = 1;
      top_size_ = 1;
    } else {
      if (cmp_(value, root_->value())) {
        T old_value = root_->value();
        root_.reset(new node<T>(root_, value));
        value = old_value;
      }
      router push_router(root_, size_, top_size_);
      while (!push_router.end_of_routing()) {
        if (cmp_(value, push_router.child()->value())) {
          T old_value = push_router.child()->value();
          push_router.parent()->child_[push_router.child_no()].reset(
              new node<T>(push_router.child(), value));

          value = old_value;
        }
        push_router.next();
      }
      push_router.parent()->child_[push_router.child_no()].reset(
          new node<T>(value));

      size_++;
      if (size_ - 1 == 2 * top_size_) {
        top_size_ = size_;
      }
      //std::cout << size_ << std::endl;
    }
  }

  void root(){
    std::cout << "root " <<root_->value() << std::endl;
    std::cout << "size " <<size_ << std::endl;
    std::cout << "top " <<top_size_ << std::endl;
    if (root_->child_[0] != NULL) {
      std::cout << "left " <<root_->child_[0]->value() << std::endl;
    }

    if (root_->child_[1] != NULL) {
      std::cout << "right " << root_->child_[1]->value() << std::endl;
    }
  }

  T pop(){

    assert(!empty());
    T result = minimum();
    if (size_ == 1) {
      root_.reset();
      size_ = 0;
      top_size_ = 0;
    } else {
      if (size_ == top_size_){
        top_size_ >>= 1;
      }
      size_--;
      root_.reset(new node<T>(root_));
      router pop_router(root_, size_, top_size_);
      while (!pop_router.end_of_routing()) {
        pop_router.parent()->child_[pop_router.child_no()].reset(
            new node<T>(pop_router.child()));
        pop_router.next();
      }
      T value = pop_router.child()->value();
      pop_router.parent()->child_[pop_router.child_no()].reset();
      node_ptr* double_ptr(&root_);
      while ((*double_ptr)->child_[1] != NULL) {
        bool min_child;
        T min_child_value;
        if (cmp_((*double_ptr)->child_[0]->value(),
                 (*double_ptr)->child_[1]->value())) {
          min_child = 0;
          min_child_value = (*double_ptr)->child_[0]->value();
        } else {
          min_child = 1;
          min_child_value = (*double_ptr)->child_[1]->value();
        }
        if (cmp_(min_child_value, value)) {
          (*double_ptr).reset(new node<T>((*double_ptr), min_child_value));
          double_ptr = &((*double_ptr)->child_[min_child]);
        } else {
          break;
        }
      }
      if (((*double_ptr)->child_[1] == NULL &&
          (*double_ptr)->child_[0] != NULL &&
          cmp_((*double_ptr)->child_[0]->value(), value))) {
        (*double_ptr).reset(new node<T>((*double_ptr),
            (*double_ptr)->child_[0]->value()));
        (*double_ptr)->child_[0].reset(new node<T>((*double_ptr)->child_[0],
                                                   value));
      } else {
        (*double_ptr).reset(new node<T>((*double_ptr), value));
      }
    }
    return result;
  }

  bool empty(){
    return (root_ == NULL);
  }

  size_t size(){
    return size_;
  }

  T minimum(){
    return (root_)->value();
  }

 protected:

  Comparator cmp_;
  node_ptr root_;

  size_t size_;

  // size without last level (if size_ is (2^n -1)  => size_ == top_size_)
  size_t top_size_;

  friend class router;

  class router{
   public:
    router(node_ptr root, size_t size, size_t top_size)
      : parent_(root),
        current_top_size_(top_size),
        current_last_level_(size - top_size){
      count_next();
    }

    void next(){
      parent_ = child();
      current_top_size_ >>= 1;
      current_last_level_ %= current_top_size_ + 1;
      count_next();
    }

    node_ptr parent(){
      return parent_;
    }

    node_ptr child(){
      return parent_->child_[child_no_];
    }

    bool child_no(){
      return child_no_;
    }

    bool end_of_routing(){
      return ((current_top_size_ == 1 &&
          (current_last_level_== 0 || current_last_level_== 1)));
    }

   private:

    void count_next(){
      child_no_ = ((current_top_size_ + 1)/2 <= current_last_level_);
    }

    bool child_no_;
    node_ptr parent_;
    size_t current_top_size_;
    size_t current_last_level_;
  };

};

#endif /* PERSISTENT_HEAP_H_ */
