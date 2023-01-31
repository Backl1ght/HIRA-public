#ifndef HIRA_DS_SEGMENT_TREE_H
#define HIRA_DS_SEGMENT_TREE_H

#include "hira/common_header.h"

#include "hira/base/allocator.h"
#include "hira/ds/internal/segment_tree.h"

namespace hira {

namespace ds {

/**
 * Segment tree that offers range update and query abilities.
 *
 * features:
 *   - O(n) construct and destruct.
 *   - O(log n) range update.
 *   - O(log n) range query.
 *   - O(log n) binary search on segment tree.
 *   - lazy propagation
 *
 * TODO(backlight):
 *   - dynamic node: allocate node only when necessary, which mighe be helpful
 * for problem with large range(i.e. 1e9).
 *   - persistance: support persistant segment tree using path copying.
 *
 */
template <typename Data,
          typename Tag,
          typename AllocatorType =
              base::InstantAllocator<internal::SegmentTreeNode<Data, Tag>>>
class SegmentTree : internal::SegmentTreeImpl<Data, Tag, AllocatorType> {
 public:
  using impl_type = internal::SegmentTreeImpl<Data, Tag, AllocatorType>;

  using allocator_type = typename impl_type::allocator_type;

  using data_type = typename impl_type::data_type;
  using tag_type = typename impl_type::tag_type;
  using node_type = typename impl_type::node_type;
  using pointer = typename impl_type::pointer;
  using const_pointer = typename impl_type::const_pointer;

  using judger_type = typename impl_type::judger_type;

 public:
  pointer Build(const std::vector<data_type>& array) {
    return impl_type::Build(allocator_, array, 0, n_ - 1);
  }

  void Destroy() { impl_type::Destroy(allocator_, root_); }

  void Update(int left, int right, const tag_type& tag) {
    ASSERT(left >= 0 && right < n_);

    impl_type::Update(root_, left, right, tag);
  }

  const data_type Query(int left, int right) {
    ASSERT(left >= 0 && right < n_);

    return impl_type::Query(root_, left, right);
  }

  std::pair<int, const data_type> FindLeftmostIf(const judger_type& judger) {
    return impl_type::FindLeftmostIf(root_, judger);
  }

  std::pair<int, const data_type> FindRightmostIf(const judger_type& judger) {
    return impl_type::FindRightmost(root_, judger);
  }

 public:
  SegmentTree(const std::vector<data_type>& array) : n_(array.size()) {
    root_ = Build(array);
  }

  ~SegmentTree() { Destroy(); }

  std::string to_string() const { return impl_type::to_string(root_); }

 private:
  int n_;
  pointer root_;
  AllocatorType allocator_;
};

/*
struct Tag {
 public:
  int range_add_;

 public:
  Tag(int range_add = 0) : range_add_(range_add) {}

  bool NeedPropagation() { return range_add_ != 0; }

  void Apply(int left, int right, const Tag& tag) {
    range_add_ = range_add_ + tag.range_add_;
  }

  void Reset() { range_add_ = 0; }

  std::string to_string() const { return std::to_string(range_add_); }
};

struct Data {
 public:
  int mn1_;
  int mn2_;

 public:
  Data(int mn1 = 0x3f3f3f3f, int mn2 = 0x3f3f3f3f) : mn1_(mn1), mn2_(mn2) {}

  void Apply(int left, int right, const Tag& tag) {
    int length = right - left + 1;

    mn1_ = mn1_ + tag.range_add_;
    mn2_ = mn2_ + tag.range_add_;
  }

  friend Data operator+(const Data& lhs, const Data& rhs) {
    Data result;

    result.mn1_ = std::min(lhs.mn1_, rhs.mn1_);
    result.mn2_ = std::min(lhs.mn2_, rhs.mn2_);

    return result;
  }

  std::string to_string() const {
    return std::to_string(mn1_) + ", " + std::to_string(mn2_);
  }
};
*/

}  // namespace ds

}  // namespace hira

#endif  // HIRA_DS_SEGMENT_TREE_H