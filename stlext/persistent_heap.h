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
      size_ = 1;
      top_size_ = 1;
    } else {
      router r(root_, size_, top_size_);
      root_ = r.current_;
      while (!r.end_of_routing()) {
        swap_if_less(value, r.current_->value_);
        r.down();
      }
      swap_if_less(value, r.current_->value_);
      r.current_->child_[r.next()].reset(new node<T>(value));

      size_++;
      if (size_ - 1 == 2 * top_size_) {
        top_size_ = size_;
      }
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

      router r(root_, size_, top_size_);
      root_ = r.current_;
      node_ptr new_root = r.current_;
      while (!r.end_of_routing()) {
        r.down();
      }
      T value = r.current_->child_[r.next()]->value_;
      r.current_->child_[r.next()].reset();
      node_ptr* double_ptr(&new_root);
      node_ptr temp;
      while ((*double_ptr)->child_[1] != NULL) {
        bool min_child;
        T min_child_value;
        min_child = !(cmp_((*double_ptr)->child_[0]->value_,
                           (*double_ptr)->child_[1]->value_));
        min_child_value = (*double_ptr)->child_[min_child]->value_;
        if (cmp_(min_child_value, value)) {
          node_ptr child_temp(new node<T>((*double_ptr)->child_[min_child]));
          (*double_ptr)->child_[min_child] = child_temp;
          (*double_ptr)->value_ = min_child_value;
          temp = child_temp;
          double_ptr = &temp;
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

      } else {
        (*double_ptr)->value_ = value;
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


  void swap_if_less(T& a, T& b){
    if (cmp_(a, b)){
      std::swap(a,b);
    }
  }

  class router{
   public:
    router(const_node_ptr root, size_t size, size_t top_size)
      : current_(new node<T>(root)),
        current_top_size_(top_size),
        current_last_level_(size- top_size){
      count_next();
    }

    void down(){
      node_ptr next_node(new node<T>(current_->child_[next_]));
      current_->child_[next_] = next_node;
      current_ = next_node;
      current_top_size_ >>= 1;
      current_last_level_ %= current_top_size_ + 1;
      count_next();
    }

    bool next(){
      return next_;
    }

    bool end_of_routing(){
      return ((current_top_size_ == 1 &&
          (current_last_level_== 0 || current_last_level_== 1)));
    }

    node_ptr current_;

   private:

    void count_next(){
      next_ = ((current_top_size_ + 1)/2 <= current_last_level_);
    }

    bool next_;
    size_t current_top_size_;
    size_t current_last_level_;
  };

};


} // namespace stlext

#endif /* PERSISTENT_HEAP_H_ */
