//
// Created by lhy31 on 2023/8/29.
//
#include<iostream>

using Index=int;
namespace lhy {
    template <typename T>
    class vector {
    public:
        vector();
        explicit vector(const size_t&);
        explicit vector(T*,T*);
        vector(const std::initializer_list<T>&);
        ~vector();
        vector<T>& operator=(const std::initializer_list<T>&);
        void push_back(const T&);
        T& operator[](const Index&);
        T& At(const Index&);
        void clear();
        size_t size() const;
        bool empty() const;
        T* begin() const;
        T* rbegin() const;
        T* rend() const;
        T* end() const;
        const T* cend() const;
        const T* cbegin() const;
        void pop();
        T& front() const;
        T& back() const;

    private:
        T* start_;
        T* end_;
        T* volume_;
        void check_volume();
        void check_index(const Index&) const;
    };
    template <typename T>
    vector<T>::vector(T* start, T* end):vector() {
        for (auto *element = start; element != end; ++element) {
            push_back(*element);
        }
    }
    template <typename T>
    T* vector<T>::rend() const {
        return start_-1;
    }
    template <typename T>
    T* vector<T>::rbegin() const {
        return end_-1;
    }
    template <typename T>
    T& vector<T>::back() const {
        if (empty()) {
            throw std::range_error("no element in vector");
        }
        return *(end_ - 1);
    }
    template <typename T>
    T& vector<T>::front() const {
        if (empty()) {
            throw std::range_error("no element in vector");
        }
        return *start_;
    }
    template <typename T>
    void vector<T>::pop() {
        if (size() == 0) {
            throw std::range_error("nothing left to pop");
        }
        end_--;
    }
    template <typename T>
    T& vector<T>::operator[](const Index& index) {
        return At(index);
    }
    template <typename T>
    void vector<T>::check_index(const Index& index) const {
        if (index < 0 || index >= size()) {
            throw std::logic_error("out of range");
        }
    }
    template <typename T>
    T& vector<T>::At(const Index& index) {
        check_index(index);
        return *(start_ + index);
    }

    template <typename T>
    vector<T>::vector(const std::initializer_list<T>& element_list) : vector() {
        for (auto& element : element_list) {
            push_back(element);
        }
    }
    template <typename T>
    vector<T>& vector<T>::operator=(const std::initializer_list<T>& element_list) {
        clear();
        for (auto& element : element_list) {
            push_back(element);
        }
        return *this;
    }
    template <typename T>
    vector<T>::vector(const size_t& size) {
        start_ = new T[size];
        end_ = start_;
        volume_ = start_ + size;
    }
    template <typename T>
    const T* vector<T>::cbegin() const {
        return const_cast<const T*>(end_);
    }
    template <typename T>
    const T* vector<T>::cend() const {
        return const_cast<const T*>(start_);
    }
    template <typename T>
    T* vector<T>::end() const {
        return end_;
    }
    template <typename T>
    T* vector<T>::begin() const {
        return start_;
    }
    template <typename T>
    bool vector<T>::empty() const {
        return size() == 0;
    }
    template <typename T>
    size_t vector<T>::size() const {
        return end_ - start_;
    }
    template <typename T>
    void vector<T>::clear() {
        delete[] start_;
        start_ = new T[1];
        end_ = start_;
        volume_ = start_ + 1;
    }
    template <typename T>
    void vector<T>::check_volume() {
        if (end_ == volume_) {
            int64_t size = (volume_ - start_);
            T* start_substitute = new T[size * 2];
            end_ = start_substitute + size;
            volume_ = start_substitute + size * 2;
            for (int i = 0; i < size; ++i) {
                *(start_substitute + i) = std::move(*(start_ + i));
            }
            delete[] start_;
            start_ = start_substitute;
        }
    }
    template <typename T>
    void vector<T>::push_back(const T& element) {
        check_volume();
        *end_ = element;
        end_++;
    }
}
namespace lhy {
    template <typename T>
    vector<T>::vector() : vector(1) {}
    template <typename T>
    vector<T>::~vector() {
        delete[] start_;
    }

}
namespace lhy {
    template<typename T, typename CmpType = std::less<T>>
    class priority_queue {
    public:
        priority_queue();

        priority_queue(T *, T *);

        ~priority_queue();

        void push(T);

        void pop();

        T &top() const;

        bool empty() const;

    private:
        const Index root = 0;
        vector<T> priority_queue_;
        CmpType compare_function_;

        void Up(Index);

        void Down(Index);

        Index GetFather(Index) const;

        size_t size() const;

        Index GetLeftChild(Index) const;

