#ifndef HIRA_DS_INTERNAL_SEGMENT_TREE_H
#define HIRA_DS_INTERNAL_SEGMENT_TREE_H

#include "hira/common_header.h"

namespace hira {

namespace ds {

namespace internal {

template <typename Data, typename Tag>
struct SegmentTreeNode {
 public:
  void ApplayUpdate(const Tag& tag) {
    data_.Apply(left_bound_, right_bound_, tag);
    tag_.Apply(left_bound_, right_bound_, tag);
  }

  void MaintainInfomation() {
    ASSERT(left_child_ && right_child_);

    data_ = left_child_->data_ + right_child_->data_;
  }

  void Propagation() {
    if (tag_.NeedPropagation()) {
      right_child_->ApplayUpdate(tag_);
      left_child_->ApplayUpdate(tag_);
      tag_.Reset();
    }
  }

  SegmentTreeNode()
      : left_child_(nullptr),
        right_child_(nullptr),
        left_bound_(-1),
        right_bound_(-1) {}

 public:
  SegmentTreeNode* left_child_;
  SegmentTreeNode* right_child_;

  int left_bound_;
  int right_bound_;

  Data data_;
  Tag tag_;
};

template <typename Data, typename Tag, typename AllocatorType>
class SegmentTreeImpl {
 public:
  using allocator_type = AllocatorType;

  using data_type = Data;
  using tag_type = Tag;
  using node_type = SegmentTreeNode<data_type, tag_type>;
  using pointer = typename allocator_type::pointer;
  typedef const pointer const_pointer;

  /*
   * Used for binary search on segment tree. That is, find the
   * leftmost(rightmost) position satisfying some condition.
   *
   * If it should go to the desire(or optimal) direction, then return ture.
   * Otherwise return false.
   *
   * For example, if you want to find the leftmost
   * position satisfying some condition, then return true to go left.
   */
  using judger_type = std::function<bool(const data_type&, const data_type&)>;

 protected:
  pointer Build(allocator_type& allocator,
                const std::vector<data_type>& array,
                int left,
                int right) {
    pointer p = allocator.Allocate();
    p->left_bound_ = left;
    p->right_bound_ = right;

    if (left == right) {
      p->data_ = array[left];
    } else {
      int middle = (left + right) >> 1;
      p->left_child_ = Build(allocator, array, left, middle);
      p->right_child_ = Build(allocator, array, middle + 1, right);
      p->MaintainInfomation();
    }

    return p;
  }

  void Destroy(allocator_type& allocator, pointer p) {
    if (!p)
      return;
    Destroy(allocator, p->left_child_);
    Destroy(allocator, p->right_child_);
    allocator.Free(p);
  }

  std::string to_string(const_pointer root) const {
    std::stringstream ss;

    ss << "SegmentTree [\n";
    std::function<void(const_pointer)> dfs = [&](const_pointer p) {
      if (p->left_bound_ == p->right_bound_) {
        ss << "  [" << p->left_bound_ << "]: {" << p->data_.to_string()
           << "}, {" << p->tag_.to_string() << "}\n";
        return;
      }
      dfs(p->left_child_);
      dfs(p->right_child_);
    };
    dfs(root);
    ss << "]\n\n";

    return ss.str();
  }

 protected:
  void Update(pointer p, int left, int right, const tag_type& tag) {
    ASSERT(p);

    if (p->left_bound_ >= left && p->right_bound_ <= right) {
      p->ApplayUpdate(tag);
      return;
    }

    p->Propagation();

    if (p->left_child_->right_bound_ >= left)
      Update(p->left_child_, left, right, tag);
    if (p->right_child_->left_bound_ <= right)
      Update(p->right_child_, left, right, tag);

    p->MaintainInfomation();
  }

  const data_type Query(pointer p, int left, int right) {
    ASSERT(p);

    if (p->left_bound_ >= left && p->right_bound_ <= right)
      return p->data_;

    p->Propagation();

    data_type result;
    if (p->left_child_->right_bound_ >= left)
      result = result + Query(p->left_child_, left, right);
    if (p->right_child_->left_bound_ <= right)
      result = result + Query(p->right_child_, left, right);

    return result;
  }

  std::pair<int, const data_type> FindLeftmostIf(pointer p,
                                                 const judger_type& judger) {
    ASSERT(p);

    if (p->left_bound_ == p->right_bound_)
      return {p->left_bound_, p->data_};

    p->Propagation();

    if (judger(p->left_child_->data_, p->right_child_->data_))
      return FindLeftmostIf(p->left_child_, judger);
    return FindLeftmostIf(p->right_child_, judger);
  }

  std::pair<int, const data_type> FindRightmostIf(pointer p,
                                                  const judger_type& judger) {
    ASSERT(p);

    if (p->left_bound_ == p->right_bound_)
      return {p->left_bound_, p->data_};

    p->Propagation();

    if (judger(p->left_child_->data_, p->right_child_->data_))
      return FindRightmostIf(p->right_child_, judger);
    return FindRightmostIf(p->left_child_, judger);
  }
};

}  // namespace internal

}  // namespace ds

}  // namespace hira

#endif  // HIRA_DS_INTERNAL_SEGMENT_TREE_H