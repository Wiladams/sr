// Base class for subsequent inheritance
class Object {
    virtual ~Object() {}
};

struct IEnumerator {
    virtual bool moveNext() = 0;
    virtual void reset() = 0;
    
    virtual Object getCurrent() =0;
};

template <typename T>
struct IIterable {

    virtual IEnumerator<T> GetEnumerator() = 0;
};

