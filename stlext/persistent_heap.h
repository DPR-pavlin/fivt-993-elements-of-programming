#ifndef PERSISTENT_HEAP_H_
#define PERSISTENT_HEAP_H_

#include <algorithm>
#include <functional>
#include <memory>
#include <iostream>

namespace stlext{



template<class T>
struct node {

  typedef std::shared_ptr<node<T> > node_ptr;
  typedef std::shared_ptr<const node<T> > const_node_ptr;

  ~node(){
    //std::cout << "delete node value =" << value_ << std::endl;
  }

  explicit node(T value)
    : value_(value) {
    //std::cout << "new node value =" << value_ << std::endl;
  }

  node(T value, const_node_ptr left, const_node_ptr right)
    : value_(value) {
    child_[0] = left;
    child_[1] = right;
    //std::cout << "new node value =" << value_ << std::endl;
  }

  explicit node(const_node_ptr base_node)
    : value_(base_node->value_) {
    child_[0] = base_node->child_[0];
    child_[1] = base_node->child_[1];
    //std::cout << "new node value =" << value_ << std::endl;
  }

  node(const_node_ptr base_node, T value)
    : value_(value){
    child_[0] = base_node->child_[0];
    child_[1] = base_node->child_[1];
    //std::cout << "new node value =" << value_ << std::endl;
  }

  T value_;

  const_node_ptr child_[2];

 private:


};

template<class T, class Comparator = std::less<T> >
class persistent_heap {

 public:

  typedef std::shared_ptr<node<T> > node_ptr;
  typedef std::shared_ptr<const node<T> > const_node_ptr;

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
      root_.reset(new const node<T>(value));
      //root_ = std::make_shared<const node<T> >(value);
      size_ = 1;
      top_size_ = 1;
      //std::cout << "ok" << std::endl;
    } else {
      /*
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
      */

      //node_ptr temp = std::make_shared<node<T> >(root_);
      node_ptr temp(new node<T>(root_));
      //temp.reset(new node<T>(root_));
      root_ = temp;
      router push_router(size_, top_size_);
      //std::cout << "ok" << std::endl;
      while (!push_router.end_of_routing()) {
        if (cmp_(value, temp->value_)) {
          T old_value = temp->value_;
          temp->value_ = value;
          value = old_value;
        }
        node_ptr child_temp(new node<T>(temp->child_[push_router.child()]));
        //child_temp.reset(new node<T>(temp->child_[push_router.child()]));
        temp->child_[push_router.child()] = child_temp;
        temp = child_temp;
        push_router.next();
        //std::cout << "ok" << std::endl;
      }
      if (cmp_(value, temp->value_)) {
        T old_value = temp->value_;
        temp->value_ = value;
        value = old_value;
      }
      temp->child_[push_router.child()].reset(new node<T>(value));
      //std::cout << size_ << std::endl;

      size_++;
      if (size_ - 1 == 2 * top_size_) {
        top_size_ = size_;
      }
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

      node_ptr temp;
      temp.reset(new node<T>(root_));
      node_ptr new_root = temp;
      root_ = temp;

      //root_.reset(new node<T>(root_));
      router pop_router(size_, top_size_);
      while (!pop_router.end_of_routing()) {
        node_ptr child_temp;
        child_temp.reset(new node<T>(temp->child_[pop_router.child()]));
        temp->child_[pop_router.child()] = child_temp;
        temp = child_temp;
        pop_router.next();
        //std::cout << "ok" << std::endl;
      }
      T value = temp->child_[pop_router.child()]->value_;
      //T value = pop_router.child()->value();
      temp->child_[pop_router.child()].reset();
      //pop_router.parent()->child_[pop_router.child_no()].reset();
      node_ptr* double_ptr(&new_root);
      while ((*double_ptr)->child_[1] != NULL) {
        //std::cout << "ok1" << std::endl;
        bool min_child;
        T min_child_value;
        // TODO remove if
        /*
        if (cmp_((*double_ptr)->child_[0]->value(),
                 (*double_ptr)->child_[1]->value())) {
          min_child = 0;
          min_child_value = (*double_ptr)->child_[0]->value();
        } else {
          min_child = 1;
          min_child_value = (*double_ptr)->child_[1]->value();
        }*/

        min_child = !(cmp_((*double_ptr)->child_[0]->value_,
                           (*double_ptr)->child_[1]->value_));

        min_child_value = (*double_ptr)->child_[min_child]->value_;

        if (cmp_(min_child_value, value)) {
          //std::cout << "ok2" << std::endl;
          //std::cout << min_child_value << std::endl;
          node_ptr child_temp(new node<T>((*double_ptr)->child_[min_child]));
          (*double_ptr)->child_[min_child] = child_temp;
          (*double_ptr)->value_ = min_child_value;
          temp = child_temp;
          double_ptr = &temp;
          //(*double_ptr).reset(new node<T>((*double_ptr), min_child_value));
          //double_ptr = &((*double_ptr)->child_[min_child]);
        } else {
          break;
        }
      }
      if (((*double_ptr)->child_[1] == NULL &&
          (*double_ptr)->child_[0] != NULL &&
          cmp_((*double_ptr)->child_[0]->value_, value))) {

        (*double_ptr)->value_ = (*double_ptr)->child_[0]->value_;
        (*double_ptr)->child_[0].reset(new const node<T>(
            (*double_ptr)->child_[0], value));

        /*
        (*double_ptr).reset(new node<T>((*double_ptr),
            (*double_ptr)->child_[0]->value_));
        (*double_ptr)->child_[0].reset(new node<T>((*double_ptr)->child_[0],
                                                   value));
                                                   */
      } else {
        (*double_ptr)->value_ = value;
        //(*double_ptr).reset(new node<T>((*double_ptr), value));
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
    return (root_)->value_;
  }

 protected:

  Comparator cmp_;
  const_node_ptr root_;

  size_t size_;

  // size without last level (if size_ is (2^n -1)  => size_ == top_size_)
  size_t top_size_;

  friend class router;

  class router{
   public:
    router(size_t size, size_t top_size)
      : current_top_size_(top_size),
        current_last_level_(size - top_size){
      count_next();
    }

    void next(){
      //parent_ = child();
      current_top_size_ >>= 1;
      current_last_level_ %= current_top_size_ + 1;
      count_next();
    }

    /*
    node_ptr parent(){
      return parent_;
    }

    node_ptr child(){
      return parent_->child_[child_no_];
    }
    */

    bool child(){
      return child_;
    }

    bool end_of_routing(){
      return ((current_top_size_ == 1 &&
          (current_last_level_== 0 || current_last_level_== 1)));
    }

   private:

    void count_next(){
      child_ = ((current_top_size_ + 1)/2 <= current_last_level_);
    }

    bool child_;
    size_t current_top_size_;
    size_t current_last_level_;
  };

};


} // namespace stlext

#endif /* PERSISTENT_HEAP_H_ */
