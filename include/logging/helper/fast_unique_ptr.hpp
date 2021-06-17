#include <utility>
#include <type_traits>

namespace logging {

template <class T, std::size_t Size, std::size_t Alignment>
class FastUniquePtr
{
public:

    FastUniquePtr() : is_null{true} {}

    template <typename... Args>
    explicit FastUniquePtr(Args&&... args) noexcept : is_null{false}
    {
        new (ptr()) T(std::forward<Args>(args)...);
    }

    ~FastUniquePtr() noexcept
    {
        validate_size<sizeof(T), alignof(T)>();
        if (!is_null) {
            ptr()->~T();
        }
    }

    FastUniquePtr(FastUniquePtr &rhs) = delete;
    FastUniquePtr& operator = (FastUniquePtr &rhs) = delete;

    FastUniquePtr(FastUniquePtr &&rhs) noexcept
    {
        if (!rhs.is_null) {
            new (ptr()) T(std::move(*rhs));
            is_null = false;
            rhs.ptr()->~T();
            rhs.is_null = true;
        } else {
            is_null = true;
        }
    }

    FastUniquePtr& operator = (FastUniquePtr &&rhs) noexcept
    {
        if (!rhs.is_null) {
            if (is_null) {
                new (ptr()) T(std::move(*rhs));
                is_null = false;
            } else {
                *ptr() = std::move(*rhs);
            }
            rhs.ptr()->~T();
            rhs.is_null = true;
        } else if (!is_null) {
            ptr()->~T();
            is_null = true;
        }
        return *this;
    }

    T* get() const noexcept { return !is_null ? ptr() : nullptr; }

    T* operator -> () const noexcept { return get(); }

    T& operator * () const { return *get(); }

    explicit operator bool() const noexcept { return !is_null; }

private:

    T* ptr() const noexcept
    {
        return reinterpret_cast<T*>(const_cast<std::aligned_storage_t<Size, Alignment>*>(&data));
    }

    template <std::size_t ActualSize, std::size_t ActualAlignment>
    static void validate_size()
    {
        static_assert(ActualSize <= Size, 
            "FastUniquePtr Size less than sizeof(T)");
        static_assert(ActualAlignment <= Alignment && Alignment % ActualAlignment == 0,
            "FastUniquePtr Alignment and alignof(T) mismath");
    }

    std::aligned_storage_t<Size, Alignment> data;
    bool is_null;
};

} // namespace logging