        Index GetRightChild(Index) const;

        bool CheckRange(Index) const;

        void CreateHeap();
    };

    template<typename T, typename CmpType>
    T &priority_queue<T, CmpType>::top() const {
        return priority_queue_.front();
    }

    template<typename T, typename CmpType>
    void priority_queue<T, CmpType>::CreateHeap() {
        for (int i = size() >> 1; i >= 0; --i) {
            Down(i);
        }
    }

    template<typename T, typename CmpType>
    priority_queue<T, CmpType>::priority_queue(T *start, T *end) {
        for (auto &element = start; element != end; element++) {
            push(*element);
        }
    }

    template<typename T, typename CmpType>
    bool priority_queue<T, CmpType>::empty() const {
        return priority_queue_.empty();
    }

    template<typename T, typename CmpType>
    void priority_queue<T, CmpType>::pop() {
        if (empty()) {
            throw std::range_error("nothing left to pop");
        }
        std::swap(priority_queue_.back(), priority_queue_.front());
        priority_queue_.pop();
        Down(0);
    }

    template<typename T, typename CmpType>
    void priority_queue<T, CmpType>::push(T element) {
        priority_queue_.push_back(element);
        Up(size() - 1);
    }

    template<typename T, typename CmpType>
    priority_queue<T, CmpType>::~priority_queue() = default;

    template<typename T, typename CmpType>
    priority_queue<T, CmpType>::priority_queue() = default;

    template<typename T, typename CmpType>
    bool priority_queue<T, CmpType>::CheckRange(Index index) const {
        return index < size() && index >= 0;
    }

    template<typename T, typename CmpType>
    Index priority_queue<T, CmpType>::GetRightChild(Index index) const {
        return 2 * index + 2;
    }

    template<typename T, typename CmpType>
    Index priority_queue<T, CmpType>::GetLeftChild(Index index) const {
        return 2 * index + 1;
    }

    template<typename T, typename CmpType>
    size_t priority_queue<T, CmpType>::size() const {
        return priority_queue_.size();
    }

    template<typename T, typename CmpType>
    void priority_queue<T, CmpType>::Down(Index index) {
        Index left_child_index = GetLeftChild(index);
        Index right_child_index = GetRightChild(index);
        if (!CheckRange(left_child_index) && !CheckRange(right_child_index)) {
            return;
        } else {
            Index better_child_index;
            if (CheckRange(left_child_index) && CheckRange(right_child_index)) {
                if (compare_function_(priority_queue_[left_child_index], priority_queue_[right_child_index])) {
                    better_child_index = left_child_index;
                } else {
                    better_child_index = right_child_index;
                }
            } else if (CheckRange(left_child_index)) {
                better_child_index = left_child_index;
            } else if (CheckRange(right_child_index)) {
                better_child_index = right_child_index;
            }
            if (compare_function_(priority_queue_[better_child_index], priority_queue_[index])) {
                std::swap(priority_queue_[better_child_index], priority_queue_[index]);
                Down(better_child_index);
            }
        }
    }

    template<typename T, typename CmpType>
    void priority_queue<T, CmpType>::Up(Index index) {
        if (index == root) {
            return;
        }
        Index father_index = GetFather(index);
        if (compare_function_(priority_queue_[index], priority_queue_[father_index])) {
            std::swap(priority_queue_[index], priority_queue_[father_index]);
            Up(father_index);
        }
    }

    template<typename T, typename CmpType>
    Index priority_queue<T, CmpType>::GetFather(Index index) const {
        return (index - 1) / 2;
    }
}
using namespace lhy;
int a[100001];
int b[100001];

struct NODE{
    int value;
    int col;
    int row;
    NODE(){
        value=0;
        col=0;
        row=0;
    }
    NODE(int v, int r, int c){
        value = v;
        row = r;
        col = c;
    }
    friend bool operator< (NODE n1, NODE n2)
    {
        return n1.value < n2.value;
    }
};
using std::cin;
using std::cout;
int main(){
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
        cin >> a[i];
    for (int i = 0; i < n; i++)
        cin >> b[i];

    priority_queue <NODE> pque;

    for (int i = 0; i < n; i++){
        NODE nd(a[i] + b[0], i, 0);
        pque.push(nd);
    }

    int cnt = 0;
    while (cnt < n){
        NODE min = pque.top(); pque.pop();
        cnt++;
        cout << min.value << " ";

        int row = min.row;
        int col = min.col;
        NODE nd(a[row] + b[col + 1], row, col + 1);
        pque.push(nd);
    }
    return 0;
}