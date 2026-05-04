#include <iostream>
#include <cstddef>
#include <utility>
#include <tuple>

template< class T >
struct BiTree
{
  T val;
  BiTree< T > * lt, * rt, * parent;
};

enum Dir {
  fall_left_dir,
  parent_dir
};

template< class T >
std::pair< size_t, BiTree< T > * > fall_left(BiTree< T > * root)
{
  size_t path = 0;
  while (root->lt)
  {
    root = root->lt;
    path++;
  }
  return {path, root};
}

template< class T >
std::pair< size_t, BiTree< T > * > parent(BiTree< T > * root)
{
  if (!root)
  {
    return {0, nullptr};
  }
  size_t path = 0;
  BiTree< T > * parent = root->parent;
  while (parent && parent->lt != root)
  {
    root = parent;
    parent = root->parent;
    path++;
  }
  return {path, parent};
}

template< class T >
BiTree< T > * prev(BiTree< T > * curr)
{
  BiTree< T > * next = curr;
  if (next->lt)
  {
    next = next->lt;
    while (next->rt)
    {
      next = next->rt;
    }
  }
  else
  {
    BiTree< T > * parent = next->parent;
    while (parent && parent->rt != next)
    {
      next = parent;
      parent = next->parent;
    }
    next = parent;
  }
  return next;
}

template< class T >
std::tuple< Dir, size_t, BiTree< T > * > nextStruct(BiTree< T > * root)
{
  if (root->rt)
  {
    auto result = fall_left(root->rt);
    return {fall_left_dir, result.first, result.second};
  }
  auto result = parent(root);
  return {parent_dir, result.first, result.second};
}

template< class T >
std::tuple< BiTree< T > *, BiTree< T > *, bool > isEqualStructStart(BiTree< T > * lhs_start, BiTree< T > * rhs_start)
{
  auto ln = nextStruct(lhs_start);
  auto rn = nextStruct(rhs_start);

  while (std::get< 0 >(ln) == std::get< 0 >(rn) &&
        std::get< 1 >(ln) == std::get< 1 >(rn) &&
        std::get< 2 >(ln) &&
        std::get< 2 >(rn))
  {
    ln = nextStruct(std::get< 2 >(ln));
    rn = nextStruct(std::get< 2 >(rn));
  }

  bool dir = std::get< 0 >(ln) == std::get< 0 >(rn);
  return {std::get< 2 >(ln), std::get< 2 >(rn), dir};
}

template< class T >
struct InclusionIt
{
  std::pair< BiTree< T > *, BiTree< T > * > incl;
};

template< class T >
std::pair< BiTree< T > *, BiTree< T > * > inclusionStart(BiTree< T > * lhs, BiTree< T > * pattern)
{
  if (!lhs || !pattern)
  {
    return {nullptr, nullptr};
  }

  BiTree< T > * lhs_curr = lhs;
  BiTree< T > * pattern_begin = fall_left(pattern).second;

  while (lhs_curr)
  {
    auto result = isEqualStructStart(lhs_curr, pattern_begin);

    if (!std::get< 1 >(result) && std::get< 2 >(result))
    {
      BiTree< T > * last_lhs_next = std::get< 0 >(result);

      if (!last_lhs_next)
      {
        BiTree< T > * lhs_end = lhs_curr;

        while (std::get< 2 >(nextStruct(lhs_end)))
        {
          lhs_end = std::get< 2 >(nextStruct(lhs_end));
        }

        return {lhs_curr, lhs_end};
      }

      BiTree< T > * lhs_end = prev(last_lhs_next);
      return {lhs_curr, lhs_end};
    }

    lhs_curr = std::get< 2 >(nextStruct(lhs_curr));
  }

  return {nullptr, nullptr};
}

template< class T >
InclusionIt< T > begin(BiTree< T > * lhs, BiTree< T > * pattern)
{
  if (!lhs || !pattern)
  {
    return {{nullptr, nullptr}};
  }

  auto result = inclusionStart(fall_left(lhs).second, pattern);
  return {{result.first, result.second}};
}

template< class T >
InclusionIt< T > next(InclusionIt< T > curr, BiTree< T > * pattern)
{
  if (!curr.incl.second)
  {
    return {{nullptr, nullptr}};
  }

  BiTree< T > * start = std::get< 2 >(nextStruct(curr.incl.second));

  auto result = inclusionStart(start, pattern);
  return {{result.first, result.second}};
}

template< class T >
bool hasNext(InclusionIt< T > curr, BiTree< T >* pattern)
{
  return curr.incl.first != nullptr;
}

int main()
{}
